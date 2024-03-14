/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4tcs.glsl

   Description: The tessellation control shader for the
   program. Colouring happens elsewhere, so we just
   copy the input of the vertices to the output.
------------------------------------------------------*/

#version 400

// this is invoked once for each vertex in the input patch
// the vertex has already gone through the vertex shader
// each invocation has access to all of the patch's data, in arrays

// default input is in an array gl_in[GL_PATCH_VERTICES] (from C++ code)
// it is an array of structs; one field is gl_Position from the vertex shader

// the number of vertices in the output patch; here it is the same as input
layout (vertices = 16) out;

// the color array is received from the output of the vertex shader
// (again, one array element per vertex in the patch)
// the output is sent to the tessellation evaluation shader
//in vec4 color[];
//out vec4 colorTES[];

void main() {
	if (gl_InvocationID == 0) {
		gl_TessLevelInner[0] = 11;
		gl_TessLevelInner[1] = 11;
		gl_TessLevelOuter[0] = 11;
		gl_TessLevelOuter[1] = 11;
		gl_TessLevelOuter[2] = 11;
		gl_TessLevelOuter[3] = 11;
	}//end if
	
	// copy input to output
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	//colorTES[gl_InvocationID] = color[gl_InvocationID];
}//end main