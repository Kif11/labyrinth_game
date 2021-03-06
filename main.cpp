//
//  main.cpp
//  Labirinth
//
//  Created by Amy on 10/15/15.
//  Copyright (c) 2015 Amy. All rights reserved.
//

/*
 Labyrinth
 
 Write a program that outputs a simple grid­based game board to the screen using characters.
 Each turn, the player will have a chance to move.  If the player steps on a trap, they lose.  If the
 player makes it to the exit, they escape the labyrinth with their life!
 ● Choose unique characters to represent the player, exit, traps, and empty spaces.
 ● Display the new state of the board after every player turn.
 ● Do not allow the player to move outside of the game board!
 For example: ...............
 ..G............ G=Player
 ............... T=Trap
 ..T......T..... .=Emptyspace
 ............... X=Exit
 ....T..........
 .....T.........
 ...X...........
 Suggestions for additional features: (optional, but encouraged)
 ● Add some enemies that randomly move in a direction every turn.  If they encounter the
 player, the player loses.
 ● Add walls within the labyrinth that the player (and any enemies) cannot move through.
 ● Support multiple levels, so the exit of one level leads to the entrance of another.
 ● Add a notion of player score, which can be increased by picking up valuable objects
 (gold, etc.)
 ● Add support for color­coding the objects in the level:
 http://stackoverflow.com/questions/9262270/color­console­output­with­c­in­windows
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>     /* srand, rand */

#include "SDL2/SDL.h"
#include "OpenGL/gl3.h"
#include "GLUT/glut.h"


#define PROGRAM_NAME "Labirinth"

float cx = 0;
float cy = 0;
float cz = 0;

class Grid {
private:
    const int _gridSize = 10;
    //TODO: How to dynamicaly resize base on grid size?
    int _types[10][10];
public:
    
    GLfloat angle = 0;
    GLint orientX = 0;
    GLint orientY = 0;
    GLint orientZ = 0;
    GLfloat size;
    GLint LinesX;
    GLint LinesY;
    

    
    void draw() {
        // Set grid orientation
        glRotatef(angle, orientX, orientY, orientZ);
        glBegin(GL_LINES);
        for (int xc = 0; xc < LinesX; xc++)
        {
            glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
                       size / 2.0,
                       0.0);
            glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
                       size / -2.0,
                       0.0);
        }
        for (int yc = 0; yc < LinesY; yc++)
        {
            glVertex3f(	size / 2.0,
                        -size / 2.0 + yc / (GLfloat)(LinesY-1)*size,
                        0.0);
            glVertex3f(	size / -2.0,
                       -size / 2.0 + yc / (GLfloat)(LinesY-1)*size,
            0.0);
        }
        glEnd();
    }
    
    void placeTypes() {
        
        int typeNum = 4;
        
        srand(time(NULL));
        
        for (int i = 0; i < _gridSize; i++) {
            for (int j = 0; j < _gridSize; j++) {
                _types[i][j] = rand() % typeNum + 1;
            }
        }
    }
    
    int getTypes() {
        return _types[_gridSize][_gridSize];
    }
    
    void printTypes() {
        for (int i = 0; i < _gridSize; i++){
            std::cout << std::endl;
            for (int j = 0; j < _gridSize; j++){
                std::cout << _types[i][j];
            }
        }
        std::cout << std::endl;
    }
};

class Char {
protected:
    GLfloat _cR;
    GLfloat _cG;
    GLfloat _cB;
public:
    
    GLfloat posX = 0;
    GLfloat posY = 0;
    GLfloat posZ = 0;
    GLfloat posLimit = 4;
    
    void move(GLfloat x, GLfloat y, GLfloat z){
        if (x > 0) {
            if (posX < posLimit) {
                posX += x;
            }
        } else if (x < 0) {
            if (posX > -posLimit) {
                posX += x;
            }
        }
        if (y > 0) {
            if (posY < posLimit) {
                posY += y;
            }
        } else if (y < 0) {
            if (posY > -posLimit) {
                posY += y;
            }
        }
        
        // Not implemented
        if (posZ < posLimit) {
            posZ += z;
        }
        
    }
    
    void setColor( GLfloat cR,
                   GLfloat cG,
                   GLfloat cB) {
        _cR = cR;
        _cG = cG;
        _cB = cB;
    }
    
    
    void draw(){
        glColor3f(_cR, _cG, _cB);
        glTranslatef(posX, posY, posZ);
        GLUquadric *quad;
        quad = gluNewQuadric();
        gluSphere(quad,0.4,8,8);
        glColor3f(1, 1, 1);
    }
};

