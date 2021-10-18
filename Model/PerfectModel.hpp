//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_PERFECTMODEL_HPP
#define CADSS_PROTOTYPR_PERFECTMODEL_HPP


#include <AgentLog.hpp>
#include <memory>

class PerfectModel {

public:
    static void PerfectRun(std::shared_ptr<AgentLog> AgentPtr, int CurrentTimeStamp);
};



#endif //CADSS_PROTOTYPR_PERFECTMODEL_HPP
