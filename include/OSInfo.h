#ifndef _OSINFO_H
#define _OSINFO_H


// struct OSInfo
// {
// 	//���ڲ���ϵͳ�ĸ�����Ϣ�����
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
        �жϵ�ǰ�����Ƿ�������64λ����ϵͳ��
   Return
        ���������64Ϊ����ϵͳ�ϣ�����true�����򷵻�false
*  
*/
LIB_API bool IsWow64();






#ifdef __cplusplus
}
#endif

#endif  //#ifndef _OSINFO_H