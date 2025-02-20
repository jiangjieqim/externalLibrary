#ifndef _DEF_H_
#define _DEF_H_
//启用debug模式
//#define DEBUG

#ifndef DLLIMPORT
	#ifdef DLL_EXPORT
		#define DLLIMPORT __declspec(dllexport)
	#endif

	#ifdef COMMON_PRO
		//common dll项目宏
		#define DLLIMPORT __declspec(dllexport)
	#endif
	#ifdef TEST_PRO
		//Test
		#define DLLIMPORT extern
	#endif
	#ifdef _ALL
		#define DLLIMPORT extern
	#endif
#endif
#endif