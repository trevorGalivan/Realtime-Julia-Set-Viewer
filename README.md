# Realtime-Julia-Set-Viewer
Uses GLSL compute shaders to render an image of a julia set in real-time, based on your mouse location

Allows free zooming / panning, 4x supersampling, and choice between logarithmic and linear colour scale.
Also supports rendering the mandlebrot set, and quick switching between the julia and mandlebrot set rendering.

Requires GLAD, GLM, GLFW, and OpenGL 4.6

You can get the GLAD file used by this project here:
https://glad.dav1d.de/#profile=core&language=c&specification=gl&loader=on&api=gl%3D4.6
