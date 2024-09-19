#ifndef POWER_QUERY_INTEGRATION_H
#define POWER_QUERY_INTEGRATION_H

#include <string>
#include <mutex>
#include <vector>
#include "src/core/data/DataStore.h"

// Structure to hold details about a Power Query
struct QueryDetails {
    std::string name;
    std::string query;
    std::string outputRange;
    std::string connectionName;
    int refreshInterval;
};

class PowerQueryIntegration {
public:
    // Constructor: Initializes the PowerQueryIntegration with a reference to the DataStore and Power Query endpoint
    PowerQueryIntegration(DataStore* store, const std::string& endpoint);

    // Executes a Power Query and stores the result in the specified range
    bool executeQuery(const std::string& query, const std::string& outputRange);

    // Refreshes a previously executed Power Query
    bool refreshQuery(const std::string& queryName);

    // Creates a new connection for a Power Query
    bool createQueryConnection(const std::string& connectionName, const std::string& connectionString);

    // Lists all available Power Queries
    std::vector<std::string> listQueries();

    // Retrieves details about a specific Power Query
    QueryDetails getQueryDetails(const std::string& queryName);

    // Sets the refresh interval for a Power Query
    bool setQueryRefreshInterval(const std::string& queryName, int intervalMinutes);

private:
    DataStore* dataStore;
    std::string powerQueryEndpoint;
    mutable std::mutex queryMutex;
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples and best practices
// TODO: Implement error handling and input validation for all public methods
// TODO: Add support for query parameters in the executeQuery method
// TODO: Implement a caching mechanism for frequently used queries
// TODO: Add support for query folding optimization
// TODO: Implement a mechanism for handling data type conversions between Power Query and Excel
// TODO: Add support for Power Query scripting language (M language) execution
// TODO: Implement progress reporting for long-running queries
// TODO: Add support for cancelling ongoing query executions
// TODO: Implement logging and diagnostics for query execution and performance

#endif // POWER_QUERY_INTEGRATION_H