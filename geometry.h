#pragma once
#include "vector.h"

typedef struct {
  Vector center;
  Vector vertices[8];
  int vertices_num;
  float side;
  int starting_index; // starting index of the cube pixels in the pixels stack
} Cube;

void CUBE_init(Cube* cube, Vector center, float side_length, int starting_index);
void CUBE_rotate(Cube* cube, float yz_angle, float xz_angle);
