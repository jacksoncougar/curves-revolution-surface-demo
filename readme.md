# CPSC 453 Assignment 5: Modelling
Jackson Cougar Wiebe

## Building

  mkdir build
  cd build
  cmake ../

## Params

arg0 is normal texture filename
arg1 is ambient occlusion texture filename
arg2 is roughness texture filename

## Controls

[Ctrl-S]	saves the current model to "model.data".
[Ctrl-L]	loads the current model from "model.data".
[D]			deletes the selected control point.
[E]			copies the selected control point.
[S]			adds a control point between the selected control point and its neighbor.

[Left MouseButton]		selects the control point under the mouse.
[Left MouseButton+Drag] moves the camera around the mesh viewport.

## Bonus Features implemented

-[x] Load and save the models
-[x] Improve rendering using texture mapping
-[x] Improve rendering using procedural textures

