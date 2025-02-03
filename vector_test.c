#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define PI 3.14159

Vector camera_pos = {0, 0, 0};
Basis camera_basis = {
  .x = {1, 0, 0},
  .y = {0, 1, 0},
  .z = {0, 0, 1}
};
Vector test_point = {0, 0, 2};

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

void updateGameState(char c) {
  if (c == 'w') {
    VEC_updateCameraBasis(&camera_basis, 0.01, 0);
  } else if (c == 's') {
    VEC_updateCameraBasis(&camera_basis, -0.01, 0);
  } else if (c == 'd') {
    VEC_updateCameraBasis(&camera_basis, 0, -0.01);
  } else if (c == 'a') {
    VEC_updateCameraBasis(&camera_basis, 0, 0.01);
  }
}

int main() {
  set_terminal_raw_mode(); // Set terminal to raw mode
  atexit(reset_terminal_mode); // Ensure terminal is reset on exit
  
  while (1) {
    char c;
    printf("\033[H\033[J");
    VEC_printBasis(&camera_basis);
    printf("The angles away from the test point:\n");
    AngleComps angles = VEC_getAngles(&camera_pos, &camera_basis, &test_point);
    printf("{ %f, %f }\n", angles.x_comp, angles.y_comp);
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
