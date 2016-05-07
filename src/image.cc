#include "image.h"

#include <png.h>

#include <iostream>
#include <fstream>

static void pngWriteFunc(png_structp pngWrite, png_bytep data, png_size_t size)
{
  auto file = (std::ofstream *)png_get_io_ptr(pngWrite);
  file->write((const char *)data, size);
}

static void pngFlushFunc(png_structp pngWrite)
{
  auto file = (std::ofstream *)png_get_io_ptr(pngWrite);
  file->flush();
}

static void saveImagePNG(std::ofstream &file, const void *data, int width, int height, unsigned stride)
{
  png_structp  pngWrite  = nullptr;
  png_infop    pngInfo   = nullptr;
  png_bytep    pngData   = (png_bytep)data;

  pngWrite = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!pngWrite) {
   std::cerr << "PNG: Could not allocate write struct" << std::endl;
   exit(EXIT_FAILURE);
  }

  pngInfo = png_create_info_struct(pngWrite);
  if (!pngInfo) {
    std::cerr << "PNG: Could not allocate info struct" << std::endl;
    exit(EXIT_FAILURE);
  }

  if (setjmp(png_jmpbuf(pngWrite))) {
    std::cerr << "PNG: Something went wrong ¯\\_(ツ)_/¯" << std::endl;
    exit(EXIT_FAILURE);
  }

  png_set_write_fn(pngWrite, &file, pngWriteFunc, pngFlushFunc);  

  png_set_IHDR(pngWrite, pngInfo, width, height, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_write_info(pngWrite, pngInfo);

  for (int y=0; y<height; y++) {
    png_write_row(pngWrite, pngData + (height - y - 1) * stride);
  }

  png_write_end(pngWrite, NULL);

  png_free_data(pngWrite, pngInfo, PNG_FREE_ALL, -1);
  png_destroy_write_struct(&pngWrite, nullptr);
}

void saveImage(const char *outputFileName, const void *data, int width, int height, unsigned stride)
{
  std::ofstream file;
  file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

  try {
    file.open(outputFileName, std::ifstream::binary);
    saveImagePNG(file, data, width, height, stride);
    file.close();
  } catch (std::ifstream::failure e) {
    std::cerr << "Exception opening/writing/closing file "<< outputFileName <<"\n";
    std::cerr << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
}
