#pragma once

#include "base.h"
#include "item.h"

//大地在XoY平面上，Z方向是高度。
class Ground : public Item
{
public:
	Ground(GLdouble minLongtitude, GLdouble maxLongtitude, 
		GLdouble minLatitude, GLdouble maxLatitude);
public:
	void render();
private:
	//经纬度范围
	GLdouble m_dMinLongtitude;
	GLdouble m_dMaxLongtitude;
	GLdouble m_dMinLatitude;
	GLdouble m_dMaxLatitude;
	//地球半径
	GLdouble m_dRaduis;
	//地面高度
	GLdouble m_dHeight;
	//绘制颜色
	GLbyte m_Color[3];
	//线条颜色
	GLfloat m_fLineWidth;
	//线条绘制因子
	GLint   m_iLineFactor;
	//线条模式
	GLushort m_usPattern;
	//经纬度之间的间隔
	GLdouble m_dStepLongtitude;
	GLdouble m_dStepLatitude;

	//默认角度
	GLdouble m_dRotateX;
	GLdouble m_dRotateY;
	GLdouble m_dRotateZ;
};

Ground::Ground(GLdouble minLongtitude, GLdouble maxLongtitude, GLdouble minLatitude, GLdouble maxLatitude)
{
	if (minLongtitude >= maxLongtitude || minLatitude >= maxLatitude)
		throw ;
 	m_dMinLongtitude = minLatitude;
 	m_dMaxLongtitude = maxLongtitude;
 	m_dMinLatitude = minLatitude;
 	m_dMaxLatitude = maxLatitude;
	m_dRaduis = 6371.393;
	m_dHeight = -200;
	m_Color[0] = 63;
	m_Color[1] = 52;
	m_Color[2] = 27;
	m_fLineWidth = 0.0f;
	m_iLineFactor = 1;
	m_usPattern = 0xFFFF;
	m_dStepLongtitude = 10.0;
	m_dStepLatitude = 10.0;
	m_dRotateX = -60.0;
	m_dRotateY = 0.0;
	m_dRotateZ = -15.0;
}

void Ground::render()
{
	glPushMatrix();

	glLoadIdentity();
	//S-R-T
	glRotated(m_dRotateX, AxisX[0], AxisX[1], AxisX[2]);
	glRotated(m_dRotateY, AxisY[0], AxisY[1], AxisY[2]);
	glRotated(m_dRotateZ, AxisZ[0], AxisZ[1], AxisZ[2]);
	glTranslated(0, 0, -m_dRaduis);

	glColor3b(m_Color[0], m_Color[1], m_Color[2]);
	glLineWidth(m_fLineWidth);
	if (m_usPattern != 0xFFFF){
		glEnable(GL_LINE_STIPPLE);
        glLineStipple(m_iLineFactor, m_usPattern);
	}
	GLdouble height = m_dRaduis + m_dHeight;
	for (double start = m_dMinLongtitude; start < m_dMaxLatitude + m_dStepLongtitude / 2; start += m_dStepLongtitude){
		glBegin(GL_LINES);
		  glVertex3d(start, m_dMinLatitude, height);
		  glVertex3d(start, m_dMaxLatitude, height);
		glEnd();
	}
	for (double start = m_dMinLatitude; start < m_dMaxLatitude + m_dStepLatitude / 2; start += m_dStepLatitude){
		glBegin(GL_LINES);
		glVertex3d(m_dMinLatitude, start, height);
		glVertex3d(m_dMaxLatitude, start, height);
		glEnd();
	}
	if (m_usPattern != 0xFFFF){
		glDisable(GL_LINE_STIPPLE);
	}
	glPopMatrix();
}

