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
	//初始化模块，申请资源
	LIB_API void* init();
	LIB_API long set(LIB_IN void* pVoid, LIB_IN int x, LIB_IN int y, LIB_IN int cx, LIB_IN int cy);
	//pVoid用于维护程序使用的资源；pScreenData返回得到的屏幕数据；len是数据的长度，大小以字节轮；width是屏幕数据的宽度
	LIB_API long getScreen(LIB_IN void* pVoid, LIB_OUT void** pScreenData, LIB_OUT int* len, LIB_OUT int width);
	//释放资源
	LIB_API long uninit(LIB_IN void* pVoid);

#ifdef __cplusplus
}
#endif

#endif//__CAPTURE_H_
