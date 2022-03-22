cmake_minimum_required (VERSION 3.8)

project (Render)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

# Set output directory for executable files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY})

# find packages
find_package(glbinding REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Render
file(GLOB IMGUI_SRC CONFIGURE_DEPENDS "external/imgui/src/*.cpp")
set(Render_SRC "src/shaderSystem/shaders.cpp" "src/render/Render.cpp" "src/render/window.cpp")
set(RenderApp app)
add_library(${RenderApp} OBJECT ${Render_SRC} ${IMGUI_SRC})
target_link_libraries(${RenderApp} PUBLIC glfw glbinding::glbinding glm::glm)
target_include_directories(${RenderApp} PUBLIC external/imgui/include include)

# app
add_executable(${PROJECT_NAME} "app/main.cpp")

# link packages
target_link_libraries(${PROJECT_NAME} PUBLIC ${RenderApp})
