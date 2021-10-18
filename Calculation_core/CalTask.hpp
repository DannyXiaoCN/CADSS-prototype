//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_CALTASK_HPP
#define CADSS_PROTOTYPR_CALTASK_HPP

#include "AgentLog.hpp"
#include "SimuClock.hpp"


class CalTask {


public:

    CalTask(){};
    virtual void Run(SimuClock* simuClock, int ClockTime, SimuClock::SimulationType simulationType);
    virtual int checkReady(std::shared_ptr<AgentLog> agent_ibt);

};
#endif //CADSS_PROTOTYPR_CALTASK_HPP
