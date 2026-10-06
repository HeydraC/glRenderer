#include "Engine3D.h"

inline glm::vec4 idToColor(int id) {
    int r = (id & 0x000000FF);
    int g = (id & 0x0000FF00) >> 8;
    int b = (id & 0x00FF0000) >> 16;
    return glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}

inline int colorToId(unsigned char r, unsigned char g, unsigned char b) {
    return r + (g << 8) + (b << 16);
}

void Engine3D::selectModel(float xpos, float ypos){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader.program);
    glUniformMatrix4fv(shader.projection, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(shader.view, 1, GL_FALSE, &view[0][0]);

    glUniform3fv(shader.lightColor, 1, &glm::vec3(0.0f)[0]);
    glUniform3fv(shader.ambientLight, 1, &glm::vec3(1.0f)[0]);

    for (size_t i = 0; i < models.size(); ++i) {
        glm::vec4 pickColor = idToColor(i+1);

        for (Mesh& mesh : models[i].meshes) {
            glUniformMatrix4fv(shader.model, 1, GL_FALSE, &mesh.model[0][0]);
            glUniform4fv(shader.objectColor, 1, &pickColor[0]);

            glBindVertexArray(mesh.vao);
            glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
            glBindVertexArray(0);
        }
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    
    float scaleX = (float)fbWidth / width;
    float scaleY = (float)fbHeight / height;

    int readX = static_cast<int>(xpos * scaleX);
    int readY = static_cast<int>((height - 1 - ypos) * scaleY);

    unsigned char pixel[4] = {0, 0, 0, 0};
    glReadPixels(readX, readY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    int pickedId = colorToId(pixel[0], pixel[1], pixel[2]);

    if (pickedId > 0 && pickedId <= models.size()){
        selectedModel = pickedId - 1;
        mColor[0] = models[selectedModel].objectColor.r;
        mColor[1] = models[selectedModel].objectColor.g;
        mColor[2] = models[selectedModel].objectColor.b;
        mColor[3] = models[selectedModel].objectColor.a;
    }
}


void Engine3D::selectMesh(float xpos, float ypos){
    if (selectedModel < 0) return;

    Model& currentModel = models[selectedModel];
    std::vector<Mesh>& meshes = currentModel.meshes;

    if (meshes.size() == 1) return;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shader.program);
    glUniformMatrix4fv(shader.projection, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(shader.view, 1, GL_FALSE, &view[0][0]);

    glUniform3fv(shader.lightColor, 1, &glm::vec3(0.0f)[0]);
    glUniform3fv(shader.ambientLight, 1, &glm::vec3(1.0f)[0]);

    for (int i = 0; i < meshes.size(); ++i){
        glm::vec4 pickColor = idToColor(i + 1);

        glUniformMatrix4fv(shader.model, 1, GL_FALSE, &meshes[i].model[0][0]);
        glUniform4fv(shader.objectColor, 1, &pickColor[0]);

        glBindVertexArray(meshes[i].vao);
        glDrawArrays(GL_TRIANGLES, 0, meshes[i].vertices.size());
        glBindVertexArray(0);
    }

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    
    float scaleX = (float)fbWidth / width;
    float scaleY = (float)fbHeight / height;

    int readX = static_cast<int>(xpos * scaleX);
    int readY = static_cast<int>((height - 1 - ypos) * scaleY);

    unsigned char pixel[4] = {0, 0, 0, 0};
    glReadPixels(readX, readY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    int pickedId = colorToId(pixel[0], pixel[1], pixel[2]);

    if (pickedId > 0 && pickedId <= meshes.size()){
        selectedMesh = pickedId - 1;

        sColor[0] = meshes[selectedMesh].objectColor.r;
        sColor[1] = meshes[selectedMesh].objectColor.g;
        sColor[2] = meshes[selectedMesh].objectColor.b;
        sColor[3] = meshes[selectedMesh].objectColor.a;
    }
}