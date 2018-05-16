#include "camera.h"
#include "utils.h"
#include "scene.h"
#include <GL/glut.h>
#include <math.h>
#include "glutcallbacks.h"
#include <windows.h>
#define SPRINTSPEED 3.6f
// #define SPRINTSPEED 10.6f


void init_camera(struct Camera* camera)
{
	camera->position.x = 0;
	camera->position.y = 0;
	camera->position.z = 1;

	camera->pose.x = 0;
	camera->pose.y = 0;
	camera->pose.z = 0;
}

void set_view_point(const struct Camera* camera)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(-(camera->pose.x + 90), 1.0, 0, 0);
	glRotatef(-(camera->pose.z - 90), 0, 0, 1.0);
	glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(struct Camera* camera, double horizontal, double vertical)
{
	camera->pose.z += horizontal;
	camera->pose.x += vertical;

	if (camera->pose.z < 0) {
		camera->pose.z += 360.0;
		
	}

	if (camera->pose.z > 360.0) {
		camera->pose.z -= 360.0;
		
	}

	if (camera->pose.x < 90.0 ) {
		
	}

	if (camera->pose.x > 90.0) {
		camera->pose.x = 90.0;
	}
	if (camera->pose.x < -90.0) {
		camera->pose.x = -90.0;
	}
	
}

void move_camera_forward(struct Camera* camera, double distance)
{
	double angle = degree_to_radian(camera->pose.z);

	camera->position.x += cos(angle) * distance;
	camera->position.y += sin(angle) * distance;
}

void move_camera_backward(struct Camera* camera, double distance)
{
	double angle = degree_to_radian(camera->pose.z);

	camera->position.x -= cos(angle) * distance;
	camera->position.y -= sin(angle) * distance;
}

void step_camera_left(struct Camera* camera, double distance)
{
	double angle = degree_to_radian(camera->pose.z + 90.0);

	camera->position.x += cos(angle) * distance;
	camera->position.y += sin(angle) * distance;
}

void step_camera_right(struct Camera* camera, double distance)
{
	double angle = degree_to_radian(camera->pose.z - 90.0);

	camera->position.x += cos(angle) * distance;
	camera->position.y += sin(angle) * distance;
}

void lower_camera(struct Camera* camera, double elapsed_time)
{
	camera->position.z -= 2*elapsed_time;
	if(camera->position.z <0.53){
		camera->position.z = 0.5;
	}
	
}

void raise_camera(struct Camera* camera, double elapsed_time)
{
	camera->position.z += 2*elapsed_time;
	if(camera->position.z >0.96){
		camera->position.z = 1;
	}
}

void car_camera(struct Camera* camera, double elapsed_time){
	camera->pose.x=car.pose.x;
	camera->pose.y=car.pose.y;
	camera->pose.z=car.pose.z-90;
	camera->position.z = 1.1;
	float pos_x,pos_y,pos_z;
	camera->position.x=-4.55f;
	camera->position.y=car.position.x;

	
}

void update_camera_position(struct Camera* camera, double elapsed_time)
{
	double distance;
	distance = elapsed_time * CAMERA_SPEED;
	if (action.move_forward == TRUE) {
		if(action.sprint==TRUE){
			move_camera_forward(camera, distance*SPRINTSPEED);
		}else
		move_camera_forward(camera, distance);
		
	}

	if (action.move_backward == TRUE) {
		if(action.sprint==TRUE){
			move_camera_backward(camera, distance*SPRINTSPEED);
		}else
		move_camera_backward(camera, distance);
	}

	if (action.step_left == TRUE) {
		if(action.sprint==TRUE){
			step_camera_left(camera, distance*SPRINTSPEED);
		}else
		step_camera_left(camera, distance);
	}

	if (action.step_right == TRUE) {
		if(action.sprint==TRUE){
			step_camera_right(camera, distance*SPRINTSPEED);
		}else
		step_camera_right(camera, distance);
	}
	if (action.crouch == TRUE) {
		lower_camera(camera, elapsed_time);
	}
	
	if (action.crouch == FALSE) {
		raise_camera(camera, elapsed_time);
	} 
	
	if(get_in_car==TRUE){
		car_camera(camera, elapsed_time);
	}
	
	//bounding box
	if(get_in_car==FALSE){
		if(camera->position.y<-24.4f){
			camera->position.y=-24.4;
		}
		if(camera->position.y>24.4f){
			camera->position.y=24.4;
		}
		if(camera->position.x<-42.9f){
			camera->position.x=-42.9;
		}
		if(camera->position.x>43.1f){
			camera->position.x=43.1;
		}
	
	}
	
	
	// if(action.crouch==TRUE){
	// printf("\n_______________________");
	// printf("\ncamera position x: %lf",camera->position.x);
	// printf("\ncamera position y: %lf",camera->position.y);
	// printf("\ncamera position z: %lf",camera->position.z);
	// }
	
}