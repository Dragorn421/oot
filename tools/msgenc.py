#!/usr/bin/env python3
#
#   message_data_static text encoder
#

import argparse, ast, re
import sys

def read_charmap(path):
    with open(path) as infile:
        charmap = infile.read()

    charmap = ast.literal_eval(charmap)
    charmap = { repr(k)[1:-1] : chr(v) for k,v in charmap.items() }

    return charmap

# From https://stackoverflow.com/questions/241327/remove-c-and-c-comments-using-python
def remove_comments(text):
    def replacer(match):
        s = match.group(0)
        if s.startswith('/'):
            return " " # note: a space and not an empty string
        else:
            return s

    pattern = re.compile(
        r'//.*?$|/\*.*?\*/|\'(?:\\.|[^\\\'])*\'|"(?:\\.|[^\\"])*"',
        re.DOTALL | re.MULTILINE
    )
    return re.sub(pattern, replacer, text)

def convert_text(text, charmap, ignore_hash_lines: bool):
    def cvt_str(m):
        string = m.group(0)

        for orig,char in charmap.items():
            string = string.replace(orig, char)

        return string

    # Naive string matcher, assumes single line strings and no comments, handles escaped quotations
    string_regex = re.compile(r'"((?:[^\\"\n]|\\.)*)"')

    # Collapse escaped newlines
    text = text.replace("\\\n", "")
    # Encode according to charmap
    text = re.sub(string_regex, cvt_str, text)

    return text

def main():
    parser = argparse.ArgumentParser(description="Encode message_data_static text headers")
    parser.add_argument(
        "input",
        help="path to file to be encoded, or - for stdin",
    )
    parser.add_argument(
        "--output",
        "-o",
        help="path to write encoded file, or - for stdout",
        required=True,
    )
    parser.add_argument(
        "--charmap",
        help="path to charmap file specifying custom encoding elements",
        required=True,
    )
    parser.add_argument(
        "--remove-comments",
        dest="remove_comments",
        help="Remove C comments ('//', '/*...*/') from the input before processing",
        action="store_true",
    )
    parser.add_argument(
        "--ignore-hash-lines",
        dest="ignore_hash_lines",
        help="Ignore lines starting with '#', such as C preprocessor directives and linemarkers",
        action="store_true",
    )
    args = parser.parse_args()

    charmap = read_charmap(args.charmap)

    text = ""
    if args.input == "-":
        text = sys.stdin.read()
    else:
        with open(args.input, "r") as infile:
            text = infile.read()

    if args.remove_comments:
        text = remove_comments(text)
    text = convert_text(text, charmap, args.ignore_hash_lines)

    if args.output == "-":
        sys.stdout.buffer.write(text.encode("raw_unicode_escape"))
    else:
        with open(args.output, "w", encoding="raw_unicode_escape") as outfile:
            outfile.write(text)

if __name__ == "__main__":
    main()
