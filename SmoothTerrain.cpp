/*----------------------------------------------------
   COMP 4490 Winter 2024 Assignment 4
   Tara Boulanger (7922331)
   John Braico
   File name:   SmoothTerrain.cpp

   Description: This program draws an infinitely scrolling
   3D world using tesselation shaders to draw smooth
   terrain made from Bezier patches.
------------------------------------------------------*/

//Includes
#include "common.h"

#include <iostream>
#include <chrono>
#include <cstdlib>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

//Program Constants
const char *WINDOW_TITLE = "Flight Simulator";
const double FRAME_RATE_MS = 1000.0 / 60.0;

int windowWidth = 0;
int windowHeight = 0;


// Bezier Patches Requirements
//---------------------------------------------------------------------------
const int NUM_PATCHES = 4;
const int NUM_COLOURS = 5;

typedef glm::vec4  point4;

const int PATCH_VERTICES = 16 * 4; // patches; don't worry about duplicate vertices
glm::vec4 patchesVertices[PATCH_VERTICES];

//The points of the vertices of the patch
point4 points[PATCH_VERTICES] = {
  // vertex order: p00 p01 p02 p03 p10 p11 p12 ... p33 with p00 bottom-left, p01 closer to the next patch + 1
  
  point4( -1.0, -0.5,  1.0, 1.0 ),
  point4( -0.5,  0.5,  1.0, 1.0 ),
  point4(  0.5,  0.5,  1.0, 1.0 ),
  point4(  1.0, 0.25,  1.0, 1.0 ),

  point4( -1.0,-0.75,  0.5, 1.0 ),
  point4( -0.5,-0.75,  0.5, 1.0 ),
  point4(  0.5, -0.5,  0.5, 1.0 ),
  point4(  1.0, -0.5,  0.5, 1.0 ),

  point4( -1.0, -0.5, -0.5, 1.0 ),
  point4( -0.5, -0.5, -0.5, 1.0 ),
  point4(  0.5,-0.44, -0.5, 1.0 ),
  point4(  1.0,-0.15, -0.5, 1.0 ),

  point4( -1.0,  0.4, -1.0, 1.0 ),
  point4( -0.5, -0.5, -1.0, 1.0 ),
  point4(  0.5,  0.5, -1.0, 1.0 ),
  point4(  1.0, -0.5, -1.0, 1.0 ),


  point4( -1.0, 0.25,  1.0, 1.0 ),
  point4( -0.5,  0.5,  1.0, 1.0 ),
  point4(  0.5,  0.5,  1.0, 1.0 ),
  point4(  1.0, -0.5,  1.0, 1.0 ),

  point4( -1.0, -0.5,  0.5, 1.0 ),
  point4( -0.5,  0.5,  0.5, 1.0 ),
  point4(  0.5,-0.75,  0.5, 1.0 ),
  point4(  1.0,-0.75,  0.5, 1.0 ),

  point4( -1.0,-0.15, -0.5, 1.0 ),
  point4( -0.5, -0.5, -0.5, 1.0 ),
  point4(  0.5,-0.44, -0.5, 1.0 ),
  point4(  1.0, -0.5, -0.5, 1.0 ),

  point4( -1.0, -0.5, -1.0, 1.0 ),
  point4( -0.5, -0.5, -1.0, 1.0 ),
  point4(  0.5,  0.5, -1.0, 1.0 ),
  point4(  1.0,  0.4, -1.0, 1.0 ),


  point4( -1.0,  0.4, 1.0, 1.0 ),
  point4( -0.5, -0.5, 1.0, 1.0 ),
  point4(  0.5,  0.5, 1.0, 1.0 ),
  point4(  1.0, -0.5, 1.0, 1.0 ),

  point4( -1.0,-0.75,  0.5, 1.0 ),
  point4( -0.5,-0.75,  0.5, 1.0 ),
  point4(  0.5, -0.5,  0.5, 1.0 ),
  point4(  1.0, -0.5,  0.5, 1.0 ),

  point4( -1.0, -0.5, -0.5, 1.0 ),
  point4( -0.5, -0.5, -0.5, 1.0 ),
  point4(  0.5,-0.44, -0.5, 1.0 ),
  point4(  1.0, -0.5, -0.5, 1.0 ),

  point4( -1.0, -0.5, -1.0, 1.0 ),
  point4( -0.5,  0.5, -1.0, 1.0 ),
  point4(  0.5,  0.5, -1.0, 1.0 ),
  point4(  1.0, 0.25, -1.0, 1.0 ),


  point4( -1.0, -0.5, 1.0, 1.0 ),
  point4( -0.5, -0.5, 1.0, 1.0 ),
  point4(  0.5,  0.5, 1.0, 1.0 ),
  point4(  1.0,  0.4, 1.0, 1.0 ),

  point4( -1.0, -0.5,  0.5, 1.0 ),
  point4( -0.5,  0.5,  0.5, 1.0 ),
  point4(  0.5,-0.75,  0.5, 1.0 ),
  point4(  1.0,-0.75,  0.5, 1.0 ),

  point4( -1.0, -0.5, -0.5, 1.0 ),
  point4( -0.5, -0.5, -0.5, 1.0 ),
  point4(  0.5,-0.44, -0.5, 1.0 ),
  point4(  1.0, -0.5, -0.5, 1.0 ),

  point4( -1.0, 0.25, -1.0, 1.0 ),
  point4( -0.5,  0.5, -1.0, 1.0 ),
  point4(  0.5,  0.5, -1.0, 1.0 ),
  point4(  1.0, -0.5, -1.0, 1.0 )
};

