#include "StdAfx.h"
#include "KrazyRangToonaMovingPolicy_01Phase.h"
#include "KrazyLevelThriver.h"
//
KrazyRangToonaMovingPolicy_01Phase::KrazyRangToonaMovingPolicy_01Phase(void)
{
}
//
KrazyRangToonaMovingPolicy_01Phase::~KrazyRangToonaMovingPolicy_01Phase(void)
{
}
//
void KrazyRangToonaMovingPolicy_01Phase::onExecute(KrazyVehical* _vehical , KrazyALGODiagnostic** diagnostic)
{
	ON_EXECUTE_DEFAULT_GAMEPLAYSTATE();
	*diagnostic = &_AlgoDiagnostic;
}
//
void KrazyRangToonaMovingPolicy_01Phase::reset()
{
}
//
