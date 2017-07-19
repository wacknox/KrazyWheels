#pragma once
#include "KrazyInterfaceGamePlayPolicy.h"
#include "KrazyAlgoDiagnosticData_01Phase.h"
#include "KrazyRedundentClassDeclarationMacros.h"
#include "KrazyPrimitiveDataCollections.h"
#include "KrazyRangToonaMovingPolicy_01Phase.h"
#include "KrazyVehicalGameplayStateMovingPolicy_01Phase.h"
#include "KrazyBangToona.h"
#include "KrazySherrifMovingPolicy_01Phase.h"
#include "KrazyGanguniMovingPolicy_01Phase.h"
#include "KrazyUnaBrothersMovingPolicy_01Phase.h"
#include "KrazyUnaFamilyTutorialPolicy.h"
#include "KrazyToonaBrotherTutorialPolicy.h"
#include "KrazySherrifTutorialMovingPolicy.h"
////////////////////////////
class KrazyMovingPlayerCarPolicy_01Phase : public KrazyInterfaceGamePlayPolicy<KrazyALGODiagnostic>
{	

protected:
	bool onExecute();
	void reset();
	KrazyMovingPlayerCarPolicy_01Phase();
	~KrazyMovingPlayerCarPolicy_01Phase(void);
	
	KrazyRangToonaMovingPolicy_01Phase			_RangtoonaMoving;
	KrazyBangToona								_BangtoonaMoving;

	KrazyGanguniMovingPolicy_01Phase			_GangiMoving;
	KrazySherrifMovingPolicy_01Phase			_SherrifMoving;
	KrazyUnaBrothersMovingPolicy_01Phase		_PangunaMoving;
	KrazyUnaBrothersMovingPolicy_01Phase		_NangunaMoving;
	//..,
	KrazyGanguniMovingTutorialPolicy          _GangiTutorial;
	KrazyUnaaFamilyTutorialPolicy			  _PangunaTutorial;
	KrazyUnaaFamilyTutorialPolicy			  _NangunaTutorial;
	KrazyToonaBrotherTutorialPolicy			  _RangToonaTutorial;
	KrazyToonaBrotherTutorialPolicy			  _BangToonaTutorial;
	KrazySherrifTutorialMovingPolicy		  _SherrifTutorial;


};


