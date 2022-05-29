#include <string>
class Place{
    public:
        Place(std::string placeName, double lat, double lon){
            placeName_ = placeName;
            lat_ = lat;
            lon_ = lon;
        }
        std::string toString();
    private:
        std::string placeName_;
        double lat_;
        double lon_;
};