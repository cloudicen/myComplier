//
// Created by cloudicen on 2021/8/31.
//

#include "logger.h"

void logger::debug(std::string msg) {
    //this->log4cppCategory->debug(std::format("{[]} {}",moduleName,msg));
    this->log4cppCategory.debug("[%s] %s",moduleName.c_str(),msg.c_str());
}

void logger::info(std::string msg) {
    //this->log4cppCategory->info(std::format("{[]} {}",moduleName,msg));
    this->log4cppCategory.info("[%s] %s",moduleName.c_str(),msg.c_str());
}

void logger::warn(std::string msg) {
    //this->log4cppCategory->warn(std::format("{[]} {}",moduleName,msg));
    this->log4cppCategory.warn("[%s] %s",moduleName.c_str(),msg.c_str());
}

void logger::error(std::string msg) {
    //this->log4cppCategory->error(std::format("{[]} {}",moduleName,msg));
    this->log4cppCategory.error("[%s] %s",moduleName.c_str(),msg.c_str());
}
