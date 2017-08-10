#include "context.h"
#include <cstdlib>
#include <iostream>

#ifdef USE_API_EGL

#include <EGL/egl.h>

static EGLDisplay eglDisplay;
static EGLConfig eglConfig;
static EGLContext eglContext;
static EGLSurface eglSurface;

void setupContext(int width, int height)
{
	const EGLint configAttrs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
		EGL_CONFORMANT, EGL_OPENGL_BIT,
		EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 0,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};

	const EGLint pbufferAttrs[] = {
		EGL_LARGEST_PBUFFER, EGL_TRUE,

		EGL_WIDTH, width,
		EGL_HEIGHT, height,
		EGL_NONE
	};

	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	EGLint maj, min;
	if (!eglInitialize(eglDisplay, &maj, &min)) {
		std::cerr << "EGL failed to initialize" << std::endl;
		exit(EXIT_FAILURE);
	}

	EGLint numConfigs;
	if (!eglChooseConfig(eglDisplay, configAttrs, &eglConfig, 1, &numConfigs)) {
		std::cerr << "Could not chose a valid pixel configuration" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!eglBindAPI(EGL_OPENGL_API)) {
		std::cerr << "Could not bind OpenGL API" << std::endl;
		exit(EXIT_FAILURE);
	}

	eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, pbufferAttrs);
	if (eglSurface == EGL_NO_SURFACE) {
		std::cerr << "Could not create pbuffer surface" << std::endl;
		exit(EXIT_FAILURE);
	}

	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, nullptr);
	if (!eglContext) {
		std::cerr << "Could not create OpenGL context" << std::endl;
		exit(EXIT_FAILURE);
	}

	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
}

void teardownContext()
{
	eglTerminate(eglDisplay);
}

#endif 