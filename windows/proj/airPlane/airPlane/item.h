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
	//TODO: 矩阵求逆
	void transposeMatrix(dMatrix44 m, dMatrix44 om);
protected:
	dMatrix44 m_xNone2WorldMotion;  //世界的绝对运动矩阵，也就是相对于虚空的运动。
	dMatrix44 m_xWorld2SelfMotion;  //自身相对于世界而言的运动矩阵

protected:
	static RenderMode m_sMode;
	static dMatrix44 m_sxWorld2BeeMotion; //自身相对bee的运动矩阵
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
