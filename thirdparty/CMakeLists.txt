# CPM - package manager
include(CPM)

# OpenGL
find_package(OpenGL REQUIRED)

# glad https://glad.dav1d.de/
set(GLAD_DIR ${CMAKE_CURRENT_LIST_DIR}/glad)
set(glad_SOURCE_DIR ${GLAD_DIR}/include CACHE INTERNAL "")
add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)

# stb_image https://github.com/nothings/stb
set(STB_IMAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/stb_image)
set(stb_image_SOURCE_DIR ${STB_IMAGE_DIR} CACHE INTERNAL "")
add_library(stb_image STATIC ${STB_IMAGE_DIR}/stb_image.cpp)
target_include_directories(stb_image PRIVATE ${STB_IMAGE_DIR})

# glfw https://github.com/glfw/glfw
CPMAddPackage("gh:glfw/glfw#3.4")

# glm https://github.com/g-truc/glm
CPMAddPackage("gh:g-truc/glm#1.0.1")

# Freetype https://github.com/freetype/freetype
CPMAddPackage("gh:freetype/freetype#VER-2-13-3")

# ImGui https://github.com/ocornut/imgui
CPMAddPackage("gh:ocornut/imgui@1.91.9b-docking")

# spdlog https://github.com/gabime/spdlog
CPMAddPackage("gh:gabime/spdlog@1.15.2")

set(imgui_SOURCE_DIR ${imgui_SOURCE_DIR} CACHE INTERNAL "")
add_library(imgui STATIC ${imgui_SOURCE_DIR}/imgui.cpp
					     #${imgui_SOURCE_DIR}/imgui_demo.cpp
					     ${imgui_SOURCE_DIR}/imgui_draw.cpp
						 ${imgui_SOURCE_DIR}/imgui_tables.cpp
					     ${imgui_SOURCE_DIR}/imgui_widgets.cpp)

target_compile_definitions(imgui PUBLIC IMGUI_ENABLE_FREETYPE)
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)

set(imgui_FREETYPE_SOURCE ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp CACHE INTERNAL "")
set(imgui_BACKEND_SOURCE ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp CACHE INTERNAL "")
						 
set_target_properties(glad
                      stb_image
                      glfw
                      glm
                      freetype
                      imgui
                      spdlog PROPERTIES FOLDER "thirdparty")

if (TARGET zlibstatic)
    set_target_properties(zlibstatic PROPERTIES FOLDER "thirdparty")
endif()