#include "Sparklines.h"
#include "DataStore.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <mutex>

// Constructor implementation
Sparklines::Sparklines(DataStore* store) : dataStore(store) {}

// Create a new sparkline
std::string Sparklines::createSparkline(const std::string& dataRange, const std::string& location, SparklineType type) {
    std::lock_guard<std::mutex> lock(sparklineMutex);

    // Validate input parameters
    if (dataRange.empty() || location.empty()) {
        throw std::runtime_error("Invalid data range or location");
    }

    // Get data from the specified range
    std::vector<double> data = getDataFromRange(dataRange);

    // Generate sparkline data based on the type
    std::vector<double> sparklineData = generateSparklineData(data, type);

    // Create a SparklineInfo object
    SparklineInfo info{dataRange, location, type, sparklineData};

    // Generate a unique identifier for the sparkline
    std::string sparklineId = "sparkline_" + std::to_string(sparklines.size() + 1);

    // Store the SparklineInfo in the sparklines map
    sparklines[sparklineId] = info;

    // Apply the sparkline to the specified location
    // Note: This step would involve interacting with the Excel API or the appropriate interface
    // to actually render the sparkline in the specified location. The implementation details
    // would depend on the specific Excel integration method being used.

    return sparklineId;
}

// Update an existing sparkline
bool Sparklines::updateSparkline(const std::string& sparklineId, const std::string& dataRange, SparklineType type) {
    std::lock_guard<std::mutex> lock(sparklineMutex);

    // Validate input parameters
    auto it = sparklines.find(sparklineId);
    if (it == sparklines.end()) {
        return false;
    }

    // Retrieve the existing SparklineInfo from the sparklines map
    SparklineInfo& info = it->second;

    // Get new data from the specified range
    std::vector<double> data = getDataFromRange(dataRange);

    // Generate new sparkline data based on the type
    std::vector<double> sparklineData = generateSparklineData(data, type);

    // Update the SparklineInfo object
    info.dataRange = dataRange;
    info.type = type;
    info.data = sparklineData;

    // Apply the updated sparkline to the existing location
    // Note: This step would involve interacting with the Excel API or the appropriate interface
    // to update the rendered sparkline. The implementation details would depend on the specific
    // Excel integration method being used.

    return true;
}

// Delete an existing sparkline
bool Sparklines::deleteSparkline(const std::string& sparklineId) {
    std::lock_guard<std::mutex> lock(sparklineMutex);

    // Validate the sparkline ID
    auto it = sparklines.find(sparklineId);
    if (it == sparklines.end()) {
        return false;
    }

    // Clear the sparkline from its location in the worksheet
    // Note: This step would involve interacting with the Excel API or the appropriate interface
    // to remove the rendered sparkline. The implementation details would depend on the specific
    // Excel integration method being used.

    // Remove the sparkline from the sparklines map
    sparklines.erase(it);

    return true;
}

// Retrieve data from the specified Excel range
std::vector<double> Sparklines::getDataFromRange(const std::string& range) {
    // Parse the range string to determine sheet and cell references
    // This is a simplified example; actual implementation would need to handle various range formats
    std::string sheet = "Sheet1"; // Assume Sheet1 for this example
    std::string cellRange = range;

    // Call DataStore::getCellRange to retrieve the data
    std::vector<Cell> cells = dataStore->getCellRange(sheet, cellRange);

    // Convert the cell data to numeric values
    std::vector<double> numericData;
    for (const auto& cell : cells) {
        // This is a simplified conversion; actual implementation would need to handle different data types
        try {
            numericData.push_back(std::stod(cell.getValue()));
        } catch (const std::invalid_argument&) {
            // Skip non-numeric values or handle them as needed
        }
    }

    return numericData;
}

// Generate sparkline data based on the input data and type
std::vector<double> Sparklines::generateSparklineData(const std::vector<double>& data, SparklineType type) {
    switch (type) {
        case SparklineType::LINE:
            return data; // Return the data as is for line sparklines

        case SparklineType::COLUMN: {
            // Normalize the data to fit in the cell
            auto [minIt, maxIt] = std::minmax_element(data.begin(), data.end());
            double minVal = *minIt;
            double maxVal = *maxIt;
            double range = maxVal - minVal;

            std::vector<double> normalizedData;
            for (double val : data) {
                normalizedData.push_back((val - minVal) / range);
            }
            return normalizedData;
        }

        case SparklineType::WIN_LOSS: {
            // Convert data to 1 (win), -1 (loss), or 0 (tie)
            std::vector<double> winLossData;
            double avg = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
            for (double val : data) {
                if (val > avg) winLossData.push_back(1.0);
                else if (val < avg) winLossData.push_back(-1.0);
                else winLossData.push_back(0.0);
            }
            return winLossData;
        }

        default:
            throw std::runtime_error("Unsupported sparkline type");
    }
}

// Human tasks:
// TODO: Implement support for custom sparkline colors and styles
// TODO: Add error handling for invalid data ranges or locations
// TODO: Implement sparkline grouping functionality
// TODO: Implement partial update support (e.g., update only data or only type)
// TODO: Add support for updating sparkline location
// TODO: Implement undo/redo functionality for sparkline updates
// TODO: Implement cleanup of any resources associated with the sparkline
// TODO: Add logging for sparkline deletion events
// TODO: Implement error handling for non-numeric data
// TODO: Add support for handling date and time values in sparklines
// TODO: Implement additional sparkline types (e.g., area, stacked)
// TODO: Add support for custom data transformations
// TODO: Implement data smoothing options for line sparklines