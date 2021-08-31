//
// Created by cloudicen on 2021/8/30.
//

#ifndef MY_COMPILER_LOGGERFACTORY_H
#define MY_COMPILER_LOGGERFACTORY_H

#include <iostream>
#include <mutex>
#include <memory>
#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>

#include "logger.h"

class LoggerFactory {
private:
    static std::once_flag config_load_flag;
public:
    static std::shared_ptr<logger> getLogger(std::string module,const std::string& loggerName = "");
};


#endif //MY_COMPILER_LOGGERFACTORY_H
