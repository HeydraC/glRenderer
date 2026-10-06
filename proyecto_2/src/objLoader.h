#pragma once

#include <tiny_obj_loader.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include "mesh.h"

#ifndef SRC
    #define SRC "."
#endif

bool loadObject(std::string, std::vector<Mesh>&);