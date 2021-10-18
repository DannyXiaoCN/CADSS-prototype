//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_CALCONTROLLER_HPP
#define CADSS_PROTOTYPR_CALCONTROLLER_HPP

#include "CalTask.hpp"
#include "../Controllers/PerfectController.hpp"
#include <limits>


class CalController : public CalTask {

public:

    explicit CalController(PerfectController* perfectController, std::shared_ptr<AgentLog> agent_ibt, int startts);
    void Run(SimuClock* simuClock, int CurrentClock, SimuClock::SimulationType simulationType) override;
    int checkReady(std::shared_ptr<AgentLog> agent_ibt) override;

private:

    int start_timestamp_;
    int InnerClock;
    std::shared_ptr<AgentLog> agent_ibt_;
    PerfectController* perfectController_;
};




#endif //CADSS_PROTOTYPR_CALCONTROLLER_HPP
