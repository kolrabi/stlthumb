#include "stl.h"
#include "render.h"

#include <cstdlib>
#include <iostream>

static const char *inputFileName  = nullptr;
static const char *outputFileName = nullptr;
static int         thumbWidth     = 128;
static int         thumbHeight    = 128;

int 
main(int argc, char **argv)
{
  if (argc < 3) {
    std::cerr << "Usage: stlthumb <input> <output> [<width> [<height>]]" << std::endl;
    return EXIT_FAILURE;
  }

  inputFileName = argv[1];
  outputFileName = argv[2];

  if (argc > 3) {
    thumbWidth = thumbHeight = ::atol(argv[3]);
    if (thumbWidth <= 0) {
      std::cerr << "Invalid thumbnail size '" << argv[3] << "'" << std::endl;
      return EXIT_FAILURE;
    }
  }

  if (argc > 4) {
    thumbHeight = ::atol(argv[4]);
    if (thumbHeight <= 0) {
      std::cerr << "Invalid thumbnail size '" << argv[4] << "'" << std::endl;
      return EXIT_FAILURE;
    }
  }

  STLModel *model = readSTL(inputFileName);

  std::cerr << inputFileName << ": " << model->triangles.size() << std::endl;
  renderThumb(model, outputFileName, thumbWidth, thumbHeight);

  delete model;

  return EXIT_SUCCESS;
}