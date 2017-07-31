cmake_minimum_required(VERSION 2.8.3)
project(polaris_transformation)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  roscpp
  std_msgs
  baxter_core_msgs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES polaris_transformation
#  CATKIN_DEPENDS geometry_msgs nav_msgs roscpp std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########


include_directories(
  ${catkin_INCLUDE_DIRS}
)


## Declare a C++ executable
# add_executable(polaris_transformation_node src/polaris_transformation_node.cpp)
add_executable(polaris_transformation src/polaris_transformation.cpp)
add_executable(ForceSensorNode src/ForceSensorNode.cpp)
#add_executable(gripper_control src/gripper_control.cpp)
#add_executable(gripper_test src/gripper_test.cpp)


## Specify libraries to link a library or executable target against
target_link_libraries(polaris_transformation ${catkin_LIBRARIES})
target_link_libraries(ForceSensorNode ${catkin_LIBRARIES})

