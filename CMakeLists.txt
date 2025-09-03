cmake_minimum_required(VERSION 3.16)
project(OxygenRender LANGUAGES C CXX)

# 1. 编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 添加 Assimp 子目录
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER OFF CACHE BOOL "" FORCE)

# 2. 包含路径
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/GLAD
    ${CMAKE_SOURCE_DIR}/include/glm
    ${CMAKE_SOURCE_DIR}/include/stb
    ${CMAKE_SOURCE_DIR}/tests
)

# 3. 添加GLM和GLFW子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/src/vendor/glm)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/vendor/glfw)

# 4. 源文件收集
file(GLOB_RECURSE OXYGEN_SRC
    ${CMAKE_SOURCE_DIR}/src/core/*.cpp
    ${CMAKE_SOURCE_DIR}/src/graphics/*.cpp
    ${CMAKE_SOURCE_DIR}/src/scene/*.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/*.cpp
    ${CMAKE_SOURCE_DIR}/src/vendor/GLAD/*.c
)

# 5. 添加静态库 OxygenRender
add_subdirectory(${CMAKE_SOURCE_DIR}/src/vendor/assimp)

add_library(OxygenRender STATIC ${OXYGEN_SRC})
target_link_libraries(OxygenRender assimp glm::glm glfw)

# 6. 链接外部库 
target_link_libraries(OxygenRender glfw)

# 根据平台链接OpenGL
if(WIN32)
    target_link_libraries(OxygenRender opengl32)
elseif(APPLE)
    target_link_libraries(OxygenRender "-framework OpenGL")
else()
    target_link_libraries(OxygenRender GL)
endif()

# 7. 测试程序
file(GLOB TEST_SRC ${CMAKE_SOURCE_DIR}/tests/*.cpp)
add_executable(TestApp ${TEST_SRC})
target_link_libraries(TestApp OxygenRender)