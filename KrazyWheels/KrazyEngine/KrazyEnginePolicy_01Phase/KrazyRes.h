#pragma once
#include <string>
using namespace std;
class KrazyRes
{
public:
	static KrazyRes* getDefault();
	static void releaseDefault();
	void reset();
	//..,
	string& apk_path();
protected:
	KrazyRes(void);
	virtual ~KrazyRes(void);
	//..,
	string				_APKPath;
	static KrazyRes*	_Me;
};

