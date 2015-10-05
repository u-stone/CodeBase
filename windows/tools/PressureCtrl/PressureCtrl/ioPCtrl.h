#pragma once
#define MEGABYTE          (1 << 20)
#define BUFFER_READ_SIZE  (MEGABYTE * 32)
#define BUFFER_WRITE_SIZE (MEGABYTE * 32)
#define FILE_READ_SIZE    (MEGABYTE * 512)
#define FILE_WRITE_SIZE   (MEGABYTE * 512)

class ioPCtrl
{
public:
	ioPCtrl(void);
	~ioPCtrl(void);

	bool startRead();
	bool startWrite();
	void setWriteThroughout(unsigned int megabyte){
		if (megabyte > 0 && megabyte < 1024)
			m_WriteThroughout = megabyte * MEGABYTE;
	}
	int getWriteThroughout(){
		return m_WriteThroughout;
	}
	void setReadThroughout(unsigned int megabyte){
		if (megabyte > 0 && megabyte < 1024)
			m_ReadThroughout = megabyte * MEGABYTE;
	}
	int getReadThroughout(){
		return m_ReadThroughout;
	}
	void setReadQuit(bool bQuit){
		m_bReadQuit = bQuit;
	}
	bool getReadQuit(){
		return m_bReadQuit;
	}
	void setWriteQuit(bool bQuit){
		m_bWriteQuit = bQuit;
	}
	bool getWriteQuit(){
		return m_bWriteQuit;
	}
	void setReadStart(bool bStart){
		m_bReadStart = bStart;
	}
	bool getReadStart(){
		return m_bReadStart;
	}
	void setWriteStart(bool bStart){
		m_bWriteStart = bStart;
	}
	bool getWriteStart(){
		return m_bWriteStart;
	}

	bool getReadBuffer(char** ppBuffer, unsigned int& size);
	bool getWriteBuffer(char** ppBuffer, unsigned int& size);

	wchar_t* getReadFileName(){
		return m_ReadFileName;
	}
	wchar_t* getWriteFileName(){
		return m_WriteFileName;
	}

private:
	bool init();
	bool start();
private:
	bool m_bReadStart;
	bool m_bWriteStart;
	bool m_bReadQuit;
	bool m_bWriteQuit;
	unsigned int m_WriteThroughout;
	unsigned int m_ReadThroughout;
	HANDLE m_hMainThrd;
	char* m_pReadBuffer;
	char* m_pWriteBuffer;
	wchar_t  m_ReadFileName[MAX_PATH];
	wchar_t  m_WriteFileName[MAX_PATH];
};

