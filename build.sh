#!/bin/bash

set -e

# ============ Part 1: Download backpack ============
TARGET_DIR="resources/objects/backpack"
ZIP_URL="https://learnopengl-cn.github.io/data/backpack.zip"
ZIP_FILE="backpack.zip"

if [ -d "$TARGET_DIR" ]; then
    echo "Directory '$TARGET_DIR' already exists. Skipping backpack download."
else
    echo "Directory '$TARGET_DIR' does not exist. Creating directory and downloading files..."
    mkdir -p "$TARGET_DIR"

    if command -v curl &> /dev/null; then
        curl -L -o "$ZIP_FILE" "$ZIP_URL"
    elif command -v wget &> /dev/null; then
        wget -O "$ZIP_FILE" "$ZIP_URL"
    else
        echo "Error: Neither curl nor wget is installed. Cannot download file."
        exit 1
    fi

    if [ ! -f "$ZIP_FILE" ]; then
        echo "Error: Failed to download file."
        exit 1
    fi

    unzip -q "$ZIP_FILE" -d "$TARGET_DIR"
    rm "$ZIP_FILE"

    echo "Backpack download and extraction completed."
fi

# ============ Part 2: Download images to resources/ ============
IMAGE_DIR="resources"
mkdir -p "$IMAGE_DIR"

download_file() {
    local url="$1"
    local output="$2"

    if [ -f "$output" ]; then
        echo "'$output' already exists. Skipping download."
    else
        echo "Downloading '$output'..."
        if command -v curl &> /dev/null; then
            curl -L -o "$output" "$url"
        elif command -v wget &> /dev/null; then
            wget -O "$output" "$url"
        else
            echo "Error: Neither curl nor wget is installed. Cannot download file."
            exit 1
        fi

        if [ ! -f "$output" ]; then
            echo "Error: Failed to download '$output'"
            exit 1
        fi
    fi
}

# container.jpg
download_file "https://learnopengl-cn.github.io/img/01/06/wall.jpg" "$IMAGE_DIR/container.jpg"

# awesomeface.png
download_file "https://learnopengl-cn.github.io/img/01/06/awesomeface.png" "$IMAGE_DIR/awesomeface.png"

# ============ Part 3: Download skybox images ============
SKYBOX_DIR="$IMAGE_DIR/skybox"
mkdir -p "$SKYBOX_DIR"

declare -A SKYBOX_IMAGES=(
    ["up"]="https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_up.png"
    ["dn"]="https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_dn.png"
    ["lf"]="https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_lf.png"
    ["rt"]="https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_rt.png"
    ["ft"]="https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_ft.png"
    ["bk"]="https://github.com/pengfeiw/skybox-image/raw/main/elyvisions/arch3_bk.png"
)

for face in "${!SKYBOX_IMAGES[@]}"; do
    download_file "${SKYBOX_IMAGES[$face]}" "$SKYBOX_DIR/arch3_$face.png"
done

echo "All downloads completed."

read -p "Press Enter to continue... "
