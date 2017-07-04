#include <ros/ros.h> 
#include <std_msgs/Float64.h> 
#include <geometry_msgs/PoseArray.h>


#include "baxter_core_msgs/EndEffectorCommand.h"
#include <sstream>
#include <iostream>
#include <iostream>
#include <string>
using namespace std;
//int count=1;
//variable for mapping the distance to the position of the gripper
int main(int argc, char **argv){
	ros::init(argc,argv,"gripper_test");
	ros::NodeHandle n;

	while(ros::ok()) {

		ros::Publisher gripper_pub = n.advertise<baxter_core_msgs::EndEffectorCommand>("/robot/end_effector/right_gripper/command", 1);
		//gripper_publisher=gripper_pub;
		//map_dist
		baxter_core_msgs::EndEffectorCommand controlled_grip;
		std::ostringstream args;

		std::string in_value;
		double map_dist;

	
	    cout<<endl;
	    cout << "enter a number between 0 and 100 (x to quit): ";
	    cin>>in_value;

	    map_dist= atoi(in_value.c_str());

	    if(in_value=="x"){
	    	ROS_INFO("Breaking out of the loop");
	    	break;
	    }
	    if(!(map_dist>=0 && map_dist<=100)){
	    	ROS_INFO("Wrong input, try again");
	    	continue;
	    }
		

		args << "{\"dead zone\": 5.0, \"force\": 40.0, \"holding force\": 30.0, \"position\":" << map_dist << ", \"velocity\": 50.0}";

		controlled_grip.command= baxter_core_msgs::EndEffectorCommand::CMD_GO;	
		controlled_grip.id= 65538; //65664  echo the topic 'robot/end_effector/' + name + '/state'
		controlled_grip.args=args.str();//{"position":map_dist};
		controlled_grip.sequence = 1;//3
		controlled_grip.sender = "/gripper_test";
		
		ros::Rate naptime(100);
		ros::Time start =  ros::Time::now(); 
		ros::Duration loop_time(0.5);

		ROS_INFO("sending command to gripper for: %d.%d seconds", loop_time.sec,loop_time.nsec);
		//ROS_INFO("start time : %d,%d", start.sec,start.nsec);

		ros::Duration dif_time =ros::Time::now() - start;
		//while((ros::Time::now() - start) < loop_time) {
		//while(ros::ok()) { 
		while(dif_time < loop_time) {

			gripper_pub.publish(controlled_grip);

			//ROS_INFO("publishing position %d",count++);
			naptime.sleep();

			dif_time =ros::Time::now() - start;
			//ROS_INFO("time now %d,%d", dif_time.sec,dif_time.nsec);
			//ros::spinOnce();
			//if((ros::Time::now() - start) > loop_time){break;}
		}
	}

	return 0;
}
	 
	
