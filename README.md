# The Legend of Zelda: Ocarina of Time

[![Build Status][jenkins-badge]][jenkins] [![Decompilation Progress][progress-badge]][progress] [![Contributors][contributors-badge]][contributors] [![Discord Channel][discord-badge]][discord]

[jenkins]: https://jenkins.deco.mp/job/OOT/job/main
[jenkins-badge]: https://img.shields.io/jenkins/build?jobUrl=https%3A%2F%2Fjenkins.deco.mp%2Fjob%2FOOT%2Fjob%2Fmain

[progress]: https://zelda.deco.mp/games/oot
[progress-badge]: https://img.shields.io/endpoint?url=https://zelda.deco.mp/assets/csv/progress-oot-shield.json

[contributors]: https://github.com/zeldaret/oot/graphs/contributors
[contributors-badge]: https://img.shields.io/github/contributors/zeldaret/oot

[discord]: https://discord.zelda.deco.mp
[discord-badge]: https://img.shields.io/discord/688807550715560050?color=%237289DA&logo=discord&logoColor=%23FFFFFF

```diff
- WARNING! -

This repository is a work in progress, and while it can be used to make certain changes, it's still
constantly evolving. If you use it for modding purposes in its current state, please be aware that
the codebase can drastically change at any time. Also note that some parts of the ROM may not be
'shiftable' yet, so modifying them could be difficult at this point.
```

This is a WIP decompilation of The Legend of Zelda: Ocarina of Time.
The purpose of the project is to recreate a source code base for the game from scratch, using information found inside the game along with static and/or dynamic analysis.

It builds the following versions:

