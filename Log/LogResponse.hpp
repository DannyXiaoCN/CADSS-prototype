//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_LOGRESPONSE_HPP
#define CADSS_PROTOTYPR_LOGRESPONSE_HPP
#include <memory>
#include "LogContent.hpp"
#include "AgentLog.hpp"

class LogResponse {

public:

    explicit LogResponse(std::shared_ptr<AgentLog> response_inut);
    std::shared_ptr<AgentLog> getResponse();

private:

    std::shared_ptr<AgentLog> response;

};

#endif //CADSS_PROTOTYPR_LOGRESPONSE_HPP
