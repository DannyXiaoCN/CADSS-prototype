//
// Created by 萧博成 on 2021/10/17.
//

#include "PerfectModel.hpp"

void PerfectModel::PerfectRun(std::shared_ptr<AgentLog> AgentPtr, int CurrentTimeStamp) {

    std::shared_ptr<LogContent> TempLC = AgentPtr->GetLog(CurrentTimeStamp);
    TempLC->next_state_[0] = TempLC->PlannerResult.pts.find(CurrentTimeStamp + 1)->second.x;
    TempLC->next_state_[1] = TempLC->PlannerResult.pts.find(CurrentTimeStamp + 1)->second.y;
    TempLC->next_state_[2] = TempLC->PlannerResult.pts.find(CurrentTimeStamp + 1)->second.yaw;
    TempLC->next_state_[3] = TempLC->PlannerResult.pts.find(CurrentTimeStamp + 1)->second.vx;
    TempLC->next_state_[4] = TempLC->PlannerResult.pts.find(CurrentTimeStamp + 1)->second.vy;
    TempLC->next_state_[5] = TempLC->PlannerResult.pts.find(CurrentTimeStamp + 1)->second.yaw_rate;
    std::cout << "debug: Model updating results: " << TempLC->next_state_[0] << "," << TempLC->next_state_[1] << "," << TempLC->next_state_[2] << "," << TempLC->next_state_[3] << "," << TempLC->next_state_[4] << "," << TempLC->next_state_[5] << "," << std::endl;
    AgentPtr->UpdateLog(TempLC, CurrentTimeStamp);
}
