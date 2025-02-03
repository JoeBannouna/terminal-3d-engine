#include <math.h>

#include "geometry.h"

void CUBE_init(Cube* cube, Vector center, float side_length, int starting_index) {
  cube->center = center;
  cube->vertices_num = 8;
  cube->starting_index = starting_index;

  cube->vertices[0] = (Vector) { center.x - side_length, center.y - side_length, center.z - side_length };
  cube->vertices[1] = (Vector) { center.x + side_length, center.y - side_length, center.z - side_length };
  cube->vertices[2] = (Vector) { center.x + side_length, center.y + side_length, center.z - side_length };
  cube->vertices[3] = (Vector) { center.x - side_length, center.y + side_length, center.z - side_length };

  cube->vertices[4] = (Vector) { center.x - side_length, center.y - side_length, center.z + side_length };
  cube->vertices[5] = (Vector) { center.x + side_length, center.y - side_length, center.z + side_length };
  cube->vertices[6] = (Vector) { center.x + side_length, center.y + side_length, center.z + side_length };
  cube->vertices[7] = (Vector) { center.x - side_length, center.y + side_length, center.z + side_length };

  cube->side = side_length;
}

void CUBE_rotate(Cube* cube, float yz_angle, float xz_angle) {
  if (xz_angle != 0) {
    float xz_cos = cosf(xz_angle);
    float xz_sin = sinf(xz_angle);
    // rotate each of the vertices
    for (int i = 0; i < cube->vertices_num; i++) {
      Vector* curr_vertex = &cube->vertices[i];
      // subtrract center so that cube vertex is centered around origin
      // (this is essential for the rotation matrix to work)
      VEC_add(curr_vertex, &cube->center, -1);
      *curr_vertex = (Vector) { .x = curr_vertex->x * xz_cos - curr_vertex->z * xz_sin, .y = curr_vertex->y, .z = curr_vertex->x * xz_sin + curr_vertex->z * xz_cos };
      // add the center offset back in
      VEC_add(curr_vertex, &cube->center, 1);
    }
  }

  if (yz_angle != 0) {
    float yz_cos = cosf(yz_angle);
    float yz_sin = sinf(yz_angle);

    for (int i = 0; i < cube->vertices_num; i++) {
      Vector* curr_vertex = &cube->vertices[i];
      // subtrract center so that cube vertex is centered around origin
      // (this is essential for the rotation matrix to work)
      VEC_add(curr_vertex, &cube->center, -1);
      /**curr_vertex = (Vector) { .x = curr_vertex->x * xz_cos - curr_vertex->z * xz_sin, .y = curr_vertex->y, .z = curr_vertex->x * xz_sin + curr_vertex->z * xz_cos };*/
      *curr_vertex = (Vector) { .x = curr_vertex->x, .y = curr_vertex->y * yz_cos - curr_vertex->z * yz_sin, .z = curr_vertex->y * yz_sin + curr_vertex->z * yz_cos };
      // add the center offset back in
      VEC_add(curr_vertex, &cube->center, 1);
    }
  }
}

