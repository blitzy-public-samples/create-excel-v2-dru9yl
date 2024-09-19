#include "ExternalDataConnectors.h"
#include "DataStore.h"
#include <curl/curl.h>
#include <sqlpp11/sqlpp11.h>
#include <json/json.h>
#include <sstream>
#include <stdexcept>
#include <mutex>

// Callback function for CURL to write received data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

ExternalDataConnectors::ExternalDataConnectors(DataStore* store) : dataStore(store) {}

bool ExternalDataConnectors::createConnection(const std::string& connectionName, const ConnectionInfo& info) {
    std::lock_guard<std::mutex> lock(connectorMutex);
    
    // Validate connection information
    if (info.type.empty() || info.connectionString.empty()) {
        return false;
    }
    
    // Add the connection to the connections map
    connections[connectionName] = info;
    return true;
}

bool ExternalDataConnectors::removeConnection(const std::string& connectionName) {
    std::lock_guard<std::mutex> lock(connectorMutex);
    
    // Check if the connection exists
    auto it = connections.find(connectionName);
    if (it == connections.end()) {
        return false;
    }
    
    // Remove the connection from the connections map
    connections.erase(it);
    return true;
}

bool ExternalDataConnectors::executeQuery(const std::string& connectionName, const std::string& query, const std::string& outputRange) {
    std::lock_guard<std::mutex> lock(connectorMutex);
    
    // Get the connection info for the specified connection name
    auto it = connections.find(connectionName);
    if (it == connections.end()) {
        return false;
    }
    
    const ConnectionInfo& info = it->second;
    std::vector<std::vector<std::string>> result;
    
    // Based on the connection type, call the appropriate query execution function
    try {
        if (info.type == "HTTP") {
            std::string response = executeHttpQuery(info, query);
            // Parse JSON response and convert to 2D vector
            Json::Value root;
            Json::Reader reader;
            if (reader.parse(response, root)) {
                // Assuming the response is an array of objects
                for (const auto& row : root) {
                    std::vector<std::string> rowData;
                    for (const auto& key : row.getMemberNames()) {
                        rowData.push_back(row[key].asString());
                    }
                    result.push_back(rowData);
                }
            }
        } else if (info.type == "SQL") {
            result = executeSqlQuery(info, query);
        } else {
            return false;
        }
    } catch (const std::exception& e) {
        // Log the error
        return false;
    }
    
    // Write the query result to the specified output range
    return writeResultToRange(result, outputRange);
}

std::string ExternalDataConnectors::executeHttpQuery(const ConnectionInfo& info, const std::string& query) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }
    
    std::string response;
    std::string url = info.connectionString + query;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("CURL request failed");
    }
    
    curl_easy_cleanup(curl);
    return response;
}

std::vector<std::vector<std::string>> ExternalDataConnectors::executeSqlQuery(const ConnectionInfo& info, const std::string& query) {
    sqlpp::mysql::connection db(info.connectionString);
    
    std::vector<std::vector<std::string>> result;
    auto rows = db(sqlpp::dynamic_select(db.dynamic_select()).dynamic_from(db.dynamic_from()).dynamic_where()).from_sql(query);
    
    for (const auto& row : rows) {
        std::vector<std::string> rowData;
        for (const auto& field : row) {
            rowData.push_back(field.text());
        }
        result.push_back(rowData);
    }
    
    return result;
}

bool ExternalDataConnectors::writeResultToRange(const std::vector<std::vector<std::string>>& result, const std::string& outputRange) {
    // Parse the outputRange to determine the starting cell
    // This is a simplified implementation and may need to be more robust
    char col = outputRange[0];
    int row = std::stoi(outputRange.substr(1));
    
    for (size_t i = 0; i < result.size(); ++i) {
        for (size_t j = 0; j < result[i].size(); ++j) {
            std::string cellAddress = std::string(1, col + j) + std::to_string(row + i);
            if (!dataStore->setCellValue(cellAddress, result[i][j])) {
                return false;
            }
        }
    }
    
    return true;
}

// Human tasks:
// TODO: Implement connection testing before adding to the map
// TODO: Add support for encrypted connection strings
// TODO: Implement connection pooling for better performance
// TODO: Implement cleanup of any resources associated with the connection
// TODO: Add logging for connection removal events
// TODO: Implement query parameter support
// TODO: Add query execution timeout mechanism
// TODO: Implement query result caching for improved performance
// TODO: Implement error handling for CURL operations
// TODO: Add support for different HTTP methods (GET, POST, etc.)
// TODO: Implement request retries for transient failures
// TODO: Implement connection pooling for better performance
// TODO: Add support for parameterized queries to prevent SQL injection
// TODO: Implement query timeout mechanism
// TODO: Implement data type detection and appropriate formatting
// TODO: Add support for writing to non-contiguous ranges
// TODO: Implement progress reporting for large result sets