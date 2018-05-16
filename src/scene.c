#include "scene.h"
#include "glutcallbacks.h"
#include "camera.h"
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <math.h>



typedef GLubyte Pixel[3];
Pixel* image;

GLfloat material1[] = { 0.2f, 0.2f, 0.3f, 0.1f };
GLfloat material2[] = { 0.6f, 0.6f, 0.5f, 0.4f };
GLfloat full_mat[] = { 1, 1, 1, 1 };
GLfloat no_mat[] = { 0, 0, 0, 0 };
GLfloat material_shininess[] = { 100.0 };

float currentLightStrength = 0.8f;
int sky_texture_id;
void init_texture(char* filename);
void move_car_forward(struct Car* car,double elapsed_time);
void move_car_backward(struct Car* car,double elapsed_time);
void init_cars(struct Car* car,struct Car* car2);


void set_lightings(float value)
{
	if (value == -1) currentLightStrength = 0.8f;
	else {
		if (((currentLightStrength + value) <= 1) && ((currentLightStrength + value) >= 0)) 
		currentLightStrength += value;
	}
	GLfloat light_position[] = { 0.0, 0.0, 5, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	GLfloat light_ambient[] = { currentLightStrength, currentLightStrength, currentLightStrength, 0 };
	GLfloat light_diffuse[] = { currentLightStrength, currentLightStrength, currentLightStrength, 0 };
	GLfloat light_specular[] = { currentLightStrength, currentLightStrength, currentLightStrength, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glClearColor(currentLightStrength, currentLightStrength, currentLightStrength + 0.2f, 1.0);
}

void showHelp() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, modelData[1].texID);

	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0, 0, 0);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0, SCREEN_HEIGHT, 0);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(SCREEN_WIDTH, 0, 0);
	glEnd();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



void initialize()
{
	toggleHelp = FALSE;
	rotate = FALSE;
	toggleFog = FALSE;
	toggleSkin = 0;
	rotateTimer = 0;
	moveForward=-1;
	get_in_car=FALSE;
	init_cars(&car,&car2);
	int i;
	srand(time(NULL));
	set_lightings(-1);


	action.move_forward = FALSE;
	action.move_backward = FALSE;
	action.step_left = FALSE;
	action.step_right = FALSE;
	action.crouch = FALSE;

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glClearColor(1, 1, 1, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0.0, 0.0, 0.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0
	);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearDepth(1.0);
	glEnable(GL_TEXTURE_2D);
	initialize_texture();
	gluPerspective(VIEWPORT_ASPECT, VIEWPORT_RATIO, 0.01, 1000.0);
	glMatrixMode(GL_PROJECTION);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

void floor2(){
	glBegin(GL_QUADS);
	glVertex3f(-1,-1,-1);
	glVertex3f(1,-1,-1);
	glVertex3f(1,-1,1);
	glVertex3f(-1,-1,1);

glEnd(); 
}

void draw_grass(){
	int i=0, j=0;
	glTranslatef(10.0f, 5.0f, 0.0f);
	for(i=0;i<8;i++){
	glBindTexture(GL_TEXTURE_2D, modelData[4].texID);
	glPushMatrix();
	// init_texture("Textures/floor.png");
	glTranslatef(-5.0f*i, -4.0f, -0.9f);
	glScalef(10.0f, 10.0f, 1.0f);
	glRotatef(90, 1, 0, 0);
	draw_model(&model[4]);
	glPopMatrix();
		
	}
}

void cylinder(GLfloat radius,
                   GLfloat height,
                   GLubyte R,
                   GLubyte G,
                   GLubyte B)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;

    /** Draw the tube */
    glColor3ub(R-40,G-40,B-40);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*M_PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            glVertex3f(x, y , 0.0);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
        glVertex3f(radius, 0.0, 0.0);
    glEnd();

    /** Draw the circle on top of cylinder */
    glColor3ub(R,G,B);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*M_PI ) {
            x = radius * cos(angle);
            y = radius * sin(angle);
            glVertex3f(x, y , height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, 0.0, height);
    glEnd();
}

