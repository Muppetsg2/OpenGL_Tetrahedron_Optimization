#pragma once

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGui
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#define IMGUI_ENABLE_FREETYPE
#include <imgui.h>
#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>

// SPDLOG
#include <spdlog/spdlog.h>

// STB_IMAGE
#include <stb_image.h>

// STD
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>