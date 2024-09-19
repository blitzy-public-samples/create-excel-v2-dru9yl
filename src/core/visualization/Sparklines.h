#ifndef SPARKLINES_H
#define SPARKLINES_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

// Forward declaration of DataStore
class DataStore;

// Enum class representing different types of sparklines
enum class SparklineType {
    LINE,
    COLUMN,
    WIN_LOSS
};

// Structure to hold information about a sparkline
struct SparklineInfo {
    std::string dataRange;
    std::string location;
    SparklineType type;
    std::vector<double> data;
};

// Manages the creation and manipulation of sparklines in Excel
class Sparklines {
public:
    // Constructor: Initializes the Sparklines with a reference to the DataStore
    Sparklines(DataStore* store);

    // Creates a new sparkline based on the specified parameters
    std::string createSparkline(const std::string& dataRange, const std::string& location, SparklineType type);

    // Updates an existing sparkline with new data or settings
    bool updateSparkline(const std::string& sparklineId, const std::string& dataRange, SparklineType type);

    // Deletes an existing sparkline
    bool deleteSparkline(const std::string& sparklineId);

    // Retrieves information about a specific sparkline
    SparklineInfo getSparklineInfo(const std::string& sparklineId);

    // Lists all sparklines in the current workbook
    std::vector<std::string> listSparklines();

private:
    DataStore* dataStore;
    std::unordered_map<std::string, SparklineInfo> sparklines;
    mutable std::mutex sparklineMutex;
};

// Human tasks:
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement error handling and input validation for all public methods
// - Add support for custom sparkline styles (e.g., colors, line thickness, markers)
// - Implement sparkline grouping functionality
// - Add support for axis scaling and custom min/max values
// - Implement date axis support for time-based sparklines
// - Add support for highlighting specific points (e.g., first, last, min, max)
// - Implement tooltips for sparkline data points
// - Add support for sparkline legends and labels
// - Implement accessibility features for sparklines (e.g., alternative text descriptions)

#endif // SPARKLINES_H