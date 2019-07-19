#pragma once

#include "ofMain.h"

#pragma comment(lib, "k4a.lib")
#include <k4a/k4a.h>

class ofxAzureKinect {
public:

	ofxAzureKinect();
	~ofxAzureKinect();

	 bool init(k4a_image_format_t color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32, 
		 k4a_color_resolution_t color_resolution = K4A_COLOR_RESOLUTION_3072P, 
		 k4a_depth_mode_t depth_mode = K4A_DEPTH_MODE_NFOV_2X2BINNED, 
		 k4a_fps_t camera_fps = K4A_FRAMES_PER_SECOND_30);
	 
	 void clear();
	 
	 bool open(int deviceIndex = 0);
	 bool open(string serial);

	 bool close();

	 bool isConnected();
	 bool isFrameNew();
	 bool hasAccelControl();

	 void enableDepthNearValueWhite(bool val);
	 bool isDepthNearValueWhite();

	 void getMksAccel();
	 void getDistanceAt(int x, int y);
	 void getColorAt(int x, int y);
	 void getWorldCoordinateAt(int x, int y);

	 void getDepthPixels();

	 void setLED();

	 void update();

	 void draw();
	 void drawDepth();

	 /// \section Util

	 /// get the unique serial number
	 /// returns an empty string "" if not connected
	 ///
	 /// NOTE: currently, libfreenect returns a serial number with all 0s for
	 /// kinect models > 1414, so this will only work with the original xbox kinect
	 string getSerial() const;

	 /// static kinect image size
	 const static int width = 640;
	 const static int height = 480;
	 float getHeight() const;
	 float getWidth() const;

	 /// \section Static global kinect context functions

		 /// print the device list
	 static void listDevices();

	 /// get the total number of devices
	 static int numTotalDevices();

	 /// get the number of available devices (not connected)
	 static int numAvailableDevices();

	 /// get the number of currently connected devices
	 static int numConnectedDevices();

	 /// is a device already connected?
	 static bool isDeviceConnected(int id);
	 static bool isDeviceConnected(string serial);

	 /// get the id of the next available device,
	 /// returns -1 if nothing found
	 static int nextAvailableId();

	 /// get the serial number of the next available device,
	 /// returns an empty string "" if nothing found
	 static string nextAvailableSerial();

	 /// set the time to wait when not getting data before attempting to re-open device.
	 static void setReconnectWaitTime(float waitTime);

protected:

	int deviceId;	///< -1 when not connected
	string serial;	///< unique serial number, "" when not connected


	bool bUseTexture;
	ofTexture depthTex; ///< the depth texture
	ofTexture videoTex; ///< the RGB texture
	bool bGrabberInited;

	ofPixels videoPixels;
	ofPixels depthPixels;
	ofShortPixels depthPixelsRaw;
	ofFloatPixels distancePixels;

	ofPoint rawAccel;
	ofPoint mksAccel;

	// for auto connect tries
	float timeSinceOpen;
	static float reconnectWaitTime;
	int lastDeviceIndex;
	bool bGotDataDepth;
	bool bGotDataVideo;
	bool bFirstUpdate;
	int tryCount;

private:

	k4a_device_t device;
	k4a_device_configuration_t config;
	
	size_t serial_number_length;
	char * serial_number;


};