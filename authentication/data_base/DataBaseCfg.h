//TO DO READ FROM ENV 
#pragma once

#include <string>

struct DataBaseCfg
{
    std::string host = "0.0.0.0";
    std::string port = "5432";
    std::string dbname = "ñoin_pulse";
    std::string user = "admin";
    std::string password = "admin";
};