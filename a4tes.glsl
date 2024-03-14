/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4tes.glsl

   Description: The tessellation evaluation shader for
   this program. It outputs the vertex position that
   it received from input.
------------------------------------------------------*/

#version 400

// this is invoked from the TCS, once for each generated vertex
// like a vertex shader, it needs to output vertex position in gl_Position
// as input, it receives:
//  - gl_in[GL_PATCH_VERTICES] as the TCS
//  - vec3 gl_TessCoord, which is the barycentric coordinate of the vertex
//  - any arrays ouput from the TCS (must be declared)
// it can also output per-fragment values to the fragment shader

// this line is required: layout (...) in;
// the first value "quads" is required
//   and is used to choose how the patch will be tessellated
// the other two are the default (spacing between vertices, winding)
layout (quads, equal_spacing, ccw) in;

// just regular uniform variables
uniform mat4 ModelView;
uniform mat4 Projection;

//in vec4 colorTES[];
//out vec4 out_colour;

void main() {
	float u = gl_TessCoord.s;
	float v = gl_TessCoord.t;

	//Only renders the corners
	//Use the bezier patches stuff
	//Multiply it through the bezier matrix and the control point matrix and u vector and then v vector, and transpose bezier matrix
	//Do it for x, y, and z coordinates, then put it in the position
	vec4 p1 = gl_in[0].gl_Position*(1-u) + gl_in[3].gl_Position*u;
	vec4 p2 = gl_in[12].gl_Position*(1-u) + gl_in[15].gl_Position*u;
	vec4 pos = p1*(1-v) + p2*v;
	
	// pos is now vertex position interpolated from barycentric coordinates
	gl_Position = Projection * ModelView * pos;

	//Normals are derivatives of the original vectors
	//Cross product the derivative to get the actual normal

	// we could also do per-vertex lighting calculations here, which would then
	// be done once per tessellated vertex instead of once per patch vertex
	
	// and of course here is where we can do interesting things with our vertex
	// e.g.: push each patch "out" based on its distance from the vertices
	//       (this has to be done in model space in order to be interesting;
	//        don't forget to turn off transformation in the vshader too)
	// gl_Position = Projection * ModelView * (pos + vec4(0,0,0.2,0) * (length(gl_TessCoord)-sqrt(2)));
	
	// interpolate the color and send it to the fragment shader
	/*out_colour = gl_TessCoord.s * colorTES[0] +
			gl_TessCoord.t * colorTES[1] +
			gl_TessCoord.p * colorTES[2];*/
}