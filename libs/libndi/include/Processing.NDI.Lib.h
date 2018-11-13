#pragma once

//-------------------------------------------------------------------------------------------------------------------
// (c)2016 NewTek, inc.
//
// This library is provided under the license terms that are provided within the 
// NDI SDK installer. If you do not expressely agree to these terms then this library
// may be used for no purpose at all.
//
// This current SDK is BETA and may not be used in any product release. It is provided
// for development use only.
//
// For any questions or comments please email: ndi@newtek.com
// 
//-------------------------------------------------------------------------------------------------------------------

// Is this library being compiled, or imported by another application.
#ifdef _WIN32
#ifdef PROCESSINGNDILIB_EXPORTS
#ifdef __cplusplus
#define PROCESSINGNDILIB_API extern "C" __declspec(dllexport)
#else
#define PROCESSINGNDILIB_API __declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define PROCESSINGNDILIB_API extern "C" __declspec(dllimport)
#else
#define PROCESSINGNDILIB_API __declspec(dllimport)
#endif
#endif
#else
#ifdef __cplusplus
#define PROCESSINGNDILIB_API extern "C" __attribute((visibility("default")))
#else
#define PROCESSINGNDILIB_API __attribute((visibility("default")))
#endif
#endif

// Data structures shared by multiple SDKs
#include "Processing.NDI.compat.h"
#include "Processing.NDI.structs.h"

// This is not actually required, but will start and end the libraries which might get
// you slightly better performance in some cases. In general it is more "correct" to 
// call these although it is not required. There is no way to call these that would have
// an adverse impact on anything (even calling destroy before you've deleted all your
// objects). This will return false if the CPU is not sufficiently capable to run NDILib
// currently NDILib requires SSE4.2 instructions (see documentation). You can verify 
// a specific CPU against the library with a call to NDIlib_is_supported_CPU()
PROCESSINGNDILIB_API
const bool NDIlib_initialize(void);

PROCESSINGNDILIB_API
void NDIlib_destroy(void);

PROCESSINGNDILIB_API
const char* NDIlib_version(void);

// Recover whether the current CPU in the system is capable of running NDILib. Currently
// NDILib requires SSE4.1 https://en.wikipedia.org/wiki/SSE4 Creating devices when your 
// CPU is not capable will return NULL and not crash. This function is provided to help
// understand why they cannot be created or warn users before they run.
PROCESSINGNDILIB_API
const bool NDIlib_is_supported_CPU(void);

// The main SDKs
#include "Processing.NDI.Find.h"
#include "Processing.NDI.Recv.h"
#include "Processing.NDI.Send.h"

// Utility functions
#include "Processing.NDI.utilities.h"
