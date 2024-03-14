/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4fshader.glsl

   Description: 
------------------------------------------------------*/
#version 150

//in vec4 input_colour;

out vec4 fColor;
uniform bool Outline;

//Use the z value to determine the colours.

void main() {
    if (Outline) {
        fColor = vec4(0.0,0.0,0.0,1.0);
    } else {
        fColor = vec4(0.0,1.0,1.0,1.0);//input_colour;
    }//end if-else
}//end main
