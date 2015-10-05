#ifndef __IVIDEORECORD_H
#define __IVIDEORECORD_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define SWITCH_DECLARE(type) __declspec(dllexport) type __cdecl
#else
#define	SWITCH_DECLARE(type) type
#endif

SWITCH_DECLARE(void *) CreateVideoRecord(const char * lpFileName,long lWidth,long lHeight,int iKeyFrameInterval,int iOnlyVideo = 1);
SWITCH_DECLARE(void) IncomingVideo(void *pRecord,void * pBuffer,long lLength);
SWITCH_DECLARE(void) IncomingAudio(void *pRecord,void * pBuffer,long lLength);
SWITCH_DECLARE(void) ReleaseVideoRecord(void *);
SWITCH_DECLARE(void) write_arbitrary_av(void *);

#ifdef __cplusplus
}
#endif

#endif