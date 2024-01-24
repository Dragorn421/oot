#!/bin/sh
set -e
VENV=$1
test -d $VENV || python3 -m venv $VENV
PYTHON=$VENV/bin/python
$PYTHON -m pip install -U pip
$PYTHON -m pip install -U -r requirements.txt
