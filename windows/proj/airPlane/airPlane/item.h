#pragma once

#include "base.h"

class RenderMode{//not used
public:
	RenderMode(){
		m_mode = RENDER_GOD;
	}
	RenderMode(int mode){
		m_mode = mode;
	}
public:
	int getMode(){
		return m_mode;
	}
private:
	int m_mode; // RENDER_BEE or RENDER_GOD
};

class Item{
public:
	Item(){
		m_xNone2WorldMotion = 0;
		m_xWorld2SelfMotion = 0;
	}
	virtual ~Item(){
		if (m_xNone2WorldMotion) delete m_xNone2WorldMotion;
		if (m_xWorld2SelfMotion) delete m_xWorld2SelfMotion;		
	}
public:
	virtual void calcSRT(){}

protected: //alg
	//TODO: ��������
	void transposeMatrix(dMatrix44 m, dMatrix44 om);
protected:
	dMatrix44 m_xNone2WorldMotion;  //����ľ����˶�����Ҳ�����������յ��˶���
	dMatrix44 m_xWorld2SelfMotion;  //���������������Ե��˶�����

protected:
	static RenderMode m_sMode;
	static dMatrix44 m_sxWorld2BeeMotion; //�������bee���˶�����
};


class ItemWithCamera : public Item{
public:
	ItemWithCamera(){}
	~ItemWithCamera(){}

public:
	virtual void calcSRT();
	virtual bool isOpenCamera();
private:
	bool m_bOpenCamera;
};

class ItemWithNoCamera : public Item{
public:
	ItemWithNoCamera();
	~ItemWithNoCamera();
public:
	virtual void calcSRT();
};
