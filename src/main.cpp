#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h>  // Initialize with gladLoadGL()
#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>

// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

// OpenGL Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Mine
#include <Shader.h>
#include <Texture.h>

GLFWwindow* window = nullptr;
const char* WINDOW_NAME = "Test";
constexpr int32_t WINDOW_WIDTH = 1280;
constexpr int32_t WINDOW_HEIGHT = 720;

// Change these to lower GL version like 4.5 if GL 4.6 can't be initialized on your machine
const     char* glsl_version = "#version 450";
constexpr int32_t GL_VERSION_MAJOR = 4;
constexpr int32_t GL_VERSION_MINOR = 5;

glm::vec4 clear_color = glm::vec4(.55f, .55f, .55f, 1.f);

#pragma region TetrahedronDeclaration

std::vector<glm::mat4> modelTransforms = std::vector<glm::mat4>();

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

Vertex vertices[] {
    { .Position = glm::vec3(0.0f, -0.333333f, 0.57735f),    .TexCoords = glm::vec2(0.5f, 1.0f), .Normal = glm::vec3(0.0f, -1.0f, 0.0f),                 .Tangent = glm::vec3(-1.0f, 0.0f, 0.0f),        .Bitangent = glm::vec3(0.0f, 0.0f, 1.0f)                    },
    { .Position = glm::vec3(0.5f, -0.333333f, -0.288675f),  .TexCoords = glm::vec2(0.0f, 0.0f), .Normal = glm::vec3(0.0f, -1.0f, 0.0f),                 .Tangent = glm::vec3(-1.0f, 0.0f, 0.0f),        .Bitangent = glm::vec3(0.0f, 0.0f, 1.0f)                    },
    { .Position = glm::vec3(-0.5f, -0.333333f, -0.288675f), .TexCoords = glm::vec2(1.0f, 0.0f), .Normal = glm::vec3(0.0f, -1.0f, 0.0f),                 .Tangent = glm::vec3(-1.0f, 0.0f, 0.0f),        .Bitangent = glm::vec3(0.0f, 0.0f, 1.0f)                    },
    { .Position = glm::vec3(0.0f, -0.333333f, 0.57735f),    .TexCoords = glm::vec2(0.0f, 1.0f), .Normal = glm::vec3(0.344124f, 0.917663f, 0.19868f),    .Tangent = glm::vec3(0.5f, 0.0f, -0.866025f),   .Bitangent = glm::vec3(0.344124f, -0.917663f, 0.19868f)     },
    { .Position = glm::vec3(0.5f, -0.333333f, -0.288675f),  .TexCoords = glm::vec2(1.0f, 1.0f), .Normal = glm::vec3(0.344124f, 0.917663f, 0.19868f),    .Tangent = glm::vec3(0.5f, 0.0f, -0.866025f),   .Bitangent = glm::vec3(0.344124f, -0.917663f, 0.19868f)     },
    { .Position = glm::vec3(0.0f, 0.333333f, 0.0f),         .TexCoords = glm::vec2(0.5f, 0.0f), .Normal = glm::vec3(0.344124f, 0.917663f, 0.19868f),    .Tangent = glm::vec3(0.5f, 0.0f, -0.866025f),   .Bitangent = glm::vec3(0.344124f, -0.917663f, 0.19868f)     },
    { .Position = glm::vec3(0.5f, -0.333333f, -0.288675f),  .TexCoords = glm::vec2(0.0f, 1.0f), .Normal = glm::vec3(-0.0f, 0.917663f, -0.39736f),       .Tangent = glm::vec3(-1.0f, 0.0f, 0.0f),        .Bitangent = glm::vec3(-0.0f, -0.917663f, -0.39736f)        },
    { .Position = glm::vec3(-0.5f, -0.333333f, -0.288675f), .TexCoords = glm::vec2(1.0f, 1.0f), .Normal = glm::vec3(-0.0f, 0.917663f, -0.39736f),       .Tangent = glm::vec3(-1.0f, 0.0f, 0.0f),        .Bitangent = glm::vec3(-0.0f, -0.917663f, -0.39736f)        },
    { .Position = glm::vec3(0.0f, 0.333333f, 0.0f),         .TexCoords = glm::vec2(0.5f, 0.0f), .Normal = glm::vec3(-0.0f, 0.917663f, -0.39736f),       .Tangent = glm::vec3(-1.0f, 0.0f, 0.0f),        .Bitangent = glm::vec3(-0.0f, -0.917663f, -0.39736f)        },
    { .Position = glm::vec3(-0.5f, -0.333333f, -0.288675f), .TexCoords = glm::vec2(0.0f, 1.0f), .Normal = glm::vec3(-0.344124f, 0.917663f, 0.19868f),   .Tangent = glm::vec3(0.5f, 0.0f, 0.866025f),    .Bitangent = glm::vec3(-0.344124f, -0.917663f, 0.19868f)    },
    { .Position = glm::vec3(0.0f, -0.333333f, 0.57735f),    .TexCoords = glm::vec2(1.0f, 1.0f), .Normal = glm::vec3(-0.344124f, 0.917663f, 0.19868f),   .Tangent = glm::vec3(0.5f, 0.0f, 0.866025f),    .Bitangent = glm::vec3(-0.344124f, -0.917663f, 0.19868f)    },
    { .Position = glm::vec3(0.0f, 0.333333f, 0.0f),         .TexCoords = glm::vec2(0.5f, 0.0f), .Normal = glm::vec3(-0.344124f, 0.917663f, 0.19868f),   .Tangent = glm::vec3(0.5f, 0.0f, 0.866025f),    .Bitangent = glm::vec3(-0.344124f, -0.917663f, 0.19868f)    }
};

