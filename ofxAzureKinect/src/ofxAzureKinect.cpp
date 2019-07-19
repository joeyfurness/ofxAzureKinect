#include "ofxAzureKinect.h"



ofxAzureKinect::ofxAzureKinect()
{
	device = NULL;
	config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;

	serial_number_length = 0;
	serial_number = NULL;
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
	k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	config.camera_fps = camera_fps;
	config.color_format = color_format;
	config.color_resolution = color_resolution;
	config.depth_mode = depth_mode;

	ofLog(OF_LOG_NOTICE, "k4a device succesfully configured");

	return true;
}

bool ofxAzureKinect::open(int deviceIndex)
{
	// Open the first plugged in Kinect device
	if (K4A_FAILED(k4a_device_open(K4A_DEVICE_DEFAULT, &device)))
	{
		ofLog(OF_LOG_ERROR, "Failed to open k4a device!\n");
		return false;
	}

	// Start the camera with the given configuration
	if (K4A_FAILED(k4a_device_start_cameras(device, &config)))
	{
		ofLog(OF_LOG_ERROR, "Failed to start cameras!\n");
		k4a_device_close(device);
		return false;
	}

	ofLog(OF_LOG_NOTICE, "k4a device succesfully opened");
	return false;
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
