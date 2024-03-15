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


// Terrain Requirements
//---------------------------------------------------------------------------
const int NUM_PATCHES = 4; //Number of patches in a group
const int NUM_ROWS = 10;
const int NUM_COLUMNS = 12;

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


const glm::vec3 START_POSITION = glm::vec3(-14.0,0.0,0.5);


// Camera Requirements
//--------------------------------------------------------------------------
const float FLY_SPEED = 0.01f; //How fast the camera is flying over the terrain
const float EXTREME = 10.0f;

// Array of rotation angles for each coordinate axis, in angles (taken from ex8)
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int Axis = Yaxis;
GLfloat Tilt[NumAxes] = { 0.0, 0.0, 0.0 };
glm::vec3 CameraPosition = glm::vec3(0.0,0.0,0.0);

bool isMoving = true;
glm::vec3 Movement = glm::vec3(0.0,0.0,FLY_SPEED);


// Subdivisions
//---------------------------------------------------------------------------
int m = 2;
int n = 0;
int sub = pow(n+m,2);

bool displayOutlines = true;



// Shader program variables
//------------------------------------------------------------
GLuint Program;

// Model-view matrix uniform location
GLuint ModelView, Projection, Outline, Tessellation;


// Start of OpenGL drawing
//-------------------------------------------------------------------

// OpenGL initialization
void init() {
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
    // Making sure it has enough space for a single group of patches
    glBufferData(GL_ARRAY_BUFFER, sizeof(patchPoints), patchPoints, GL_STATIC_DRAW);
    
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
    Tessellation = glGetUniformLocation(Program, "Tessellation");

    glUniform1i(Tessellation, sub);

    glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

    //Determines how many vertices are grouped into a patch (patches, so 16)
    glPatchParameteri(GL_PATCH_VERTICES, PATCH_VERTICES);

    glEnable(GL_DEPTH_TEST);

    glShadeModel(GL_FLAT);

    glClearColor(0.539, 0.7539, 0.8984, 1.0);
}//end init


// Helper drawing function
//----------------------------------------------------------------

//Used for drawing a singular patch
void drawPatch( glm::mat4 modelView, int patchNum ) {
    glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(modelView));
    glUniform1i(Outline, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //Fill in the vertices
    glDrawArrays(GL_PATCHES, PATCH_VERTICES*patchNum, PATCH_VERTICES);
    
    glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(modelView * glm::translate(glm::mat4(), glm::vec3(0.0,0.0,0.001))));
    if (displayOutlines) {
        glUniform1i(Outline, 1);
    }//end if
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Drawing the outlines
    glLineWidth(1.5f);
    glDrawArrays(GL_PATCHES, PATCH_VERTICES*patchNum, PATCH_VERTICES);
}//end drawPatch


//Used for drawing a group of 4 patches on the terrain
void drawGroup( glm::mat4 modelView ) {
    //First patch
    glm::mat4 patchModel = modelView;
    drawPatch(patchModel,0);

    //Second patch
    patchModel = modelView*glm::translate(glm::mat4(), glm::vec3(2.0,0.0,0.0));
    drawPatch(patchModel,1);

    //Third patch
    patchModel = modelView*glm::translate(glm::mat4(), glm::vec3(0.0,0.0,2.0));
    drawPatch(patchModel,2);

    //Fourth patch
    patchModel = modelView*glm::translate(glm::mat4(), glm::vec3(2.0,0.0,2.0));
    drawPatch(patchModel,3);
}//end drawGroup


//Used for generating the entire terrain
void drawTerrain( glm::mat4 modelView ) {
    glm::mat4 groupView = modelView;
    glm::vec3 translation = glm::vec3();
    float xOffset = START_POSITION.x;
    float zOffset = START_POSITION.z;
    //Need to offset them by 4 units as each group is 4 units wide
    for ( int i=0; i<NUM_ROWS; i++ ) {
        translation.z = zOffset;
        for ( int j=0; j<NUM_COLUMNS; j++ ) {
            translation.x = xOffset;
            groupView = modelView*glm::translate(glm::mat4(),translation);
            drawGroup(groupView);
            xOffset += 4.0f;
        }//end for
        zOffset -= 4.0f;
        xOffset = START_POSITION.x;
    }//end for
}//end drawTerrain


