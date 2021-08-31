//
// Created by cloudicen on 2021/8/30.
//

#include "LoggerFactory.h"

std::once_flag LoggerFactory::config_load_flag;

std::shared_ptr<logger> LoggerFactory::getLogger(std::string module,const std::string& loggerName) {
    std::call_once(config_load_flag,[](){
        log4cpp::PropertyConfigurator::configure("./config/logger.conf");
    });
    return std::make_shared<logger>(log4cpp::Category::getInstance(loggerName),module);
}
