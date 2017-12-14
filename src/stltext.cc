#include "stl.h"

#include <iostream>
#include <fstream>

static void eatToken(std::ifstream &file, const char *string)
{
  std::string token;
  file >> token;

  if (token != string) {
    std::cerr << "Unexpected token '" << token << "'. Expected "<<string<<std::endl;
    exit(EXIT_FAILURE);
  }
}

static std::string readString(std::ifstream &file)
{
  std::string token;
  file >> token;
  return token;
}

static std::string readLine(std::ifstream &file)
{
  std::string line;
  std::getline(file,line);

  return line;
}

static Vec3 readVec3(std::ifstream &file)
{
  Vec3 v;
  file >> v.x >> v.y >> v.z;
  return v;
}

STLModel *readSTLText(std::ifstream &file)
{
  STLModel *model = new STLModel;

  eatToken(file, "solid");
  model->name = readLine(file);

  while(readString(file) != "endsolid") {
    //eatToken(file, "facet");
    eatToken(file, "normal");

      STLTriangle triangle;
      triangle.normal = readVec3(file);

      eatToken(file, "outer");
      eatToken(file, "loop");

        for (int i=0; i<3; i++) {
          eatToken(file, "vertex");
          Vec3 vec = readVec3(file);
          triangle.vertices[i] = vec;

          if (i == 0 && model->triangles.size() == 0) {
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

        if (triangle.normal.Mag() == 0.0f) {
          triangle.RecalculateNormal();
        }
        model->triangles.push_back(triangle);

      eatToken(file, "endloop");

    eatToken(file, "endfacet");
  }
  return model;
}
