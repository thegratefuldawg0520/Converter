#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/PointCloud2.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <ros/console.h>
#include <pcl/console/print.h>
#include <pcl/console/parse.h>
#include <pcl/io/vtk_lib_io.h>
#include <pcl/io/obj_io.h>
#include <pcl/conversions.h>
#include <pcl_conversions/pcl_conversions.h>

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Publisher image_pub_;
  sensor_msgs::PointCloud2 output;
  ros::Publisher pc_pub;
  
public:
  ImageConverter()
    : it_(nh_)
  {
    image_pub_ = it_.advertise("image_stream", 1);
    pc_pub = nh_.advertise<sensor_msgs::PointCloud2>("depth",1);
    
    convertImages();
  }

  ~ImageConverter()
  {
  	  int x = 0;
  }

  void convertImages()
  {
    cv::Mat image;
    cv_bridge::CvImage cv_ptr;
    pcl::PolygonMesh depthMesh;
    pcl::PCLPointCloud2 depthCloud;
    
    ROS_DEBUG_STREAM(depthCloud.header.frame_id);
    for (int i = 13; i < 55; i++)
    {
    	std::stringstream imageStream;
    	imageStream << "/home/doopy/Documents/ros_tango/src/data_converter/src/data/pc_15073808_0" << i << ".vtk.jpg";
    	std::string imageName = imageStream.str();
    	
    	std::stringstream depthStream;
    	depthStream << "/home/doopy/Documents/ros_tango/src/data_converter/src/data/pc_15073808_0" << i << ".vtk";
    	std::string depthName = depthStream.str();
    	
    	ROS_DEBUG_STREAM(imageName);
    	ROS_DEBUG_STREAM(depthName);
    	
    	//depthCloud.header.stamp = ros::Time::now();
    	try
    	{
    		cv_ptr.image = cv::imread(imageName, CV_LOAD_IMAGE_COLOR);
    		cv_ptr.encoding = "bgr8";
    		cv::waitKey(300);
    		image_pub_.publish(cv_ptr.toImageMsg());
    		
    		pcl::io::loadPolygonFileVTK(depthName, depthMesh);
    		//pcl_conversions::toPCL(depthMesh, depthCloud);
    		depthCloud = depthMesh.cloud;
    		depthCloud.header.frame_id = "my_frame";
    		pc_pub.publish(depthCloud);
    	}
    	
    	catch (cv::Exception& e)
    	{
    		ROS_ERROR("cv error: %s", imageName.c_str());
    		ROS_DEBUG_STREAM("FAIL");
    		return;
    	}
    }
  }
};

int main(int argc, char** argv)
{
	if (ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Debug)) { // Change the level to fit your needs
	ros::console::notifyLoggerLevelsChanged();
	}
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
