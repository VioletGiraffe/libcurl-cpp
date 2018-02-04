#pragma once

#if defined _WIN32 && defined LIBCURL_CPP_DYNAMIC_LIB
	#ifdef BUILDING_LIBCURL_CPP
		#define LIBCURL_CPP_EXPORT __declspec(dllexport)
	#else
		#define LIBCURL_CPP_EXPORT __declspec(dllimport)
	#endif
#else
	#define LIBCURL_CPP_EXPORT
#endif
