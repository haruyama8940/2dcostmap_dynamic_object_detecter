#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <map_msgs/OccupancyGridUpdate.h>
#include <actionlib/server/simple_action_server.h>
#include <costmap_dynamic_object_detecter/CostDiffAction.h>

using namespace::std;

class CostDiff
{
protected:
    ros::NodeHandle nh_obs_;
    ros::Subscriber sub_;
    actionlib::SimpleActionServer<costmap_dynamic_object_detecter::CostDiffAction> as_;
    
    std::string action_name_;
    
    costmap_dynamic_object_detecter::CostDiffFeedback feedback_;
    costmap_dynamic_object_detecter::CostDiffResult result_;

    bool first_flag_;
    int cnt_old_, cnt_new_;
    uint diff_, diff_old_, diff_new_;

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

    ~CostDiff() {}

    void VariableInitialization()
    {
        cnt_old_ = 0, cnt_new_ = 0;
        diff_ = 0, diff_old_ = 0, diff_new_ = 0;
    }

    void CostMapDiffCb(const map_msgs::OccupancyGridUpdate& msg)
    {
        if (first_flag_){
            VariableInitialization();
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
        }
    }

    void executeCB(const costmap_dynamic_object_detecter::CostDiffGoalConstPtr &goal)
    {
        if (goal->order){
            bool success = false;
            result_.observation_results = false;

            first_flag_ = true;
            ROS_INFO("%sWaiting for variable initialization processing", action_name_.c_str());
            ros::Duration duration(4.0);
            duration.sleep();
            ROS_INFO("%sVariable initialization completed", action_name_.c_str());

            while (ros::ok){
                cout << diff_ << "\n";
                if (diff_ >= 20000){
                    success = true;
                    break;
                }
                ros::Duration duration(1.0);
                duration.sleep();
            }

            if (success){
                result_.observation_results = true;
                ROS_INFO("%s: Succeeded", action_name_.c_str());
                ros::Duration duration(1.0);
                duration.sleep();
                as_.setSucceeded(result_);
            }
        }  
        else if (!goal->order)
            ros::shutdown();
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "costmap_dynamic_object_detecter");

    CostDiff CostDiff("cost_diff");

    ros::Rate loop_rate(0.3);
    while (ros::ok){
        ros::spinOnce();
        loop_rate.sleep();
    }    
    return 0;
}
