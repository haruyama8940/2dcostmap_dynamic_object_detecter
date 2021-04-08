#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <map_msgs/OccupancyGridUpdate.h>
#include <actionlib/server/simple_action_server.h>
#include <2dcostmap_dynamic_object_detecter/obs_diff.h>

using namespace::std;
class obs_diff
{
    protected:
    //ノードハンドル
    ros::NodeHandle nh_obs;
    //アクションサーバの宣言
    acctionlib::SimpleActionServer<ros_tutorials_action::obs_diff> as_;
    //
    std::string action_name;
    //
    2dcostmap_dynamic_object_detecter::obs_diff feedback;
    2dcostmap_dynamic_object_detecter::obs_diff result;

    public:

    nav_diff(std::string name) :
        as_(nav_obs, name, boost::bind($obs_diff::executeCB, this, _1),
            false),
    action_name_(name)
    {
    as_.start();
    }
    ~obs_diff(void)
    {
    }
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
    void executeCB
    {
    ros::Rate r(1);
    bool success = true;
    ros::Subscriber sub = nh_obs.subscribe("/move_base/local_costmap/costmap_updates", 1, CostMapDiffCb);

        if(success)
        {
            result_.sequence=feedback_.sequence;
            ROS_INFO("%s: Succeeded", action_name_.c_str());
            as_.setSucceeded(result_);
        }
    }
};



int main(int argc, char **argv)
{
    ros::init(argc, argv, "costmap_dynamic_object_detecter");
    //obs_diffをcost_diffのアクション名で初期化
    2dcostmap_dynamic_object_detecter obs_diff("cost_diff");
    ros::spin();
    /*
    //ros::NodeHandle nh;
    //ros::Subscriber sub = nh.subscribe("/move_base/local_costmap/costmap_updates", 1, CostMapDiffCb);

    ros::Rate loop_rate(0.3);
    while (ros::ok){
      ros::spinOnce();
        loop_rate.sleep();
    
    }
    */    
    return 0;
    
}