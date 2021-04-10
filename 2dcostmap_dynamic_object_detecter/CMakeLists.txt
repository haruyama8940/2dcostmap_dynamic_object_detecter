cmake_minimum_required(VERSION 3.0.2)
project(costmap_dynamic_object_detecter)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  map_msgs
  nav_msgs
  roscpp
  actionlib_msgs
  actionlib
)

## System dependencies are found with CMake's conventions
## boost liblary
find_package(Boost REQUIRED COMPONENTS system)

## Generate actions in the 'action' folder
add_action_files(
  DIRECTORY 
    action
  FILES
    CostDiff.action
 )

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
    actionlib_msgs
    std_msgs
  )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  CATKIN_DEPENDS 
    map_msgs nav_msgs std_msgs 
    actionlib_msgs actionlib 
    roscpp
  DEPENDS Boost
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure

add_library(2dcostmap_dynamic_object_detecter 
  src/2dcostmap_dynamic_object_detecter.cpp
)
add_dependencies(2dcostmap_dynamic_object_detecter 
  ${TARGET_NAME} ${PROJECT_NAME}_generate_messages_cpp
)
target_link_libraries(2dcostmap_dynamic_object_detecter
  ${catkin_LIBRARIES}
 )

#############
## Install ##
#############

#############
## Testing ##
#############