| Name         | Build timestamp   | Description                               | MD5 hash of input ROM(s) |
|--------------|-------------------|-------------------------------------------|--------------------------|
| ntsc-1.0     | 98-10-21 04:56:31 | NTSC 1.0 (Japan/US)                       | `9f04c8e68534b870f707c247fa4b50fc`<br>`5bd1fe107bf8106b2ab6650abecd54d6` |
| ntsc-1.1     | 98-10-26 10:58:45 | NTSC 1.1 (Japan/US)                       | `1bf5f42b98c3e97948f01155f12e2d88`<br>`721fdcc6f5f34be55c43a807f2a16af4` |
| pal-1.0      | 98-11-10 14:34:22 | PAL 1.0 (Europe)                          | `e040de91a74b61e3201db0e2323f768a` |
| ntsc-1.2     | 98-11-12 18:17:03 | NTSC 1.2 (Japan/US)                       | `2258052847bdd056c8406a9ef6427f13`<br>`57a9719ad547c516342e1a15d5c28c3d` |
| pal-1.1      | 98-11-18 17:36:49 | PAL 1.1 (Europe)                          | `d714580dd74c2c033f5e1b6dc0aeac77` |
| gc-jp        | 02-10-29 23:49:53 | GameCube Japan                            | `33fb7852c180b18ea0b9620b630f413f` |
| gc-jp-mq     | 02-10-30 00:15:15 | GameCube Japan Master Quest               | `69895c5c78442260f6eafb2506dc482a` |
| gc-us        | 02-12-19 13:28:09 | GameCube US                               | `cd09029edcfb7c097ac01986a0f83d3f` |
| gc-us-mq     | 02-12-19 14:05:42 | GameCube US Master Quest                  | `da35577fe54579f6a266931cc75f512d` |
| gc-eu-mq-dbg | 03-02-21 00:16:31 | GameCube Europe/PAL Master Quest Debug    | `75e344f41c26ec2ec5ad92caa9e25629`<br>`8ca71e87de4ce5e9f6ec916202a623e9`<br>`f751d1a097764e2337b1ac9ba1e27699`<br>`dde376d47187b931820d5b2957cded14` |
| gc-eu        | 03-02-21 20:12:23 | GameCube Europe/PAL                       | `2c27b4e000e85fd78dbca551f1b1c965` |
| gc-eu-mq     | 03-02-21 20:37:19 | GameCube Europe/PAL Master Quest          | `1618403427e4344a57833043db5ce3c3` |
| gc-jp-ce     | 03-10-08 21:53:00 | GameCube Japan (Collector's Edition Disc) | `0c13e0449a28ea5b925cdb8af8d29768` |

The default version is `gc-eu-mq-dbg`, i.e. the GameCube Europe/PAL Master Quest Debug ROM.

**Note: This repository does not include any of the assets necessary to build the ROM. A prior copy of the game is required to extract the needed assets.**

**Website:** <https://zelda.deco.mp>

**Discord:** <https://discord.zelda.deco.mp>

## Installation

### Windows

For Windows 10 or 11, install WSL and a distribution by following this
[WSL Installation Guide](https://docs.microsoft.com/en-us/windows/wsl/install).
We recommend using Debian or Ubuntu 20.04 Linux distributions.

For older versions of Windows, install a Linux VM or refer to [Docker](#Docker) instructions.

### macOS

For macOS, use homebrew to install the following dependencies:

* coreutils
* make
* python3
* md5sha1sum
* libpng

You can install them with the following commands:

```bash
brew update
brew install coreutils make python3 md5sha1sum libpng libiconv
```

You'll also need to [build and install mips-linux-binutils](docs/BUILDING_BINUTILS_MACOS.md).

Going forward in this guide, please use `gmake` whenever you encounter a `make` command.
The `make` that comes with macOS behaves differently than GNU make and is incompatible with this project.

You should now be able to continue from [step 2](#2-clone-the-repository) of the Linux instructions.

### Linux (Native or under WSL / VM)

#### 1. Install build dependencies

The build process has the following package requirements:

* git
* build-essential
* binutils-mips-linux-gnu
* python3
* python3-pip
* python3-venv
* libpng-dev
* libxml2-dev

Under Debian / Ubuntu (which we recommend using), you can install them with the following commands:

```bash
sudo apt-get update
sudo apt-get install git build-essential binutils-mips-linux-gnu python3 python3-pip python3-venv libpng-dev libxml2-dev
```

If you are using GCC as the compiler for Ocarina of Time, you will also need:

* gcc-mips-linux-gnu

#### 2. Clone the repository

Clone `https://github.com/zeldaret/oot.git` where you wish to have the project, with a command such as:

```bash
git clone https://github.com/zeldaret/oot.git
```

#### 3. Prepare a base ROM

Place a copy of the Master Quest (Debug) ROM inside the `baseroms/gc-eu-mq-dbg/` folder.
If you are under WSL, you can run the command `explorer.exe .` to open the current directory in the Windows file explorer.

Rename the file to `baserom.z64`, `baserom.n64` or `baserom.v64`, depending on the original extension.

#### 4. Setup the ROM and build process

Setup and extract everything from your ROM with the following command:

```bash
make setup
```

This downloads some dependencies (from pip), and compiles tools for the build process.
Then it generates a new ROM `baseroms/gc-eu-mq-dbg/baserom-decompressed.z64` that will have the overdump removed and the header patched.
It will also extract the individual assets from the ROM.

#### 5. Build the ROM

Run make to build the ROM.
Make sure your path to the project is not too long, otherwise this process may error.

```bash
make
```

If all goes well, a new ROM should be built at `build/gc-eu-mq-dbg/oot-gc-eu-mq-dbg.z64`, and the following text printed:

```bash
build/gc-eu-mq-dbg/oot-gc-eu-mq-dbg.z64: OK
```

If you instead see the following:

```bash
build/gc-eu-mq-dbg/oot-gc-eu-mq-dbg.z64: FAILED
md5sum: WARNING: 1 computed checksum did NOT match
```

This means that the built ROM isn't the same as the base one, so something went wrong or some part of the code doesn't match.

### Docker

#### 1. Setup requirements

To use Docker, you'll need either Docker Desktop or Docker Toolbox installed and setup based on your system.

You'll also need to prepare a local version of the project with a copied base ROM (see steps [2](#2-clone-the-repository) and [3](#3-prepare-a-base-rom) of the Linux instructions).

#### 2. Create the Docker image

From inside your local project, run the following command:

```bash
docker build . -t oot
```

#### 3. Start the container

To start the container, you can mount your local filesystem into the Docker container and run an interactive bash session.

```bash
docker run -it --rm --mount type=bind,source="$(pwd)",destination=/oot oot /bin/bash
```

#### 4. Setup and Build the ROM

Once inside the container, you can follow steps [4](#4-setup-the-rom-and-build-process) and [5](#5-build-the-rom) of the Linux instructions to setup and build the ROM, or run any other command you need.

## Changing build options

The project Makefile is fairly configurable and can be used to build other versions of the game or prepare the repo for modding.
See the options outlined at the top of the Makefile for more information.

## Contributing

All contributions are welcome. This is a group effort, and even small contributions can make a difference.
Some tasks also don't require much knowledge to get started.

Most discussions happen on our [Discord Server][discord], where you are welcome to ask if you need help getting started, or if you have any questions regarding this project and other decompilation projects.
