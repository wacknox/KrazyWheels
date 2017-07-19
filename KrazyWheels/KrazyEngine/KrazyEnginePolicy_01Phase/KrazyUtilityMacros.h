#pragma once
#ifndef _KRAZYUTILITYMACROS_H_
#define _KRAZYUTILITYMACROS_H_
//.....
#define _KRAZY_deallocate(_ptr) \
	if(_ptr){\
		delete _ptr;_ptr = 0;\
	}\
//.....
#endif