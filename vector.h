#pragma once

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

void VEC_init(Vector* vec, float x, float y, float z);
void VEC_add(Vector* vec, const Vector* added_vec, float scale);
float VEC_dot(const Vector* vec1, const Vector* vec2);
float VEC_length(const Vector* vec);
AngleComps VEC_getAngles(const Vector* camera_pos, const Basis* camera_basis, const Vector* vertex);
void VEC_updateCameraBasis(Basis* camera_basis, float yz_angle, float xz_angle);
void VEC_printBasis(const Basis* b);
