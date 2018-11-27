#include "ofxNDISender.h"

ofxNDISender::ofxNDISender(std::string name){
	const NDIlib_send_create_t descriptor = {name.c_str(), NULL, TRUE, FALSE};
	_sender = NDIlib_send_create(&descriptor);
	if (!NDIlib_initialize())
	{
		ofLogFatalError("ofxNDISender")<<"cannot run NDI";
		ofLogNotice("ofxNDI")<<"Most likely because the CPU is not sufficient (see SDK documentation). You can check this directly with a call to NDIlib_is_supported_CPU()";
	}

	unsigned int width = 640;
	unsigned int height = 480;
	_frame = {
		// Resolution
		width,
		height,
		// We will stick with RGB color space. Note however that it is generally better to
		// use YCbCr colors spaces if you can since they get end-to-end better video quality
		// and better performance because there is no color dconversion
		NDIlib_FourCC_type_BGRA,
		// The frame-eate
		30000, 1001,
		// The aspect ratio (16:9)
		(float)(width)/
		height,
		// This is not a progressive frame
		FALSE,
		// Timecode (synthesized for us !)
		NDIlib_send_timecode_synthesize,
		// The video memory used for this frame
		(BYTE*)malloc(width* height * 4),
		// The line to line stride of this image
		(unsigned int)(width) * 4
	};
	memset((void*)_frame.p_data, 0, width * height * 4);
}

void ofxNDISender::setMetaData(std::string longName, std::string shortName, std::string manufacturer, std::string version, std::string session, std::string modelName, std::string serial){
	std::string metaDataString;
	metaDataString += "<ndi_product long_name=\"+longName+\" ";
	metaDataString += "short_name=\"+shortName+\" ";
	metaDataString += "manufacturer=\"+manufacturer+\" ";
	metaDataString += "version=\"+version+\" ";
	metaDataString += "session=\"+session+\" ";
	metaDataString += "model_name=\"+modelName+\" ";
	metaDataString += "serial=\"+serial+\">";
	const NDIlib_metadata_frame_t metaData = {
		// The length
		(DWORD)::strlen(metaDataString.c_str()),
		// Timecode (synthesized for us !)
		NDIlib_send_timecode_synthesize,
		// The string
		(CHAR*)metaDataString.c_str()
	};
	NDIlib_send_add_connection_metadata(_sender, &metaData);
}

void ofxNDISender::send(ofPixels & pixels){
	if(_frame.xres != pixels.getWidth() || _frame.yres != pixels.getHeight()){
		ofLogNotice()<<"allocate";
		_frame = {
			// Resolution
			(unsigned int) (pixels.getWidth()),
			(unsigned int) (pixels.getHeight()),
			// We will stick with RGB color space. Note however that it is generally better to
			// use YCbCr colors spaces if you can since they get end-to-end better video quality
			// and better performance because there is no color dconversion
			NDIlib_FourCC_type_BGRA,
			// The frame-eate
			30000, 1001,
			// The aspect ratio (16:9)
			(float) (pixels.getWidth())/
			(float) (pixels.getHeight()),
			// This is not a progressive frame
			FALSE,
			// Timecode (synthesized for us !)
			NDIlib_send_timecode_synthesize,
			// The video memory used for this frame
			(BYTE*)malloc(pixels.getWidth() * pixels.getHeight() * 4),
			// The line to line stride of this image
			(unsigned int)(pixels.getWidth()) * 4
		};
	}

    switch(pixels.getPixelFormat()){
        case OF_PIXELS_RGB:
        case OF_PIXELS_BGR:
        {
            int index = 0;
            for(auto line: pixels.getLines()){
                for(auto pixel: line.getPixels()){
                    _frame.p_data[index] = pixel[2];
                    _frame.p_data[index+1] = pixel[1];
                    _frame.p_data[index+2] = pixel[0];
                    _frame.p_data[index+3] = 255;
                    index += 4;
                }
            }
            break;
        }
        case OF_PIXELS_RGBA:
        case OF_PIXELS_BGRA:
        {
            int index = 0;
            for(auto line: pixels.getLines()){
                for(auto pixel: line.getPixels()){
                    _frame.p_data[index] = pixel[2];
                    _frame.p_data[index+1] = pixel[1];
                    _frame.p_data[index+2] = pixel[0];
                    _frame.p_data[index+3] = pixel[3];
                    index += 4;
                }
            }
        }
        default:{
            ofLogError("ofxNDISender")<<"pixel type "<<ofToString(pixels.getPixelFormat())<<" is not supported yet";
            return;
            break;
        }
    }
    
    NDIlib_send_send_video(_sender, &_frame);
}


