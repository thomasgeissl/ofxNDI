#pragma once
#include "ofMain.h"

#include "Processing.NDI.compat.h"
#include "Processing.NDI.structs.h"
#include "Processing.NDI.Lib.h"
#include "Processing.NDI.Find.h"
#include "Processing.NDI.utilities.h"
#include "Processing.NDI.Recv.h"
#include "Processing.NDI.Send.h"

class ofxNDIGrabber :
		public ofBaseVideoGrabber,
		public ofThread
{
	public:
		ofxNDIGrabber();
        ~ofxNDIGrabber();
    
        bool setup(int w, int h) override;
        void update() override;
        void draw(float x, float y) const;
        void draw(float x, float y, float width, float height) const;
        void close() override;

		void threadedFunction() override;
        bool isInitialized() const override;
		bool isFrameNew() const override;

        bool setPixelFormat(ofPixelFormat pixelFormat) override;

        float getHeight() const override;
        float getWidth() const override;
        ofPixelFormat getPixelFormat() const override;
    
        std::vector<ofVideoDevice> listDevices() const override;
        void setDevice(ofVideoDevice device);
        void setDevice(int index);
        void setDevice(std::string name);

		ofPixels& getPixels() override;
		const ofPixels& getPixels() const override;
    
        std::string getNDIVersion();

        void setLowBandwidth(bool value = true);
        bool createFinder();
        int findSources();
        bool createReceiver(int id = 0);
	private:
		NDIlib_recv_instance_t _receiver;
        NDIlib_find_instance_t _finder;
        const NDIlib_source_t* _sources;
        int _numberOfSources;
    
        NDIlib_recv_bandwidth_e _bandWidth;

		ofPixels _pixels;
		ofImage _image;
        std::vector<ofVideoDevice> _devices;
    
        bool _initialized;
        bool _newFrame;
};
