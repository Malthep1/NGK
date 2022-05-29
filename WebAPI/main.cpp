#include <iostream>
#include <restinio/all.hpp>
#include "WeatherRegistration.hpp"

using namespace restinio;

auto part1Handler(WeatherRegistration * reg){
    auto router = std::make_unique<router::express_router_t<>>();
    router->http_get(
            R"(/single)",
            [reg](auto req, auto params) {
                const auto qp = parse_query(req->header().query());
                return req->create_response()
                    .set_body((*reg).toJson())
                    .done();
            });

    router->non_matched_request_handler(
        []( auto req ){
            return
                req->create_response(restinio::status_not_found())
                    .append_header_date_field()
                    .connection_close()
                    .done();
        } );
    return router;
}

void runPart2(){
    //Assignment part 2 server
}

void runPart3(){
    //Assignment part 3 server
}

void runPart1(WeatherRegistration * reg){
    //Assignment part 1 server
    run(
    on_this_thread()
    .port(8080)
    .address("localhost")
    .request_handler(part1Handler(reg)));
}

int main(){
    int regNumber = 0;
    int * rptr = &regNumber;
    std::string date = "052022";
    std::string time = "200352";
    std::string placeName = "Assville";
    WeatherRegistration reg(date, time, placeName, 32.3, 10.1, 19.2, 10,rptr);
    WeatherRegistration * preg = &reg;
    runPart1(preg); // Del 1
    //runPart2(); // Del 2
    //runPart3(); // Del 3
    return 0;
}