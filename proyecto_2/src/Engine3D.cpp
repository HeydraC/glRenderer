#include "Engine3D.h"

Engine3D::Engine3D(int _width, int _height){
    width = _width;
    height = _height;
    aspect = (float)width/height;

    lastFrame = 0.0f;
    timeElapsed = 0.0f;

    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    lightDir     = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
    lightColor   = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientLight = glm::vec3(0.15f, 0.15f, 0.15f);

    backgroundColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);

    createGLFWwindow();

    imguiInit();

    processShaders();

    getUniformLocations();

    cam.updateVectors();
}

Engine3D::~Engine3D(){
    for (Model& model : models) model.destroy();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

bool Engine3D::closedWindow(){ return glfwWindowShouldClose(window); }

void Engine3D::getUniformLocations(){
    shader.model        = glGetUniformLocation(shader.program, "model");
    shader.view         = glGetUniformLocation(shader.program, "view");
    shader.projection   = glGetUniformLocation(shader.program, "projection");
    shader.objectColor  = glGetUniformLocation(shader.program, "objectColor");
    shader.lightDir     = glGetUniformLocation(shader.program, "lightDir");
    shader.lightColor   = glGetUniformLocation(shader.program, "lightColor");
    shader.ambientLight = glGetUniformLocation(shader.program, "ambientLight");
}

void Engine3D::createGLFWwindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Proyecto 2 Carlos Heydra | FPS: 0.0", NULL, NULL);

    if (!window){
        glfwTerminate();
        std::cout<<"Error al crear ventana"<<std::endl;
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (!gladLoadGL(glfwGetProcAddress)){
        std::cout<<"Error al cargar punteros de GLAD"<<std::endl;
        exit(-1);
    }

    glViewport(0, 0, width, height);
    glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetFramebufferSizeCallback(window, windowSizeCallback);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
}

void Engine3D::imguiInit(){
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void Engine3D::processShaders(){
    std::filesystem::path shaderPath = std::filesystem::path(SRC) / "shaders";

    std::ifstream vertexReader(shaderPath / "base.vert");

    if (!vertexReader.is_open()){
        std::cout<<"Error al leer vertex shader"<<std::endl;
        exit(-1);
    }

    std::stringstream vertexBuffer;

    vertexBuffer << vertexReader.rdbuf();

    std::string vertexStr = vertexBuffer.str();
    const char* vertexSource = vertexStr.c_str();

    vertexReader.close();

    std::ifstream fragmentReader(shaderPath / "base.frag");

    if (!fragmentReader.is_open()){
        std::cout<<"Error al leer fragment shader"<<std::endl;
        exit(-1);
    }

    std::stringstream fragmentBuffer;

    fragmentBuffer << fragmentReader.rdbuf();

    std::string fragmentStr = fragmentBuffer.str();
    const char* fragmentSource = fragmentStr.c_str();
    
    fragmentReader.close();

    int success;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        std::cout<<"Error al compilar vertex shader"<<std::endl;
        exit(-1);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        std::cout<<"Error al compilar fragment shader"<<std::endl;
        exit(-1);
    }

    shader.program = glCreateProgram();
    glAttachShader(shader.program, vertexShader);
    glAttachShader(shader.program, fragmentShader);
    glLinkProgram(shader.program);
    glGetProgramiv(shader.program, GL_LINK_STATUS, &success);
    if (!success) {
        std::cout<<"Error al enlazar programa"<<std::endl;
        exit(-1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Engine3D::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key < 0 || key >= GLFW_KEY_LAST) return;
	Engine3D* engine = static_cast<Engine3D*>(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS) {
		engine->onKeyDown(key);
	}
	else if (action == GLFW_RELEASE) {
		engine->onKeyUp(key);
	}
}

void Engine3D::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods){
	if (button < 0 || button >= GLFW_MOUSE_BUTTON_LAST) return;
	Engine3D* engine = static_cast<Engine3D*>(glfwGetWindowUserPointer(window));
	glm::vec2 mousePos = engine->getMousePosition();
	if (action == GLFW_PRESS) {
		engine->onMouseButtonDown(button, mousePos.x, mousePos.y);
	}
	else if (action == GLFW_RELEASE) {
		engine->onMouseButtonUp(button, mousePos.x, mousePos.y);
	}
}

void Engine3D::cursorPosCallback(GLFWwindow* window, double xpos, double ypos){
	Engine3D* engine = static_cast<Engine3D*>(glfwGetWindowUserPointer(window));
	if (engine) {
		engine->onMouseMove(xpos, ypos);
	}
}

void Engine3D::windowSizeCallback(GLFWwindow* window, int width, int height){
    Engine3D* engine = static_cast<Engine3D*>(glfwGetWindowUserPointer(window));
    glViewport(0, 0, width, height);
    engine->aspect = (float)width/height;
}