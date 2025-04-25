// Tetrahedron Optimization v3.0
// Marceli Antosik (Muppetsg2)

extern "C" {
    _declspec(dllexport) unsigned long NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Mine
#include <Shader.hpp>
#include <Texture.hpp>

GLFWwindow* window = nullptr;
const char* WINDOW_NAME = "Tetrahedron";
constexpr int32_t WINDOW_WIDTH = 1280;
constexpr int32_t WINDOW_HEIGHT = 720;

std::string exeDirPath;

// Change these to lower GL version like 4.5 if GL 4.6 can't be initialized on your machine
const     char* glsl_version = "#version 450";
constexpr int32_t GL_VERSION_MAJOR = 4;
constexpr int32_t GL_VERSION_MINOR = 5;

glm::vec4 clear_color = glm::vec4(.55f, .55f, .55f, 1.f);

size_t ONE_DRAW_TRANSFORMS = UINT16_MAX;

#pragma region TetrahedronDeclaration

// Instance data
struct InstanceData {
    glm::vec3 pos;  // Position
    float scale;    // Scale factor
};

std::vector<InstanceData> instances = {};
std::vector<GLsync> fences = {};

GLuint VAO, VBO, instanceSSBO;
Shader* ourShader;
Texture* texture;
glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
float rotationY = 0.f;
float rotationX = 0.f;
int rLevel = 1;
int maxRecursion = 14; // For unsigned long long this can be from 0 to 31 but we dont use this much
float cameraRadius = 3.f;

#pragma endregion

#pragma region FunctionsDeclaration

bool init();
std::string get_executable_path();
void glfw_error_callback(int error, const char* description);
void GLAPIENTRY error_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void setupObjects();
void tetrahedron(int level, int levelBefore = 0);

void input();
void update();

void imgui_init();
void imgui_begin();
void imgui_render();
void imgui_end();

void end_frame();
void cleanup();

#pragma endregion

int main(int argc, char** argv)
{
    exeDirPath = get_executable_path();

    if (!init())
    {
        spdlog::error("Failed to initialize project!");
        return EXIT_FAILURE;
    }
    spdlog::info("Initialized project.");

    imgui_init();
    spdlog::info("Initialized ImGui.");

    setupObjects();

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

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

    return EXIT_SUCCESS;
}

bool init()
{
    // Setup window
#if _DEBUG
    glfwSetErrorCallback(glfw_error_callback);
#endif
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
    glfwSwapInterval(0); // VSync ON/OFF - fixes FPS at the refresh rate of your screen (0 - OFF, 1 - ON)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    if (err)
    {
        spdlog::error("Failed to initialize OpenGL loader!");
        return false;
    }
    spdlog::info("Successfully initialized OpenGL loader!");

#if _DEBUG
    // Debugging
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(error_message_callback, 0);
#endif

    // Depth Test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Face Culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    return true;
}

std::string get_executable_path() {
#if defined(_WIN32)
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path().string();

#elif defined(__APPLE__)
    char buffer[4096];
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        return std::filesystem::weakly_canonical(buffer).parent_path().string();
    }
    else {
        printf("Error: _NSGetExecutablePath(): Buffer too small for executable path");
        exit(EXIT_FAILURE);
    }

#elif defined(__linux__)
    char buffer[4096];
    ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
    if (count != -1) {
        return std::filesystem::weakly_canonical(std::string(buffer, count)).parent_path().string();
    }
    else {
        printf("Error: readlink(): Cannot read /proc/self/exe");
        exit(EXIT_FAILURE);
    }

#else
#error "Unsupported platform"
#endif
}

#pragma region OpenGLCallbacks

#if _DEBUG
void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void GLAPIENTRY error_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;

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
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#pragma endregion

