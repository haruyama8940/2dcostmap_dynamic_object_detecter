#include <ros/ros.h>
#include <std_msgs/String.h>

#include <vector>
#include <numeric>

using namespace::std;

void nav_callback(const nav_msgs::OccupancyGrid& msg)
{//callback関数
    
    int sum_new, sum_old;//新旧合計
    //float avg_new, avg_old;
    vector<int> nav_new{msg.data};
    vector<int> nav_old;

    for (int n = 0; n < nav_new.size(); n++){//ひとつの差
        auto diff+ = nav_new[n]-nav_old[n];

    }
    //if(diff==){
        
    //}
    nav_old = nav_new;//内容の代入vectorならできるらしい
    //std::cout<<"diff"<<"\n";

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
}