//The colours of the mountain
glm::vec4 colours[NUM_COLOURS] = {
    glm::vec4(0.0588,0.3216,0.7294,1.0), //Blue water
    glm::vec4(0.2667,0.6196,0.1137,1.0), //Green grass
    glm::vec4(0.6627,0.6627,0.6627,1.0), //Grey mountains
    glm::vec4(0.8706,0.9882,1.0,1.0), //White peaks
    glm::vec4(0.0,0.0,0.0,1.0) //Black for triangles
};


// Camera Requirements
//--------------------------------------------------------------------------
const float CAMERA_SPEED = 0.05f;
bool isMoving = true;


// Subdivisions
//---------------------------------------------------------------------------
int m = 6;
int n = 1;
//pow(n+m,2)

bool displayOutlines = true;


// Program-required variables
//----------------------------------------------------------------------------

// Need global access to VAOs
GLuint VAOs[1];


// Shader program variables
//------------------------------------------------------------
GLuint Program;

// Model-view matrix uniform location
GLuint ModelView, Projection, Outline;



// Splines Functions
//----------------------------------------------------------------------------

int counterBezier = 0;
// Used to create the vertices along a bezier spline segment.
// Takes in a variable for the offset positions of the control points.
void createPatch(int p0, int p1, int p2, int p3) {
    float x = 0;
    float y = 0;
    glm::vec4 tValues;
    glm::vec4 xVertex;
    glm::vec4 yVertex;
    glm::mat4 xValues;
    /*xValues[0] = glm::vec4(initialControlPoints[p0].x,
                            initialControlPoints[p1].x,
                            initialControlPoints[p2].x,
                            initialControlPoints[p3].x);
    glm::mat4 yValues;
    yValues[0] = glm::vec4(initialControlPoints[p0].y,
                            initialControlPoints[p1].y,
                            initialControlPoints[p2].y,
                            initialControlPoints[p3].y);
    for ( float i=0; i<=1; i+=t ) {
        tValues = glm::vec4(pow(i,3),pow(i,2),i,1);
        xVertex = tValues*BezierBasis*xValues[0];
        x = xVertex[0]+xVertex[1]+xVertex[2]+xVertex[3];
        yVertex = tValues*BezierBasis*yValues[0];
        y = yVertex[0]+yVertex[1]+yVertex[2]+yVertex[3];
        bezierVertices[counterBezier++] = glm::vec4(x,y,0.0,1.0);
    }//end for
    */
}//end createPatch

// Used to build all of the vertices for the terrain
void buildTerrain() {
    for (int j=0; j<5; j++) {
        for (int i = 0; i<PATCH_VERTICES; i++) {
            //Setting the control points
            int points[4] = {i-1,i,i+1,i+2};
            if ( i == 0 ) {
                points[0] = PATCH_VERTICES-1;
            } else if ( (i+2) % PATCH_VERTICES == 0 ) {
                points[3] = 0;
            } else if ( (i+1) % PATCH_VERTICES == 0 ) {
                points[2] = 0;
                points[3] = 1;
            }//end if-else
            if ( i % 4 == 0 ) {
                createPatch(points[0],points[1],points[2],points[3]);
            }//end if
        }//end for
    }//end for
}//end buildTerrain


