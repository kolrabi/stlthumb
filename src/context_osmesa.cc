#include "context.h"
#include <cstdlib>
#include <iostream>

#ifdef USE_API_OSMESA

#include <GL/osmesa.h>

static OSMesaContext mesaContext;
static uint8_t* mesaBuffer;

void setupContext(int width, int height)
{
	const int attrs[] = {
		OSMESA_FORMAT, OSMESA_RGB,
		OSMESA_DEPTH_BITS, 24,
		OSMESA_CONTEXT_MAJOR_VERSION, 2,
		OSMESA_CONTEXT_MINOR_VERSION, 1,
		0
	};
	mesaContext = OSMesaCreateContextAttribs(attrs, nullptr);
	if (!mesaContext) {
		std::cerr << "Could not create OSMesa context" << std::endl;
		exit(EXIT_FAILURE);
	}

	mesaBuffer = new uint8_t[width*height*4];

	if (!OSMesaMakeCurrent(mesaContext, mesaBuffer, GL_UNSIGNED_BYTE, width, height)) {
		std::cerr << "Could not make OSMesa context current" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void teardownContext()
{
	OSMesaDestroyContext(mesaContext);
	delete [] mesaBuffer;
}

#endif 