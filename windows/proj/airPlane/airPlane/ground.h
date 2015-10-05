#pragma once

#include "base.h"
#include "item.h"

//�����XoYƽ���ϣ�Z�����Ǹ߶ȡ�
class Ground : public Item
{
public:
	Ground(GLdouble minLongtitude, GLdouble maxLongtitude, 
		GLdouble minLatitude, GLdouble maxLatitude);
public:
	void render();
private:
	//��γ�ȷ�Χ
	GLdouble m_dMinLongtitude;
	GLdouble m_dMaxLongtitude;
	GLdouble m_dMinLatitude;
	GLdouble m_dMaxLatitude;
	//����뾶
	GLdouble m_dRaduis;
	//����߶�
	GLdouble m_dHeight;
	//������ɫ
	GLbyte m_Color[3];
	//������ɫ
	GLfloat m_fLineWidth;
	//������������
	GLint   m_iLineFactor;
	//����ģʽ
	GLushort m_usPattern;
	//��γ��֮��ļ��
	GLdouble m_dStepLongtitude;
	GLdouble m_dStepLatitude;

	//Ĭ�ϽǶ�
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

