#pragma once

interface ITriggerCtrl{
	virtual void setQuit(bool bQuit) = 0;
	virtual bool getQuit() = 0;
	virtual bool start() = 0;
};

interface IPressureCtrl{
	virtual void setPressure(DWORD value) = 0;
	virtual DWORD getPressure() = 0;
};

class cpuPCtrl : public ITriggerCtrl, public IPressureCtrl
{
public:
	cpuPCtrl(void);
	~cpuPCtrl(void);

	void setPressure(DWORD value);
	DWORD getPressure();

	void setQuit(bool bQuit);
	bool getQuit();

	void setStart(bool bStart){
		m_bStart = bStart;
	}
	bool getStart(){
		return m_bStart;
	}

	bool start();

private:
	bool m_bStart;
	int m_nPressure;
	bool m_bQuit;
	HANDLE m_hMainWorkerThrd;
};

