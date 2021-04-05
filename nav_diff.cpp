#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <map_msgs/OccupancyGridUpdate.h>

#include <vector>
#include <numeric>

using namespace::std;

void nav_callback(const nav_msgs::OccupancyGridUpdateConstPtr& msg)
{//callback関数
    bool nav_flag=true;
    int i,n = 0;
    vector<int> nav_new;
    vector<int> nav_old;
    cout << msg->data.size() << "\n";

    if (nav_flag){
        for (i=0;i<msg->data.size();++i){

            if (msg->data[i] == NULL){
                cout << "0,";
                nav_old[i] = 0;
            }   
            if (msg->data[i] == 'd'){
                cout << "100,";
                nav_old[i] = 100;
            }

            if (msg->data[i] == 'c'){
                cout << "99,";
                nav_old[i] = 99;
            }
        }
        nav_flag = false;
    }

    else if (!nav_flag){
        for (i=0;i<msg->data.size();++i){

            if (msg->data[i] == NULL){
                //cout << "0,";
                nav_new[i] = 0;
            }
             if (msg->data[i] == 'c'){
                //cout << "99,";
                nav_new[i] = 99;
            }
            if (msg->data[i] == 'd'){
                //cout << "100,";
                nav_new[i] = 100;
            }
        }
        for (int n = 0; n <= nav_new.size(); ++n){//ひとつの差
            diff += abs(nav_new[n]-nav_old[n]);
        }
        nav_old = nav_new;//内容の代入vectorならできるらしい
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
