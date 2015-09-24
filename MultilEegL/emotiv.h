#include <sstream>
#include <windows.h>
#include <map>

#include "../include/EmoStateDLL.h"
#include "../include/edk.h"
#include "../include/edkErrorCode.h"

#pragma comment(lib, "../lib/edk.lib")

EE_DataChannel_t targetChannelList[] = {
	ED_COUNTER,
	ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
	ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
	ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
	ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
};

const char header[] = "COUNTER,AF3,F7,F3, FC5, T7, P7, O1, O2,P8" 
	", T8, FC6, F4,F8, AF4,GYROX, GYROY, TIMESTAMP, "   
	"FUNC_ID, FUNC_VALUE, MARKER, SYNC_SIGNAL,";
