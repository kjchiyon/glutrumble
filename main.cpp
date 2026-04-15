//main.cpp by CHEONG KAI JIN
//init, timing controls and frame update

//!IDEALLY, PLEASE COMPILE WITH -std=c++17
//not using hpp cpp pairs because its a relatively small project

#include <chrono> //precise clock for delta time
#include <iostream>

#include <windows.h> //compatibility
#include <GL/glut.h>

#include "game.hpp"

//init clock
std::chrono::steady_clock::time_point lastframetime = std::chrono::steady_clock::now();

//frame update
void frameUpdate(int) {
    //update with delta time
    std::chrono::steady_clock::time_point currentframetime = std::chrono::steady_clock::now(); //current
    std::chrono::duration<float> delta = currentframetime - lastframetime; //delta
    lastframetime = currentframetime;
    updateGame(delta.count()*gamespeed);
    //redisplay and recall
    glutPostRedisplay();
    glutTimerFunc(fms, frameUpdate, 0);
}

//display
void display() {
    //reset
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); //reset matrix
    //draw
    drawGame();
    //swap
    glutSwapBuffers();
}

//reshape
void reshape(int w, int h) {
    //prevent window size change
    glutReshapeWindow(800, 600);
    //projection matrix reset
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //viewport
    glViewport(0, 0, 800, 600);
    gluPerspective(45, 4.0f/3, 1, 1000);
    //back to model view
    glMatrixMode(GL_MODELVIEW);
}

//msg when program starts
void initMsg() {
    std::cout << "\nTCG project by team Affectionate\n"
    << "\nGame controls:\n" //game controls
    << "N(p1), M(p2) - choose characters\n"
    << "Space - start game / next turn / proceed\n"
    << "1, 2, 3 number keys and left, down, right arrow keys - select character skill/move\n"
    << "I - toggle computer opponent (let computer take over p2)\n"
    << "O - toggle music\n"
    << "P - pause / unpause game, reset game speed\n"
    << "[, ] - decrease / increase game speed\n"
    << "\nDeveloper controls:\n" //dev controls
    << "R - toggle axis\n"
    << "F - toggle freeview / noclip\n"
    << "WASD - freeview horizontal movement\n"
    << "Q, E - freeview vertical movement\n"
    << "Z, X - freeview zoom in and out\n"
    << "Mouse - freeview rotate camera\n"
    << "G - toggle godmode (lock health and mana for both player)\n"
    << "F1 - toggle wireframe mode\n"
    << "\nOther information:\n" //other information
    << "Skill selection is discrete, so that other real players can't see what you have selected,\n"
    << "instead, casted skills from each turn will be logged in console.\n"
    << "Changing game speed will be logged in console.\n";
}

//main
int main(int argc, char **argv) {
    //glutinit
    glutInit(&argc, argv);
    //window
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(640, 480);
    glutCreateWindow("GlutRumble");
    glClearColor(0.2, 0.2, 0.2, 1);
    //callback funcs
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    //enables
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND); //alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_ALPHA_TEST); //alpha cutoff
    glAlphaFunc(GL_GREATER, 0.99);
    glLineWidth(1.0f);
    //update loops, controlled updates
    glutTimerFunc(0, frameUpdate, 0);
    //begin
    initMsg();
    initGame();
    glutMainLoop();
}
