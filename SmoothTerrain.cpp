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
const char *WINDOW_TITLE = "Smooth Terrain";
const double FRAME_RATE_MS = 1000.0 / 60.0;

int windowWidth = 0;
int windowHeight = 0;


// Bezier Patches Requirements
//---------------------------------------------------------------------------
const int NUM_PATCHES = 4;
const int NUM_GROUPS = 20; //Arbitrary, fix later
const int ALL_PATCHES = 1;//NUM_PATCHES*NUM_GROUPS;
const int NUM_COLOURS = 5;

typedef glm::vec4 point4;

const int PATCH_VERTICES = 16;
const int GROUP_VERTICES = PATCH_VERTICES * NUM_PATCHES; // patches; don't worry about duplicate vertices

//The points of the vertices of the patch
point4 patchPoints[GROUP_VERTICES] = {
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


const int WORLD_VERTICES = GROUP_VERTICES*NUM_GROUPS;

glm::vec4 worldVertices[WORLD_VERTICES];

//The colours of the mountains
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
//Will need to translate each patch in the group by 2 units in whichever
//direction so that it can generate the patch accordingly.
//The control points in the corners must be connected.
void createPatch(int p0, int p1, int p2, int p3) {
    float x = 0;
    float y = 0;
    glm::vec4 tValues;
    glm::vec4 xVertex;
    glm::vec4 yVertex;
    glm::mat4 xValues;
}//end createPatch

// Used to build all of the vertices for the terrain
void buildTerrain() {
    //Redo logic here
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

// OpenGL initialization
void init() {
    //buildTerrain();
    // Need global access to the VAO
    GLuint VAO;
    // Create vertex array objects
    glGenVertexArrays(1, &VAO);
    // Loading in the buffer for the patches
    glBindVertexArray(VAO);

    // Here for size of stack allocation
    GLuint buffer;

    // Creating and initializing a buffer object
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // Making sure it has enough space for the whole terrain
    glBufferData(GL_ARRAY_BUFFER, sizeof(patchPoints), patchPoints, GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(patchesVertices), patchesVertices, GL_STATIC_DRAW);
    
    // Load shader set
    Program = InitShader("a4vshader.glsl", "a4fshader.glsl", "a4tcs.glsl", "a4tes.glsl");
    glUseProgram(Program);

    // Set up vertex data for the vao
    GLuint vPosition = glGetAttribLocation(Program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

    // Retrieve transformation uniform variable locations
    ModelView = glGetUniformLocation(Program, "ModelView");
    Projection = glGetUniformLocation(Program, "Projection");
    Outline = glGetUniformLocation(Program, "Outline");

    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    //Determines how many vertices are grouped into a patch (patches, so 16)
    glPatchParameteri(GL_PATCH_VERTICES, PATCH_VERTICES);

    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_FLAT);

    glClearColor(1.0, 1.0, 1.0, 1.0);
}//end init


// Helper drawing function
//----------------------------------------------------------------

//Used for drawing the splines in our scene
void drawPatches( glm::mat4 model_view ) {

    glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(model_view));
    glUniform1i(Outline, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Fill in the vertices

    glDrawArrays(GL_PATCHES, 0, PATCH_VERTICES);
    //Fill in all of the patches
    //for ( int i=0; i<ALL_PATCHES; i++ ) {
    //    glDrawArrays(GL_PATCHES, i*ALL_PATCHES, PATCH_VERTICES);
    //}//end for
    
    glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(model_view * glm::translate(glm::mat4(), glm::vec3(0.0,0.0,0.001))));
    glUniform1i(Outline, 1);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Drawing the outlines
    glLineWidth(1.5f);
    glDrawArrays(GL_PATCHES, 0, PATCH_VERTICES);

    //Draw all of the outlines to make up the patch
    //for ( int i=0; i<ALL_PATCHES; i++ ) {
    //    glDrawArrays(GL_PATCHES, i*ALL_PATCHES, PATCH_VERTICES);
    //}//end for

}//end drawPatches


// OpenGL display
//------------------------------------------------------------------
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Generate the model-view matrix
    const glm::vec3 viewer_pos(0.0, 0.0, 2.0);
    
    glm::mat4 trans, cameraRot, rot, model_view;

    trans = glm::translate(trans, -viewer_pos);

    model_view = trans;//*glm::scale(glm::mat4(), glm::vec3(0.5,0.5,0.5));

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
            break;
    }//end switch-case
}//end keyboard


//Kept here as black magic
void reshape (int width, int height) {
    glViewport( 0, 0, width, height );

    GLfloat aspect = GLfloat(width)/height;
    glm::mat4  projection = glm::perspective( glm::radians(45.0f), aspect, 0.5f, 10.0f );

    glUniformMatrix4fv( Projection, 1, GL_FALSE, glm::value_ptr(projection) );
}//end reshape
