#ifndef GLUTCALLBACKS_H
#define GLUTCALLBACKS_H 
#include "camera.h"
#include <GL/glut.h>
#include "scene.h"
#include "utils.h"

/**
* Call after windows resizing.
*/
void reshape(GLsizei width, GLsizei height);

/**
* Call when need to display the graphical content.
*/
void display();

/**
* Call after normal key keypress event.
*/
void key_handler(unsigned char key, int x, int y);


/**
* Call after special key keypress event.
*/
void key_special_handler(int key, int x, int y);

/**
* Call after key release event.
*/
void key_up_handler(unsigned char key, int x, int y);

/**
* Call after mouse button event.
*/
void mouse_handler(int button, int state, int x, int y);

/**
* Call after mouse event.
*/
void motion_handler(int x, int y);

/**
* Call when there is no other event.
*/
void idle();


#endif