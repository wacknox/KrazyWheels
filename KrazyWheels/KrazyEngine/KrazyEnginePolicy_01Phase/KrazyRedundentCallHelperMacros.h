#pragma once
#ifndef _REDUNDENTCALLHELPERMACROS_H_
#define _REDUNDENTCALLHELPERMACROS_H_
//
#define _KRAZY_REMOVE_ITEM_FROM_VECTOR_BY_INDEX(_vector , _index)\
	int _count = _vector.size();\
	for(int i = _index; i < _count - 1;i++){\
		_vector[i] = _vector[i+1];\
	}\
	_vector.pop_back();return;\
//
#define _KRAZY_REMOVE_ITEM_FROM_PTR_VECTOR_BY_INDEX(_vector , _index)\
	int _count = _vector->size();\
	for(int i = _index; i < _count - 1;i++){\
		*(_vector)[i] = *(_vector)[i+1];\
	}\
	_vector->pop_back();return;\
//
//
#define _KRAZY_REMOVE_ITEM_FROM_VECTOR_BY_ITEM_PTR(_vector , class_type , ptr_c)\
		int count = _vector.size();\
		for(int i = 0; i < count; i++){\
		  class_type* ptr = _vector[i];\
		  if(ptr == ptr_c){\
			int index = i;\
			_KRAZY_REMOVE_ITEM_FROM_VECTOR_BY_INDEX(_vector,index)\
		  }\
		}\

#define _KRAZY_REMOVE_ITEM_FROM_PTR_VECTOR_BY_ITEM_PTR(_vector , class_type , ptr_c)\
	int count = _vector->size();\
		for(int i = 0; i < count; i++){\
		  class_type* ptr = _vector->at(i);\
		  if(ptr == ptr_c){\
			int index = i;\
			_KRAZY_REMOVE_ITEM_FROM_PTR_VECTOR_BY_INDEX(_vector,index)\
		  }\
		}\

#define _KRAZY_LOOKUP(container , search)\
	container.find(search) != container.end()\
//
#define _KRAZY_CLEAR_PTR_FROM_LIST(list)\
	    int count = list.size();\
		for(int iter = 0; iter < count; iter++){\
		    delete list[iter];list[iter] = 0;\
		}\
		list.clear();\

#endif