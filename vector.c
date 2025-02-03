#include <math.h>
#include <stdio.h>
#include "vector.h"

#define PI 3.14159

// initialize a vector
void VEC_init(Vector* vec, float x, float y, float z) {
  vec->x = x;
  vec->y = y;
  vec->z = z;
}

// add a vector to another
void VEC_add(Vector* vec, const Vector* added_vec, float scale) {
  vec->x += scale * added_vec->x;
  vec->y += scale * added_vec->y;
  vec->z += scale * added_vec->z;
}

float VEC_dot(const Vector* vec1, const Vector* vec2) {
  float result = 0;
  result += vec1->x * vec2->x;
  result += vec1->y * vec2->y;
  result += vec1->z * vec2->z;
  return result;
}

float VEC_length(const Vector* vec) {
  return sqrtf(vec->x*vec->x + vec->y*vec->y + vec->z*vec->z);
}

// camera_direction will always be (0, 0, 1) as the pixel is linearly transformed
// into the desired rotation from camera_basis
// Note: returns the angles in radians
AngleComps VEC_getAngles(const Vector* camera_pos, const Basis* camera_basis, const Vector* vertex) {
  // linearly transformed vertex
  Vector lt_vertex = (Vector) {
    .x = camera_basis->x.x * vertex->x + camera_basis->x.y * vertex->y + camera_basis->x.z * vertex->z,
    .y = camera_basis->y.x * vertex->x + camera_basis->y.y * vertex->y + camera_basis->y.z * vertex->z,
    .z = camera_basis->z.x * vertex->x + camera_basis->z.y * vertex->y + camera_basis->z.z * vertex->z,
  };
  Vector lt_camera_pos = (Vector) {
    .x = camera_basis->x.x * camera_pos->x + camera_basis->x.y * camera_pos->y + camera_basis->x.z * camera_pos->z,
    .y = camera_basis->y.x * camera_pos->x + camera_basis->y.y * camera_pos->y + camera_basis->y.z * camera_pos->z,
    .z = camera_basis->z.x * camera_pos->x + camera_basis->z.y * camera_pos->y + camera_basis->z.z * camera_pos->z,
  };
  VEC_add(&lt_vertex, &lt_camera_pos, -1);
  /*VEC_add(&lt_vertex, camera_pos, -1);*/

  Vector lt_vertex_x_component = lt_vertex;
  lt_vertex_x_component.y = 0;

  Vector lt_vertex_y_component = lt_vertex;
  lt_vertex_y_component.x = 0;

  // this numerator is going to be the same for both 
  // lt_vertex_x and lt_vertex_y due to the nature of 
  // performing a dot product with the unit vector z
  float numerator = lt_vertex_x_component.z;

  float denominator_x = VEC_length(&lt_vertex_x_component);
  float x_comp = acosf(numerator/denominator_x);
  x_comp *= (lt_vertex_x_component.x < 0 ? -1 : 1);

  float denominator_y = VEC_length(&lt_vertex_y_component);
  float y_comp = acosf(numerator/denominator_y);
  y_comp *= (lt_vertex_y_component.y < 0 ? -1 : 1);

  return (AngleComps) {x_comp * 180 / PI, y_comp * 180 / PI};
}

// Node: This function always executes the yz_angle first
// This func should ideally be used one angle at a time 
// e.g. yz_angle = ..., xz_angle = 0, or vice versa
void VEC_updateCameraBasis(Basis* camera_basis, float yz_angle, float xz_angle) {
  if (xz_angle != 0) {
    float xz_cos = cosf(xz_angle);
    float xz_sin = sinf(xz_angle);

    Vector x_vec = camera_basis->x;
    Vector y_vec = camera_basis->y;
    Vector z_vec = camera_basis->z;

    x_vec = (Vector) { .x = x_vec.x * xz_cos - x_vec.z * xz_sin, .y = x_vec.y, .z = x_vec.x * xz_sin + x_vec.z * xz_cos };
    y_vec = (Vector) { .x = y_vec.x * xz_cos - y_vec.z * xz_sin, .y = y_vec.y, .z = y_vec.x * xz_sin + y_vec.z * xz_cos };
    z_vec = (Vector) { .x = z_vec.x * xz_cos - z_vec.z * xz_sin, .y = z_vec.y, .z = z_vec.x * xz_sin + z_vec.z * xz_cos };

    *camera_basis = (Basis) { .x = x_vec, .y = y_vec, .z = z_vec };
  }

  if (yz_angle != 0) {
    float yz_cos = cosf(yz_angle);
    float yz_sin = sinf(yz_angle);

    Vector x_vec = camera_basis->x;
    Vector y_vec = camera_basis->y;
    Vector z_vec = camera_basis->z;

    x_vec = (Vector) { .x = x_vec.x, .y = x_vec.y * yz_cos - x_vec.z * yz_sin, .z = x_vec.y * yz_sin + x_vec.z * yz_cos };
    y_vec = (Vector) { .x = y_vec.x, .y = y_vec.y * yz_cos - y_vec.z * yz_sin, .z = y_vec.y * yz_sin + y_vec.z * yz_cos };
    z_vec = (Vector) { .x = z_vec.x, .y = z_vec.y * yz_cos - z_vec.z * yz_sin, .z = z_vec.y * yz_sin + z_vec.z * yz_cos };

    *camera_basis = (Basis) { .x = x_vec, .y = y_vec, .z = z_vec };
  }
}

void VEC_printBasis(const Basis* b) {
  printf("{\n  {%f, %f, %f},\n  {%f, %f, %f},\n  {%f, %f, %f}\n}\n", 
         b->x.x, b->x.y, b->x.z,
         b->y.x, b->y.y, b->y.z,
         b->z.x, b->z.y, b->z.z
  );
}
