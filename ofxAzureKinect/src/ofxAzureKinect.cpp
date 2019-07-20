#include "ofxAzureKinect.h"



ofxAzureKinect::ofxAzureKinect()
{
	device = NULL;
	config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;

	serial_number_length = 0;
	serial_number = NULL;

	bUseSynchronizedImages = true;

	capture = NULL;
	image = NULL;
}

ofxAzureKinect::~ofxAzureKinect()
{
	k4a_device_stop_cameras(device);
	k4a_device_close(device);
}

bool ofxAzureKinect::init(k4a_image_format_t color_format, k4a_color_resolution_t color_resolution,
	k4a_depth_mode_t depth_mode, k4a_fps_t camera_fps)
{
	// Set the device configuration that will be passed to the device handle
	config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	config.color_format = color_format;
	config.color_resolution = color_resolution;
	config.depth_mode = depth_mode;
	config.camera_fps = camera_fps;
	config.synchronized_images_only = bUseSynchronizedImages;



	ofLog(OF_LOG_NOTICE, "k4a device succesfully configured");

	return true;
}

bool ofxAzureKinect::open(int deviceIndex)
{
	// Open the first plugged in Kinect device
	if (K4A_FAILED(k4a_device_open(deviceIndex, &device)))
	{
		ofLog(OF_LOG_ERROR, "Failed to open k4a device!\n");
		return false;
	}

	// Get device calibration to get width and height information
	if (K4A_FAILED(k4a_device_get_calibration(device, config.depth_mode, config.color_resolution, &calibration)))
	{
		ofLog(OF_LOG_ERROR, "Failed to get calibration");
		k4a_device_close(device);
		return false;
	}
	width = calibration.color_camera_calibration.resolution_width;
	height = calibration.color_camera_calibration.resolution_height;
	ofLog(OF_LOG_NOTICE, "Color camera height: %d width: %d", height, width);

	// Start the camera with the given configuration
	if (K4A_FAILED(k4a_device_start_cameras(device, &config)))
	{
		ofLog(OF_LOG_ERROR, "Failed to start cameras!\n");
		k4a_device_close(device);
		return false;
	}

	ofLog(OF_LOG_NOTICE, "k4a device succesfully opened");
	return true;
}

bool ofxAzureKinect::close()
{
	k4a_device_stop_cameras(device);
	k4a_device_close(device);

	ofLog(OF_LOG_NOTICE, "k4a device succesfully closed");

	return true;
}

//string ofxAzureKinect::getSerial() const
//{
//	k4a_device_get_serialnum(device, NULL, &serial_number_length);
//
//	// Allocate memory for the serial, then acquire it
//	char *serial = (char*)(malloc(serial_number_length));
//	k4a_device_get_serialnum(device, serial, &serial_number_length);
//	ofLog(OF_LOG_NOTICE, "Opened device: %s\n", serial);
//
//	return serial;
//}

void ofxAzureKinect::setSynchronizedImages(bool bUseSynchronizedImages)
{
	this->bUseSynchronizedImages = bUseSynchronizedImages;
}

float ofxAzureKinect::getHeight() const
{
	return height;
}

float ofxAzureKinect::getWidth() const
{
	return width;
}

void ofxAzureKinect::update()
{
	switch (k4a_device_get_capture(device, &capture, TIMEOUT_IN_MS))
	{
	case K4A_WAIT_RESULT_SUCCEEDED:
		break;
	case K4A_WAIT_RESULT_TIMEOUT:
		ofLog(OF_LOG_WARNING, "Timed out waiting for a capture");
		break;
	case K4A_WAIT_RESULT_FAILED:
		ofLog(OF_LOG_WARNING, "Failed to read a capture");
		break;
	}


	// The capture must always be released when it is done being used
	k4a_capture_release(capture);
}
