#pragma once
#ifndef _KRAZYINTERFACEGAME_POLICY_H_
#define _KRAZYINTERFACEGAME_POLICY_H_
class KrazyVehical;
template<typename KRAZY_ALGO_DIAGNOSTIC_TYPE>
class KrazyInterfaceGamePlayPolicy
{
public:
	KrazyInterfaceGamePlayPolicy() {}
	~KrazyInterfaceGamePlayPolicy(){}
	KRAZY_ALGO_DIAGNOSTIC_TYPE* GetDiagnosticPtr(){ return &_AlgoDiagnostic; }
protected:	
	KRAZY_ALGO_DIAGNOSTIC_TYPE					_AlgoDiagnostic;
};
#endif