// OpenGL display
//------------------------------------------------------------------
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Generate the model-view matrix
    const glm::vec3 viewerPos(0.0, 1.0, 0.0);
    
    glm::mat4 trans, cameraRot, rot, modelView;

    //Moving to the viewers position
    trans = glm::translate(trans, -viewerPos);

    trans = glm::translate(trans, Movement);
    if ( Movement.z > 1.0f ) {
        Movement.z = 0.0;
    }//end if
    printf("X: %f\tY: %f\tZ: %f\n", Movement.x, Movement.y, Movement.z);

    cameraRot = glm::rotate(cameraRot, glm::radians(Tilt[Xaxis]), glm::vec3(1,0,0));
    cameraRot = glm::rotate(cameraRot, glm::radians(Tilt[Zaxis]), glm::vec3(0,0,1));

    modelView = modelView*trans*glm::scale(glm::mat4(), glm::vec3(0.25,0.25,0.25));
    modelView = modelView*cameraRot;

    //Drawing the entire terrain
    drawTerrain(modelView);

    glutSwapBuffers();
}//end display


//Other OpenGL drawing functions
//----------------------------------------------------------------------------

void update(void){
    if ( isMoving ) {
        Movement[Zaxis] += FLY_SPEED;
    }//end if
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
    float rate = 1.0f; //The angle to tilt the camera by
    switch (key) {
        case 033: // Escape Key
        case 'q':
        case 'Q':
            exit(EXIT_SUCCESS);
            break;
        case '1':
            n = 0;
            sub = pow(n+m,2);
            glUniform1i(Tessellation, sub);
            break;
        case '2':
            n = 1;
            sub = pow(n+m,2);
            glUniform1i(Tessellation, sub);
            break;
        case '3':
            n = 2;
            sub = pow(n+m,2);
            glUniform1i(Tessellation, sub);
            break;
        case '4':
            n = 3;
            sub = pow(n+m,2);
            glUniform1i(Tessellation, sub);
            break;
        case '5':
            n = 4;
            sub = pow(n+m,2);
            glUniform1i(Tessellation, sub);
            break;
        case '6':
            n = 5;
            sub = pow(n+m,2);
            glUniform1i(Tessellation, sub);
            break; 
        case '7':
            n = 6;
            sub = pow(n+m,2);
            glUniform1i(Tessellation, sub);
            break; 
        case '8':
            n = 7;
            sub = pow(n+m,2);
            glUniform1i(Tessellation, sub);
            break; 
        case '9':
            n = 8;
            sub = pow(n+m,2);
            glUniform1i(Tessellation, sub);
            break;
        case 'w':
            //tilt camera down by rotating down x axis
            if ( CameraPosition.x > -EXTREME/2 ) {
                CameraPosition.x -= rate;
                Tilt[Xaxis] += rate;
            }//end if
            break;
        case 'a':
            //tilt camera left by rotating left along z axis
            if ( CameraPosition.z < EXTREME ) {
                CameraPosition.z += rate;
                Tilt[Zaxis] -= rate;
            }//end if
            break;
        case 's':
            //tilt camera up by rotating up on x axis
            if ( CameraPosition.x < EXTREME/2 ) {
                CameraPosition.x += rate;
                Tilt[Xaxis] -= rate;
            }//end if
            break;
        case 'd':
            //tilt camera right by rotating right along z axis
            if ( CameraPosition.z > -EXTREME ) {
                CameraPosition.z -= rate;
                Tilt[Zaxis] += rate;
            }//end if
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
    glm::mat4  projection = glm::perspective( glm::radians(45.0f), aspect, 0.5f, 30.0f );

    glUniformMatrix4fv( Projection, 1, GL_FALSE, glm::value_ptr(projection) );
}//end reshape
