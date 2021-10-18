//
// Created by 萧博成 on 2021/10/17.
//
#include "CalPlanner.hpp"

CalPlanner::CalPlanner(AstarPlanner *astarPlanner, std::shared_ptr<AgentLog> agent_ibt, int startts) {

    start_timestamp_ = startts;
    agent_ibt_ = agent_ibt;
    astarPlanner_ = astarPlanner;
    std::cout <<"CalPlanner____________________________________________"<<std::endl;

}

void CalPlanner::Run(SimuClock* simuClock, int CurrentClock, SimuClock::SimulationType simulationType) {
    InnerClock = CurrentClock;
    if (checkReady(agent_ibt_) == 0) {
        std::cout << "debug: Planner checkReady success" << std::endl;
        if (simulationType == SimuClock::RealTime) {
            astarPlanner_->update(CurrentClock);
        } else if (simulationType == SimuClock::STEP) {
            astarPlanner_->update(simuClock->getClock());
            simuClock->STEP_CLOCK_INDICATOR += 1;
            std::cout << "debug: STEP_CLOCK_INDICATOR: " << simuClock->STEP_CLOCK_INDICATOR << std::endl;
        } else {
            throw std::runtime_error("ILLEGAL SIMULATION TYPE");
        }
    }
}

int CalPlanner::checkReady(std::shared_ptr<AgentLog> agent_ibt) {

    if (agent_ibt->GetLog(InnerClock)->PlannerResult.pts.empty()) {
        // Ready to push
        return 0;
    } else {
        // refuse to push
        return 1;
    }
}