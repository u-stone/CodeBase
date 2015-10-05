#ifndef _OSINFO_H
#define _OSINFO_H


// struct OSInfo
// {
// 	//关于操作系统的各种信息的入口
// 	
// };

#ifdef LIB_EXPORT
  #define LIB_API __declspec(dllexport) 
#else
  #define LIB_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"{
#endif

/*
*  Function
        判断当前程序是否运行在64位操作系统上
   Return
        如果运行在64为操作系统上，返回true；否则返回false
*  
*/
LIB_API bool IsWow64();






#ifdef __cplusplus
}
#endif

#endif  //#ifndef _OSINFO_H