#! /usr/bin/env python

import rospy
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose
import math
import time
from std_srvs.srv import Empty

x = 0
y = 0
theta = 0

def poseCallback(pose_message):
    global x
    global y
    global theta
    
    x = pose_message.x
    y = pose_message.y
    theta = pose_message.theta

def move(speed, distance, is_forward):
    velocity_message = Twist()
    global x
    global y
    x0 = x0
    y0 = y0

    if (is_forward):
        velocity_message.linear.x = abs(speed)
    else:
        velocity_message.linear.x = -abs(speed)

    distance_moved = 0.0
    loop_rate = rospy.Rate(10)
    cmd_vel_topic = '/turtle1/cmd_vel'
    velocity_publisher = rospy.Publisher(cmd_vel_topic, Twist, queue_size = 10)

    while True : 
        rospy.loginfo("Turtlesim moves forwards")
        velocity_publisher.publish(velocity_message)

        loop_rate.sleep()

        distance_moved = distance_moved + abs(0.5*math.sqrt(((x-x0)**2)+((y-y0)**2)))
        print distance_moved
        if not(distance_moved < distance)
            rospy.loginfo("REACHED")
            break

def rotate (angular_speed_degree, relative_angle_degree, clockwise):
    global theta
    velocity_message = Twist()
    velocity_message.linear.x = 0
    velocity_message.linear.y = 0
    velocity_message.linear.z = 0
    velocity_message.angular.x = 0
    velocity_message.angular.y = 0
    velocity_message.angular.z = 0

    theta0 = theta
    angular_speed = math.radians(abs(angular_speed_degree))

    if (clockwise):
        velocity_message.angular.z = -abs(angular_speed)
    else:
        velocity_message.angular.z = abs(angular_speed)

    angle_moved = 0.0
    loop_rate = rospy.Rate(10)
    cmd_vel_topic = '/turtle/cmd_vel'
    velocity_publisher = rospy.Publisher(cmd_vel_topic, Twist, queue_size = 10)

    t0 = rospy.Time.now().to_sec()

    while True :
        rospy.loginfo("Turtlesim rotates")
        velocity_publisher.publish(velocity_message)

        t1 = rospy.Time.now().to_sec()
        current_angle_degree = (t1-t0)*angular_speed_degree
        loop_rate.sleep()

        if (current_angle_degree > relative_angle_degree):
            rospy.loginfo("REACHED")
            break

velocity_message.angular.z = 0
velocity_publisher.publish(velocity_message)

def go_to_goal (x_goal, ygoal):
    global x
    global y
    global theta

    velocity_message = Twist()
    cmd_vel_topic = '/turtle1/cmd_vel'

    while(True):
        kv = 0.5
        distance = abs(math.sqrt(((xgoal-x)**2)+((ygoal-y)**2)))
        linear_speed = kv * distance

        ka = 4.0
        desired_angle_goal = math.atan2(ygoal-y, xgoal-x)
        angular_speed = ka * (desired_angle_goal-theta)

        velocity_message.linear.x = linear_speed
        velocity_message.angular.z = angular_speed
        velocity_publisher.publish(velocity_message)
        print 'x=', x, 'y=', y0

        if distance < 0.01):
            break

def setDesiredOrientation (desired_angle_radians):
    relative_angle_radians = desired_angle_radians - theta
    if relative_angle_radians < 0:
        clockwise = 1
    else:
        clockwise = 0
    print relative_angle_radians
    print desired_angle_radians
    rotate (30, math.degrees(abs(relative_angle_radians)), clockwise)

def gridClean():
    desired_pose = POse()
    desired_pose.x = 1
    desired_pose.y = 1
    desired_pose.tetha = 0

    moveGoal(desired_pose, 0.01)
    setDesiredOrientation(degrees2radians(desired_pose.theta))

    move(2.0,9.0,True)
    rotate(degrees2radians(20), degrees2radians(90), False)
    move(2.0,9.0,True)
    rotate(degrees2radians(20), degrees2radians(90), False)
    move(2.0,1.0,True)
    rotate(degrees2radians(20), degrees2radians(90), False)
    move(2.0,9.0,True)
    rotate(degrees2radians(30), degrees2radians(90), True)
    move(2.0,1.0,True)
    rotate(degrees2radians(30), degrees2radians(90), True)
    move(2.0,9.0,True)
    pass

def spiralClean():
    vel_msg = Twist()
    loop_rate = rospy.Rate(1)
    wk = 4
    rk = 0

    while ((currentTurtlesimPose.x <10.5) and currentTurtlesimPose.y < 10.5)
        rk = rk+1
        vel_msg.linear.x = rk
        vel_msg.linear.y = 0
        vel_msg.linear.z = 0
        vel_msg.angular.x = 0
        vel_msg.angular.y = 0
        vel_msg.angular.z = wk
        velocity_publisher.publish(vel_msg)
        loop_rate.sleep()

if __name__ == '__main__':
    try:

        rospy.init_node('turtlesim_motion_pose', anonymous = True)

        cmd_vel_topic = '/turtle1/cmd_vel'
        velocity_publisher = rospy.Publisher(cmd_vel_topic, Twist, queue_size = 10)

        position_topic = "/turtle1/pose"
        pose_subscriber = rospy.Subscriber(position_topic, Pose, poseCallback)
        time.sleep(2)

        setDesiredOrientation(math.radians(90))

    except rospy.ROSInterruptionException:
        rospy.loginfo("node terminated.")
