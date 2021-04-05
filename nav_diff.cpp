#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <map_msgs/OccupancyGridUpdate.h>

using namespace::std;

void nav_callback(const nav_msgs::OccupancyGridUpdateConstPtr& msg)
{//callback関数

    bool nav_flag=true;
    int i, n = 0;
    static int cnt_old, cnt_new = 0;

    cout << msg->data.size() << "\n";

    if (nav_flag){
        for (i=0;i<msg->data.size();++i){

            if (msg->data[i]!== NULL){
                cnt_old++;
                //nav_old[i] = 0;
            }   
        }
        nav_flag = false;
    }

    else if (!nav_flag){
        for (i=0;i<msg->data.size();++i){

            if (msg->data[i]!== NULL){
                //cout << "0,";
                //nav_new[i] = 0;
                cnt_new++;
            }
        }

        diff = cnt_new - cnt_old;
        cnt_old = cnt_new;
        cnt_new = 0;
        cout << "diff" << "\n";
    }
    
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "nav_diff");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/move_base/local_costmap/costmap_updates", 100, nav_callback);

    ros::Rate loop_rate(1);
    while (ros::ok){
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}