void draw_skybox()
{
    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;

    int n_slices, n_stacks;
    double radius;
    int i, k;

    // n_slices = 12;
    n_slices = 23;
    // n_stacks = 6;
    n_stacks = 12;
    radius = 50;

	glBindTexture(GL_TEXTURE_2D, modelData[5].texID);
    glPushMatrix();
	// init_texture("Textures/sky.png");
	
    glScaled(radius, radius, radius);

    glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < n_stacks; ++i) {
        v1 = (double)i / n_stacks;
        v2 = (double)(i + 1) / n_stacks;
        phi1 = v1 * M_PI / 2.0;
        phi2 = v2 * M_PI / 2.0;
        for (k = 0; k <= n_slices; ++k) {
            u = (double)k / n_slices;
            theta = u * 2.0 * M_PI;
            x1 = cos(theta) * cos(phi1);
            y1 = sin(theta) * cos(phi1);
            z1 = sin(phi1);
            x2 = cos(theta) * cos(phi2);
            y2 = sin(theta) * cos(phi2);
            z2 = sin(phi2);
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, z2);
        }
    }

    glEnd();
	

    glPopMatrix();
}

void draw_cylinder(){
	glBindTexture(GL_TEXTURE_2D, modelData[5].texID);
    glPushMatrix();
	glTranslatef(3.0f, -4.0f, 0.0f);
	cylinder(2.3, 1.0f, 255, 160, 100);
	glPopMatrix();
}

void draw_fog() {
	GLfloat density = 0.05;
    float FogCol[3]={0.5f,0.5f,0.5f};
	glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR,FogCol);
	glFogf (GL_FOG_DENSITY, density);
    glFogf(GL_FOG_START, 3.f);
    glFogf(GL_FOG_END, 20.f);
}


void init_texture(char* filename)
{
	GLint level = 0;      
	GLint components = 3; 
	GLint border = 0; 

	glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    // Read the image file.
    char* texture_filename = filename;

    int width;
    int height;

    unsigned char* image = SOIL_load_image(texture_filename, &width, &height, 0, SOIL_LOAD_RGBA);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,(Pixel*)image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

	glEnable(GL_TEXTURE_2D);
}


void draw_cars(struct Car* car,double elapsed_time, struct Car* car2){
	
	
	double distance;
	distance = elapsed_time * 1.0;
	float fos;
	
	
	//Taxi
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, modelData[0].texID);
	glScalef(1.0f, 1.0f, 1.0f);
	// glTranslatef(-5.0f, 0.0f, 0.0f);
	if(get_in_car==TRUE){
		if(moveForward==1){
		move_car_forward(car,elapsed_time);
		} 
		else if(moveForward==0){
		move_car_backward(car,elapsed_time);
		} 
	}
	
	glTranslatef(-5.0f, car->position.x, 0.0f);
	glRotatef(90, 1, 0, 0);
	draw_model(&model[0]);
	glPopMatrix();

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, full_mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
	
	//kerekek
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, modelData[8].texID);
	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(-5.0f,car->position.x, 0.0f);
	glRotatef(90, 1, 0, 0);
	draw_model(&model[8]);
	glPopMatrix();
	// if(action.crouch == TRUE){
			// printf("\nposition x: %lf", car->position.x);
			// printf("\nposition y: %lf", car->position.y);
			// printf("\nposition z: %lf", car->position.z);
	// }
	
	//interior
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, modelData[10].texID);
	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(-5.0f,car->position.x, 0.0f);
	glRotatef(90, 1, 0, 0);
	draw_model(&model[10]);
	glPopMatrix();
	
	//lights
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, modelData[11].texID);
	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(-5.0f,car->position.x, 0.0f);
	glRotatef(90, 1, 0, 0);
	draw_model(&model[11]);
	glPopMatrix();
	
	
	
//_________________________________________________________
	//Jeep
	glBindTexture(GL_TEXTURE_2D, modelData[3].texID);
	glPushMatrix();
	glTranslatef(3.0f, 5.0f, 0.0f);
	//nem tudom miért kell ennyi skálázás bele, de valamiért ezt a modellt nagyon nagyban jeleníti meg, ha nem kicsinyítem
	glScalef(0.4f, 0.4f, 0.4f);
	// glScalef(0.1f, 0.1f, 0.1f);
	// glScalef(0.1f, 0.1f, 0.1f);
	// glScalef(0.3f, 0.3f, 0.3f);
	
	glRotatef(90, 1, 0, 0);
	draw_model(&model[3]);
	glPopMatrix();
		
	
    