GLuint indices[] = {
    0, 2, 1,
    3, 4, 5,
    6, 7, 8,
    9, 10, 11
};

GLuint VBO, VAO, EBO, instanceVBO;
Shader ourShader;
Texture texture;
glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
float rotationY = 0.f;
float rotationX = 0.f;
int rLevel = 1;
float cameraRadius = 3.f;

#pragma endregion

#pragma region OpenGLCallbacks

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void GLAPIENTRY ErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return; // Chce ignorowa� notyfikacje

    std::string severityS = "";
    if (severity == GL_DEBUG_SEVERITY_HIGH) severityS = "HIGHT";
    else if (severity == GL_DEBUG_SEVERITY_MEDIUM) severityS = "MEDIUM";
    else if (severity == GL_DEBUG_SEVERITY_LOW) severityS = "LOW";
    else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) severityS = "NOTIFICATION";

    if (type == GL_DEBUG_TYPE_ERROR) {
        spdlog::error("GL CALLBACK: type = ERROR, severity = {0}, message = {1}\n", severityS, message);
    }
    else if (type == GL_DEBUG_TYPE_MARKER) {
        spdlog::info("GL CALLBACK: type = MARKER, severity = {0}, message = {1}\n", severityS, message);
    }
    else {
        std::string typeS = "";
        if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) typeS = "DEPRACTED BEHAVIOUR";
        else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) typeS = "UNDEFINED BEHAVIOUR";
        else if (type == GL_DEBUG_TYPE_PORTABILITY) typeS = "PORTABILITY";
        else if (type == GL_DEBUG_TYPE_PERFORMANCE) typeS = "PERFORMANCE";
        else if (type == GL_DEBUG_TYPE_PUSH_GROUP) typeS = "PUSH GROUP";
        else if (type == GL_DEBUG_TYPE_POP_GROUP) typeS = "POP GROUP";
        else if (type == GL_DEBUG_TYPE_OTHER) typeS = "OTHER";
        spdlog::warn("GL CALLBACK: type = {0}, severity = {1}, message = {2}\n", typeS, severityS, message);
    }
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#pragma endregion