class Player: public Char {

};

class Enemie: public Char {
    
};


float earthAngle = 0;
float moonAngle = 0;

Player player;

Grid myGrid;


void renderScene(SDL_Window *mainwindow) {
    
    // Clear Color
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();
    
    gluLookAt(0, 0, 80,
              0, 0, 0,
              1, 0, 0);
    
    
    myGrid.draw();
    player.draw();
    
    SDL_GL_SwapWindow(mainwindow);

}

void placeObjects(){
    
    // TODO: Figure out how to handle arrays in C++ in procedurel way.
    
    //int types[10][] = myGrid.getTypes();
//    for (int i = 0; i < 10; i++){
//        std::cout << std::endl;
//        for (int j = 0; j < 10; j++){
//            std::cout << types[i][j];
//        }
//    }

}

void changeSize(int w, int h) {
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;
    float ratio = 1.0* w / h;
    
    // Set the viewport to be the entire window
    //glViewport(0, 0, std::min(w, h), std::min(w, h));
    glViewport(0, 0, w, h);
    
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    
    std::cout << "GL Projection: " << GL_PROJECTION << std::endl;
    
    // Reset Matrix
    glLoadIdentity();
    
    // Set the correct perspective.
    // https://www.opengl.org/sdk/docs/man2/xhtml/gluPerspective.xml
    gluPerspective(25, ratio, 0.01, 100.0);

    
    //GLKMatrix4 perspMatrix = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(26.0), ratio, 0.1, 1000.0);
    //glMultMatrixf(perspMatrix.m);
    
    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
    
    std::cout << "Window size: " << w << " x " << h << std::endl;
    std::cout << "Ratio: " << ratio << std::endl;
}

int main(int argc, const char * argv[]) {
    
    // Window size
    int SCREEN_WIDTH = 800;
    int SCREEN_HEIGHT = 600;
    
    SDL_Window *mainwindow; /* Our window handle */
    SDL_GLContext maincontext; /* Our opengl context handle */
    
    SDL_Init(SDL_INIT_VIDEO);
    
    mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    /* Create our opengl context and attach it to our window */
    maincontext = SDL_GL_CreateContext(mainwindow);
    
    /* This makes our buffer swap syncronized with the monitor's vertical refresh */
    SDL_GL_SetSwapInterval(1);
    
    player.setColor(0, 1, 0);
    myGrid.size = 9;
    myGrid.LinesX = 10;
    myGrid.LinesY = 10;
    myGrid.placeTypes();
    myGrid.printTypes();
    
    SDL_Event event;
    bool running = true;
    // Initialize camera
    changeSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    while (running) {
        Uint32 start = SDL_GetTicks();
        //std::cout << "Start: " << start << std::endl;
        while (SDL_PollEvent(&event)) {
            
            switch (event.type) {
                    
                // Capture any window event
                // https://wiki.libsdl.org/SDL_WindowEvent
                case SDL_WINDOWEVENT:
                    switch (event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            int w = event.window.data1;
                            int h = event.window.data2;
                            changeSize(w, h);
                            break;
                    }
                    break;
                    
                // Handle keyboar events
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            std::cout << "Up key pressed" << std::endl;
                            player.move(1, 0, 0);
                            std::cout << "Player pos X " << player.posX << std::endl;
                            break;
                        case SDLK_DOWN:
                            std::cout << "Down key pressed" << std::endl;
                            player.move(-1, 0, 0);
                            std::cout << "Player pos X " << player.posX << std::endl;
                            break;
                        case SDLK_LEFT:
                            std::cout << "Left key pressed" << std::endl;
                            player.move(0, 1, 0);
                            std::cout << "Player pos Y " << player.posY << std::endl;
                            break;
                        case SDLK_RIGHT:
                            std::cout << "Right key pressed" << std::endl;
                            player.move(0, -1, 0);
                            std::cout << "Player pos Y " << player.posY << std::endl;
                            break;
                    }
                    break;
                    
                case SDL_QUIT:
                    running = false;
                    break;
            }
        }
        
        renderScene(mainwindow);
        
        // Insure that screen update happen no more then 60 fps
        //std::cout << "End: " << SDL_GetTicks() << std::endl;
        if (1000/60>(SDL_GetTicks() - start)) {
            SDL_Delay(1000/60 - (SDL_GetTicks() - start));
        }
    }
    
    /* Delete our opengl context, destroy our window, and shutdown SDL */
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
    
    return 0;
}
