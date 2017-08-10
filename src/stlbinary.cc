#include "stl.h"

#include <iostream>
#include <fstream>

// FIXME: endianness?

static uint32_t readU32(std::ifstream &file)
{
  uint32_t value = 0;
  file.read(reinterpret_cast<char*>(&value), 4);
  return value;
}

static Vec3 fixVector(const float* v)
{
  return Vec3(v[0], v[2], -v[1]);
}

static inline uint16_t readU16(std::ifstream &file)
{
  uint16_t value = 0;
  file.read(reinterpret_cast<char*>(&value), 2);
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
    float rawData[12];
    file.read(reinterpret_cast<char*>(rawData), 12 * sizeof(float));

    STLTriangle triangle;
    triangle.normal = fixVector(rawData + 0);

    for (int j=0; j<3; j++) {
      Vec3& vec = triangle.vertices[j] = fixVector(rawData + 3 + j*3);

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

    if (!std::isnormal(triangle.normal.x)) {
      triangle.RecalculateNormal();
    }
    model->triangles.push_back(triangle);
  }

  return model;
}