#include "stl.h"

#include <iostream>
#include <fstream>

static uint32_t readU32(std::ifstream &file)
{
  // FIXME: endiannes?
  uint32_t value = 0;
  file.read((char *)&value, 4);
  return value;
}

static float readF32(std::ifstream &file)
{
  float value;
  file.read((char *)&value, 4);
  return value;
}

static Vec3 readVec3(std::ifstream &file)
{
  Vec3 vec = Vec3(readF32(file), readF32(file), readF32(file));
  return Vec3(vec.z, vec.x, -vec.y);
}

static inline uint16_t readU16(std::ifstream &file)
{
  uint16_t value = 0;
  file.read((char *)&value, 2);
  return value;
}

STLModel *readSTLBinary(std::ifstream &file)
{
  STLModel *model = new STLModel;

  // skip header
  file.seekg(80, std::ios_base::beg);

  uint32_t triangleCount = readU32(file);
  model->triangles.reserve(triangleCount);

  for (size_t i=0; i<triangleCount; i++) {
    STLTriangle triangle;
    triangle.normal = readVec3(file);

    for (int j=0; j<3; j++) {
      Vec3 vec = readVec3(file);
      triangle.vertices[j] = vec;

      if (j == 0 && i == 0) {
        model->min = model->max = vec;
      } else {
        model->max.x = std::max(vec.x, model->max.x);
        model->max.y = std::max(vec.y, model->max.y);
        model->max.z = std::max(vec.z, model->max.z);
        model->min.x = std::min(vec.x, model->min.x);
        model->min.y = std::min(vec.y, model->min.y);
        model->min.z = std::min(vec.z, model->min.z);
      }
    }

    // ignore attributes
    readU16(file);

    if (true || !std::isnormal(triangle.normal.x)) {
      triangle.RecalculateNormal();
    }
    model->triangles.push_back(triangle);
  }

  return model;
}