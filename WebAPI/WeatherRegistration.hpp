#include <string>
#include <ctime>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "Place.hpp"

class WeatherRegistration{
    public:
        WeatherRegistration(std::string date, std::string time, std::string placename, double lat, double lon, double temp, int humidity, int * regNumber) : place_(placename, lat, lon) {
            (*regNumber)++;
           id_ = (*regNumber);
           date_ = date;
           time_ = time;
           temp_ = temp;
           humidity_ = humidity;
        }
        std::string toJson();
    private:
        int id_;
        std::string date_;
        std::string time_;
        Place place_;
        double temp_;
        int humidity_;
};