#ifndef CAMERA_H
#define CAMERA_H
#define CAMERA_SPEED 1.0
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define VIEWPORT_RATIO (16.0 / 9.0)
#define VIEWPORT_ASPECT 50.0
#include "model.h"
#include "scene.h"

struct Camera
{
	struct Vertex position;
	struct Vertex pose;
};

struct Action
{
	int move_forward;
	int move_backward;
	int step_left;
	int step_right;
	int crouch;
	int sprint;
} action;

struct Camera camera;
int mouse_x, mouse_y;


/**
 * Initialize the camera position and direction.
 */
void init_camera(struct Camera* camera);

/**
 * Transform the models into the view point of the camera.
 */
void set_view_point(const struct Camera* camera);

/**
 * Rotate the camera horizontally and vertically.
 */
void rotate_camera(struct Camera* camera, double horizontal, double vertical);

/**
 * Move the camera forward.
 */
void move_camera_forward(struct Camera* camera, double distance);

/**
 * Move the camera backward.
 */
void move_camera_backward(struct Camera* camera, double distance);

/**
 * Step the camera left.
 */
void step_camera_left(struct Camera* camera, double distance);

/**
 * Step the camera right.
 */
void step_camera_right(struct Camera* camera, double distance);

void lower_camera(struct Camera* camera, double distance);
void raise_camera(struct Camera* camera, double distance);

/**
 * Updates camera position.
 */
void update_camera_position(struct Camera* camera, double elapsed_time);
#endif // CAMERA_H

