#include "gl.h"


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKeys);
	glutSpecialFunc( onSpecialKeys);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMouseMotion);
	glutMouseWheelFunc(onMouseWheel);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}