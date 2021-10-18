//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_PREDICTOR_HPP
#define CADSS_PROTOTYPR_PREDICTOR_HPP


#include <memory>

class Predictor{
public:
    Predictor(std::shared_ptr<AgentLog> agent_ibt, LogManager* logManager, double time_step, double horizon); // ibt means `it belongs to`

    virtual void update(int CurrentTimeStamp) = 0; // the main function
    virtual void set_traj(PredictTra traj) = 0; // set the predicted results from the client

protected:
    std::shared_ptr<AgentLog> agent_ibt_; // agent_ibt means `the agent it belongs to`
    LogManager* LM;
    int time_step_;
    double horizon_;

    PredictorState state;
};

#endif //CADSS_PROTOTYPR_PREDICTOR_HPP
