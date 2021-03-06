// ObjLoader.h
//
// Functions to parse .obj files and store inside Geometry
//
// Author: Robin Skånberg

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <string>
#include <vector>

#include "Types.h"
#include "Geometry.h"
#include "Tokenizer.h"

// Load the obj-data and pack into a single Geometry
int loadObj( Geometry &geom, const std::string &filename, float scale = 1.0f );
// Load the obj-data and fill a vector of Geometry
int loadObj( std::vector<Geometry> &geomList, const std::string &filename, float scale = 1.0f );

#endif
