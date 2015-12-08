import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import cv2
import sys

class imageConverter:
	
	def __init__(self):
		
		self.imagePub = rospy.Publisher("image", Image)
		self.bridge = CvBridge()
		
		self.convertImages()
		
	def convertImages(self):
		for i in range(2141,2210):
		
			image = cv2.imread('DJI0' + str(i) + '.JPG')
		
			try:
				self.imagePub.publish(self.bridge.cv2_to_imgmsg(image, 'bgr8'))
			except CvBridgeError, e:
				print e
			
			print 'bamn\n'
		
def main(args):
	
	rospy.init_node('imageConverter', anonymous=True)
	imgConv = imageConverter()
	
	try:
		rospy.spin()
	except KeyboardInterrupt:
		print "Shutting down"
	cv2.destroyAllWindows()
	
if __name__=='__main__':
	main(sys.argv)


	

