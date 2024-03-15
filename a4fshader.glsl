/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4fshader.glsl

   Description: This program does the colouring for the
   terrain, whether it's the outlines, or the actual
   mountains.
------------------------------------------------------*/
#version 400

in vec4 N, L, E;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

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
    //If we're just drawing the outline...
    if (Outline) {
        fColor = vec4(0.0,0.0,0.0,1.0);
    //If we're doing the coloured parts
    } else {
        //fColor = vec4(0.0,1.0,1.0,1.0);//input_colour;
        vec4 H = normalize( L + E );
        vec4 ambient = AmbientProduct;

        float Kd = max( dot(L, N), 0.0 );
        vec4  diffuse = Kd * DiffuseProduct;

        float Ks = pow( max(dot(N, H), 0.0), Shininess );
        vec4  specular = Ks * SpecularProduct;
        
        if ( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }//end if

        fColor = ambient + diffuse + specular;
        fColor.a = 1.0;
    }//end if-else
}//end main
