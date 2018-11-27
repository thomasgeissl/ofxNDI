#include "ofxNDIGrabber.h"

ofxNDIGrabber::ofxNDIGrabber(){
    _finder = NULL;
    _receiver = NULL;
    _sources = NULL;
    _bandWidth = NDIlib_recv_bandwidth_highest;
    
    _initialized = NDIlib_initialize();
	if (!_initialized)
	{
		ofLogFatalError("ofxNDIGrabber")<<"cannot run NDI";
		ofLogNotice("ofxNDI")<<"Most likely because the CPU is not sufficient (see SDK documentation). You can check this directly with a call to NDIlib_is_supported_CPU()";
	}
    
    createFinder();
    findSources();
    createReceiver();
}

ofxNDIGrabber::~ofxNDIGrabber(){
    if(_receiver) NDIlib_recv_destroy(_receiver);
    if(_finder) NDIlib_find_destroy(_finder);
    if(isInitialized())	NDIlib_destroy();
}

bool ofxNDIGrabber::setup(int w, int h){
    ofLogNotice("ofxNDIGrabber")<<"ofxNDIGrabber::setup has no effect";
    return true;
}

void ofxNDIGrabber::update(){
    if(!isInitialized()) return;
    {	// The descriptors
        NDIlib_video_frame_t video_frame;
        NDIlib_audio_frame_t audio_frame;
        NDIlib_metadata_frame_t metadata_frame;

        switch (NDIlib_recv_capture(_receiver, &video_frame, &audio_frame, &metadata_frame, 0))
        {
        // No data
        case NDIlib_frame_type_none:
            ofLogVerbose("ofxNDIGrabber") << "No data received";
            break;
        case NDIlib_frame_type_error:
            break;
            // Video data
        case NDIlib_frame_type_video:
            _newFrame = true;
            ofLogVerbose("ofxNDIGrabber") << "Video data received "<< video_frame.xres << ", "<< video_frame.yres;
            _pixels.clear();
            if(!_pixels.isAllocated()){
                _pixels.allocate(video_frame.xres, video_frame.yres, 4);
            }
            for(int y = 0; y < video_frame.yres; y++){
                for(int x = 0; x < video_frame.xres-3; x++){
                    auto index = (x*4 + y*_pixels.getWidth()*4);
                    _pixels.setColor(x,y,
                                     ofColor(video_frame.p_data[index+2],
                                     video_frame.p_data[index+1],
                            video_frame.p_data[index]));



                }
            }
//            ofLogNotice("pixels")<<"width "<<_pixels.getWidth()<<" height "<<_pixels.getHeight();
            NDIlib_recv_free_video(_receiver, &video_frame);
            break;

            // Audio data
        case NDIlib_frame_type_audio:
            printf("Audio data received (%d samples).\n", audio_frame.no_samples);
            NDIlib_recv_free_audio(_receiver, &audio_frame);
            break;

            // Meta data
        case NDIlib_frame_type_metadata:
            printf("Meta data received.\n");
            NDIlib_recv_free_metadata(_receiver, &metadata_frame);
            break;
        }
    }
    _image.setFromPixels(_pixels);
}

void ofxNDIGrabber::draw(float x, float y) const {
    if(_image.isAllocated()){
        _image.draw(x,y);
    }
}

void ofxNDIGrabber::draw(float x, float y, float width, float height) const {
    if(_image.isAllocated()){
        _image.draw(x, y, width, height);
    }
}

void ofxNDIGrabber::close(){
    if(_receiver){
        NDIlib_recv_destroy(_receiver);
    }
}


void ofxNDIGrabber::threadedFunction(){
    while (isThreadRunning()) {
        update();
    }
}

bool ofxNDIGrabber::isFrameNew() const{
    return _newFrame;
}
bool ofxNDIGrabber::setPixelFormat(ofPixelFormat pixelFormat){
    ofLogNotice("ofxNDIGrabber")<<"ofxNDIGrabber::setPixelFormat has no effect";
    return true;
}

bool ofxNDIGrabber::isInitialized() const{
    return _initialized;
}

float ofxNDIGrabber::getHeight() const{
    return _pixels.getHeight();
}

float ofxNDIGrabber::getWidth() const{
    return _pixels.getWidth();
}

ofPixelFormat ofxNDIGrabber::getPixelFormat() const{
    return _pixels.getPixelFormat();
}

std::vector<ofVideoDevice> ofxNDIGrabber::listDevices() const{ 
    std::vector<ofVideoDevice> devices;
    for(auto i = 0; i < _numberOfSources; i++){
        devices.push_back(ofVideoDevice());
        devices[i].deviceName = _sources[i].p_ndi_name;
        devices[i].id = i;
    }
    return devices;
}
void ofxNDIGrabber::setDevice(ofVideoDevice device){
    createReceiver(device.id);
}
void ofxNDIGrabber::setDevice(int id){
    createReceiver(id);
}
void ofxNDIGrabber::setDevice(std::string name){
    for(auto i = 0; i < _numberOfSources; i++){
        auto source = _sources[i];
        if(source.p_ndi_name == name){
            createReceiver(i);
        }
    }
}


std::string ofxNDIGrabber::getNDIVersion(){
    return NDIlib_version();
}

ofPixels &ofxNDIGrabber::getPixels(){
    _newFrame = false;
    return _pixels;
}

const ofPixels &ofxNDIGrabber::getPixels() const{
    return _pixels;
}

void ofxNDIGrabber::setLowBandwidth(bool value){
    if(value){
        _bandWidth = NDIlib_recv_bandwidth_lowest;
    }else{
        _bandWidth = NDIlib_recv_bandwidth_highest;
    }
}

bool ofxNDIGrabber::createFinder(){
    if(_finder){
        NDIlib_find_destroy(_finder);
    }
    
    const NDIlib_find_create_t NDI_find_create_desc = { TRUE, NULL };
    _finder = NDIlib_find_create(&NDI_find_create_desc);
    if(!_finder){
        ofLogError("ofxNDIGrabber")<<"could not create finder";
    }else{
        ofLogNotice("ofxNDIGrabber")<<"successfully created finder";
    }
    
    return _finder;
}
int ofxNDIGrabber::findSources(){
    uint32_t no_sources = 0;
    ofLogNotice("main")<<"waiting for sources";
    _sources = NDIlib_find_get_sources(_finder, &no_sources, 3000);
    ofLogNotice("ofxNDIGrabber")<<"found " << no_sources <<" sources ";
    _numberOfSources = no_sources;
    return no_sources;
}

bool ofxNDIGrabber::createReceiver(int id){
    if(id >= _numberOfSources) return false;
    if(_receiver){
        NDIlib_recv_destroy(_receiver);
    }
    
#ifdef TARGET_WIN32
    NDIlib_recv_create_t NDI_recv_create_desc = { _sources[id], NDIlib_recv_color_format_e_RGBX_RGBA, _bandWidth, TRUE };
#endif
#ifdef TARGET_OSX
    NDIlib_recv_create_t NDI_recv_create_desc = { _sources[id], FALSE, _bandWidth, TRUE };
#endif
    _receiver = NDIlib_recv_create2(&NDI_recv_create_desc);
    
    if (!_receiver){
        ofLogFatalError("ofxNDIGrabber")<<"cannot create NDI receiver";
    }
    return _receiver;
}
