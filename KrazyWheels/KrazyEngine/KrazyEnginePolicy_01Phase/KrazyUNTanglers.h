#pragma once
#include "KrazyVehical.h"
class KrazyUNTanglers
{
public:
	
	static void _check_queued_vehical(KrazyVehical* _vehical);
	static void _unqueue_prev_vehical_after_turn(KrazyVehical* _vehical);
	static KrazyBool _lost_track(KrazyVehical* _vehical);

};

