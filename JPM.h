#ifndef JPM_H_INCLUDED
#define JPM_H_INCLUDED

#include "JuicyGUI_Definitions.h"

JD_INDEX JPM_GetEventIndex(JD_FLAG iEvent);

class JPM {
	public:
		JPM();
		JPM(void* iDefault);
		JPM(JD_INDEX iNumExtras);
		JPM(JD_INDEX iNumExtras, void* iDefault);
		~JPM();
		bool PollProperties(void** oProperties, JD_INDEX* oIndex);
		void SetPropertiesAtIndex(JD_INDEX iIndex, void* iProperty);
		void* GetPropertiesAtIndex(JD_INDEX iIndex);
		void SetProperties(JD_FLAG iEvent, void* iProperty);
		void* GetProperties(JD_FLAG iEvent);
		JD_INDEX GetSize() {return numTotal;};
	private:
	    void init(JD_INDEX iNumExtras, void* iDefault);
	    JD_INDEX ctrPoll;
		JD_INDEX numProperties;
		JD_INDEX numExtras;
		JD_INDEX numTotal;
		JD_INDEX indexDefault;
		void** properties;
};

#endif // JPM_H_INCLUDED
