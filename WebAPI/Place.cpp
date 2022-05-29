#include "Place.hpp"

std::string Place::toString(){
    std::string jsonObject = "\"Place\" : {" + "\"Placename\" : " + "\"" + placeName_ + "\"," + "\"lat\" : " + "\"" + lat_ + "\"," + "\"lon\" : " + "\"" + lon_ + "\"" + "},";
    return jsonObject;
}