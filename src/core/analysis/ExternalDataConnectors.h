#ifndef EXTERNAL_DATA_CONNECTORS_H
#define EXTERNAL_DATA_CONNECTORS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

// Forward declaration of DataStore
class DataStore;

// Enum class representing different types of external data connections
enum class ConnectionType {
    HTTP,
    SQL,
    ODBC,
    OData
};

// Structure to hold information about an external data connection
struct ConnectionInfo {
    ConnectionType type;
    std::string connectionString;
    std::string username;
    std::string password;
    std::unordered_map<std::string, std::string> additionalParams;
};

class ExternalDataConnectors {
public:
    // Constructor: Initializes the ExternalDataConnectors with a reference to the DataStore
    ExternalDataConnectors(DataStore* store);

    // Creates a new connection to an external data source
    bool createConnection(const std::string& connectionName, const ConnectionInfo& info);

    // Removes an existing connection
    bool removeConnection(const std::string& connectionName);

    // Executes a query on the specified connection and stores the result in the given range
    bool executeQuery(const std::string& connectionName, const std::string& query, const std::string& outputRange);

    // Lists all available connections
    std::vector<std::string> listConnections();

    // Retrieves information about a specific connection
    ConnectionInfo getConnectionInfo(const std::string& connectionName);

    // Tests the specified connection
    bool testConnection(const std::string& connectionName);

private:
    DataStore* dataStore;
    std::unordered_map<std::string, ConnectionInfo> connections;
    mutable std::mutex connectorMutex;
};

// Human tasks (to be implemented):
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement error handling and input validation for all public methods
// - Add support for connection string encryption and secure storage of credentials
// - Implement connection pooling for improved performance with database connections
// - Add support for asynchronous query execution
// - Implement a mechanism for handling large result sets efficiently
// - Add support for data type mapping between external sources and Excel cell formats
// - Implement a query builder interface for constructing complex queries
// - Add support for real-time data streaming from supported external sources
// - Implement logging and diagnostics for connection and query operations

#endif // EXTERNAL_DATA_CONNECTORS_H