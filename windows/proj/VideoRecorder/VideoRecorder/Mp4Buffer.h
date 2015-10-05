#pragma once


class CMP4Buffer{
private:
	uint8_t * m_pBuffer;
	uint16_t  m_Len;
	uint16_t  m_WritePos;
public:
	CMP4Buffer(){
		m_pBuffer = 0;
		m_Len = 0;
		m_WritePos = 0;
	}
	~CMP4Buffer(){
		if(m_pBuffer){
			free(m_pBuffer);
		}
	}
public:
	void Initial(int iSize){
		if(m_pBuffer){
			free(m_pBuffer);
			m_WritePos = 0;
			m_Len = 0;
		}
		m_Len = iSize;
		m_pBuffer = (uint8_t *)malloc(m_Len);
	}
	uint16_t GetBufferLen(){
		return m_WritePos;
	}
	uint8_t * GetBuffer()
	{
		return m_pBuffer;
	}
	uint8_t * PreWrite(uint16_t uSize){
		uint16_t uMoveLen = 0, uStartMovePos = 0;
		if(uSize > m_Len) return 0;
		if(m_WritePos + uSize > m_Len){
			uStartMovePos = m_WritePos + uSize - m_Len;
			uMoveLen = m_WritePos - uStartMovePos;
			m_WritePos = m_WritePos - uStartMovePos;
			memmove(m_pBuffer,m_pBuffer+uStartMovePos,uMoveLen);	
		}
		memset(m_pBuffer+m_WritePos,0,uSize);
		return m_pBuffer+m_WritePos;
	}
	void AfterWrite(uint16_t uSize){
		m_WritePos = m_WritePos + uSize;
	}
	void AfterRead(uint16_t uSize){
		if(uSize > m_Len) return;
		if(uSize < m_WritePos){
			memmove(m_pBuffer,m_pBuffer+uSize,m_WritePos-uSize);
			m_WritePos = m_WritePos - uSize;
		}else{
			m_WritePos = 0;
		}		
	}
};



