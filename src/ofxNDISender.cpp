#include "ofxNDISender.h"

ofxNDISender::ofxNDISender(string name)
{
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
		(uint8_t*)malloc(width* height * 4),
		// The line to line stride of this image
		(unsigned int)(width) * 4
	};
	memset((void*)_frame.p_data, 0, width * height * 4);
}

void ofxNDISender::setMetaData(string longName,
string shortName,
string manufacturer,
string version,
string session,
string modelName,
string serial){

	NDIlib_metadata_frame_t metaData;

	std::string xml =	"<ndi_product long_name=\""	+ longName 		+ "\" " +
	 					"short_name=\""				+ shortName 	+ "\" " +
	 					"manufacturer=\""			+ manufacturer 	+ "\" " + 
	 					"version=\"" 				+ version 		+ "\" " +
	 					"session=\"" 				+ session 		+ "\" " +
	 					"model_name=\"" 			+ modelName 	+ "\" " +
	 					"serial=\"" 				+ serial 		+ "\">" ;
	
	// char *cstr = new char[xml.length() + 1];
	// strcpy(cstr, xml.c_str());
	

	std::vector<char> chars(xml.c_str(), xml.c_str() + xml.size() + 1u);

	metaData.p_data  =  &chars[0];
	metaData.timecode = NDIlib_send_timecode_synthesize;
	//  = {
	// 	// The length
	// 	(DWORD)::strlen(metaDataString.c_str()),
	// 	// Timecode (synthesized for us !)
	// 	NDIlib_send_timecode_synthesize,
	// 	// The string
	// 	(CHAR*)metaDataString.c_str()
	// };
	NDIlib_send_add_connection_metadata(_sender, &metaData);
}

void ofxNDISender::send(ofPixels & pixels){
	if(_frame.xres != pixels.getWidth() || _frame.yres != pixels.getHeight()){
		ofLogNotice() << "allocate";

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
			(uint8_t*)malloc(pixels.getWidth() * pixels.getHeight() * 4),
			// The line to line stride of this image
			(unsigned int)(pixels.getWidth()) * 4
		};
	}

	switch(pixels.getPixelFormat()){
	case OF_PIXELS_RGB:
	case OF_PIXELS_BGR:
	{
		for(int y = 0; y < pixels.getHeight(); y++){
			for(int x = 0; x < pixels.getWidth(); x++){
				auto index = (x*4 + y*pixels.getWidth()*4);
				_frame.p_data[index] = pixels.getColor(x, y).b;
				_frame.p_data[index+1] = pixels.getColor(x, y).g;
				_frame.p_data[index+2] = pixels.getColor(x, y).r;
				_frame.p_data[index+3] = 255;
			}
		}
		break;
	}
	case OF_PIXELS_RGBA:
	case OF_PIXELS_BGRA:
	{
		for(int y = 0; y < pixels.getHeight(); y++){
			for(int x = 0; x < pixels.getWidth(); x++){
				auto index = (x*4 + y*pixels.getWidth()*4);
				_frame.p_data[index] = pixels.getColor(x, y).b;
				_frame.p_data[index+1] = pixels.getColor(x, y).g;
				_frame.p_data[index+2] = pixels.getColor(x, y).r;
				_frame.p_data[index+3] = pixels.getColor(x, y).a;
			}
		}
	}
	default:{
		ofLogError("ofxNDISender")<<"pixel type "<<pixels.getPixelFormat()<<" is not supported yet";
		return;
		break;
	}
	}

	NDIlib_send_send_video_v2(_sender, &_frame);
}


