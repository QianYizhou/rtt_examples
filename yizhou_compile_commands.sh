#!/bin/bash
set -e

rm -rf compile_commands.json

# for dir in $(find ./build -type d -name "build"); do
for f in $(find ./rtt-exercises/ -name "compile_commands.json"); do
    echo "Found file: $f"
    cat $f >> compile_commands.json
done

sed -i 's/\]\[/,/g' compile_commands.json -i 