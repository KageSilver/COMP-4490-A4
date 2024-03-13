/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4fshader.glsl

   Description: 
------------------------------------------------------*/
#version 150

in vec4 input_colour;

out vec4 out_colour;
uniform bool Outline;

void main() {
    if (Outline) {
        out_colour = vec4(0.0,0.0,0.0,1.0);
    } else {
        out_colour = input_colour;
    }//end if-else
}//end main
