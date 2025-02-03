#pragma once

#include "pixel.h"
typedef struct Vector {
  float x;
  float y;
  float z;
} Vector;

typedef struct Basis {
  Vector x;
  Vector y;
  Vector z;
} Basis;

typedef struct AngleComps {
  float x_comp;
  float y_comp;
} AngleComps;

typedef struct IntVec {
  int x;
  int y;
  int z;
} IntVec;

void VEC_print(const Vector* vec);
void VEC_init(Vector* vec, float x, float y, float z);
void VEC_add(Vector* vec, const Vector* added_vec, float scale);
IntVec VEC_convertToInt(const Vector* vec);
float VEC_dot(const Vector* vec1, const Vector* vec2);
float VEC_length(const Vector* vec);
void updateCameraPos(Vector* camera_pos, const Basis* camera_basis, const Vector delta_pos, float scale_factor);
AngleComps VEC_getAngles(const Vector* camera_pos, const Basis* camera_basis, const Vector* vertex);
void updateCameraBasis(Basis* camera_basis, float yz_angle, float xz_angle);
void printBasis(const Basis* b);
Pixel2D getPixelPos(const AngleComps* angles, const AngleComps max_angles, const int screen_dim[static 2]);
