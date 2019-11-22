#!/usr/bin/env python2
# license removed for brevity
import rospy
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose

tpos = Pose()


def callback(data):
    global tpos
    tpos = data

def move():
    global tpos
    pub = rospy.Publisher('turtle1/cmd_vel', Twist, queue_size=10)
    rospy.init_node('turtle_controller', anonymous=True)
    rospy.Subscriber("turtle1/pose", Pose, callback) 
    rate = rospy.Rate(10) # 10hz
    vel_msg = Twist()
    speed = 1
    distance = 2

    vel_msg.linear.x = 0
    vel_msg.linear.y = 0
    vel_msg.linear.z = 0
    vel_msg.angular.x = 0
    vel_msg.angular.y = 0
    vel_msg.angular.z = 0
    angle = 0

    print('Helllppp')

    while not rospy.is_shutdown():
        #Setting the current time for distance calculus
        t0 = rospy.Time.now().to_sec()
        current_distance = 0
        j = 0
        while j < 4:
          #Loop to move the turtle in an specified distance
          while(current_distance < distance):
              if j == 0:
                vel_msg.linear.x = speed
                vel_msg.linear.y = 0
                angle = 1.57
              if j == 1:
                vel_msg.linear.x = 0
                vel_msg.linear.y = speed
                angle = 3.14
              if j == 2:
                vel_msg.linear.x = -speed
                vel_msg.linear.y = 0
                angle = 0
              if j == 3:
                vel_msg.linear.x = 0
                vel_msg.linear.y = -speed
                angle = 0
              #Publish the velocity
              pub.publish(vel_msg)
              #Takes actual time to velocity calculus
              t1=rospy.Time.now().to_sec()
              #Calculates distancePoseStamped
              current_distance= speed*(t1-t0)
          current_distance = 0
          j += 1
          vel_msg.linear.x = 0
          vel_msg.linear.y = 0
          vel_msg.angular.z = 1         
          while tpos.theta < angle:
            print(tpos.theta, angle)
            pub.publish(vel_msg)
          vel_msg.angular.z = 0
        #After the loop, stops the robot
        vel_msg.linear.x = 0
        vel_msg.linear.y = 0
        #Force the robot to stop
        pub.publish(vel_msg)

if __name__ == '__main__':
  try:
    move()
  except rospy.ROSInterruptException:
    pass