#!/usr/bin/env python3

import argparse, json, os, signal, time
from multiprocessing import Pool, cpu_count, Event, Manager, ProcessError
from pathlib import Path

from tools import version_config


def SignalHandler(sig, frame):
    print(f'Signal {sig} received. Aborting...')
    mainAbort.set()
    # Don't exit immediately to update the extracted assets file.

def ExtractFile(assetConfig: version_config.AssetConfig, outputPath: Path, outputSourcePath: Path):
    xmlPath = assetConfig.xml_path
    version = globalVersionConfig.version
    if globalAbort.is_set():
        # Don't extract if another file wasn't extracted properly.
        return

    execStr = f"tools/ZAPD/ZAPD.out e -eh -i {xmlPath} -b extracted/{version}/baserom -o {outputPath} -osf {outputSourcePath} -gsf 1 -rconf tools/ZAPDConfigs/{version}/Config.xml --cs-float both"
    
    if "code" in xmlPath.parts or "overlays" in xmlPath.parts:
        assert assetConfig.start_offset is not None
        assert assetConfig.end_offset is not None

        execStr += f" --start-offset 0x{assetConfig.start_offset:X}"
        execStr += f" --end-offset 0x{assetConfig.end_offset:X}"

    if "overlays" in xmlPath.parts:
        overlayName = xmlPath.stem
        baseAddress = globalVersionConfig.dmadata_segments[overlayName].vram + assetConfig.start_offset

        execStr += f" --base-address 0x{baseAddress:X}"
        execStr += " --static"

    if globalUnaccounted:
        execStr += " -wu"

    print(execStr)
    exitValue = os.system(execStr)
    if exitValue != 0:
        globalAbort.set()
        print("\n")
        print("Error when extracting from file " + xmlPath, file=os.sys.stderr)
        print("Aborting...", file=os.sys.stderr)
        print("\n")

def ExtractFunc(assetConfig: version_config.AssetConfig):
    objectName = assetConfig.name
    xml_path = assetConfig.xml_path
    xml_path_str = str(xml_path)

    version = globalVersionConfig.version
    outPath = Path("extracted") / version / "assets" / objectName
    outSourcePath = outPath

    if xml_path_str in globalExtractedAssetsTracker:
        timestamp = globalExtractedAssetsTracker[xml_path_str]["timestamp"]
        modificationTime = int(os.path.getmtime(xml_path))
        if modificationTime < timestamp:
            # XML has not been modified since last extraction.
            return

    currentTimeStamp = int(time.time())

    ExtractFile(assetConfig, outPath, outSourcePath)

    if not globalAbort.is_set():
        # Only update timestamp on succesful extractions
        if xml_path_str not in globalExtractedAssetsTracker:
            globalExtractedAssetsTracker[xml_path_str] = globalManager.dict()
        globalExtractedAssetsTracker[xml_path_str]["timestamp"] = currentTimeStamp

def initializeWorker(versionConfig: version_config.VersionConfig, abort, unaccounted: bool, extractedAssetsTracker: dict, manager):
    global globalVersionConfig
    global globalAbort
    global globalUnaccounted
    global globalExtractedAssetsTracker
    global globalManager
    globalVersionConfig = versionConfig
    globalAbort = abort
    globalUnaccounted = unaccounted
    globalExtractedAssetsTracker = extractedAssetsTracker
    globalManager = manager

def main():
    parser = argparse.ArgumentParser(description="baserom asset extractor")
    parser.add_argument("-v", "--oot-version", dest="oot_version", help="OOT game version", default="gc-eu-mq-dbg")
    parser.add_argument("-s", "--single", help="Extract a single asset by name, e.g. objects/gameplay_keep")
    parser.add_argument("-f", "--force", help="Force the extraction of every xml instead of checking the touched ones (overwriting current files).", action="store_true")
    parser.add_argument("-u", "--unaccounted", help="Enables ZAPD unaccounted detector warning system.", action="store_true")
    args = parser.parse_args()

    version: str = args.oot_version
    versionConfig = version_config.load_version_config(version)

    global mainAbort
    mainAbort = Event()
    manager = Manager()
    signal.signal(signal.SIGINT, SignalHandler)

    extraction_times_p = Path("extracted") / version / "assets_extraction_times.json"
    extractedAssetsTracker = manager.dict()
    if extraction_times_p.exists() and not args.force:
        with extraction_times_p.open(encoding='utf-8') as f:
            extractedAssetsTracker.update(json.load(f, object_hook=manager.dict))

    singleAssetName = args.single
    if singleAssetName is not None:
        assetConfig = None
        for asset in versionConfig.assets:
            if asset.name == singleAssetName:
                assetConfig = asset
                break
        else:
            print(f"Error. Asset {singleAssetName} not found in config.", file=os.sys.stderr)
            exit(1)

        initializeWorker(versionConfig, mainAbort, args.unaccounted, extractedAssetsTracker, manager)
        # Always extract if -s is used.
        xml_path_str = str(assetConfig.xml_path)
        if xml_path_str in extractedAssetsTracker:
            del extractedAssetsTracker[xml_path_str]
        ExtractFunc(assetConfig)
    else:
        try:
            numCores = cpu_count()
            print("Extracting assets with " + str(numCores) + " CPU cores.")
            with Pool(numCores, initializer=initializeWorker, initargs=(versionConfig, mainAbort, args.unaccounted, extractedAssetsTracker, manager)) as p:
                p.map(ExtractFunc, versionConfig.assets)
        except (ProcessError, TypeError):
            print("Warning: Multiprocessing exception ocurred.", file=os.sys.stderr)
            print("Disabling mutliprocessing.", file=os.sys.stderr)

            initializeWorker(versionConfig, mainAbort, args.unaccounted, extractedAssetsTracker, manager)
            for assetConfig in versionConfig.assets:
                ExtractFunc(assetConfig)

    with extraction_times_p.open('w', encoding='utf-8') as f:
        serializableDict = dict()
        for xml, data in extractedAssetsTracker.items():
            serializableDict[xml] = dict(data)
        json.dump(dict(serializableDict), f, ensure_ascii=False, indent=4)

    if mainAbort.is_set():
        exit(1)

if __name__ == "__main__":
    main()