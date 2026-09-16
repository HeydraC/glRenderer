#include "proyecto2.h"

int main(){
    GLFWwindow* window = createGlfwWindow();

    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)){
        std::cout<<"Error al cargar punteros de GLAD"<<std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    
    GLuint shaderProgram = processShaders();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // Vértice Inferior izquierdo
        0.5f, -0.5f, 0.0f, // Vértice Inferior derecho
        0.0f,  0.5f, 0.0f  // Vértice Superior centro
    };

    Mesh triangle(vertices, sizeof(vertices), 3);

    glm::mat4 view, projection;
    glm::vec4 lightDir, lightColor, ambientLight;

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        triangle.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

GLFWwindow* createGlfwWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Proyecto 2 Carlos Heydra", NULL, NULL);

    if (!window){
        glfwTerminate();
        std::cout<<"Error al crear ventana"<<std::endl;
        exit(-1);
    }

    return window;
}

GLuint processShaders(){
    std::filesystem::path shaderPath = std::filesystem::path(SHADER_DIR);

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

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        std::cout<<"Error al enlazar programa"<<std::endl;
        exit(-1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}