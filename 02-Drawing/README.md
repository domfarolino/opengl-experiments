# Drawing Polygons

This folder corresponds to the code found in the [drawing polygons](https://open.gl/drawing) section of the https://open.gl tutorial.
The Makefile works nicely on Linux and macOS. This section taught about vertex buffer objects, vertex array objects, element buffer objects,
vertex shaders, fragment shaders, and drawing from both vertex buffer objects and element buffer objects and how we can be more memory efficient
by drawing from the latter. In the first part of this section we draw a white triangle:

![white triangle](screenshot0.png)

And in the second part we draw two triangles together to form a rectangle. Each vertex of the rectangle has a color associated with it which gives
us the rainbow effect below:

![rainbow rect](screenshot1.png)

# Exercises

 - To flip the rectangle (or equivalently the triangle) upside down, I modified the vertex shader to multiply
   the y position coordinate by -1.
 - To invert the colors of the rectangle I subtracted `1.0f` from each color component in the fragment shader
 - To make each vertex have a single color which represents it's shade of gray I removed two of the attributes
   from each vertex in the array, then varied the third attribute. I then cracked open the vertex shader to ensure
   that the `in color` and `out Color` were `float`s and not `vec3`s. This makes it a single value we're working with.
   Changes in the fragment shader must be made in order to accept this `float`, then the we must modify where we set our
   fragment color in the fragment shader, to use the `in float Color` for the `r`, `g`, and `b` values of the fragment color.
   This ensures the color is some shade of gray. Finally both our calls to `glVertexAttribPointer` must be modified to represent
   the new stride and offset. The `stride` changes from `5*sizeof(float)` to `3*sizeof(float)` for both calls, and the `offset`
   remains unchanged.
