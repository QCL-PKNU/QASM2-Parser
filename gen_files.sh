#!/bin/bash

DIRECTORY="."
OUTPUT_FILE="file_contents.txt"

> "$OUTPUT_FILE" 

find "$DIRECTORY" -type f -name "*" ! -path "./build/*" ! -path "./thirdparty/*" ! -path "*/__pycache__/*" ! -path "*/.vscode/*" ! -name "build" ! -name "qplayer" ! -name "$OUTPUT_FILE" | while IFS= read -r FILE; do
    if [[ ! "$FILE" =~ \.(qasm|inc|jar|pyc)$ ]]; then
        echo "// - ${FILE##*/}" >> "$OUTPUT_FILE"
        cat "$FILE" >> "$OUTPUT_FILE"
        echo "" >> "$OUTPUT_FILE" 
    fi
done

echo "File contents have been written to $OUTPUT_FILE"
