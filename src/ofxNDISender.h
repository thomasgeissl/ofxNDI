#pragma once
#include "ofMain.h"

#include "Processing.NDI.compat.h"
//#include "Processing.NDI.structs.h"
#include "Processing.NDI.Lib.h"
#include "Processing.NDI.Find.h"
#include "Processing.NDI.utilities.h"
#include "Processing.NDI.Recv.h"
#include "Processing.NDI.Send.h"

class ofxNDISender{
	public:
		ofxNDISender(string name = "ofxNDISender");

		void setMetaData(string longName, string shortName, string manufacturer, string version, string session, string modelName, string serial);
		void send(ofPixels & pixels);

	private:
	NDIlib_send_instance_t _sender;
	NDIlib_video_frame_v2_t _frame;
};
