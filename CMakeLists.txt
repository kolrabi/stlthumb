##
# Library dependencies:
#
#   - SDL2, EGL or OSMESA
#   - png / zlib
#   - OpenGL

# To build create a build directory and use CMake to build a project
# of your choice.

CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( stlthumb CXX C )
ENABLE_TESTING( )

# include cmake files from there
SET( CMAKE_MODULE_PATH  "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

INCLUDE (CompilerSettings)
INCLUDE (Libraries)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${API_INCLUDES})

ADD_DEFINITIONS(-DUSE_API_${USE_API})

# engine
ADD_EXECUTABLE( 
  stlthumb
  src/glew.c
  src/context_sdl2.cc
  src/context_osmesa.cc
  src/context_egl.cc
  src/image.cc
  src/main.cc
  src/render.cc
  src/stl.cc
  src/stlbinary.cc
  src/stltext.cc
)

TARGET_LINK_LIBRARIES( stlthumb ${LIBRARIES})
