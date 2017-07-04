#include <ros/ros.h> 
#include <std_msgs/Float64.h> 
#include <geometry_msgs/PoseArray.h>


#include "baxter_core_msgs/EndEffectorCommand.h"
#include <sstream>

//https://groups.google.com/a/rethinkrobotics.com/forum/#!topic/brr-users/ZlfK-fqRfuw


//ros::Publisher gripper_publisher;
double map_dist;
bool get_polaris = false;

//variable for mapping the distance to the position of the gripper


void polarisCB(const geometry_msgs::PoseArray& targets) 
{ 
	//TODO
	double distance;
	double conv_dist;

	//calculate the distance between the two fingers
	distance=sqrt(pow(targets.poses[0].position.x-targets.poses[1].position.x,2)+pow(targets.poses[0].position.y-targets.poses[1].position.y,2)+pow(targets.poses[0].position.z-targets.poses[1].position.z,2));
	
	//convererte the distance to a useful number related to baxter gripper
	conv_dist=((distance-0.03) *909);
	
	// set the rnnage of position to be from 0 to 100 
	//if no information is given, hold the last position
	if(conv_dist>100){
		map_dist=100;
	}
	else if(conv_dist<0){
		map_dist=0;
	}
	else if(conv_dist>0 && conv_dist<100){
		map_dist=conv_dist;
	}
	//for convenience print left & right arm coordinates, and the mapped distancs.
	ROS_INFO("Right x= %f y= %f z= %f",targets.poses[0].position.x,targets.poses[0].position.y,targets.poses[0].position.z);
	ROS_INFO("Left x= %f y= %f z= %f",targets.poses[1].position.x,targets.poses[1].position.y,targets.poses[1].position.z);
	ROS_INFO("mapped_dist= %f",map_dist);
	
	get_polaris = true;
}

int main(int argc, char **argv){
	ros::init(argc,argv,"gripper_control");
	ros::NodeHandle n;

	ros::Subscriber polaris_subscriber= n.subscribe("/polaris_sensor/targets",1,polarisCB); 

	while(get_polaris == false){
	ROS_WARN("waiting on polaris callback");
	ros::spinOnce();
	ros::Duration(1.0).sleep();

	}

	while(ros::ok()) {

		ros::Publisher gripper_pub = n.advertise<baxter_core_msgs::EndEffectorCommand>("/robot/end_effector/right_gripper/command", 1);
		//gripper_publisher=gripper_pub;
		//map_dist
		baxter_core_msgs::EndEffectorCommand controlled_grip;
		std::ostringstream args;
		

	
		args << "{\"position\":" << map_dist << ", \"dead zone\": 5.0, \"force\": 40.0, \"holding force\": 30.0, \"velocity\": 50.0}";

		controlled_grip.command= baxter_core_msgs::EndEffectorCommand::CMD_GO;	
		controlled_grip.id= 65538; //65664  
		controlled_grip.args=args.str();//{"position":map_dist};
		controlled_grip.sequence = 1;//3
		controlled_grip.sender = "/gripper_control";
		
		ros::Rate naptime(1000);
		ros::Time start =  ros::Time::now(); 
		ros::Duration loop_time(0.1);

		ROS_INFO("publishing and latching message for %d.%d seconds", loop_time.sec,loop_time.nsec);
		ros::Duration dif_time =ros::Time::now() - start;

		while(dif_time < loop_time) {

			//ROS_INFO("publishing position= %f",map_dist);
			gripper_pub.publish(controlled_grip);
			naptime.sleep();
			dif_time =ros::Time::now() - start;
			ros::spinOnce(); 

		}

		//ros::spinOnce(); 
	}

	return 0;
}
	 
	
