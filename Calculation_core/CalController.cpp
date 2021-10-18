//
// Created by 萧博成 on 2021/10/17.
//

#include "CalController.hpp"

CalController::CalController(PerfectController *perfectController, std::shared_ptr<AgentLog> agent_ibt, int startts) {

    start_timestamp_ = startts;
    perfectController_ = perfectController;
    agent_ibt_ = agent_ibt;
    std::cout <<"CalController____________________________________________"<<std::endl;
}

void CalController::Run(SimuClock* simuClock, int CurrentClock, SimuClock::SimulationType simulationType) {
    InnerClock = CurrentClock;
    if (checkReady(agent_ibt_) == 0) {
        std::cout << "debug: controller checkReady success" << std::endl;
        if (simulationType == SimuClock::RealTime) {
            perfectController_->update(CurrentClock);
        } else if (simulationType == SimuClock::STEP) {
            perfectController_->update(simuClock->getClock());
            simuClock->STEP_CLOCK_INDICATOR += 1;
            std::cout << "debug: STEP_CLOCK_INDICATOR: " << simuClock->STEP_CLOCK_INDICATOR << std::endl;
        } else {
            throw std::runtime_error("ILLEGAL SIMULATION TYPE");
        }
    }
}

int CalController::checkReady(std::shared_ptr<AgentLog> agent_ibt) {

    if (agent_ibt->GetLog(InnerClock)->current_control[0] == std::numeric_limits<double>::max() && !agent_ibt->GetLog(start_timestamp_)->PlannerResult.pts.empty()) {
        // Ready to push
        return 0;
    } else {
        // refuse to push
        return 1;
    }
}