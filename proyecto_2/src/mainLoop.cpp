#include "Engine3D.h"
#include <fstream>
#include <sstream>

void Engine3D::run(){
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    view = glm::lookAt(cam.pos, cam.pos + cam.front, cam.up);

    glUseProgram(shader.program);
    glUniformMatrix4fv(shader.projection, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(shader.view, 1, GL_FALSE, &view[0][0]);
    glUniform3fv(shader.lightDir, 1, &lightDir[0]);
    glUniform3fv(shader.lightColor, 1, &lightColor[0]);
    glUniform3fv(shader.ambientLight, 1, &ambientLight[0]);

    for (Model &model: models) model.draw(shader.model, shader.objectColor);

    drawGUI();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();

    frameCount();
    checkKeyboard();
}

void Engine3D::frameCount(){
    float currentFrame = glfwGetTime();
    delta = currentFrame - lastFrame;
    lastFrame = currentFrame;

    timeElapsed += delta;

    if (timeElapsed >= 1.0f){
        timeElapsed = 0;
        
        std::string title = "Proyecto 2 Carlos Heydra | FPS: " + std::to_string(1.0f/delta);

        glfwSetWindowTitle(window, title.c_str());
    }
}

void Engine3D::checkKeyboard(){
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard) return;

    glm::vec3 moveDir(0.0f);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) moveDir += cam.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) moveDir -= cam.front;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) moveDir += cam.right;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) moveDir -= cam.right;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) moveDir += cam.up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) moveDir -= cam.up;
    

    if (glm::length(moveDir) > 0.0f) {
        cam.pos += glm::normalize(moveDir) * moveSpeed * delta;
    }

    bool rotated = false;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cam.yaw -= rotSpeed * delta;
        rotated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cam.yaw += rotSpeed * delta;
        rotated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cam.pitch += rotSpeed * delta;
        rotated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cam.pitch -= rotSpeed * delta;
        rotated = true;
    }

    if (rotated) {
        cam.pitch = glm::clamp(cam.pitch, -89.0f, 89.0f);
        cam.updateVectors();
    }
}

void Engine3D::onKeyDown(int key){
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard) return;

    if (key == GLFW_KEY_Q){
        mouseCamera = !mouseCamera;

        if (mouseCamera){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        }else{
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
        }        
    }
}

void Engine3D::onKeyUp(int key){}

glm::vec2 Engine3D::getMousePosition(){
    double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

void Engine3D::onMouseButtonDown(int button, float xpos, float ypos){
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && !mouseCamera) {
        if (selectedModel == -1) selectModel(xpos, ypos);
        else if (selectedMesh == -1) selectMesh(xpos, ypos);
    }
}

void Engine3D::onMouseButtonUp(int button, float xpos, float ypos){}

void Engine3D::onMouseMove(float xpos, float ypos){
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureKeyboard){
        lastMouseY = ypos;
        lastMouseX = xpos;
    }

    if (!mouseCamera) return;

    cam.pitch += (lastMouseY - ypos) * sensitivity;
    cam.yaw -= (lastMouseX - xpos) * sensitivity;

    lastMouseY = ypos;
    lastMouseX = xpos;

    cam.pitch = glm::clamp(cam.pitch, -89.0f, 89.0f);
    cam.updateVectors();
}

