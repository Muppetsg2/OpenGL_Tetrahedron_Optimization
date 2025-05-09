# Precompiled header
set(PCH_FILE ${CMAKE_CURRENT_SOURCE_DIR}/pch.hpp)

# Add source files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.c *.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.h *.hpp)

# Search for the assets files
file(GLOB_RECURSE ASSETS_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/res/*.*)

set_source_files_properties(${ASSETS_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("Resources"    FILES ${ASSETS_FILES})

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${imgui_FREETYPE_SOURCE} ${imgui_BACKEND_SOURCE} ${ASSETS_FILES})

# Precompiled headers
target_precompile_headers(${PROJECT_NAME} PUBLIC ${PCH_FILE})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
												  ${glad_SOURCE_DIR}
												  ${stb_image_SOURCE_DIR}
												  ${imgui_SOURCE_DIR})

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC NOMINMAX)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:preprocessor")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY_SUFFIX="")
target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG="$<IF:$<CONFIG:Debug>,1,0>")

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __APPLE__)
elseif(UNIX)
    target_compile_definitions(${PROJECT_NAME} PRIVATE __linux__)
endif()

# Optimizations
target_compile_options(${PROJECT_NAME} PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Debug>:/RTC1>
    >
    $<$<CXX_COMPILER_ID:GNU>:
        $<$<CONFIG:Release>:-O2>
    >
    $<$<CXX_COMPILER_ID:Clang>:
        $<$<CONFIG:Release>:-O2>
    >
)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} glad
									  stb_image
									  glfw
									  freetype
									  imgui
									  spdlog
									  glm::glm)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC "-framework CoreFoundation")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
				   COMMAND ${CMAKE_COMMAND} -E create_symlink 
				   ${CMAKE_SOURCE_DIR}/res 
				   ${OUTPUT_DIR}/res)