// Start of OpenGL drawing
//-------------------------------------------------------------------

// Used to load the buffer
void loadBuffer(GLuint vPosition) {
    // Here for size of stack allocation
    GLuint buffer;

    // Loading in the buffer for the curves
    glBindVertexArray(VAOs[0]);

    // Creating and initializing a buffer object
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Making sure it has enough space for just the curves
    glBufferData(GL_ARRAY_BUFFER, sizeof(patchesVertices), patchesVertices, GL_STATIC_DRAW);
    
    // Set up vertex data for this vao
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}//end loadBuffer


// OpenGL initialization
void init() {
    buildTerrain();

    // Create vertex array objects
    glGenVertexArrays(1, VAOs);

    // Load shader set
    Program = InitShader("a4vshader.glsl", "a4fshader.glsl", "a4tcs.glsl", "a4tes.glsl");
    //Might need something more for tesselation shader
    glUseProgram(Program);
    GLuint vPosition = glGetAttribLocation(Program, "vPosition");

    loadBuffer(vPosition);
    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation(Program, "ModelView");
    Projection = glGetUniformLocation(Program, "Projection");
    Outline = glGetUniformLocation(Program, "Outline");

    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    //Determines how many vertices are grouped into a patch (triangles, so 3)
    glPatchParameteri(GL_PATCH_VERTICES, 3);

    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_FLAT);

    glClearColor(1.0, 1.0, 1.0, 1.0);
}//end init


// Helper drawing function
//----------------------------------------------------------------

//Used for drawing the splines in our scene
void drawPatches( glm::mat4 model_view ) {
    glUseProgram(Program);

    glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(model_view));
    glUniform1i(Outline, 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Drawing the outlines
    glLineWidth(1.5f);

    //Draw all of the outlines to make up the patch
    for ( int i=0; i<NUM_PATCHES; i++ ) {
        glDrawArrays(GL_PATCHES, i, PATCH_VERTICES);
    }//end for

    glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(model_view * glm::translate(glm::mat4(), glm::vec3(0.0,0.0,0.01))));
    glUniform1i(Outline, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Fill in the vertices

    //Fill in all of the patches
    for ( int i=0; i<NUM_PATCHES; i++ ) {
        glDrawArrays(GL_PATCHES, i, PATCH_VERTICES);
    }//end for

}//end drawPatches


// OpenGL display
//------------------------------------------------------------------
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Generate the model-view matrix
    const glm::vec3 viewer_pos(0.0, 2.0, 2.0);
    
    glm::mat4 trans, cameraRot, rot, model_view;

    //Drawing the splines
    drawPatches(model_view);

    glutSwapBuffers();
}//end display


//Other OpenGL drawing functions
//----------------------------------------------------------------------------

void update(void){
    //Move the terrain in this function
}//end update

//For mouse inputs
//Clicking stops the movement of the terrain
void mouse(int button, int state, int x, int y) {
    if ( state==GLUT_DOWN) {
        if ( isMoving ) 
            isMoving = false;
        else
            isMoving = true;
    }//end if
}//end mouse


//Z key toggles outlines display
//WASD keys rotate the camera
//Number keys change the tesselation values accordingly.
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 033: // Escape Key
        case 'q':
        case 'Q':
            exit(EXIT_SUCCESS);
            break;
        case '1':
            n = 1;
            break;
        case '2':
            n = 2;
            break;
        case '3':
            n = 3;
            break;
        case '4':
            n = 4;
            break;
        case '5':
            n = 5;
            break;
        case '6':
            n = 6;
            break; 
        case '7':
            n = 7;
            break; 
        case '8':
            n = 8;
            break; 
        case '9':
            n = 9;
            break;
        case 'w':
            //tilt camera down by rotating down x axis
            break;
        case 'a':
            //tilt camera left by rotating left along z axis
            break;
        case 's':
            //tilt camera up by rotating up on x axis
            break;
        case 'd':
            //tilt camera right by rotating right along z axis
            break;
        case 'z':
            if (displayOutlines)
                displayOutlines = false;
            else
                displayOutlines = true;
    }//end switch-case
}//end keyboard


//Kept here as black magic
void reshape (int width, int height) {
    glViewport( 0, 0, width, height );

    GLfloat aspect = GLfloat(width)/height;
}//end reshape
