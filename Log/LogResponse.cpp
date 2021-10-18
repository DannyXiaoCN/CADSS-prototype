//
// Created by 萧博成 on 2021/10/17.
//

#include "LogResponse.hpp"

#include <utility>

std::shared_ptr<AgentLog> LogResponse::getResponse() {

    return response;
}

LogResponse::LogResponse(std::shared_ptr<AgentLog> response_inut) {

    response = std::move(response_inut);

}
