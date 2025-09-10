#!/bin/bash

# 设置目标目录和下载链接
TARGET_DIR="resources/objects/backpack"
ZIP_URL="https://learnopengl-cn.github.io/data/backpack.zip"
ZIP_FILE="backpack.zip"

# 检查目标目录是否存在
if [ -d "$TARGET_DIR" ]; then
    echo "Directory '$TARGET_DIR' already exists. Skipping backpack download."
else
    echo "Directory '$TARGET_DIR' does not exist. Creating directory and downloading files..."

    # 创建目标目录
    mkdir -p "$TARGET_DIR"

    # 下载 zip 文件
    if command -v curl &> /dev/null; then
        curl -L -o "$ZIP_FILE" "$ZIP_URL"
    elif command -v wget &> /dev/null; then
        wget -O "$ZIP_FILE" "$ZIP_URL"
    else
        echo "Error: Neither curl nor wget is installed. Cannot download file."
        exit 1
    fi

    # 检查下载是否成功
    if [ ! -f "$ZIP_FILE" ]; then
        echo "Error: Failed to download file."
        exit 1
    fi

    # 解压文件到目标目录
    if ! unzip "$ZIP_FILE" -d "$TARGET_DIR" > /dev/null 2>&1; then
        echo "Error: Failed to extract zip file."
        rm "$ZIP_FILE"
        exit 1
    fi

    # 删除临时 zip 文件
    rm "$ZIP_FILE"

    echo "Backpack download and extraction completed."
fi

# ============ Part 2: Download images to resources/ ============

IMAGE_DIR="resources"
mkdir -p "$IMAGE_DIR"

# 下载 wall.jpg 并保存为 container.jpg
WALL_URL="https://learnopengl-cn.github.io/img/01/06/wall.jpg"
CONTAINER_FILE="$IMAGE_DIR/container.jpg"

if [ -f "$CONTAINER_FILE" ]; then
    echo "'$CONTAINER_FILE' already exists. Skipping download."
else
    echo "Downloading container.jpg..."
    if command -v curl &> /dev/null; then
        curl -L -o "$CONTAINER_FILE" "$WALL_URL"
    elif command -v wget &> /dev/null; then
        wget -O "$CONTAINER_FILE" "$WALL_URL"
    else
        echo "Error: Neither curl nor wget is installed. Cannot download image."
        exit 1
    fi

    if [ ! -f "$CONTAINER_FILE" ]; then
        echo "Error: Failed to download container.jpg"
        exit 1
    fi
fi

# 下载 awesomeface.png
AWESOME_URL="https://learnopengl-cn.github.io/img/01/06/awesomeface.png"
AWESOME_FILE="$IMAGE_DIR/awesomeface.png"

if [ -f "$AWESOME_FILE" ]; then
    echo "'$AWESOME_FILE' already exists. Skipping download."
else
    echo "Downloading awesomeface.png..."
    if command -v curl &> /dev/null; then
        curl -L -o "$AWESOME_FILE" "$AWESOME_URL"
    elif command -v wget &> /dev/null; then
        wget -O "$AWESOME_FILE" "$AWESOME_URL"
    else
        echo "Error: Neither curl nor wget is installed. Cannot download image."
        exit 1
    fi

    if [ ! -f "$AWESOME_FILE" ]; then
        echo "Error: Failed to download awesomeface.png"
        exit 1
    fi
fi

echo "All downloads completed."

# 暂停
read -p "Press Enter to continue... " dummy