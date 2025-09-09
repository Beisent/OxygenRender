#!/bin/bash

# 设置目标目录和下载链接
TARGET_DIR="resources/objects/backpack"
ZIP_URL="https://learnopengl-cn.github.io/data/backpack.zip"
ZIP_FILE="backpack.zip"

# 检查目标目录是否存在
if [ -d "$TARGET_DIR" ]; then
    echo "Directory '$TARGET_DIR' already exists. No need to download."
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
    if ! unzip "$ZIP_FILE" -d "$TARGET_DIR"; then
        echo "Error: Failed to extract zip file."
        rm "$ZIP_FILE"
        exit 1
    fi

    # 删除临时 zip 文件
    rm "$ZIP_FILE"

    echo "Download and extraction completed."
fi

# 暂停
read -p "Press Enter to continue... " dummy