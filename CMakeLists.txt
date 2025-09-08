
cmake_minimum_required(VERSION 3.16)
project(OxygenRender LANGUAGES C CXX)

# 编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" FORCE)
endif()

# Windows: auto-export all symbols for shared library to avoid needing __declspec(dllexport)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# 添加 Assimp 子目录
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER OFF CACHE BOOL "" FORCE)

# 包含路径
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/GLAD
    ${CMAKE_SOURCE_DIR}/include/glm
    ${CMAKE_SOURCE_DIR}/include/stb
    ${CMAKE_SOURCE_DIR}/tests
)

# 添加GLM和GLFW子目录
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

add_subdirectory(${CMAKE_SOURCE_DIR}/src/vendor/assimp)
set_target_properties(assimp PROPERTIES 
    POSITION_INDEPENDENT_CODE ON
)
add_library(OxygenRender SHARED ${OXYGEN_SRC})

# Prefer modern target include dirs; keep global includes for existing code, but ensure PUBLIC install interface
target_include_directories(OxygenRender
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(OxygenRender
    PUBLIC glm::glm
    PUBLIC glfw
    PUBLIC   assimp
)

# 根据平台链接OpenGL
if(WIN32)
    target_link_libraries(OxygenRender PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(OxygenRender PRIVATE "-framework OpenGL")
else()
    target_link_libraries(OxygenRender PRIVATE GL)
endif()

file(GLOB TEST_SRC ${CMAKE_SOURCE_DIR}/tests/*.cpp)
add_executable(TestApp ${TEST_SRC})
target_link_libraries(TestApp PRIVATE OxygenRender)

if(MINGW)
    target_link_options(OxygenRender PRIVATE -static-libgcc -static-libstdc++)
    target_link_options(TestApp PRIVATE -static-libgcc -static-libstdc++)
    target_link_options(OxygenRender PRIVATE -Wl,--exclude-symbols,_Unwind_Resume)
endif()


include(GNUInstallDirs)

install(TARGETS OxygenRender
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})