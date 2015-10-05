#ifndef __CAPTURE_H_
#define __CAPTURE_H_


#ifdef LIB_EXPORT
  #define  LIB_API __declspec(dllexport)
#else
  #define  LIB_API __declspec(dllimport)
#endif

#define LIB_IN
#define LIB_OUT


#ifdef __cplusplus
extern "C" {
#endif
	//��ʼ��ģ�飬������Դ
	LIB_API void* init();
	LIB_API long set(LIB_IN void* pVoid, LIB_IN int x, LIB_IN int y, LIB_IN int cx, LIB_IN int cy);
	//pVoid����ά������ʹ�õ���Դ��pScreenData���صõ�����Ļ���ݣ�len�����ݵĳ��ȣ���С���ֽ��֣�width����Ļ���ݵĿ��
	LIB_API long getScreen(LIB_IN void* pVoid, LIB_OUT void** pScreenData, LIB_OUT int* len, LIB_OUT int width);
	//�ͷ���Դ
	LIB_API long uninit(LIB_IN void* pVoid);

#ifdef __cplusplus
}
#endif

#endif//__CAPTURE_H_
