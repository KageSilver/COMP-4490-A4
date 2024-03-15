/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4fshader.glsl

   Description: 
------------------------------------------------------*/
#version 150

//in vec4 input_colour;

out vec4 fColor;
uniform bool Outline;


//The colours of the mountains
uniform vec4 colours[5] = {
    vec4(0.2353, 0.3961, 0.6314, 1.0), //Blue water
    vec4(0.5843, 0.8588, 0.4667, 1.0), //Green grass
    vec4(0.6627,0.6627,0.6627,1.0), //Grey mountains
    vec4(0.8706,0.9882,1.0,1.0), //White peaks
    vec4(0.0,0.0,0.0,1.0) //Black for outlines
};



//Use the z value to determine the colours.

void main() {
    if (Outline) {
        fColor = vec4(0.0,0.0,0.0,1.0);
    } else {
        fColor = vec4(0.0,1.0,1.0,1.0);//input_colour;
    }//end if-else
}//end main
