# CMakeList.txt : CMake project for proyecto#1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (proyecto2 "src/proyecto2.cpp" "src/proyecto2.h" "src/objLoader.cpp" "src/objLoader.h" "src/mesh.h" "src/mesh.cpp" "vertex.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET proyecto2 PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(proyecto2 PRIVATE include)

target_link_libraries(proyecto2 PRIVATE 
    glfw  
    glm
    glad
    imgui
    OpenGL::GL
    tol
)

# Para acceder a los shaders
target_compile_definitions(proyecto2 PRIVATE
    SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/"
)