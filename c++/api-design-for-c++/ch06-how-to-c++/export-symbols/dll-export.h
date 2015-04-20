#ifndef INCLUDE_APIDES_DLL_EXPORT_H
#define INCLUDE_APIDES_DLL_EXPORT_H

#if defined _WIN32 || defined __CYGWIN__
	#ifdef EXPORT_DLL
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__((dllexport))
		#else
			#define DLL_PUBLIC __declspec(dllexport)
		#endif //__GNUC__
	#else
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__((dllimport))
		#else
			#define DLL_PUBLIC __declspec(dllimport)
		#endif //__GNUC__
	#endif //EXPORT_DLL
	#define DLL_HIDDEN
#else
	#if __GNUC__ >= 4
		#define DLL_PUBLIC __attribute__((visibility("defailt")))
		#define DLL_HIDDEN __attribute__((visibility("hidden")))
	#else
		#define DLL_PUBLIC
		#define DLL_HIDDEN
	#endif //__GNUC__
#endif //_WIN32 || __CYGWIN__

#endif //INCLUDE_APIDES_DLL_EXPORT_H
