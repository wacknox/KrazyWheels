#pragma once
#include<string>
#include <vector>
#include <hash_map>
#include <unordered_map>
//..,

static float _KRAZY_RED[]    = {1.0f , 0.0f , 0.0f , 1.0f};
static float _KRAZY_GREEN[]  = {0.0f , 1.0f , 0.0f , 1.0f};
static float _KRAZY_BLUE[]   = {0.0f , 0.0f , 1.0f , 1.0f};
static float _KRAZY_BLACK[]  = {0.0f , 0.0f , 0.0f , 1.0f};
static float _KRAZY_WHITE[]  = {1.0f , 1.0f , 1.0f , 1.0f};
static float _KRAZY_YELLOW[] = {1.0f , 1.0f , 0.0f , 1.0f};

//..,
#define _KRAZY_COPY_LIST(_source , _destination) \
	    int _src_size = _source.size();_destination.clear(); \
		for(int i = 0; i < _src_size; i++){ \
			_destination.push_back(_source[i]); \
		}\

#define _KRAZY_COPY_LIST_PTR(_source , _destination , src_size) \
	_destination.clear(); \
	for(int i = 0; i < src_size; i++){ \
		_destination.push_back(_source[i]);\
	} \
//........................

#define _KRAZY_DEALLOCATE_LIST(_list) \
	int _count = _list.size(); \
	for(int i = 0; i < _count; i++){ \
	   delete _list[i]; \
	} \
	_list.clear(); \

//..,
#define	KLEAN_a_bit_of_KRAZINESS(_to_be_deleted) \
	    if(_to_be_deleted){ \
		   delete _to_be_deleted;_to_be_deleted = 0; \
		} \


using namespace std;
class KrazyIntList         : public vector<int>{};
class KrazyFloatList       : public vector<float>{};
class KrazyUnsignedCharList: public vector<unsigned char>{};
class KrazyListToFloatList : public vector<KrazyFloatList>{};
class KrazyStringList      : public vector<string>{};
//
template<typename ARG_KEY , typename ARG_VAL>
class KrazyKeyPTRToObjectMap
{
public:
	bool find(ARG_KEY key)
	{
		int count = _Keys.size();
		for(int i = 0; i < count; i++)
		{
			if(_Keys[i] == key)
			{
				return true;
			}
		}
		return false;
	}

	ARG_VAL* operator[](ARG_KEY key)
	{
		int count = _Keys.size();
		for(int i = 0; i < count; i++)
		{
			if(_Keys[i] == key)
			{
				return &_Vals[i];
			}
		}
		
		int index = _Keys.size();
		_Keys.push_back(key);
		_Vals.push_back(ARG_VAL());
		return &_Vals[index];
	}

private:
	vector<ARG_KEY> _Keys;
	vector<ARG_VAL> _Vals;

};
template<typename KRAZY_PTR>
class KrazyDynamicallyAllocatedPtrList
{
public:
	virtual ~KrazyDynamicallyAllocatedPtrList()
	{
		clean_up();
	}
	
	void push_back(KRAZY_PTR* ptr_object)
	{
		_Vec.push_back(ptr_object);
	}

	int size()
	{
		return _Vec.size();
	}

	KRAZY_PTR* at(int index)
	{
		return _Vec[index];
	}

protected:
	void clean_up()
	{
		int count = _Vec.size();
		for(int i = 0; i < count; i++){
			KRAZY_PTR* ptr = _Vec.at(i);
			delete ptr;ptr = 0;
		}
		_Vec.clear();
	}

	vector<KRAZY_PTR*> _Vec;
};
//
template<typename _Key , typename _Val>
class KrazyMap
{
public:
	bool find(_Key _key)
	{
		return ((key_index(_key) > -1) ? true : false);
	}
	//..,
	void insert(_Key _key , _Val _val)
	{
		_Keys.push_back(_key);
		_Vals.push_back(_val);
	}
	//..,
	void replaceat(_Key _key , _Val _val)
	{
		_Vals[key_index(_key)] = _val;
	}
	//..,
	_Val operator[](_Key _key)
	{
		return _Vals[key_index(_key)];
	}
	//..,
	void erase(_Key _key)
	{
		int _index = key_index(_key);
		_Keys.erase(_Keys.begin() + _index);
		_Vals.erase(_Vals.begin() + _index);
	}
	//..,
private:
	int key_index(_Key _key)
	{
		int _count = _Keys.size();
		for(int i = 0; i < _count; i++){
			if(_Keys[i] == _key){
				return i;
			}
		}
		//..,
		return -1;
	}
	//..,
	vector<_Key> _Keys;
	vector<_Val> _Vals;
};
class KrazyVehical;
class __KrazyVehicalList : public vector<KrazyVehical*> {};
//..,
const float DEG_2_RAD = 3.141593f / 180.0f;
const float KrazyPIXEL = 0.01f;