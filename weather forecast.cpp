#include <iostream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Please replace "YOUR_API_KEY" with your actual OpenWeatherMap API key
const std::string API_KEY = "YOUR_API_KEY";
const std::string API_ENDPOINT = "http://api.openweathermap.org/data/2.5/weather";

// Function to perform HTTP GET request using libcurl
size_t curlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string getWeatherData(const std::string& city) {
    CURL* curl = curl_easy_init();
    std::string url = API_ENDPOINT + "?q=" + city + "&appid=" + API_KEY;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "Failed to fetch weather data. Error: " << curl_easy_strerror(res) << std::endl;
            return "";
        }

        return response;
    }

    return "";
}

void displayWeather(const std::string& city, const nlohmann::json& weatherData) {
    std::cout << "\nWeather Forecast for " << city << ":\n";

    if (weatherData["cod"] == 200) {
        std::cout << "Temperature: " << weatherData["main"]["temp"] << "°C\n";
        std::cout << "Description: " << weatherData["weather"][0]["description"] << "\n";
        std::cout << "Humidity: " << weatherData["main"]["humidity"] << "%\n";
        std::cout << "Wind Speed: " << weatherData["wind"]["speed"] << " m/s\n";
    } else {
        std::cout << "Error: " << weatherData["message"] << "\n";
    }
}

int main() {
    std::string city;

    std::cout << "Enter city name: ";
    std::getline(std::cin, city);

    std::string weatherData = getWeatherData(city);

    if (!weatherData.empty()) {
        nlohmann::json jsonData = nlohmann::json::parse(weatherData);
        displayWeather(city, jsonData);
    }

    return 0;
}