#pragma region FunctionsDeclaration

bool init();

void setupObjects();
void tetrahedron(int level, int levelBefore = 0);

void input();
void update();

void init_imgui();
void imgui_begin();
void imgui_render();
void imgui_end();

void end_frame();
void cleanup();

#pragma endregion

int main(int, char**)
{
    if (!init())
    {
        spdlog::error("Failed to initialize project!");
        return EXIT_FAILURE;
    }
    spdlog::info("Initialized project.");

    init_imgui();
    spdlog::info("Initialized ImGui.");

    setupObjects();

    GLfloat deltaTime = 0.0f; // Czas pomi�dzy obecn� i poprzedni� klatk�  
    GLfloat lastFrame = 0.0f; // Czas ostatniej ramki

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Update game time value
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        input();

        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        update();

        // IMGUI
        imgui_begin();
        imgui_render();
        imgui_end();

        end_frame();
    }

    cleanup();

    return 0;
}

bool init()
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        spdlog::error("Failed to initalize GLFW!");
        return false;
    }

    // GL 4.5 + GLSL 450
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL)
    {
        spdlog::error("Failed to create GLFW Window!");
        return false;
    }
    spdlog::info("Successfully created GLFW Window!");

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1); // Enable VSync - fixes FPS at the refresh rate of your screen
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (err)
    {
        spdlog::error("Failed to initialize OpenGL loader!");
        return false;
    }
    spdlog::info("Successfully initialized OpenGL loader!");

    // Debugging
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(ErrorMessageCallback, 0);

    // Depth Test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Face Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    return true;
}

void setupObjects()
{
    texture = Texture("./res/textures/stone.jpg", GL_RGB, GL_RGB, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);
    ourShader = Shader("./res/shaders/instance.vert", "./res/shaders/instance.frag");

    texture.use(0);
    ourShader.use();
    ourShader.setInt("tex", 0);
    ourShader.setVec3("c", color);
    ourShader.setMatrix4x4("projection", glm::perspective(glm::radians(45.0f), WINDOW_HEIGHT != 0 ? (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT : 0.f, 0.1f, 1000.0f));
    ourShader.setMatrix4x4("view", glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -cameraRadius)));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, modelTransforms.size() * sizeof(glm::mat4), modelTransforms.data(), GL_STATIC_DRAW);

    // Transform Matrix
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(8);

    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);
    glVertexAttribDivisor(8, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    tetrahedron(rLevel, 0.f);
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void tetrahedron(int level, int levelBefore)
{
    if (level == 0) {
        modelTransforms.clear();
        return;
    }
    else if (level == 1) {
        modelTransforms.clear();
        modelTransforms.push_back(glm::mat4(1.f));
    }
    else {
        int toAdd = level - levelBefore;
        std::vector<glm::mat4> temps = std::vector<glm::mat4>();
        temps.swap(modelTransforms);
        if (toAdd > 0) {
            for (int i = toAdd; i > 0; --i)
            {
                size_t size = temps.size();
                for (int j = 0; j < size; ++j) {
                    temps.push_back(glm::translate(glm::scale(temps[j], glm::vec3(.5f)), glm::vec3(-.5f, -0.333333f, -0.288675f)));
                    temps.push_back(glm::translate(glm::scale(temps[j], glm::vec3(.5f)), glm::vec3(0.f, -0.333333f, 0.57735f)));
                    temps.push_back(glm::translate(glm::scale(temps[j], glm::vec3(.5f)), glm::vec3(.5f, -0.333333f, -0.288675f)));
                    temps.push_back(glm::translate(glm::scale(temps[j], glm::vec3(.5f)), glm::vec3(0.f, 0.333333f, 0.f)));
                }

                temps.erase(temps.begin(), temps.begin() + size);
            }
        }
        else if (toAdd < 0) {
            for (int i = -toAdd; i > 0; --i)
            {
                size_t size = temps.size();
                for (int j = 0; j < size; j += 4) {
                    temps.push_back(glm::scale(glm::translate(temps[j], glm::vec3(.5f, 0.333333f, 0.288675f)), glm::vec3(2.f)));
                }

                temps.erase(temps.begin(), temps.begin() + size);
            }
        }
        else {
            modelTransforms.swap(temps);
            temps.clear();
            return;
        }

        modelTransforms.swap(temps);
        temps.clear();
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, modelTransforms.size() * sizeof(glm::mat4), modelTransforms.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    /* RECURSION
    if (level == 0)
    {
        return;
    }

    if (level == 1)
    {
        sh.setMatrix4x4("model", model);
        sh.use();

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glm::mat4 trans = glm::scale(model, glm::vec3(.5f));
        tetrahedron(sh, glm::translate(trans, glm::vec3(-.5f, -0.333333f, -0.288675f)), level - 1);     // Left, Down, Backward
        tetrahedron(sh, glm::translate(trans, glm::vec3(0.f, -0.333333f, 0.57735f)), level - 1);        // Center, Down, Forward
        tetrahedron(sh, glm::translate(trans, glm::vec3(.5f, -0.333333f, -0.288675f)), level - 1);      // Right, Down, Backward
        tetrahedron(sh, glm::translate(trans, glm::vec3(0.f, 0.333333f, 0.f)), level - 1);              // Center, Up, Backward
    }
    */
}

void input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void update()
{
    /*
    glm::mat4 model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.f, 0.f, 0.f));
    */

    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0, modelTransforms.size());
    glBindVertexArray(0);
}

void init_imgui()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
}

