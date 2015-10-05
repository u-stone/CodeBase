#include "item.h"

RenderMode Item::m_sMode;
dMatrix44 Item::m_sxWorld2BeeMotion = 0;


void Item::transposeMatrix(dMatrix44 m, dMatrix44 om)
{
	for (int i = 0; i < 16; ++i)
		om[i] = -m[i];
}



void ItemWithCamera::calcSRT()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (m_sMode.getMode() == RENDER_BEE){
		dMatrix44 tmp = new GLdouble[16];
		//get bee to self matrix
		transposeMatrix(m_sxWorld2BeeMotion, tmp);
		glMultMatrixd(tmp);
		glMultMatrixd(m_xWorld2SelfMotion);
		delete tmp;
	} else if (m_sMode.getMode() == RENDER_GOD) {
		glMultMatrixd(m_xNone2WorldMotion);
		glMultMatrixd(m_xWorld2SelfMotion);
	} else {
		throw ;
	}
}

bool ItemWithCamera::isOpenCamera()
{
	return m_bOpenCamera;
}



void ItemWithNoCamera::calcSRT()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(m_xNone2WorldMotion);
	glMultMatrixd(m_xWorld2SelfMotion);
}


