#include "PowerQueryIntegration.h"
#include "DataStore.h"
#include <curl/curl.h>
#include <json/json.h>
#include <sstream>
#include <stdexcept>
#include <mutex>

// Callback function for CURL to write received data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

PowerQueryIntegration::PowerQueryIntegration(DataStore* store, const std::string& endpoint)
    : dataStore(store), powerQueryEndpoint(endpoint) {
    // Constructor implementation
}

bool PowerQueryIntegration::executeQuery(const std::string& query, const std::string& outputRange) {
    std::lock_guard<std::mutex> lock(queryMutex);

    try {
        // Send query request to Power Query endpoint
        std::string rawResult = sendQueryRequest(query);

        // Parse the query result
        Json::Value parsedResult = parseQueryResult(rawResult);

        // Write the result to the specified output range
        return writeResultToRange(parsedResult, outputRange);
    } catch (const std::exception& e) {
        // Log the error
        // TODO: Implement proper error logging
        return false;
    }
}

bool PowerQueryIntegration::refreshQuery(const std::string& queryName) {
    std::lock_guard<std::mutex> lock(queryMutex);

    // TODO: Implement a mechanism to store and retrieve query metadata
    // For now, we'll assume queryName is the actual query and outputRange
    std::string query = queryName;
    std::string outputRange = "A1"; // Default output range

    return executeQuery(query, outputRange);
}

std::string PowerQueryIntegration::sendQueryRequest(const std::string& query) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, powerQueryEndpoint.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, query.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("CURL request failed: " + std::string(curl_easy_strerror(res)));
    }

    curl_easy_cleanup(curl);
    return response;
}

Json::Value PowerQueryIntegration::parseQueryResult(const std::string& rawResult) {
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(rawResult, root)) {
        throw std::runtime_error("Failed to parse JSON result");
    }

    return root;
}

bool PowerQueryIntegration::writeResultToRange(const Json::Value& result, const std::string& outputRange) {
    // Parse the outputRange to determine the starting cell
    // For simplicity, we'll assume outputRange is just the starting cell
    std::string startCell = outputRange;

    // TODO: Implement proper range parsing

    int row = 0;
    int col = 0;

    for (const auto& row_data : result) {
        col = 0;
        for (const auto& cell_data : row_data) {
            std::string cellValue = cell_data.asString();
            if (!dataStore->setCellValue(startCell, row, col, cellValue)) {
                return false;
            }
            col++;
        }
        row++;
    }

    return true;
}

// Human tasks:
// TODO: Implement error handling for network failures
// TODO: Add support for query parameters
// TODO: Implement caching mechanism for frequently used queries
// TODO: Implement a mechanism to store and retrieve query metadata
// TODO: Add support for scheduling automatic query refreshes
// TODO: Implement proper error handling for CURL operations
// TODO: Add support for authentication if required by the Power Query endpoint
// TODO: Implement error handling for malformed JSON responses
// TODO: Add support for different result formats (e.g., CSV, XML)
// TODO: Implement support for different data types in the result
// TODO: Add error handling for range overflow
// TODO: Implement formatting options for the output range