void imgui_begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_render()
{
    static int counter = 0;

    ImGui::Begin("Sierpinski Tetrahedron");                         // Create a window called "Sierpinski Tetrahedron" and append into it.

    ImGui::Text("Change this Tetrahedron as you wish.");            // Display some text

    bool dirtyView = false;
    float rx = rotationX;
    ImGui::SliderFloat("Rotation X", &rx, 0.f, 360.f);       // Slider for rotation in X axes from 0 to 360

    if (rx != rotationX) {
        rotationX = rx;
        dirtyView = true;
    }

    /*
    float ry = rotationY;
    ImGui::SliderFloat("Rotation Y", &ry, 0.f, 360.f);       // Slider for rotation in Y axes from 0 to 360

    if (ry != rotationY) {
        rotationY = ry;
        dirtyView = true;
    }
    */

    if (dirtyView) {
        // Only X Rotation
        ourShader.setMatrix4x4("view", glm::rotate(
                                            glm::translate(
                                                glm::mat4(1.f), 
                                                glm::vec3(-sinf(glm::radians(rotationX)) * cameraRadius, 0.0f, -cosf(glm::radians(rotationX)) * cameraRadius)
                                            ),
                                            0.f,//glm::radians(360.f - rotationX),
                                            glm::vec3(0.f, 1.f, 0.f)
                                        )
                                );
    }

    int rl = rLevel;
    ImGui::SliderInt("Recursion", &rl, 0, 15);                  // Slider for recursion level from 0 to 10

    if (rl != rLevel) {
        glBindVertexArray(VAO);
        tetrahedron(rl, rLevel);
        glBindVertexArray(0);
        rLevel = rl;
    }

    glm::vec3 c = color;
    ImGui::ColorEdit3("Shape Color", (float*)&c);               // Edit 3 floats representing a shape color

    if (c != color) {
        color = c;
        ourShader.use();
        ourShader.setVec3("c", color);
    }

    glm::vec4 cc = clear_color;
    ImGui::ColorEdit3("Background Color", (float*)&cc);    // Edit 3 floats representing a background color

    if (cc != clear_color) {
        clear_color = cc;
    }

    ImGui::Separator();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void imgui_end()
{
    ImGui::Render();
    glfwMakeContextCurrent(window);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void end_frame()
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();
    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
}

void cleanup()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}