//______________________________________________________
	//porsche
	if(toggleSkin==1){
		// init_texture("Textures/porsche_1.bmp");
		glBindTexture(GL_TEXTURE_2D, modelData[6].texID);
		
	} else if (toggleSkin==2) {
		// init_texture("Textures/porsche_2.bmp");
		glBindTexture(GL_TEXTURE_2D, modelData[7].texID);
	} else{
		glBindTexture(GL_TEXTURE_2D, modelData[2].texID);
	}
	glPushMatrix();
	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(3.0f, -4.0f, 1.6f);
	glRotatef(90, 1, 0, 0);
	if (rotate == TRUE) {
		rotateTimer += 0.5 * elapsed_time;
		if (rotateTimer > 1) {
			rotate = FALSE;
		}
	}else {
		if (rotateTimer > 0) rotateTimer -=1;
	}
	
	glRotatef(rotateTimer*360, 0, 1, 0);
	draw_model(&model[2]);
	glPopMatrix();
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material2);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, full_mat);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess); 
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, modelData[9].texID);
	glScalef(1.0f, 1.0f, 1.0f);
	glTranslatef(3.0f, -4.0f, 1.6f);
	glRotatef(90, 1, 0, 0);
	if (rotate == TRUE) {
		rotateTimer += 0.5 * elapsed_time;
		if (rotateTimer > 1) {
			rotate = FALSE;
		}
	}else {
		if (rotateTimer > 0) rotateTimer -=1;
	}
	
	glRotatef(rotateTimer*360, 0, 1, 0);
	draw_model(&model[9]);
	glPopMatrix();

	
//________________________________________________________
	
	//Natla
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, modelData[12].texID);
	glScalef(1.0f, 1.0f, 1.0f);
	// glTranslatef(-5.0f, 0.0f, 0.0f);
	if(get_in_car==FALSE){
		if(moveForward==1){
			if(car2->position.x<11){
				move_car_backward(car2,elapsed_time*1.6);
			}
		} 
		else if(moveForward==0){
			if(car2->position.x+5>0){
			move_car_forward(car2,elapsed_time*1.2);
			}
		} 
	}
	glTranslatef(-10.0f, 4.0f, car2->position.x+5.0);
	glRotatef(90, 1, 0, 0);
	draw_model(&model[12]);
	glPopMatrix();
	
	

}



void draw_scene(double elapsed_time)
{
	int i=0;
	double r;
	
	
	
	draw_skybox();
	draw_fog();
	
	if(toggleFog==TRUE){
		glEnable(GL_FOG);
	} else glDisable(GL_FOG);
	
	
	draw_cars(&car,elapsed_time, &car2);
	
	
	//"állvány" kirajzolása
	draw_cylinder();
	
	//grass
	draw_grass();
	
	
	//Draw floor
	glBindTexture(GL_TEXTURE_2D, modelData[4].texID);
	glTranslatef(-5.0f, -4.0f, 1.0f);
	glScalef(100.0f, 100.0f, 1.0f);
	glRotatef(90, 1, 0, 0);
	floor2();
	
	glRotatef(-90, 1, 0, 0);
	glScalef(1.0f, 1.0f, 1.0f);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	


	if (toggleHelp == TRUE)
	{
		showHelp();
		return;
	}

}

void init_cars(struct Car* car,struct Car* car2){
	car->position.x = -5.0f;
	car->position.y = 0;
	car->position.z = 1;

	car->pose.x = 0;
	car->pose.y = 0;
	car->pose.z = 0;
	
	car2->position.x = -5.0f;
	car2->position.y = 0;
	car2->position.z = 1;

	car2->pose.x = 0;
	car2->pose.y = 0;
	car2->pose.z = 0;
	
}

void move_car_forward(struct Car* car,double elapsed_time)
{
	// if(car->position.x>-45.0f){
	if(car->position.x>-24.4f){
		
		car->position.x -= 4*elapsed_time;
		car->position.y -= 4*elapsed_time;
	} 
	
	
}

void move_car_backward(struct Car* car,double elapsed_time)
{
	// if(car->position.x<45.0f){
	if(car->position.x<24.4f){
	car->position.x += 2*elapsed_time;
	car->position.y += 2*elapsed_time;
	}
}
