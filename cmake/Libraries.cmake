# hack for mingw, prefer libz.a in lib/ folder over some .dll
FIND_LIBRARY( ZLIB_LIBRARY NAMES libz.a z zlib )
FIND_LIBRARY( SDL2_LIBRARY NAMES libSDL2.a )

# get required pacakges
FIND_PACKAGE( SDL2       REQUIRED )
FIND_PACKAGE( PNG        REQUIRED )
FIND_PACKAGE( OpenGL     REQUIRED )

SET( LIBRARIES 
  ${SDL2_LIBRARY_TEMP}
  ${OPENGL_LIBRARIES}
  ${PNG_LIBRARY}
  ${ZLIB_LIBRARY}
)

IF ( WIN32 )
  SET( LIBRARIES ${LIBRARIES} winmm imm32 version ws2_32 )
ENDIF ()
