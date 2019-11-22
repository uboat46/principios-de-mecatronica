#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>

ros::NodeHandle nh;

void messageCb1( const std_msgs::Empty& toggle_msg1){
  digitalWrite(13,1);
  delay(500);
  digitalWrite(13,0);
  delay(500);
}

void messageCb2( const std_msgs::Empty& toggle_msg2){
  digitalWrite(13,1);
  delay(250);
  digitalWrite(13,0);
  delay(250);  
}

ros::Subscriber<std_msgs::Empty> enc("freq1", &messageCb1 );
ros::Subscriber<std_msgs::Empty> apa("freq2", &messageCb2 );

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);

  nh.initNode();
  nh.subscribe(freq1);
  nh.subscribe(freq2);
}

void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  delay(1);
}
