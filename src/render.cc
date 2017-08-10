#include "render.h"
#include "stl.h"
#include "image.h"
#include "context.h"

#include <GL/glew.h>

#include <unistd.h>
#include <iostream>

static GLuint         shaderProgram = 0;
static GLuint         vertexBuffer = 0;
static GLuint         vertexCount = 0;

static GLubyte *      pixelData;
static unsigned       pixelDataStride;
static size_t         pixelDataSize;

static void createWindow(int width, int height)
{
  setupContext(width, height);

  glewInit();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glDisable(GL_CULL_FACE);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glClearColor(0.25,0.25,0.75,0);

  glViewport(0, 0, width, height);
}

static void shutdown()
{
  delete [] pixelData;
  pixelData = nullptr;

  if (shaderProgram) glDeleteProgram(shaderProgram);
  if (vertexBuffer) glDeleteBuffers(1, &vertexBuffer);

  shaderProgram = 0;
  vertexBuffer = 0;

  teardownContext();
}

static void checkShader(GLuint shader)
{
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

  if (status)
    return;

  // output link log
  char    outlog[8192];
  GLint   len = sizeof(outlog);
  glGetShaderInfoLog(shader, len-1, &len, outlog);
  outlog[len] = 0;
  std::cerr << "Shader compile result:\n" << "\n" << outlog << std::endl;

  exit(EXIT_FAILURE);
}

static void checkProgram()
{
  GLint status;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);

  if (status)
    return;

  // output link log
  char    outlog[8192];
  GLint   len = sizeof(outlog);
  glGetProgramInfoLog(shaderProgram, len, &len, outlog);
  std::cerr << "Program link result:\n" << outlog << std::endl;

  exit(EXIT_FAILURE);
}

void buildShaders()
{

  const char *vertexShader = 
    "#version 120\n"
    "uniform mat4 uMatMVP;"
    "varying vec3 vNormal;"
    "void main() {"
    "gl_Position = uMatMVP * gl_Vertex;"
    "vNormal = gl_Normal;"
    "}";

  const char *fragmentShader = 
    "#version 120\n"
    "varying vec3 vNormal;"
    "const vec3 lightDir = vec3(0, 3, 1);"
    "void main() {"
    "float l=dot(vNormal, normalize(lightDir));"
    "float l1=clamp(l,0,1);"
    "float l2=clamp(-l,0,1);"
    "gl_FragColor = vec4( l1 * vec3(1,0.9,0.8) + l2 * vec3(0.7,0.8,1) + vec3(0.1), 1);"
    "}";

  shaderProgram = glCreateProgram();

  GLuint vertexShaderObject   = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

  const GLchar *sourceData    = vertexShader;

  glShaderSource(vertexShaderObject, 1, &sourceData, nullptr);
  glCompileShader(vertexShaderObject);
  checkShader(vertexShaderObject);
  glAttachShader(shaderProgram, vertexShaderObject);

  sourceData    = fragmentShader;
  glShaderSource(fragmentShaderObject, 1, &sourceData, nullptr);
  glCompileShader(fragmentShaderObject);
  checkShader(fragmentShaderObject);
  glAttachShader(shaderProgram, fragmentShaderObject);

  glLinkProgram(shaderProgram);
  checkProgram();

  glDeleteShader(vertexShaderObject);
  glDeleteShader(fragmentShaderObject);
}

static void setupBuffers(const STLModel *model)
{
  struct Vertex 
  {
    float xyz[3];
    float normal[3];
  };

  vertexCount = model->triangles.size() * 3;

  Vertex *  vertices    = new Vertex[vertexCount];
  size_t    index       = 0;

  for (size_t i=0; i<model->triangles.size(); i++) {
    Vec3 normal = model->triangles[i].normal;
    for (int j=0; j<3; j++) {
      vertices[index].xyz[0] = model->triangles[i].vertices[j].x;
      vertices[index].xyz[1] = model->triangles[i].vertices[j].y;
      vertices[index].xyz[2] = model->triangles[i].vertices[j].z;
      vertices[index].normal[0] = normal.x;
      vertices[index].normal[1] = normal.y;
      vertices[index].normal[2] = normal.z;

      index++;
    }
  }

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, vertices, GL_STATIC_DRAW);

  delete [] vertices;

  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, xyz));
  glEnableClientState(GL_VERTEX_ARRAY);

  glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, normal));
  glEnableClientState(GL_NORMAL_ARRAY);
}

static void setupView(const STLModel *model)
{
  Vec3 size   = model->max - model->min;
  Vec3 center = model->min + size * 0.5f;

  float diagonal = size.Mag()*0.6f;

  Vec3 eye = Vec3(1,1,1).Normalized(); // * 1.1f * diagonal + center;

  Matrix4 mvp = 
    //Matrix4::LookFrom(eye, center-eye, Vec3(0,1,0)) * 
    Matrix4::Ortho(-diagonal, diagonal, -diagonal, diagonal, -diagonal*2, diagonal*2) *
    Matrix4::LookFrom(eye, -eye, Vec3(0,1,0)) *
    Matrix4::Translate(-center)
    ;

  GLint loc = glGetUniformLocation(shaderProgram, "uMatMVP");

  glUseProgram(shaderProgram);
  glUniformMatrix4fv(loc, 1, GL_FALSE, mvp.m);
}

static void draw()
{
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glUseProgram(shaderProgram);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glDrawArrays(GL_TRIANGLES, 0, vertexCount);

  glFinish();
}

static void getPixelData(int width, int height)
{
  pixelDataStride = width * 4;
  pixelDataSize   = height * pixelDataStride;
  pixelData       = new GLubyte[pixelDataSize];

  glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
}

void renderThumb(STLModel *model, const char *outputFileName, int thumbWidth, int thumbHeight)
{
  createWindow(thumbWidth, thumbHeight);
  buildShaders();
  setupBuffers(model);
  setupView(model);

  draw();
  getPixelData(thumbWidth, thumbHeight);
  saveImage(outputFileName, pixelData, thumbWidth, thumbHeight, pixelDataStride);

  shutdown();
}
