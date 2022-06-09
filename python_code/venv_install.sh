#!/usr/bin/env sh

echo "Removing .venv"
rm -r .venv
echo "Making .venv"
py -m venv .venv
echo "Activating venv"
source .venv/Scripts/activate
where python
echo "Installing required libraries"
pip install -r requirements.txt