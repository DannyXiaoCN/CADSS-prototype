//
// Created by 萧博成 on 2021/10/17.
//

#include "PerfectController.hpp"

#include <utility>

PerfectController::PerfectController(std::shared_ptr<AgentLog> AgentPtr) {

    agent_ibt = std::move(AgentPtr);
    std::cout <<"CalPredictor____________________________________________"<<std::endl;

}

void PerfectController::update(int CurrentTimeStamp) {

    std::shared_ptr<LogContent> TempLC = agent_ibt->GetLog(CurrentTimeStamp);
    TempLC->current_control[0] = 1;
    TempLC->current_control[1] = 1;
    TempLC->current_control[2] = 1;
    agent_ibt->UpdateLog(TempLC, CurrentTimeStamp);
    PerfectModel::PerfectRun(agent_ibt, CurrentTimeStamp);
}
