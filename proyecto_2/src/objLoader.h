#pragma once
#define TINYOBJLOADER_IMPLEMENTATION

#include <tiny_obj_loader.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "vertex.h"

bool loadObject(std::string, std::vector<Vertex>&);