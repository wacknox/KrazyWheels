#include "StdAfx.h"
#include "KrazyGanguniMovingPolicy_01Phase.h"
#include "KrazyLevelThriver.h"
//
KrazyGanguniMovingPolicy_01Phase::KrazyGanguniMovingPolicy_01Phase(void)
{
}
//
KrazyGanguniMovingPolicy_01Phase::~KrazyGanguniMovingPolicy_01Phase(void)
{
}
//
void KrazyGanguniMovingPolicy_01Phase::onExecute(KrazyVehical* _vehical , KrazyALGODiagnostic** diagnostic)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
	*diagnostic = &_AlgoDiagnostic;
}
//
void KrazyGanguniMovingPolicy_01Phase::reset()
{
}
//