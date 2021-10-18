//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_PERFECTCONTROLLER_HPP
#define CADSS_PROTOTYPR_PERFECTCONTROLLER_HPP

#include <AgentLog.hpp>
#include "../Model/PerfectModel.hpp"
#include "Controller.hpp"


class PerfectController {

public:
    explicit PerfectController(std::shared_ptr<AgentLog> AgentPtr);
    void update(int CurrentTimeStamp);

private:
    std::shared_ptr<AgentLog> agent_ibt;
};


#endif //CADSS_PROTOTYPR_PERFECTCONTROLLER_HPP
