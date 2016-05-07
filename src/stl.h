#pragma once

#include "geo.h"

#include <vector>
#include <string>

struct STLTriangle
{
  Vec3  normal;
  Vec3  vertices[3];

  void  RecalculateNormal() 
  {
    normal = (vertices[1]-vertices[0]).Cross(vertices[2]-vertices[0]).Normalized();
  }
};

struct STLModel
{
  std::string               name;
  std::vector<STLTriangle>  triangles;
  Vec3                      min, max; 
};

STLModel *readSTL(const char *fileName);
