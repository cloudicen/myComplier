//
// Created by cloudicen on 2021/8/30.
//

#include "LoggerFactory.h"

std::shared_ptr<log4cpp::Category> LoggerFactory::root;
std::once_flag LoggerFactory::config_load_flag;

std::shared_ptr<logger> LoggerFactory::getLogger(std::string module) {
    std::call_once(config_load_flag,[](){
        log4cpp::PropertyConfigurator::configure("./conf/logger.conf");
        root.reset(&log4cpp::Category::getRoot());
    });
    return std::make_shared<logger>(root,module);
}
