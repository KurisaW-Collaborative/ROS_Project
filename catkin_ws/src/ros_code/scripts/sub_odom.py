#!/usr/bin/env python
import rospy
from turtlesim.msg import Pose

def callback(data):
    rospy.loginfo(data.pose.pose.orientation.w)
       
def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("/turtle1/pose", Pose, callback)

    rospy.spin()

if __name__ == '__main__':
    listener()

