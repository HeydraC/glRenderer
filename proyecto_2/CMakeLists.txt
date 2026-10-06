# CMakeList.txt : CMake project for proyecto#1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (proyecto2
    "src/proyecto2.cpp"
    "src/objLoader.cpp" "src/objLoader.h"
    "src/mesh.cpp" "src/mesh.h"
    "src/Model.cpp" "src/Model.h"
    "src/Engine3D.cpp" "src/Engine3D.h"
    "src/mainLoop.cpp"
    "src/colorPick.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET proyecto2 PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(proyecto2 PRIVATE include)

target_link_libraries(proyecto2 PRIVATE 
    glfw  
    glm::glm
    glad
    imgui
    OpenGL::GL
    tol
)

target_compile_definitions(proyecto2 PRIVATE
    SRC="${CMAKE_CURRENT_SOURCE_DIR}"
)