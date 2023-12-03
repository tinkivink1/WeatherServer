#include "json/single_include/nlohmann/json.hpp"
#include <spdlog/spdlog.h>

#include "cpp-httplib/httplib.h"

#include <iostream>
#include <string>
#include <chrono>


#define MY_API_KEY std::string("08b5033082b8949843c3005d9f5313d5")

using json = nlohmann::json;


void log(const std::string& message) {
    spdlog::info(message);
}

std::string getWeatherData(const std::string& format = "") {
    httplib::Client client("http://api.openweathermap.org");

    std::string req = "/data/2.5/weather?q=Simferopol&units=metric&mode=" + format + "&appid=" + MY_API_KEY;

    auto res = client.Get(req);
    log("Request to weather service: " + req.replace(req.size() - MY_API_KEY.size(), MY_API_KEY.size(), "") + "MY_API_KEY");
    
    if (res && res->status == 200) {
        return res->body;
    }
    else {
        log("Failed to retrieve weather data: " + res->body);
        return "";
    }
}

void handleRequest(const httplib::Request& req, httplib::Response& res) {
    auto start_time = std::chrono::high_resolution_clock::now();

    log("Request received - Method: " + req.method + ", Path: " + req.path);

    std::string format = req.get_param_value("mode");
    std::string weatherData = getWeatherData(format);


    if (format == "html") {
        res.set_content(weatherData, "text/html");
    }
    else {
        res.set_content(weatherData, "application/json");
    }
    

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    log("Request completed in " + std::to_string(duration.count()) + " microseconds");
}

int main() {
    httplib::Server server;

    auto sb = std::stringstream();
    for (int i = 0; i < MY_API_KEY.size(); i++) {
        if (i < 5 || i > MY_API_KEY.size() - 3)
            sb << MY_API_KEY[i];
        else
            sb << "*";
    }

    log("API_KEY: " + sb.str());
    for (const auto& c : MY_API_KEY) {
    }
    server.Get("/weather", handleRequest);

    log("Server started on http://localhost:8080");
    server.listen("localhost", 8080);

    return 0;
}
