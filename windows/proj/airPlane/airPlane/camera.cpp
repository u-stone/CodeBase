#include "camera.h"


camera::camera(void)
{
	memset(m_Pos, 0, sizeof(m_Pos));
	memset(m_vForward, 0, sizeof(m_vForward));
	memset(m_vUp, 0, sizeof(m_vUp));
}


camera::~camera(void)
{
}

void camera::setForward(vector3d vForward)
{
	memcpy(m_vForward, vForward, sizeof(vector3d));
}

void camera::setPos(vector3d pos)
{
	memcpy(m_Pos, pos, sizeof(vector3d));
}

void camera::setUpward(vector3d vUp)
{
	memcpy(m_vUp, vUp, sizeof(vector3d));
}

void camera::forward(GLdouble step)
{
	m_Pos[0] += m_vForward[0] * step;
	m_Pos[1] += m_vForward[1] * step;
	m_Pos[2] += m_vForward[2] * step;
}

void camera::rotate(GLdouble xBySelf, GLdouble yBySelf, GLdouble zBySelf)
{
	
}

void camera::setup()
{
	matrix44d m;
#define M(r, c) m[r*4 + c]
	//M(0, 0) = 
#undef M
}
