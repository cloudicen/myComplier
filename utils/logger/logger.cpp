//
// Created by cloudicen on 2021/8/31.
//

#include "logger.h"

void logger::debug(std::string msg) {
    //this->log4cppCategory->debug(std::format("{[]} {}",moduleName,msg));
    this->log4cppCategory.debug(std::string("["+moduleName+"] "+msg));
}

void logger::info(std::string msg) {
    //this->log4cppCategory->info(std::format("{[]} {}",moduleName,msg));
    this->log4cppCategory.info(std::string("["+moduleName+"] "+msg));
}

void logger::warn(std::string msg) {
    //this->log4cppCategory->warn(std::format("{[]} {}",moduleName,msg));
    this->log4cppCategory.warn(std::string("["+moduleName+"] "+msg));
}

void logger::error(std::string msg) {
    //this->log4cppCategory->error(std::format("{[]} {}",moduleName,msg));
    this->log4cppCategory.error(std::string("["+moduleName+"] "+msg));
}
