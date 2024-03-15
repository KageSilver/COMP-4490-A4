/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4tcs.glsl

   Description: The tessellation control shader for the
   program. Colouring happens elsewhere, so we just
   copy the input of the vertices to the output.
------------------------------------------------------*/

#version 400

// the number of vertices in the output patch; here it is the same as input
layout (vertices = 16) out;

//Takes in the number of subdivisions to have on each side of the quad
uniform int Tessellation;


void main() {
	if (gl_InvocationID == 0) {
		gl_TessLevelInner[0] = Tessellation;
		gl_TessLevelInner[1] = Tessellation;
		gl_TessLevelOuter[0] = Tessellation;
		gl_TessLevelOuter[1] = Tessellation;
		gl_TessLevelOuter[2] = Tessellation;
		gl_TessLevelOuter[3] = Tessellation;
	}//end if
	
	// copy input to output
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}//end main