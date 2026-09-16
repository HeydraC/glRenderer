add_library(glad STATIC glad/src/gl.c)
target_include_directories(glad PUBLIC glad/include)

add_library(tol INTERFACE)
target_include_directories(tol INTERFACE tol/include)