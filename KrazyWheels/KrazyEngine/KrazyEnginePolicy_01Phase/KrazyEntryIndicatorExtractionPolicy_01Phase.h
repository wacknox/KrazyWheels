#pragma once
#include "KrazyInterfaceGamePlayPolicy.h"
#include "KrazyAlgoDiagnosticData_01Phase.h"
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyTrafficIndicator.h"
//class KrazyTrafficIndicator;
class KrazyVehical;
class KrazyEntryIndicatorExtractionPolicy_01Phase : public KrazyInterfaceGamePlayPolicy<KrazyALGODiagnostic>
{
protected:
	KrazyEntryIndicatorExtractionPolicy_01Phase();
	virtual ~KrazyEntryIndicatorExtractionPolicy_01Phase(void);
	bool onExecute();
	void reset(){}
	vector<KrazyTrafficIndicator*>* getEntryIndicators();

	//vector<KrazyTrafficIndicator*>* _PTREntryIndicators;
	unsigned int _Wait;
	unsigned int _LastIndex;
	KrazyVehical* _PTRPrev;
	int _Counter;
};




