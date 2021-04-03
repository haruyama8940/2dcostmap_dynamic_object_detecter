#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>

#include <vector>
#include <numeric>

using namespace::std;

void nav_callback(const nav_msgs::OccupancyGrid& msg)
{//callback関数
    bool nav_flag=true;
    
    //float avg_new, avg_old;
    vector<int8_t> nav_new;
    vector<int8_t> nav_old;
    
    if (nav_flag){
        nav_new = msg.data;
        nav_old = msg.data;
        nav_flag = false;
    }
    else if (!nav_flag){
        nav_new = msg.data;
        for (int n = 0; n <= nav_new.size(); ++n){//ひとつの差
            diff += abs(nav_new[n]-nav_old[n]);
        }
        nav_old = nav_new;//内容の代入vectorならできるらしい
        cout << "diff" << "\n";
    }
    /*平均
    avg_new = std::accumulate(nav_new.begin(),nav_new.end(), 0); 
    auto diff= avg_new - avg_old
    std::cout << "sum_new = "<<std::accumulate(nav_new.begin(),nav_new.end() 0) << "\n";
    std::cout << "avg= "<<std::accumulate(nav_new.begin(),nav_new.end() 0) << "\n";
    */
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "nav_diff");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.Subscribe("/move_base/local_costmap/costmap", 1, nav_callback);
    
    bool TwoLoopCnt = 0;

    ros::Rate loop_rate(1);
    while (ros::ok){
        if (TwoLoopCnt == 2)
            break;
        TwoLoopCnt++;
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}
