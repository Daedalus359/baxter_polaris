
#include <ros/ros.h> 
#include <baxter_core_msgs/EndEffectorState.h>
#include <std_msgs/Float64.h>

double gripper_force = 0.0;

void CallBack(const baxter_core_msgs::EndEffectorState& new_state){
 	gripper_force = 0.7* gripper_force + 0.3 * new_state.force; //kevin's smoothness method
}

int main(int argc, char** argv){
	ros::init(argc, argv, "GripperForceSensor");
	ros::NodeHandle nh;
	ros::Rate naptime(100);

	ros::Subscriber gripper_state_subscriber = nh.subscribe("/robot/end_effector/right_gripper/state",1,CallBack);
	ros::Publisher force_sensor_publisher = nh.advertise<std_msgs::Float64>("GripperForceSensor", 1);

	// for(int i=0; i=100; i++){
	// gripper_force=(float)i;
	force_sensor_publisher.publish(gripper_force);
	ROS_INFO("force applied = %f", gripper_force);
	// }
	
	ros::spinOnce();
	naptime.sleep(); 
}