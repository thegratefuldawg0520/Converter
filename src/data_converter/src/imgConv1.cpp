#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <ros/console.h>

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Publisher image_pub_;
  
public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_pub_ = it_.advertise("image_stream", 1);
    
    cv::namedWindow(OPENCV_WINDOW);
    
    convertImages();
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void convertImages()
  {
    cv::Mat image;
    std::string imageName;
    std::stringstream imageStream;
    cv_bridge::CvImagePtr cv_ptr(new cv_bridge::CvImage());
    
    for (int i = 2141; i < 2210; i++)
    {
    	imageStream << '~/doopy/Documents/ros_tango/src/data_converter/src/DJI0' << i << '.JPG';
    	imageName = imageStream.str();
    	try
    	{
    		image = cv::imread(imageName.c_str(), cv::IMREAD_COLOR);
    		cv::waitKey(3);
    		// Update GUI Window
    		ROS_DEBUG_STREAM("Hello " << "World");
    		//image_pub_.publish(cv_ptr->toImageMsg());
    	}
    	
    	catch (cv::Exception& e)
    	{
    		ROS_ERROR("cv error: %s", imageName.c_str());
    		return;
    	}
    }
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
