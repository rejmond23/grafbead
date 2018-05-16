#include "glutcallbacks.h"

void set_callbacks()
{
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_handler);
	glutKeyboardUpFunc(key_up_handler);
	glutMouseFunc(mouse_handler);
	glutPassiveMotionFunc(motion_handler);
	//glutMotionFunc(motion_handler);
	glutIdleFunc(idle);
	glutSpecialUpFunc(key_special_handler);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	// parseModels();
	readModelDataFromFile();
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	int window = glutCreateWindow("Grafika beadando");
	glutSetWindow(window);
    initialize();
    init_camera(&camera);
	set_callbacks();
    glutMainLoop();
    return 0;
}

