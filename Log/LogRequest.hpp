//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_LOGREQUEST_HPP
#define CADSS_PROTOTYPR_LOGREQUEST_HPP


class LogRequest {

public:

    explicit LogRequest(int agnetId);
    int getAgent_id() const;

private:

    int Agent_id;

};



#endif //CADSS_PROTOTYPR_LOGREQUEST_HPP
