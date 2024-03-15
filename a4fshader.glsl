/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4fshader.glsl

   Description: This program does the colouring for the
   terrain, whether it's the outlines, or the actual
   mountains.
------------------------------------------------------*/
#version 400

in vec3 N, L, E;
in float yValue;

uniform vec4 LightAmbient, LightDiffuse, LightSpecular;
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

uniform float intervals[4] = {-0.25,0.0,0.3,0.5};


//Use the z value to determine the colours.

void main() {
    //If we're just drawing the outline...
    if (Outline) {
        fColor = vec4(0.0,0.0,0.0,1.0);
    //If we're doing the coloured parts
    } else {
        //Choosing which colour to display
        vec4 chosenColor = vec4(0.0,0.0,0.0,1.0);
        if ( yValue <= intervals[3] ) {
            chosenColor = colours[3];
            if ( yValue <= intervals[2] ) {
                chosenColor = colours[2];
                if ( yValue <= intervals[1] ) {
                    chosenColor = colours[1];
                    if ( yValue <= intervals[0] ) {
                        chosenColor = colours[0];
                    }//end if
                }//end if
            }//end if
        }//end if

        vec3 H = normalize( L + E );
        vec4 ambient = LightAmbient*chosenColor;

        float Kd = max( dot(L, N), 0.0 );
        vec4  diffuse = Kd * LightDiffuse*chosenColor;

        float Ks = pow( max(dot(N, H), 0.0), Shininess );
        vec4  specular = Ks * LightSpecular*chosenColor;
        
        if ( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }//end if

        fColor = ambient + diffuse + specular;
        fColor.a = 1.0;
    }//end if-else
}//end main
