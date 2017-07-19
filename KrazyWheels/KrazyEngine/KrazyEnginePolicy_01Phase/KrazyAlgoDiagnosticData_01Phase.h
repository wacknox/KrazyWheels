#ifndef _KRAZY_ALGO_DIAGNOSTIC_DATA_01PHASE_H_
#define _KRAZY_ALGO_DIAGNOSTIC_DATA_01PHASE_H_
//
enum NextFrameCommandType
{
	NXT_FRAME_COMMAND_DEFAULT,
	NXT_FRAME_COMMAND_REMOVE,
	NXT_FRAME_COMMAND_ADD,
	NXT_FRAME_CONTINUE,
};
//
struct KrazyALGODiagnostic
{
	KrazyALGODiagnostic() :
	_eNextCommand(NXT_FRAME_COMMAND_DEFAULT),
	_iUpdateScore(0)
	{
	}
	NextFrameCommandType _eNextCommand;
	int					 _iUpdateScore;
};
//
#endif