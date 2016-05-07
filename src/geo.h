#pragma once

#include <cmath>
#include <cassert>
#include <cstring>

struct Vec3
{
  float x, y, z;

        Vec3()                           : x{0}, y{0}, z{0} {}
        Vec3(float x, float y, float z)  : x{x}, y{y}, z{z} {}

  Vec3  operator  -() const { return *this * (-1.0f); }

  Vec3  operator  +(const Vec3 & o) const
  { 
    return Vec3(x+o.x, y+o.y, z+o.z); 
  }

  Vec3  operator  -(const Vec3 & o) const
  { 
    return Vec3(x-o.x, y-o.y, z-o.z); 
  }

  Vec3  operator  *(float f) const
  { 
    return Vec3(x*f, y*f, z*f); 
  }

  Vec3  operator  /(float f) const
  { 
    return Vec3(x/f, y/f, z/f); 
  }

  Vec3  Cross(const Vec3 &o) const  
  { 
    return Vec3( o.z * y - o.y * z, o.x * z - o.z * x, o.y * x - o.x * y); 
  }

  Vec3  Normalized() const
  { 
    return *this / this->Mag();
  }

  float Mag() const
  { 
    return std::sqrt( x*x + y*y + z*z );       
  }
};

struct Matrix4 {
  float           m[16];

                  /** Identity matrix. */
                  Matrix4() {
                    m[ 0] = 1; m[ 1] = 0; m[ 2] = 0; m[ 3] = 0;
                    m[ 4] = 0; m[ 5] = 1; m[ 6] = 0; m[ 7] = 0;
                    m[ 8] = 0; m[ 9] = 0; m[10] = 1; m[11] = 0;
                    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
                  }

                  /** Copy c'tor. */
                  Matrix4(const Matrix4 &o)
                  {
                    ::memcpy(this->m, o.m, sizeof(this->m));
                  }

  /** Access members by column @a i and row @a j. */
  inline float &  operator ()(unsigned i, unsigned j)             { assert(i<4); assert(j<4); return m[i*4+j]; }

  /** Const access members by column @a i and row @a j. */
  inline float    operator ()(unsigned i, unsigned j)      const  { assert(i<4); assert(j<4); return m[i*4+j]; }

  /** Concatenate two matrices. */
  inline Matrix4  operator  *(const Matrix4 & a) const
  {
    const Matrix4 &b = *this;
    Matrix4 t;

    for (unsigned i=0; i<4; i++) {
      for (unsigned j=0; j<4; j++) {
        t(i,j) = a(i,0)*b(0,j) + a(i,1)*b(1,j) + a(i,2)*b(2,j) + a(i,3)*b(3,j);
      }
    }

    return t;
  }

  static Matrix4  Ortho      (float left, float right, float bottom, float top, float znear, float zfar)
  {
    Matrix4 t;
    t(0,0) =  2 / (right-left); t(3,0) = - (right+left) / (right-left);
    t(1,1) =  2 / (top-bottom); t(3,1) = - (top+bottom) / (top-bottom);
    t(2,2) = -2 / (zfar-znear); t(3,2) = - (zfar+znear) / (zfar-znear);
    return t;
  }

  /**
   * Create view matrix, looking from @a eye using @a fwd as the
   * look direction and @a up as the up vector.
   */
  static Matrix4  LookFrom   (const Vec3 &eye, const Vec3 &fwd, const Vec3 &up)
  {
    Vec3 f = fwd.Normalized();
    Vec3 u = up.Normalized();
    Vec3 s = f.Cross(u).Normalized();
    u = s.Cross(f);

    Matrix4 t;
    t(0,0) =  s.x; t(1,0) =  s.y; t(2,0) =  s.z;
    t(0,1) =  u.x; t(1,1) =  u.y; t(2,1) =  u.z;
    t(0,2) = -f.x; t(1,2) = -f.y; t(2,2) = -f.z;
    return t * Translate(-eye);
  }

  /** Create a translation matrix. */
  static Matrix4  Translate  (const Vec3 &v)
  {
    Matrix4 t;
    t(3,0) = v.x;
    t(3,1) = v.y;
    t(3,2) = v.z;
    return t;
  }

  /** Create a uniform scaling matrix. */
  static Matrix4  Scale      (float f)
  {
    Matrix4 t;
    t(0,0) = f;
    t(1,1) = f;
    t(2,2) = f;
    return t;
  }
};
