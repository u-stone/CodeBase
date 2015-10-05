#pragma once
#include <freeglut.h>
#include <stdio.h>
#include <math.h>
#include "ground.h"

GLdouble eyex = 0, eyey = 0, eyez = 200.0;

GLdouble angleX = 0, angleY = 0, angleZ = 0;
GLdouble fovy = 35.0;

GLdouble offsetX = 0, offsetY = 0, offsetZ = 0;


void crossProduct(GLdouble* result, GLdouble* u, GLdouble* v){
	result[0] = u[1]*v[2] - v[1]*u[2];
	result[1] = -u[0]*v[2] + v[0]*u[2];
	result[2] = u[0]*v[1] - v[0]*u[1];
}

void unit(GLdouble * v){
	GLdouble len = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	len = sqrt(len);
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
}

void setCamera(){
	glLoadIdentity();
	glScaled(1.0, 1.0, 1.0);
	glRotated(angleX, AxisX[0], AxisX[1], AxisX[2]);
	glRotated(angleY, AxisY[0], AxisY[1], AxisY[2]);
	glRotated(angleZ, AxisZ[0], AxisZ[1], AxisZ[2]);
	glTranslated(-eyex, -eyey, -eyez);
}

void init(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void reshape(int w, int h){
	if (h == 0)
		h = 1;
	GLfloat fAspect = (GLfloat)w/(GLfloat)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0);
	gluPerspective(35.0, fAspect, 1.5, 2000.0);
	glMatrixMode(GL_MODELVIEW);
}

GLdouble plane[][3] = {
	{-2, 0, 0},
	{2, 0, 0},//JL
	{-1, 2, 0},
	{1, 2, 0},//MN
	{0, 0, 0},
	{0, 4, 0},//KA
	{-1, 2.5, 0},
	{-1, 1.5, 0},//BC
	{1, 2.5, 0},
	{1, 1.5, 0},//DE
	{-2, 0.5, 0},
	{-2, -0.5, 0},//FG
	{-2, 0, 0.5},
	{-2, 0, -0.5},//F'G'
	{2, 0.5, 0},
	{2, -0.5, 0},//HI
	{2, 0, -0.5},
    {2, 0, 0.5},//H'I'
	{0, 0, 0},
	{0, -1, 1}, //KP
	{0, 0, 0},
	{0, -1, -1},
	{0, -1, 1},
	{0, -1, -1}
};

void DrawPlane(){
	glLineWidth(3.0f);
	glColor3f(1.0, 0.0, 0.0);
	glRotated(-45, 1.0, 0.0, 0.0);
	glRotated(-30, 0.0, 1.0, 0.0);
	glTranslated(offsetX, offsetY, offsetZ);
	for(int i = 0; i < sizeof(plane)/(3*sizeof(GLdouble)); i+=2){
		glBegin(GL_LINES);
		  glVertex3d(plane[i][0], plane[i][1], plane[i][2]);
		  glVertex3d(plane[i+1][0], plane[i+1][1], plane[i+1][2]);
		glEnd();
	}
	glLineWidth(1.0f);
}

Ground ground(-400.0, 400.0, -400.0, 400.0);
void DrawCrossLines()
{
	ground.render();
}

void DrawCube()
{
	GLdouble cube[8][3] = {
		{-0.5, 0.5, -0.5},
		{-0.5, 0.5, 0.5},
		{0.5, 0.5, 0.5},
		{0.5, 0.5, -0.5},
		{-0.5, -0.5, -0.5},
		{-0.5, -0.5, 0.5},
		{0.5, -0.5, 0.5},
		{0.5, -0.5, -0.5}
	};
	glColor3f(0.0, 0.0, 1.0);
	glTranslated(0, 0, -50);
	glLineWidth(4.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3d(cube[0][0], cube[0][1], cube[0][2]);
		glVertex3d(cube[1][0], cube[1][1], cube[1][2]);
		glVertex3d(cube[2][0], cube[2][1], cube[2][2]);
		glVertex3d(cube[3][0], cube[3][1], cube[3][2]);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3d(cube[4][0], cube[4][1], cube[4][2]);
		glVertex3d(cube[5][0], cube[5][1], cube[5][2]);
		glVertex3d(cube[6][0], cube[6][1], cube[6][2]);
		glVertex3d(cube[7][0], cube[7][1], cube[7][2]);
	glEnd();

	glBegin(GL_LINES);
		glVertex3d(cube[0][0], cube[0][1], cube[0][2]);
		glVertex3d(cube[4][0], cube[4][1], cube[4][2]);
		glVertex3d(cube[1][0], cube[1][1], cube[1][2]);
		glVertex3d(cube[5][0], cube[5][1], cube[5][2]);

		glVertex3d(cube[2][0], cube[2][1], cube[2][2]);
		glVertex3d(cube[6][0], cube[6][1], cube[6][2]);
		glVertex3d(cube[3][0], cube[3][1], cube[3][2]);
		glVertex3d(cube[7][0], cube[7][1], cube[7][2]);
	glEnd();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	printf("fovy(%f), eye(%f, %f, %f),angle(%f, %f, %f)\n", fovy, eyex, eyey, eyez, angleX, angleY, angleZ);

	glPushMatrix();
	  DrawCrossLines();
	glPopMatrix();

// 	glPushMatrix();
// 	  DrawCube();
// 	glPopMatrix();

	glPushMatrix();
	  setCamera();
	  DrawPlane();
	glPopMatrix();
 

	glutSwapBuffers();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, 1, 1.5, 2000.0);

	glutPostWindowRedisplay(glutGetWindow());
}

//eye point
void onKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case 'Q':
	case 'q':
		angleY += 1;
		break;
	case 'E':
	case 'e':
		angleY -= 1;
		break;

	case 'A':
	case 'a':
		angleZ += 1;
		break;
	case 'D':
	case 'd':
		angleZ -= 1;
		break;

	case 'W':
	case 'w':
		angleX += 1;
		break;;
	case 'S':
	case 's':
		angleX -= 1;
		break;

	case 'o':
		fovy += 1.0;
		if (fovy > 180.0)
			fovy = 180.0;
		break;

	case 'i':
		fovy -= 1.0;
		if (fovy < 1.0)
			fovy = 1.0;
		break;
	}
	glutPostRedisplay();
}

//eye point
void onSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		eyex += 1;
		break;
	case GLUT_KEY_UP:
		eyey -= 1;
		break;
	case GLUT_KEY_RIGHT:
		eyex -= 1;
		break;;
	case GLUT_KEY_DOWN:
		eyey += 1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		eyez -= 1;
		break;
	case GLUT_KEY_PAGE_UP:
		eyez += 1;
		break;
	}
	glutPostRedisplay();

}

void onMouse(int button, int updown, int x, int y)
{
	if (updown == GLUT_UP){
		
	} else if (updown == GLUT_DOWN){
		
	}
	//printf("onMouse : %d, %d\n", x, y);
}
//center point
void onMouseMotion(int x, int y)
{
	static int oldx = x, oldy = y;
	if (x - oldx > 0){
		angleY += .1;
	} else {
		angleY -= .1;
	}
	if (y - oldy > 0){
		angleX += .1;
	} else {
		angleX -= .1;
	}
	oldx = x;
	oldy = y;
}

//eye point
void onMouseWheel(int wheel_number, int direction, int x, int y)
{
	if (direction > 0){
		eyez -= 1;
	} else if (direction < 0){
		eyez += 1;
	}
	glutPostRedisplay();
}

void idle()
{
	glutPostWindowRedisplay(glutGetWindow());
}


