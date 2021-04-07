#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <map_msgs/OccupancyGridUpdate.h>

using namespace::std;

void CostMapDiffCb(const map_msgs::OccupancyGridUpdateConstPtr& msg)
{
    static bool first_flag=true;
    static int cnt_old, cnt_new = 0;
    static uint diff = 0, diff_old = 0, diff_new = 0;

    if (first_flag){
        for (int i=0;i<msg->data.size();++i){
            if (msg->data[i] == '\0')
                cnt_old++;
        }
        first_flag = false;
    }
    else if (!first_flag){
        for (int i=0;i<msg->data.size();++i){
            if (msg->data[i] == '\0')
                cnt_new++;
        }
        diff_new = abs(cnt_new - cnt_old);
        diff = diff_new + diff_old;
        diff_old = diff_new;
        cnt_old = cnt_new;
        cnt_new = 0;
        cout << diff << "\n";
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "costmap_dynamic_object_detecter");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/move_base/local_costmap/costmap_updates", 1, CostMapDiffCb);

    ros::Rate loop_rate(0.3);
    while (ros::ok){
        ros::spinOnce();
        loop_rate.sleep();
    }    
    return 0;
}
