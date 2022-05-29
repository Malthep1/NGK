#include "WeatherRegistration.hpp"

std::string WeatherRegistration::toJson(){
    char* json;
    sprintf(json,"{ \"ID\" :  %i, \"Time\" : { \"Date\" : \"%s\", \"Time\" : \"%s\"}, \"Place\" : %s \"Temperature\" : %d, \"Humidity\" : %i",id_, date_, time_, place_.toString(), temp_, humidity_);
    return  json;
}