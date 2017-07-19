#pragma once
#ifndef _KRAZYREDUNDENTCLASSDECLARATION_H_
#define _KRAZYREDUNDENTCLASSDECLARATION_H_
///////////////////////////////////////////////////////
#define KRAZY_DATA_ENCAPSULATION(class_type , base)\
	protected:\
	class class_type : public base\
	{ public:void reset(){base::reset();}\
	  public:virtual ~class_type(){}\
	};\
/////////////////////////////////////////////////////////////
#define _KRAZY_DATA_ENCAPSULATION_PARAM_INIT_(class_type , base , param)\
	protected:\
	class class_type : public base\
	{ \
	   public:class_type(param* obj) : base(obj){}\
	   public:void reset(){base::reset();}\
	   public:virtual ~class_type(){}\
	};\
///////////////////////////////////////////////////////////////
#define  KRAZY_DATA_LIST_ENCAPSULATION(class_type , base)\
	protected:\
	class class_type : public base\
	{\
	  public:void reset(){base::reset();}\
	  public:void clean_up(){base::clean_up();}\
	  public:virtual ~class_type(){}\
	};\
///////////////////////////////////////////////////////////////
#define _KRAZY_DATA_POLICY_ENCAPSULATION(class_type ,base)\
	private:\
	class class_type : public base\
	{\
	public:class_type(){}\
    public:bool onExecute(){return base::onExecute();}\
	public:virtual ~class_type(){}\
	public:void reset(){ base::reset(); }\
	};\
//////////////////////////////////////////////////////////////////
#define _KRAZY_DATA_POLICY_ENCAPSULATION_PARAM_RES(class_type ,base , param , res)\
	private:\
	class class_type : public base\
	{\
	public:class_type(){}\
    public:void onExecute(param* ptr_param , res** ptr_res){ base::onExecute(ptr_param,ptr_res);}\
	public:virtual ~class_type(){}\
	public:void reset(){ base::reset(); }\
	};\
///////////////////////////////////////////////////////////////////	
#endif

