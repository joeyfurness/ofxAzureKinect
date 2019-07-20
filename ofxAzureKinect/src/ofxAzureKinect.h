#pragma once

#include "ofMain.h"

#pragma comment(lib, "k4a.lib")
#include <k4a/k4a.h>

class ofxAzureKinect {
public:

	ofxAzureKinect();
	~ofxAzureKinect();

	 bool init(k4a_image_format_t color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32, 
		 k4a_color_resolution_t color_resolution = K4A_COLOR_RESOLUTION_1080P, 
		 k4a_depth_mode_t depth_mode = K4A_DEPTH_MODE_NFOV_2X2BINNED, 
		 k4a_fps_t camera_fps = K4A_FRAMES_PER_SECOND_30);
	 
	 void clear();
	 
	 bool open(int deviceIndex = 0);
	 bool open(string serial);

	 bool close();

	 void update();

	 void draw();
	 void drawDepth();

	 /// \section Util

	 /// get the unique serial number
	 /// returns an empty string "" if not connected
	 ///
	 /// NOTE: currently, libfreenect returns a serial number with all 0s for
	 /// kinect models > 1414, so this will only work with the original xbox kinect
	 // string getSerial() const;

	 void setSynchronizedImages(bool bUseSynchronizedImages = true);

	 /// static kinect image size
	 float getHeight() const;
	 float getWidth() const;


protected:

	int deviceId;	///< -1 when not connected
	string serial;	///< unique serial number, "" when not connected

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
	k4a_calibration_t calibration;
	
	size_t serial_number_length;
	char * serial_number;

	bool bUseSynchronizedImages;

	const int32_t TIMEOUT_IN_MS = 1000;
	k4a_capture_t capture;
	k4a_image_t colorImage;
	k4a_image_t depthImage;

	int height;
	int width;

	ofTexture depthTexture;
	ofTexture colorTexture;
	ofTexture irTexture;

	ofPixels depthPixels;
	ofPixels colorPixels;
	ofPixels irPixels;

};