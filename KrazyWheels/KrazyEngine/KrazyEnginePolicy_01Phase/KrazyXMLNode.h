#pragma once
#include "KrazyPrimitiveDataCollections.h"
using namespace std;
//..,,
#define _KRAZY_GET_INT(res , ptr) res = (ptr) ? atoi(ptr->get_Data().c_str()) : -1;
#define _KRAZY_GET_FLOAT(res,ptr) res = (ptr) ? float(atof(ptr->get_Data().c_str())) : 0.0f;
#define _KRAZY_GET_INT_INDICES_FROM_STRING(indices , ptr_i) if(ptr_i){\
		string str = ptr_i->get_Data();\
		int count = str.size();\
		string aux; \
		for(int i = 0;i<count;i++){\
			char ch = str.at(i);\
			switch(ch){\
			case ',':indices.push_back(atoi(aux.c_str()));aux.clear();break;\
			default :aux.push_back(ch);\
			         if( i + 1 == count ){\
						indices.push_back(atoi(aux.c_str()));\
						aux.clear();}\
						break;\
			}\
		}\
	}\
//
#define _KRAZY_GET_FLOAT_INDICES_FROM_STRING(indices , ptr)\
 if(ptr){\
		string str = ptr->get_Data();\
		int count = str.size();\
		string aux; \
		for(int i = 0; i < count; i++){\
			char ch = str.at(i);\
			switch(ch){\
			case ',':indices.push_back(float(atof(aux.c_str())));aux.clear();break;\
				default :aux.push_back(ch);\
			         if( i + 1 == count ){\
					 indices.push_back(float(atof(aux.c_str())));aux.clear();}\
					 break;\
			}\
		}\
	}\

//..,,
struct KrazyXMLAttribute;
class KrazyXMLNode;
class KrazyXMLNodeList : public vector<KrazyXMLNode*>{};
class KrazyXMLAttributes : public vector<KrazyXMLAttribute*>{};
//
struct KrazyXMLAttribute
{
	KrazyXMLAttribute(string name , string data) :
    _Name(name),
    _Data(data)
	{
	
	}
	void get_Name_Data(string& name , string& data)
	{
		name = _Name;
		data = _Data;
	}
	string get_Name(){ return _Name;}
	string get_Data(){ return _Data;}

private:
	
	string _Name;
	string _Data;
};



class KrazyXMLNode
{


public:

	static KrazyXMLNode* getXMLNodeParent();

	static void releaseParent()
	{
		if(_PTRMe)
		{
			delete _PTRMe;_PTRMe = 0;
		}
	}

	static void cleanUpData()
	{
		if(_PTRMe)
		{
			_PTRMe->cleanUP();
		}
	}
	
	static void loadXML(string xml)
	{
		if(!_PTRMe)
			return;
		
		_PTRMe->load_XML(xml);
	}

	static string getXML()
	{
		if(!_PTRMe)
			return string("");

		return _PTRMe->get_XML();
	}

	 KrazyXMLNode* getNewNode()
	 {
		 int count = _Kids.size();
		 _Kids.push_back(new KrazyXMLNode(this));
		 return _Kids[count] ;
     }

	 KrazyXMLAttribute* getNewAttribute(string name , string value)
	 {
		 int count = _Attributes.size();
		 _Attributes.push_back(new KrazyXMLAttribute(name,value));
		 return _Attributes[count];
	 }


	void				set_NodeName(string name) {_NodeName = name;}	
	
	KrazyXMLNode*		get_PTRParent() {return _PTRParent;}
	string			    get_NodeName()  {return _NodeName;}
	int				    get_NodeIndex() {return _NodeIndex;}
	int					get_Kids_Count(){return _Kids.size();}
	int					get_Attributes_Count(){return _Attributes.size();}

	KrazyXMLNode*		get_KidsNode(int index) {return _Kids[index];}
	KrazyXMLAttribute*  get_Attribute(int index){return _Attributes[index];}
	KrazyXMLAttribute*  get_AttributeByTypeIfAny(string type);

protected:	
	
	
KrazyXMLNode(KrazyXMLNode* parent = 0) : 
  _NodeIndex(0),
  _PTRParent(parent)
  {
  
  }

 virtual ~KrazyXMLNode()
  {
	  cleanUP();
  }

  void cleanUP(){
	int count = _Kids.size();  
	for(int i = 0; i < count; i++)
	{
		delete _Kids[i];
	}
	_Kids.clear();
	//..,
	count = _Attributes.size();
	for(int i = 0; i < count; i++)
	{
		delete _Attributes[i];
	}
	_Attributes.clear();
  }


 
  string get_XML();
  void   load_XML(string xml);
  void   parse();
   
private:

  
  KrazyXMLNode*      _PTRParent;
  string			 _NodeName;
  int				 _NodeIndex;
  KrazyXMLNodeList   _Kids;
  KrazyXMLAttributes _Attributes;
  string			 _XML;

public:
  static KrazyXMLNode* _PTRMe;
};






  