void setupObjects()
{
    texture = new Texture(std::string(exeDirPath).append("/res/textures/stone.jpg").c_str(), GL_SRGB, GL_RGB, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);
    ourShader = new Shader(std::string(exeDirPath).append("/res/shaders/basic.vert").c_str(), std::string(exeDirPath).append("/res/shaders/basic.geom").c_str(), std::string(exeDirPath).append("/res/shaders/basic.frag").c_str());

    // Define a single point for instancing
    glm::vec3 point = glm::vec3(0.0f);

    texture->use(0);
    ourShader->use();
    ourShader->setInt("tex", 0);
    ourShader->setVec3("c", color);
    ourShader->setMatrix4x4("projection", glm::perspective(glm::radians(45.0f), WINDOW_HEIGHT != 0 ? (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT : 0.f, 0.1f, 1000.0f));
    ourShader->setMatrix4x4("view", glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -cameraRadius)));
    ourShader->setFloat("h_2", 1.0f / 3.0f);
    float r = (float)M_SQRT3 / 3.0;
    ourShader->setFloat("r", r);
    ourShader->setFloat("r_2", r * 0.5f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), &point, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // SSBO for instance
    glGenBuffers(1, &instanceSSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, instanceSSBO);

    // Max SSBO size
    GLint maxSSBOSize = 0;
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSSBOSize);
    ONE_DRAW_TRANSFORMS = maxSSBOSize / sizeof(InstanceData);

    glBufferData(GL_SHADER_STORAGE_BUFFER, ONE_DRAW_TRANSFORMS * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    tetrahedron(rLevel, 0.f);
    glBindVertexArray(0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void tetrahedron(int level, int levelBefore)
{
    if (level <= 0) {
        instances.clear();
        return;
    }

    if (level == 1) {
        instances.clear();
        instances.emplace_back(InstanceData{ glm::vec3(0.f), 1.f });
        return;
    }

    int delta = level - levelBefore;
    std::vector<InstanceData> temps;
    temps.swap(instances);

    static glm::vec3 offsets[] = {
        {-0.5f, -0.333333f, -0.288675f},
        { 0.0f, -0.333333f,  0.57735f},
        { 0.5f, -0.333333f, -0.288675f},
        { 0.0f,  0.333333f,  0.0f}
    };

    if (delta > 0) {
        for (int i = 0; i < delta; ++i) {
            std::vector<InstanceData> nextGen;
            nextGen.reserve(temps.size() * 4); // reserve space ahead of time
            for (const auto& instance : temps) {
                float scale = instance.scale * 0.5f;
                glm::vec3 pos = instance.pos;
                for (const auto& offset : offsets) {
                    nextGen.emplace_back(InstanceData{ pos + offset * scale, scale });
                }
            }
            temps.swap(nextGen); // nextGen becomes temps for next iteration
        }
    }
    else if (delta < 0) {
        for (int i = 0; i < -delta; ++i) {
            size_t size = temps.size();
            std::vector<InstanceData> nextGen;
            nextGen.reserve(size / 4); // estimate capacity
            for (size_t j = 0; j < size; j += 4) {
                float scale = temps[j].scale * 2.f;
                glm::vec3 pos = temps[j].pos - offsets[0] * temps[j].scale;
                nextGen.emplace_back(InstanceData{ pos, scale });
            }
            temps.swap(nextGen);
        }
    }

    instances.swap(temps);
    temps.clear();
}

void input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void update()
{
    size_t total = instances.size();
    if (total == 0) return;

    glBindVertexArray(VAO);

    size_t offset = 0;
    size_t frame = 0;

    while (offset < total) {
        size_t count = std::min(ONE_DRAW_TRANSFORMS, total - offset);

        // Fences
        if (frame < fences.size() && fences[frame]) {
            GLenum res = GL_WAIT_FAILED;
            while (res == GL_TIMEOUT_EXPIRED || res == GL_WAIT_FAILED) {
                res = glClientWaitSync(fences[frame], GL_SYNC_FLUSH_COMMANDS_BIT, 100000000); // 100ms
            }
            glDeleteSync(fences[frame]);
            fences[frame] = nullptr;
        }

        // SSBO mapping
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, instanceSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, count * sizeof(InstanceData), nullptr, GL_DYNAMIC_DRAW);
        void* ptr = glMapBufferRange(
            GL_SHADER_STORAGE_BUFFER,
            0,
            count * sizeof(InstanceData),
            GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT
        );

        if (ptr) {
            memcpy(ptr, instances.data() + offset, count * sizeof(InstanceData));
            glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        }

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, instanceSSBO); // binding = 0
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        // Drawing
        glDrawArraysInstanced(GL_POINTS, 0, 1, count);

        if (frame >= fences.size()) fences.resize(frame + 1);
        fences[frame] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

        offset += count;
        frame++;
    }

    glBindVertexArray(0);
}

void imgui_init()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

    io.ConfigDockingTransparentPayload = true;  // Enable Docking Transparent

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();

    io.Fonts->AddFontDefault();
    io.Fonts->Build();
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

    ImGui::Begin("Sierpinski Tetrahedron");                  // Create a window called "Sierpinski Tetrahedron" and append into it.

    ImGui::Text("Change this Tetrahedron as you wish.");     // Display some text

    ImGui::Text("Number of Tetrahedrons: %d", instances.size());

    bool dirtyView = false;
    float rx = rotationX;
    ImGui::SliderFloat("Rotation X", &rx, 0.f, 360.f);       // Slider for rotation in X axes from 0 to 360

    if (rx != rotationX) {
        rotationX = rx;
        dirtyView = true;
    }

    float ry = rotationY;
    ImGui::SliderFloat("Rotation Y", &ry, 0.f, 360.f);       // Slider for rotation in Y axes from 0 to 360

    if (ry != rotationY) {
        rotationY = ry;
        dirtyView = true;
    }

    if (dirtyView) {
        ourShader->use();
        ourShader->setMatrix4x4("view",
            glm::rotate(
                glm::rotate(
                    glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, -cameraRadius)),
                    glm::radians(-rotationX), glm::vec3(1.f, 0.f, 0.f)
                ),
                glm::radians(-rotationY), glm::vec3(0.f, 1.f, 0.f)
            )
        );
    }

    int rl = rLevel;
    ImGui::SliderInt("Recursion", &rl, 0, maxRecursion);  // Slider for recursion level from 0 to MAX_RECURSION

    if (rl != rLevel) {
        glBindVertexArray(VAO);
        tetrahedron(rl, rLevel);
        glBindVertexArray(0);
        rLevel = rl;
    }

    glm::vec3 c = color;
    ImGui::ColorEdit3("Shape Color", (float*)&c);       // Edit 3 floats representing a shape color

    if (c != color) {
        color = c;
        ourShader->use();
        ourShader->setVec3("c", color);
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
    glfwPollEvents();
    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
}

void cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &instanceSSBO);

    delete ourShader;
    delete texture;

    instances.clear();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}