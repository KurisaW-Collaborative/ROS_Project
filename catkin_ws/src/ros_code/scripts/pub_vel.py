#!/usr/bin/env python
# license removed for brevity
import rospy
from geometry_msgs.msg import Twist

def talker():
    pub = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10) # 10hz
    
    while not rospy.is_shutdown(): 
        vel_cmd = Twist()
        vel_cmd.linear.x = 1
        vel_cmd.angular.z = 0.5
	
           
        pub.publish(vel_cmd)
        rate.sleep()
      
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass

