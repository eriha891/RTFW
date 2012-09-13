// ObjLoader.h
//
// Functions to parse .obj files and store inside Geometry
//
// Author: Robin Skånberg

#pragma once

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <iostream>
#include <vector>

#include "Types.h"
#include "Geometry.h"

// Load the obj-data and pack into a single Geometry
int loadObj( Geometry &geom, const std::string &filename, float scale = 1.0f );
// Load the obj-data and fill a vector of Geometry
int loadObj( std::vector<Geometry> &geomList, const std::string &filename, float scale = 1.0f );

#endif
