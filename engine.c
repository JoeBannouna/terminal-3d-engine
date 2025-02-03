#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/select.h>
#include <stdlib.h>

#include "stack.h"
#include "pixel.h"
#include "vector.h"
#include "geometry.h"

#define WIDTH 360
#define HEIGHT 85
#define DEF_CHAR ' '
#define DRAW_CHAR '#'

Vector camera_pos = {0, 0, 0};
Vector camera_dir = {0, 0, 0};
Basis camera_basis = {
  .x = {1, 0, 0},
  .y = {0, 1, 0},
  .z = {0, 0, 1}
};

STACK(Pixel2D, 1000, pixels);

typedef struct Vector Vertex;
STACK(Vertex, 100, vertices);

STACK(Cube, 5, cubes);

typedef int Index;
STACK(Index, HEIGHT*(WIDTH+1), pixel_indices);


char* screen_string;

int getScreenStringIndex(int row, int col) {
  return row * (WIDTH + 1) + col;
}

void drawLine(Pixel2D start, Pixel2D end) {
  for (float t = 0; t < 1; t += 0.025) {
    Pixel2D interpolated_pixel = {
      start.x + t*(end.x - start.x),
      start.y + t*(end.y - start.y),
    };
    pushPixel2D(interpolated_pixel);
  }
}

void resetFrame() {
  while (pixel_indices_len > 0) {
    int i = popIndex();
    screen_string[i] = DEF_CHAR;
  }
}

void setFrame() {
  for (int i = 0; i < pixels_len; i++) {
    Pixel2D pixel = pixels[i];
    if (pixel.x < WIDTH && pixel.x >= 0 && pixel.y < HEIGHT && pixel.y >= 0) {
      int screen_index = getScreenStringIndex(pixel.y, pixel.x);
      pushIndex(screen_index);
      screen_string[screen_index] = DRAW_CHAR;
    }
  }
}

void renderFrame() {
  printf("%.*s", HEIGHT * (WIDTH+1), screen_string);
}

void initScreenString() {
  screen_string = malloc(sizeof(char)*(WIDTH+1)*HEIGHT);
  for (int row = 0; row < HEIGHT; row++) {
    for (int col = 0; col < WIDTH; col++) {
      int index = getScreenStringIndex(row, col);
      screen_string[index] = DEF_CHAR;
    }
  }
  for (int row = 0; row < HEIGHT; row++) {
    screen_string[getScreenStringIndex(row, WIDTH)] = '\n';
  }
}

#define TURN_SPEED 0.02
void updateGameState(char c) {
  if (c == 'W') {
    VEC_updateCameraBasis(&camera_basis, TURN_SPEED, 0);
  } else if (c == 'S') {
    VEC_updateCameraBasis(&camera_basis, -TURN_SPEED, 0);
  } else if (c == 'D') {
    VEC_updateCameraBasis(&camera_basis, 0, -TURN_SPEED);
  } else if (c == 'A') {
    VEC_updateCameraBasis(&camera_basis, 0, TURN_SPEED);
  } else if (c == 'w') {
    VEC_add(&camera_pos, &camera_basis.z, 0.1);
  } else if (c == 's') {
    VEC_add(&camera_pos, &camera_basis.z, -0.1);
  } else if (c == 'd') {
    VEC_add(&camera_pos, &camera_basis.x, 0.1);
  } else if (c == 'a') {
    VEC_add(&camera_pos, &camera_basis.x, -0.1);
  }
}

// Function to configure terminal to raw mode
void set_terminal_raw_mode() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term); // Get current terminal attributes
  term.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
  term.c_lflag &= ~(ISIG); // Disable canonical mode and echo
  term.c_iflag &= ~(IXON | ICRNL);
  tcsetattr(STDIN_FILENO, TCSANOW, &term); // Apply changes
}

// Function to reset terminal to normal mode
void reset_terminal_mode() {
  struct termios term;
  tcgetattr(STDIN_FILENO, &term);
  term.c_lflag |= (ICANON | ECHO); // Enable canonical mode and echo
  term.c_lflag |= (ISIG); // Enable canonical mode and echo
  term.c_iflag |= (IXON | ICRNL);
  tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// initialize the state
void initializeVariables() {
  initScreenString();

  Cube c1;
  CUBE_init(&c1, (Vector) {1, 1, 1}, 2, 0);
  pushCube(c1);
}

void connectCubeLines(const Cube* cube) {
  drawLine(pixels[cube->starting_index + 0], pixels[cube->starting_index + 1]);
  drawLine(pixels[cube->starting_index + 1], pixels[cube->starting_index + 2]);
  drawLine(pixels[cube->starting_index + 2], pixels[cube->starting_index + 3]);
  drawLine(pixels[cube->starting_index + 3], pixels[cube->starting_index + 0]);

  drawLine(pixels[cube->starting_index + 4], pixels[cube->starting_index + 5]);
  drawLine(pixels[cube->starting_index + 5], pixels[cube->starting_index + 6]);
  drawLine(pixels[cube->starting_index + 6], pixels[cube->starting_index + 7]);
  drawLine(pixels[cube->starting_index + 7], pixels[cube->starting_index + 4]);

  drawLine(pixels[cube->starting_index + 0], pixels[cube->starting_index + 4]);
  drawLine(pixels[cube->starting_index + 1], pixels[cube->starting_index + 5]);
  drawLine(pixels[cube->starting_index + 2], pixels[cube->starting_index + 6]);
  drawLine(pixels[cube->starting_index + 3], pixels[cube->starting_index + 7]);
}

int main() {
  set_terminal_raw_mode(); // Set terminal to raw mode
  atexit(reset_terminal_mode); // Ensure terminal is reset on exit

  initializeVariables();

  while (1) {
    char c = '\0';

    printf("\033[H\033[J");
    resetFrame();
    while (vertices_len != 0) popVertex();
    for (int i = 0; i < cubes_len; i++) {
      Cube* c = &cubes[i];
      for (int j = 0; j < c->vertices_num; j++) pushVertex(c->vertices[j]);
    }
    while (pixels_len != 0) popPixel2D();
    for (int i = 0; i < vertices_len; i++) {
      AngleComps angles = VEC_getAngles(&camera_pos, &camera_basis, &vertices[i]);
      pushPixel2D((Pixel2D) {(WIDTH*angles.x_comp/2/55) + (int)(WIDTH/2), (WIDTH*angles.y_comp/2/55) + (int)(HEIGHT/2)});
    }
    for (int i = 0; i < cubes_len; i++) {
      connectCubeLines(&cubes[i]);
      CUBE_rotate(&cubes[i], 0.01, 0.04);
    }
    setFrame();
    renderFrame();
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set); // Clear the set
    FD_SET(STDIN_FILENO, &set); // Add standard input to the set

    timeout.tv_sec = 0;
    timeout.tv_usec = 16000; // 100ms timeout
    
    // Wait for input using select
    int rv = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
    if (rv == -1) {
      perror("select");
      break;
    } else if (rv > 0) {
      c = getchar(); // Read the character
      if (c == 'q') break; // Quit if 'q' is pressed
      else updateGameState(c);
    }
  }

  return 0;
}

