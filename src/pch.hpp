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
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

// SPDLOG
#include <spdlog/spdlog.h>

// STB_IMAGE
#include <stb_image.h>

// STD
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <corecrt_math_defines.h>

// PLATFORM DEPENDANT
#if defined(_WIN32)
#define NOGDI
#define NOATOM
#define NOMINMAX
#include <windows.h>
#undef near
#undef far
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#endif

// DEFINES
#define M_SQRT3    1.73205080756887729353   // sqrt(3)