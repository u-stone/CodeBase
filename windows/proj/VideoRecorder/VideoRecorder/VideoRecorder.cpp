// VideoRecorder.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "VideoLivRecord.h"
#include "videoRecord.h"

void ReleaseVideoRecord(void * pObj)
{
	OutputDebugString("[ReleaseVideoRecord] -- enter");
	CVideoLivRecord * pFactory = (CVideoLivRecord *)pObj;
	delete pFactory;
}

void IncomingAudio(void * pObj, void * ppcmAudio, long iLength)
{
	CVideoLivRecord * pFactory = (CVideoLivRecord *)pObj;
	pFactory->write_audio_data((char *)ppcmAudio,iLength);
}

void * CreateVideoRecord(LPCSTR lpFileName,LONG lWidth,LONG lHeight,INT iKeyFrameInterval,int iOnlyVideo)
{
	OutputDebugString("[CreateVideoRecord] -- enter");
	CVideoLivRecord * pFactory = new CVideoLivRecord();
	if(lpFileName)
	{
		pFactory->InitRecoder(lpFileName,lWidth,lHeight,iKeyFrameInterval,iOnlyVideo);		
	}	
	return pFactory;
}

void IncomingVideo(void *pRecord,void * pBuffer,LONG lLength)
{
	OutputDebugString("[IncomingVideo] -- enter");
	CVideoLivRecord * pFactory = (CVideoLivRecord *)pRecord;
	pFactory->write_video_data(pBuffer,lLength);
}

void write_arbitrary_av(void *pRecord){
	CVideoLivRecord * pFactory = (CVideoLivRecord *)pRecord;
	pFactory->write_arbitrary_av();
}


