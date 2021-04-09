#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <map_msgs/OccupancyGridUpdate.h>
#include <actionlib/server/simple_action_server.h>
#include <costmap_dynamic_object_detecter/CostDiffAction.h>

using namespace::std;

class CostDiff
{
protected:
    //ノードハンドル
    ros::NodeHandle nh_obs_;
    ros::Subscriber sub_;
    //アクションサーバの宣言
    actionlib::SimpleActionServer<costmap_dynamic_object_detecter::CostDiffAction> as_;
    
    std::string action_name_;
    
    costmap_dynamic_object_detecter::CostDiffFeedback feedback_;
    costmap_dynamic_object_detecter::CostDiffResult result_;

    bool first_flag_=true;
    int cnt_old_, cnt_new_ = 0;
    uint diff_ = 0, diff_old_ = 0, diff_new_ = 0;

public: 
    CostDiff(std::string name) :
                              as_(nh_obs_, name, boost::bind(&CostDiff::executeCB, this, _1),false),
                              action_name_(name),
                              first_flag_(true),
                              cnt_new_(0),
                              diff_(0), diff_old_(0), diff_new_(0)
    {
        sub_ = nh_obs_.subscribe("/move_base/local_costmap/costmap_updates", 1, &CostDiff::CostMapDiffCb, this);
        as_.start();
    }

    ~CostDiff(void) {}

    void CostMapDiffCb(const map_msgs::OccupancyGridUpdate& msg)
    {
        if (first_flag_){
            for (int i=0;i<msg.data.size();++i){
                if (msg.data[i] == '\0')
                    cnt_old_++;
            }
            first_flag_ = false;
        }
        else if (!first_flag_){
            for (int i=0;i<msg.data.size();++i){
                if (msg.data[i] == '\0')
                    cnt_new_++;
            }
            diff_new_ = abs(cnt_new_ - cnt_old_);
            diff_ = diff_new_ + diff_old_;
            diff_old_ = diff_new_;
            cnt_old_ = cnt_new_;
            cnt_new_ = 0;
            cout << diff_ << "\n";
        }
    }

    void executeCB(const costmap_dynamic_object_detecter::CostDiffGoalConstPtr &goal)
    {
        bool success = false;
        result_.observation_results = false;
        ros::Rate loop_rate(0.3);

        first_flag_ = true;
        cnt_new_ = 0;
        diff_ = 0, diff_old_ = 0, diff_new_ = 0;

        while (ros::ok){
            if (diff_ >= 20000){
                success = true;
                break;
            }
            cout << diff_ << "\n";
            ros::spinOnce();
            loop_rate.sleep();
        }

        if (success){
            result_.observation_results = true;
            ROS_INFO("%s: Succeeded", action_name_.c_str());
            as_.setSucceeded(result_);
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "costmap_dynamic_object_detecter");
    // CostDiffをcost_diffのアクション名で初期化
    CostDiff CostDiff("cost_diff");
    
    return 0;
}