/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4vshader.glsl

   Description: Trivial. No illumination happens in
   here. Will need to happen elsewhere. Passes the
   vertex positions to the tcs to do the transformations.
------------------------------------------------------*/
#version 150

in vec4 vPosition;

void main() {
    gl_Position = vPosition;
}//end main

