//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_LOGCONTENT_HPP
#define CADSS_PROTOTYPR_LOGCONTENT_HPP

#include <string>
#include <vector>
#include <map>
#include <list>
#include <limits>

enum Mode {
    following = 0,
    merging,
    yielding,
    stopping,
    vanishing,
    synchronizing,
    allWayStopping,
    linechange,
    end,
    init,           //init
    stop2follow,    //there is a stopline within 10m, but a car is nearer than 10m.
};

// TODO:
class AgentInfo {
public:

    std::string agentType;
    double Length_;
    double width_;
    double mass_;

};

class TraPt{

public:
    double t;
    double x;
    double y;
    double yaw;
    double yaw_rate;
    double vx;
    // TODO: Lack of Model
    double vy = 0;
    double a;
    // TODO: Too MaFan
    double jerk = 0;
    int current_lanelet_ID;
    double s_of_current_lanelet;
    double d_of_current_lanelet;
};

class Obstacle_info{

public:
    //int Agent_ID;
    double distence;
    bool yielding=true;
    TraPt point_in;
};

class Obstacle {
public:
    // TODO: int:Agent_ID;
    std::map<int, Obstacle_info> obstacle;
};

class PlanTra {
public:
    std::map<int, TraPt> pts;
};

class OnePreTra {
public:
    double Probability;
    std::map<int, TraPt> pts;
    // TODO: MEANING?
    std::map<int, int> confilictlanesID;
};

class PredictTra {
public:
    std::vector<std::shared_ptr<OnePreTra>> Trajs;
};

class LogContent {

public:
    /**
     * @brief Attributes for Content.
     */
    AgentInfo agentInfo;
    PlanTra PlannerResult;
    Obstacle obstacle_info_;
    Mode current_mode_;
    PredictTra ex_prediction;
    PredictTra in_prediction;
    std::vector<double> current_state_ = std::vector<double>(6, std::numeric_limits<double>::max()); // Prior Update
    std::vector<double> next_state_ = std::vector<double>(6, std::numeric_limits<double>::max());
    std::vector<double> current_control = std::vector<double>(3, std::numeric_limits<double>::max());
};
#endif //CADSS_PROTOTYPR_LOGCONTENT_HPP
