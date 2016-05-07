IF( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
  MESSAGE( FATAL_ERROR
              "Prevented in-tree built. Please create a build directory "
              "outside of the source code and call cmake from there" )
ENDIF( )

# Compiler-specific C++14 activation.
IF ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" )

  EXECUTE_PROCESS( COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION )

  IF ( NOT (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9) )
    MESSAGE( FATAL_ERROR "${PROJECT_NAME} requires g++ 4.9 or greater." )
  ENDIF ()

  SET( CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS}         -std=c++14" )
  SET( CMAKE_CXX_FLAGS          "${CMAKE_CXX_FLAGS}         -Wall -Werror -pedantic" )
  SET( CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O3 -funsafe-loop-optimizations -fdevirtualize-speculatively -ffast-math" )
  SET( CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}   -O0 -g -DDEBUG")

ELSEIF ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")

  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -m32 -std=c++14" )
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-warn-absolute-paths" )

ELSEIF ( MSVC )

  MESSAGE( WARNING "Your C++ compiler may not fully support C++14. There might be errors." )

ELSE ()

  MESSAGE( FATAL_ERROR "Your C++ compiler does not support C++14." )

ENDIF ()

IF (EMSCRIPTEN)
  SET( CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} -O2" )
  SET( CMAKE_CXX_FLAGS_DEBUG    "${CMAKE_CXX_FLAGS_DEBUG}   -Og -DDEBUG")
ENDIF ()

