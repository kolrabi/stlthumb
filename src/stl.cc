#include "stl.h"

#include <iostream>
#include <fstream>

STLModel *readSTLText(std::ifstream &file);
STLModel *readSTLBinary(std::ifstream &file);

STLModel *readSTL(const char *fileName)
{
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    file.open(fileName, std::ifstream::binary);

    char test[6];
    file.read(test, 5);
    test[5] = 0;

    file.seekg(0, std::ios_base::beg);

    if (std::string(test) == "solid") {
      return readSTLText(file);
    } else {
      return readSTLBinary(file);
    }
  } catch (std::ifstream::failure e) {
    std::cerr << "Exception opening/reading/closing file "<< fileName <<"\n";
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  return nullptr;
}
