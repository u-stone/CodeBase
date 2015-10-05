#pragma once
#include <freeglut.h>

typedef GLdouble vector3d[3];
typedef GLdouble matrix44d[16];

class camera
{
public:
	camera(void);
	~camera(void);

public://set/get
	void setForward(vector3d vForward);
	void setPos(vector3d pos);
	void setUpward(vector3d vUp);

public://operation
	//forward or backword the camera
	void forward(GLdouble step);
	//rotate the camera
	void rotate(GLdouble xBySelf, GLdouble yBySelf, GLdouble zBySelf);
	//load camera to current setting
	void setup();
private:
	vector3d m_Pos;
	vector3d m_vForward;
	vector3d m_vUp;
};

