A minimal terminal based 3D engine.

https://github.com/user-attachments/assets/5a2f30fb-e1d0-4d02-9ba6-03a9bbb5088c

https://github.com/user-attachments/assets/da13f0f1-f653-476c-b665-f576aa789b95

### Compiling & Running
Runs on Linux only
```
make engine
```

### Main entry point file
```
engine.c
```

### Adding objects
In `geometry.c`, structs exist defining 3D shapes like `Cube` along with functions that initialize and manipulate it `CUBE_init(...)`, `CUBE_rotate(...)`. 
```c
typedef struct {
  Vector center;
  Vector vertices[8];
  ...
} Cube;
```
In `engine.c` 4 macro-defined 'stack' structures store the state of the engine.
```c
// Stack with maximum capacity 5 storing Cubes 
STACK(Cube, 5, cubes);

// Vertices that must be rendered by the engine
typedef struct Vector Vertex;
STACK(Vertex, 100, vertices);

// Pixels on the screen that are colored when the Vertices
// are projected onto the 2D screen
STACK(Pixel2D, 1000, pixels);

// indices of the 'pixel' characters in the string that is printed 
// in the terminal to emulate a screen render
typedef int Index;
STACK(Index, HEIGHT*(WIDTH+1), pixel_indices);
```
In the main loop, all stacks are initially empty except for the cubes in 
the Cube stack supplied by the user or by some high level logic. 

The main loop starts by pushing the vertices of each cube to the vertices 
stack, then each vertex is passed to linear algebra functions to determine 
the resulting pixel position projected onto the screen due to each vertex.

These pixels are pushed to the pixels stack along with any other pixels such 
as pixels connecting the cube vertices to visualize the sides of the cube.

Finally, the rendering process begins and the main loop figures out the 
chars in the printed string that correspond to the pixels we want to show 
on the screen and places a '#' in those chars, saving the index location of 
each pixel in the pixel_indices stack in the process.

When the string is printed into the terminal and the current frame needs to 
be reset, the pixel_indices stack we populated comes in handy as we can 
quickly go through all the pixel indices in the string and reset them to 
allow the next main loop iteration to draw pixels on an 'empty frame'.

### Key points
- Extendable to support more polygons in `geometry.h`
- Handles all linear-algebra-related logic in `vector.c`
- Rendering logic and engine state is in `engine.c`

This project is for fun/testing purposes.
