#include "Engine3D.h"

void Engine3D::run(){
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(cam.pos, cam.target, cam.up);

    glUseProgram(shader.program);

    glUniformMatrix4fv(shader.projection, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(shader.view, 1, GL_FALSE, &view[0][0]);
    glUniform3fv(shader.lightDir, 1, &lightDir[0]);
    glUniform3fv(shader.lightColor, 1, &lightColor[0]);
    glUniform3fv(shader.ambientLight, 1, &ambientLight[0]);

    for (Mesh &mesh : meshes) mesh.draw(shader.program, shader.model, shader.objectColor);

    glfwSwapBuffers(window);
    glfwPollEvents();

    frameCount();

    checkKeyboard();
}

void Engine3D::frameCount(){
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

void Engine3D::checkKeyboard(){
    float moveSpeed = 5.0f;
    float rotSpeed = 75.0f;

    glm::vec3 forward = cam.target - cam.pos;
    glm::vec3 right = glm::normalize(glm::cross(forward, cam.up));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        glm::vec3 step = glm::normalize(forward) * moveSpeed * delta;
        cam.pos += step;
        cam.target += step;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        glm::vec3 step = glm::normalize(forward) * moveSpeed * delta;
        cam.pos -= step;
        cam.target -= step;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        glm::vec3 step = glm::normalize(right) * moveSpeed * delta;
        cam.pos += step;
        cam.target += step;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        glm::vec3 step = glm::normalize(right) * moveSpeed * delta;
        cam.pos -= step;
        cam.target -= step;
    }
    

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(rotSpeed * delta), cam.up);
        cam.target = cam.pos + glm::vec3(rot * glm::vec4(forward, 0.0f));
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(-rotSpeed * delta), cam.up);
        cam.target = cam.pos + glm::vec3(rot * glm::vec4(forward, 0.0f));
    }

    forward = cam.target - cam.pos;
    right = glm::normalize(glm::cross(forward, cam.up));

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(rotSpeed * delta), right);
        cam.target = cam.pos + glm::vec3(rot * glm::vec4(forward, 0.0f));
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(-rotSpeed * delta), right);
        cam.target = cam.pos + glm::vec3(rot * glm::vec4(forward, 0.0f));
    }
}