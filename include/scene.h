#ifndef SCENE_H
#define SCENE_H
#define NUMBER_OF_MODELS 13
#include "camera.h"
#include <GL/glut.h>
#include <time.h>
#include "utils.h"

float rotateTimer;
int rotate;
int toggleHelp;
int toggleFog;
int toggleSkin;
int moveForward;
int get_in_car;

struct Model model[NUMBER_OF_MODELS];

struct ModelData {
	char* textureFileName;
	char* objFileName;
	int texID;
} modelData[NUMBER_OF_MODELS];

struct floor
{
	GLfloat x, y;
};

struct Car
{
	struct Vertex position;
	struct Vertex pose;
};

struct Car car;
struct Car car2;




/* Initializes key actions, camera and light setup */
void init_scene();

/* Prints text to screen space coordinates */
void glprint(int x, int y, char *st);

/* Sets the power of light by the given value */
void set_lightings(float lightPower);

/* Draws the scene: binds textures, places models */
void draw_scene(double elapsed_time);

void draw_fog();

void move_car_forward(struct Car* car,double elapsed_time);

void move_car_backward(struct Car* car,double elapsed_time);

void draw_cars(struct Car* car,double elapsed_time,struct Car* car2 );
void init_cars(struct Car* car,struct Car* car2);

#endif /* SCENE_H */
