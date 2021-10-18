//
// Created by 萧博成 on 2021/10/17.
//

#ifndef CADSS_PROTOTYPR_CONTROLLER_HPP
#define CADSS_PROTOTYPR_CONTROLLER_HPP


#include<vector>
typedef std::vector<double> Vector;

///
/// Parent class of all controllers.
class Controller {
public:
    explicit Controller(int a, int b);

    virtual void update(int CurrentTimeStamp) = 0;

protected:
    const int a;
    const int b;
};

#endif //CADSS_PROTOTYPR_CONTROLLER_HPP
