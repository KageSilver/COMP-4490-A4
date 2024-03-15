/*----------------------------------------------------
   Tara Boulanger (7922331)
   File name:   a4tes.glsl

   Description: The tessellation evaluation shader for
   this program. It outputs the vertex position that
   it received from input.
------------------------------------------------------*/

#version 400

layout (quads, equal_spacing, ccw) in;

// just regular uniform variables
uniform mat4 ModelView;
uniform mat4 ModelViewInverseTranspose;
uniform mat4 Projection;
uniform vec4 LightPosition;

out vec4 N, L, E;

mat4 BEZIER = mat4(-1, 3,-3, 1,
					3,-6, 3, 0,
				   -3, 3, 0, 0,
				    1, 0, 0, 0);

mat4 BEZIER_T = transpose(BEZIER);

//in vec4 colorTES[];
//out vec4 out_colour;

void main() {
	float u = gl_TessCoord.s;
	vec4 vecU = vec4(pow(u,3), pow(u,2), u, 1);
	float v = gl_TessCoord.t;
	vec4 vecV = vec4(pow(v,3), pow(v,2), v, 1);
	
	mat4 pointsX = mat4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x, gl_in[2].gl_Position.x,  gl_in[3].gl_Position.x,
						gl_in[4].gl_Position.x, gl_in[5].gl_Position.x, gl_in[6].gl_Position.x,  gl_in[7].gl_Position.x,
						gl_in[8].gl_Position.x, gl_in[9].gl_Position.x, gl_in[10].gl_Position.x, gl_in[11].gl_Position.x,
						gl_in[12].gl_Position.x,gl_in[13].gl_Position.x,gl_in[14].gl_Position.x, gl_in[15].gl_Position.x);
	mat4 pointsY = mat4(gl_in[0].gl_Position.y, gl_in[1].gl_Position.y,  gl_in[2].gl_Position.y, gl_in[3].gl_Position.y,
						gl_in[4].gl_Position.y, gl_in[5].gl_Position.y,  gl_in[6].gl_Position.y, gl_in[7].gl_Position.y,
						gl_in[8].gl_Position.y, gl_in[9].gl_Position.y,  gl_in[10].gl_Position.y,gl_in[11].gl_Position.y,
						gl_in[12].gl_Position.y,gl_in[13].gl_Position.y, gl_in[14].gl_Position.y,gl_in[15].gl_Position.y);
	mat4 pointsZ = mat4(gl_in[0].gl_Position.z, gl_in[1].gl_Position.z, gl_in[2].gl_Position.z,  gl_in[3].gl_Position.z,
						gl_in[4].gl_Position.z, gl_in[5].gl_Position.z, gl_in[6].gl_Position.z,  gl_in[7].gl_Position.z,
						gl_in[8].gl_Position.z, gl_in[9].gl_Position.z, gl_in[10].gl_Position.z, gl_in[11].gl_Position.z,
						gl_in[12].gl_Position.z,gl_in[13].gl_Position.z,gl_in[14].gl_Position.z, gl_in[15].gl_Position.z);

	//x(u,v)=U.Mb.Gbx.Mb^t.v^t
	//Need to dot since we need to transpose v
	float x = dot(vecU,(BEZIER*(pointsX*(BEZIER_T*(vecV)))));
	float y = dot(vecU,(BEZIER*(pointsY*(BEZIER_T*(vecV)))));
	float z = dot(vecU,(BEZIER*(pointsZ*(BEZIER_T*(vecV)))));

	vec4 pos = vec4(x,y,z,1);
	
	// pos is now vertex position interpolated from barycentric coordinates
	gl_Position = Projection*ModelView*pos;

	//Normals are derivatives of the original vectors
	//Cross product the derivative to get the actual normal

	// we could also do per-vertex lighting calculations here, which would then
	// be done once per tessellated vertex instead of once per patch vertex

	L = LightPosition.xyza - pos;
	E = -pos;

	vec4 vNormal = normalize(pos);

	N = ( ModelViewInverseTranspose*vNormal ).xyza;

	// interpolate the color and send it to the fragment shader
	/*out_colour = gl_TessCoord.s * colorTES[0] +
			gl_TessCoord.t * colorTES[1] +
			gl_TessCoord.p * colorTES[2];*/
}