void Engine3D::drawGUI(){
    ImGui::Begin("Herramientas");

    static char modelName[128];

    ImGui::InputText("##Nombre del modelo", modelName, 128);
    ImGui::SameLine();
    if (ImGui::Button("Agregar modelo")){
        Model newModel;

        if (newModel.load(std::string(modelName)))
            models.push_back(newModel);
    }

    static char fileName[128];

    ImGui::InputText("##Nombre del archivo", fileName, 128);

    if (ImGui::Button("Guardar estado")){
        saveState(fileName);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cargar estado")){
        loadState(fileName);
    }

    ImGui::Text("Agregar");
    ImGui::SameLine();
    if (ImGui::Button("Cubo")){
        Model newModel;
        newModel.makeCube();
        models.push_back(newModel);
    }
    ImGui::SameLine();
    if (ImGui::Button("Pirámide")){
        Model newModel;
        newModel.makePyramid();
        models.push_back(newModel);
    }
    ImGui::SameLine();
    if (ImGui::Button("Esfera")){
        Model newModel;
        newModel.makeSphere();
        models.push_back(newModel);
    }

    if (ImGui::ColorEdit3("Color Fondo", bColor)) {
        backgroundColor.r = bColor[0];
        backgroundColor.g = bColor[1];
        backgroundColor.b = bColor[2];
    }

    if (ImGui::Checkbox("Depth Test", &depthTest)){
        depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }
    
    ImGui::SameLine();

    if (ImGui::Checkbox("Back Face Culling", &backFaceCulling)){
        backFaceCulling ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }

    ImGui::InputFloat("Sensibilidad", &sensitivity, 0.01f, 1.0f, "%.3f");
    ImGui::InputFloat("Vel. rotación", &rotSpeed, 0.01f, 1.0f, "%.3f");
    ImGui::InputFloat("Vel. movimiento", &moveSpeed, 0.01f, 1.0f, "%.3f");

    if (ImGui::Button("Borrar escena")){
        for (Model& model : models) model.destroy();

        models.clear();

        selectedModel = -1;
        selectedMesh = -1;
    }

    ImGui::Separator();

    if (selectedModel >= 0){
        Model& currentModel = models[selectedModel];

        ImGui::SeparatorText("Modelo seleccionado");

        bool changed = false;

        if (ImGui::SliderFloat("Rotación en X", &currentModel.rotation.x, -180.0f, 180.0f, "%.1f°")) {
            changed = true;
        }
        if (ImGui::SliderFloat("Rotación en Y", &currentModel.rotation.y, -180.0f, 180.0f, "%.1f°")) {
            changed = true;
        }
        if (ImGui::SliderFloat("Rotación en Z", &currentModel.rotation.z, -180.0f, 180.0f, "%.1f°")) {
            changed = true;
        }

        if (ImGui::DragFloat3("Posición", &currentModel.position[0], 0.1f, 0.0f, 0.0f, "%.1f")) {
            changed = true;
        }

        if (ImGui::DragFloat3("Escala", &currentModel.scale[0], 0.1f, 0.1f, 100.0f, "%.1f")) {
            changed = true;
        }

        if (changed) {
            currentModel.updateMeshes();
        }

        if (ImGui::ColorEdit4("Color Modelo", mColor)) {
            currentModel.objectColor.r = mColor[0];
            currentModel.objectColor.g = mColor[1];
            currentModel.objectColor.b = mColor[2];
            currentModel.objectColor.a = mColor[3];
        }

        ImGui::Checkbox("Wireframe", &currentModel.wireframe);
        ImGui::SameLine();
        ImGui::Checkbox("Normales", &currentModel.showNormals);
        ImGui::SameLine();
        ImGui::Checkbox("Vértices", &currentModel.showVertices);
        ImGui::SameLine();
        ImGui::Checkbox("Bounding box", &currentModel.showBoundingBox);

        if (ImGui::Button("Eliminar")){
            currentModel.destroy();

            models.erase(models.begin() + selectedModel);

            selectedModel = -1;            
            selectedMesh = -1;
        }

        if (ImGui::Button("Deseleccionar")){
            selectedModel = -1;
            selectedMesh = -1;
        }

        if (selectedMesh >= 0){
            Mesh& currentMesh = currentModel.meshes[selectedMesh];
            changed = false;

            ImGui::SeparatorText("Sub mallado seleccionado");

            if (ImGui::SliderFloat("Rotación en X##Sub", &currentMesh.rotation.x, -180.0f, 180.0f, "%.1f°")) {
                changed = true;
            }
            if (ImGui::SliderFloat("Rotación en Y##Sub", &currentMesh.rotation.y, -180.0f, 180.0f, "%.1f°")) {
                changed = true;
            }
            if (ImGui::SliderFloat("Rotación en Z##Sub", &currentMesh.rotation.z, -180.0f, 180.0f, "%.1f°")) {
                changed = true;
            }

            if (ImGui::DragFloat3("Posición##Sub", &currentMesh.position[0], 0.1f, 0.0f, 0.0f,"%.3f")) {
                changed = true;
            }

            if (ImGui::DragFloat3("Escala##Sub", &currentMesh.scale[0], 0.1f, 0.1f, 100.0f, "%.1f")) {
                changed = true;
            }

            if (changed) {
                currentMesh.update(currentModel.model);
            }

            if (ImGui::ColorEdit4("Color Submallado", sColor)) {
                currentMesh.objectColor.r = sColor[0];
                currentMesh.objectColor.g = sColor[1];
                currentMesh.objectColor.b = sColor[2];
                currentMesh.objectColor.a = sColor[3];
            }

            ImGui::Checkbox("Wireframe##Sub", &currentMesh.wireframe);
            ImGui::SameLine();
            ImGui::Checkbox("Normales##Sub", &currentMesh.showNormals);
            ImGui::SameLine();
            ImGui::Checkbox("Vértices##Sub", &currentMesh.showVertices);
            ImGui::SameLine();
            ImGui::Checkbox("Bounding box##Sub", &currentMesh.showBoundingBox);

            if (ImGui::Button("Eliminar##Sub")){
                currentMesh.destroy();

                currentModel.meshes.erase(currentModel.meshes.begin() + selectedMesh);

                selectedMesh = -1;
            }

            if (ImGui::Button("Deseleccionar##Sub")){
                selectedMesh = -1;
            }
        }
    }


    ImGui::End();
    ImGui::Render();
}

