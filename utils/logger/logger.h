//
// Created by cloudicen on 2021/8/31.
//

#ifndef MY_COMPILER_LOGGER_H
#define MY_COMPILER_LOGGER_H

#include <log4cpp/Category.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PropertyConfigurator.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <memory>
//#include <format>

class logger {
private:
    log4cpp::Category &log4cppCategory;
    std::string moduleName;
public:
    explicit logger(log4cpp::Category &_logger,std::string _moduleName = "none") : log4cppCategory(_logger),
                                                                           moduleName(std::move(_moduleName)) {};

    void debug(std::string msg);

    void info(std::string msg);

    void warn(std::string msg);

    void error(std::string msg);
};


#endif //MY_COMPILER_LOGGER_H
