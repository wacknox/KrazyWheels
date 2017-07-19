#pragma once
#ifndef _WINDOWS
#include <android\asset_manager.h>
#include <android\asset_manager_jni.h>
#endif
#include <sys/types.h>
#include <fstream>
//#define _ENABLE_SINGLETON
#ifndef _WINDOWS
class KrazyAssetManager
{
public  :
	//..,
	static KrazyAssetManager* getDefault(AAssetManager* _asset_mgr);
	static KrazyAssetManager* getDefault(); 
	static void releaseDefault();
	//..,
	void reset();
	AAssetManager* asset_manager();
	//..,
private :
	KrazyAssetManager(AAssetManager* _asset_mgr);
	~KrazyAssetManager();
	static KrazyAssetManager* _Me;
	//..,
	AAssetManager*			_AssetManager;
};

//..,
class KrazyAsset
{
//..,
#ifdef _ENABLE_SINGLETON
public:
	static KrazyAsset* getDefault(AAssetManager* _asset_mgr);
	void releaseDefault();
#endif
	//..,
public:
	//..,
	KrazyAsset();
	virtual ~KrazyAsset(void);
	//..,
	bool open_asset(const char* _name , unsigned int _mode = 0);
	int open_filedescriptor(off_t* _start , off_t* length);
	int read_asset(char* buf , int size);
	int close_asset();
private:
	//
	void setAssetManager();
	//
	static int read_asset(void* cookie , char* buf , int size);
	static int write_asset(void* cookie , char* buf , int size);
	static int seek_asset(void* cookie , fpos_t offset , int whence);
	static int close_asset(void* cookie);
	//..,
#ifdef _ENABLE_SINGLETON
	AAssetManager*			_AssetManager;
	static KrazyAsset*		_Me;
#else
	AAssetManager*			_AssetManager;
	AAsset*					_Asset;	
#endif 
	//..,
	
};
#endif