void Engine3D::saveState(char fileName[128]){
    std::string buffer;
    std::ofstream file(fileName);

    if (!file.is_open()){
        std::cout<<"Error guardando a "<<fileName<<std::endl;
        return;
    }

    for (Model& model : models){
        buffer += model.toString();
    }

    file<<buffer;
}

void Engine3D::loadState(char fileName[128]){
    std::ifstream file(fileName);

    if (!file.is_open()){
        std::cout<<"Error cargando de "<<fileName<<std::endl;
        return;
    }

    for (Model& model : models) model.destroy();

    models.clear();

    std::string buffer;

    glm::mat4 model;
    glm::vec4 color;

    std::vector<Mesh> meshes;
    std::vector<Vertex> vertices;

    while (std::getline(file, buffer)){
        std::stringstream ss(buffer);

        ss >> buffer;

        if (buffer == "m"){
            ss>>color.r>>color.g>>color.b>>color.a;
            for (int i = 0; i < 4; ++i){
                for (int j = 0; j < 4; ++j){
                    ss>>model[i][j];
                }
            }

            Model m;

            m.model = model;
            m.objectColor = color;
            m.meshes = meshes;

            models.push_back(m);

            meshes.clear();
        }else{
            color.r = std::stof(buffer);

            ss>>color.g>>color.b>>color.a;

            ss>>buffer;

            do{
                Vertex v;
                v.pos.x = std::stof(buffer);
                ss>>v.pos.y>>v.pos.z>>v.normal.x>>v.normal.y>>v.normal.z;

                vertices.push_back(v);

                ss>>buffer;
            }while(buffer != "M");

            for (int i = 0; i < 4; ++i){
                for (int j = 0; j < 4; ++j){
                    ss>>model[i][j];
                }
            }

            meshes.emplace_back(vertices, color);
            meshes.back().model = model;

            vertices.clear();
        }
    }
}