
# OxygenRender 🌬️

> Cross-Platform 3D Rendering Engine in Modern C++

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![C++](https://img.shields.io/badge/C++-17+-f34b7d.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)

OxygenRender 是一个基于 OpenGL 3.3+ 的轻量级 3D 图形渲染引擎，使用现代 C++（C++17）编写。该项目旨在为图形编程学习、原型开发和小型游戏项目提供简洁、模块化且易于扩展的基础架构。

设计灵感源自 [LearnOpenGL](https://learnopengl.com)，并在其基础上进行了良好的封装与功能拓展，强调清晰的代码结构与开发者体验。

---

## 🌟 核心特性

- ✅ **跨平台支持**：Windows / Linux，使用 CMake 统一构建
- 🔧 **多渲染后端架构**：当前基于 OpenGL 3.3+，预留 Vulkan / DirectX 12 接口
- 🧱 **模块化设计**：组件解耦，便于扩展与复用
- 🎮 **3D 模型渲染**：通过 Assimp 支持 `.obj`, `.fbx`, `.gltf` 等多种格式
- 🖼️ **纹理系统**：支持多种纹理类型、Mipmap、压缩纹理与采样参数配置
- 🧠 **着色器管理**：自动加载编译 GLSL 着色器，支持 uniform 快速设置
- 🔋 **缓冲区抽象**：封装 VBO、IBO 与 VAO，支持动态/静态数据更新
- 📷 **相机系统**：支持 3D 摄像机（Fly/Orbit）与 2D 正交视图
- ⌨️ **事件系统**：基于 GLFW 实现键盘、鼠标、窗口事件监听与分发
- 🎨 **2D 绘图 API**：支持矩形、圆形、多边形、一元函数图像、箭头、网格、坐标轴及纹理映射
- 🎨 **3D 绘图 API**：支持Box、球体，平面，二元函数图像。
- 📦 **渲染状态管理**：统一管理深度测试、混合、面剔除等 OpenGL 状态

---

## 🗂 项目结构

```bash
OxygenRender/
├── include/OxygenRender/     # 公共头文件
├── src/                      # 源码
│   ├── core/                 # 核心类（事件、日志、应用）
│   ├── graphics/             # 图形模块（渲染器、着色器、模型等）
│   ├── utils/                # 工具类（时间等）
│   └── vendor/               # 第三方库
├── tests/                    # 示例程序
├── shaders/                  # GLSL 着色器脚本
├── resources/                # 模型、纹理等资源
├── bin/
├── CMakeLists.txt
└── build.bat / build.sh      # 跨平台构建脚本
```

---

## 🧰 核心组件

| 组件           | 功能说明                                   |
| -------------- | ------------------------------------------ |
| `Renderer`     | 渲染后端抽象层，提供统一绘制接口与状态管理 |
| `Mesh / Model` | 单网格与复杂模型封装，支持多材质与变换     |
| `Texture2D`    | 2D 纹理封装，支持多种内部格式与过滤模式    |
| `Shader`       | 着色器程序加载、编译与 uniform 设置        |
| `Buffer`       | 顶点/索引缓冲区抽象                        |
| `VertexArray`  | VAO 封装，管理顶点属性布局                 |
| `Window`       | GLFW 窗口与 OpenGL 上下文管理              |
| `EventSystem`  | 事件总线机制，支持订阅/发布模式            |
| `Graphics2D`   | 高级 2D 绘图接口（形状、纹理、坐标系）     |
| `Graphics3D`   | 基础 3D 形状绘制（立方体、球体等）         |
| `Camera`       | 支持透视/正交投影，视角控制与变换          |

---

## 🛠️ 构建依赖

| 依赖库      | 用途                         |
| ----------- | ---------------------------- |
| OpenGL 3.3+ | 图形 API 后端                |
| GLFW 3.3+   | 窗口、上下文与输入管理       |
| GLAD        | OpenGL 函数指针加载          |
| GLM 1.0+    | 数学库（向量、矩阵、四元数） |
| Assimp 5.0+ | 3D 模型导入                  |
| stb_image   | 图像加载（头文件库，零依赖） |

> 所有第三方库均通过 `git submodule` 管理，无需手动安装。

---

## 🚀 快速构建（跨平台）

OxygenRender 使用 **CMake** 实现跨平台编译，支持 Windows、Linux 和 macOS。

### 1. 克隆项目（含子模块）

```bash
git clone --recursive https://github.com/Beisent/OxygenRender.git
cd OxygenRender

# Windows
.\build.bat
# Linux
./build.sh


mkdir build
cd build
cmake ..
cmake --build . --config Release
```

构建完成后，示例程序位于 `bin` 目录下。

---

## 📚 示例程序

| 示例            | 说明                                      |
| --------------- | ----------------------------------------- |
| `Backpack`      | 加载并渲染 3D 背包模型，展示材质与光照    |
| `LightBox`      | 动态光源演示，支持 Phong 光照模型         |
| `Graph2D`       | 2D 图形绘制：坐标系、网格、形状、纹理映射 |
| `Texture2DTest` | 纹理加载与应用测试，支持重复/边缘处理     |

---

## 📄 许可证

本项目采用 [MIT 许可证](LICENSE)，欢迎自由使用、修改与分发。

---

## 🚧 未来规划

- ✅ 支持 Vulkan / DirectX 12 后端（抽象渲染接口）
- ✅ 实现 PBR 材质与基于物理的渲染（PBR）
- ✅ 添加阴影映射（Shadow Mapping）
- ✅ 支持 2D 文本渲染（集成 FreeType）
- ✅ 提供 Editor 原型（ImGui 集成）

---

> ⚠️ **注意**：项目仍在积极开发中，API 可能会发生变更。欢迎提交 Issue 或 PR！

---

💡 **灵感来源**：[LearnOpenGL](https://learnopengl.com)  
📬 **反馈建议**：欢迎提交 Issue 或 Pull Request！

```
