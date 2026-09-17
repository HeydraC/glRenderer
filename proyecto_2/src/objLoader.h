#pragma once

#include <tiny_obj_loader.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "vertex.h"
#include "objLoader.h"

#ifndef SRC
    #define SRC "."
#endif

bool loadObject(std::string, std::vector<Vertex>&);