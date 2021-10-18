//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_CALPLANNER_HPP
#define CADSS_PROTOTYPR_CALPLANNER_HPP
#include "../Planners/AstarPlanner.hpp"
#include "CalTask.hpp"
#include "AgentLog.hpp"
#include "LogManager.hpp"


class CalPlanner : public CalTask {

public:

    explicit CalPlanner(AstarPlanner *astarPlanner, std::shared_ptr<AgentLog> agent_ibt, int startts);
    void Run(SimuClock* simuClock, int CurrentClock, SimuClock::SimulationType simulationType) override;
    int checkReady(std::shared_ptr<AgentLog> agent_ibt) override;
private:

    int start_timestamp_;
    int InnerClock;
    std::shared_ptr<AgentLog> agent_ibt_;
    AstarPlanner* astarPlanner_;
};


#endif //CADSS_PROTOTYPR_CALPLANNER_HPP
