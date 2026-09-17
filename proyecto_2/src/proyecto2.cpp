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

    Mesh mesh("teapot.obj");

    glm::mat4 view, projection;
    glm::vec4 lightDir, lightColor, ambientLight;
    
    Camera cam;
    view = customLookAt(cam.pos, cam.target, cam.up);

    float aspect = 4.0f / 3.0f;
    projection = customPerspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
    GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");

    float lastFrame = 0.0f, delta;

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

        mesh.draw(shaderProgram, modelLocation);

        glfwSwapBuffers(window);
        glfwPollEvents();

        frameCount(lastFrame, delta, window);

        checkKeyboard(view, cam, delta, window);
    }

    mesh.destroy();

    return 0;
}

void checkKeyboard(glm::mat4 &view, Camera &cam, float delta, GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_UP)){
        cam.pos -= glm::normalize(cam.pos-cam.target)*delta*5.0f;
        view = customLookAt(cam.pos, cam.target, cam.up);
    }else if (glfwGetKey(window, GLFW_KEY_DOWN)){
        cam.pos += glm::normalize(cam.pos-cam.target)*delta*5.0f;
        view = customLookAt(cam.pos, cam.target, cam.up);
    }
}

void frameCount(float &lastFrame, float &delta, GLFWwindow* window){
    float currentFrame = glfwGetTime();
    delta = currentFrame - lastFrame;
    lastFrame = currentFrame;

    static float time_elapsed = 0;
    time_elapsed += delta;

    if (time_elapsed >= 1.0f){
        time_elapsed = 0;
        
        std::string title = "Proyecto 2 Carlos Heydra | FPS: " + std::to_string(1.0f/delta);

        glfwSetWindowTitle(window, title.c_str());
    }
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

glm::mat4 customLookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
    // 1. Calculate orthonormal basis
    glm::vec3 zAxis = glm::normalize(eye - target);
    glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));
    glm::vec3 yAxis = glm::cross(zAxis, xAxis);

    // 2. Construct matrix (GLM stores matrices in column-major order)
    glm::mat4 viewMatrix(1.0f);

    viewMatrix[0][0] = xAxis.x;
    viewMatrix[1][0] = xAxis.y;
    viewMatrix[2][0] = xAxis.z;

    viewMatrix[0][1] = yAxis.x;
    viewMatrix[1][1] = yAxis.y;
    viewMatrix[2][1] = yAxis.z;

    viewMatrix[0][2] = zAxis.x;
    viewMatrix[1][2] = zAxis.y;
    viewMatrix[2][2] = zAxis.z;

    viewMatrix[3][0] = -glm::dot(xAxis, eye);
    viewMatrix[3][1] = -glm::dot(yAxis, eye);
    viewMatrix[3][2] = -glm::dot(zAxis, eye);

    return viewMatrix;
}

glm::mat4 customPerspective(float fovRadians, float aspect, float zNear, float zFar) {
    float tanHalfFov = std::tan(fovRadians * 0.5f);

    glm::mat4 result(0.0f);

    // GLM matrix access is [column][row]
    result[0][0] = 1.0f / (aspect * tanHalfFov);
    result[1][1] = 1.0f / tanHalfFov;
    result[2][2] = -(zFar + zNear) / (zFar - zNear);
    result[2][3] = -1.0f;
    result[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);

    return result;
}