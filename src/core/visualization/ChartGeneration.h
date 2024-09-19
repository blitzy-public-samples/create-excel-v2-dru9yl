#ifndef CHART_GENERATION_H
#define CHART_GENERATION_H

#include <string>
#include <vector>
#include <mutex>

// Forward declaration of DataStore
class DataStore;

// Enum class representing different types of charts
enum class ChartType {
    BAR,
    COLUMN,
    LINE,
    PIE,
    SCATTER,
    AREA,
    DOUGHNUT,
    RADAR
};

// Class representing a chart object with its properties and data
class ChartObject {
public:
    ChartType type;
    std::string title;
    std::string xAxisLabel;
    std::string yAxisLabel;
    std::vector<std::vector<double>> data;
    std::vector<std::string> categories;

    // Sets the data for the chart
    void setData(const std::vector<std::vector<double>>& newData) {
        data = newData;
    }

    // Sets the categories for the chart
    void setCategories(const std::vector<std::string>& newCategories) {
        categories = newCategories;
    }
};

// Manages the creation and manipulation of charts in Excel
class ChartGeneration {
private:
    DataStore* dataStore;
    mutable std::mutex chartMutex;

public:
    // Initializes the ChartGeneration with a reference to the DataStore
    ChartGeneration(DataStore* store) : dataStore(store) {}

    // Creates a new chart based on the specified parameters
    std::string createChart(const std::string& dataRange, ChartType type, const std::string& title,
                            const std::string& xAxisLabel, const std::string& yAxisLabel);

    // Updates an existing chart with new data or settings
    bool updateChart(const std::string& chartId, const std::string& dataRange,
                     const std::string& title, const std::string& xAxisLabel,
                     const std::string& yAxisLabel);

    // Deletes an existing chart
    bool deleteChart(const std::string& chartId);

    // Retrieves information about a specific chart
    ChartObject getChartInfo(const std::string& chartId);

    // Lists all charts in the current workbook
    std::vector<std::string> listCharts();
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples and best practices
// TODO: Implement error handling and input validation for all public methods
// TODO: Add support for more advanced chart customization options (e.g., colors, fonts, legends)
// TODO: Implement chart templates for quick creation of common chart types
// TODO: Add support for dynamic charts that update automatically when data changes
// TODO: Implement chart animation capabilities for presentations
// TODO: Add support for exporting charts to various image formats
// TODO: Implement accessibility features for charts (e.g., alt text, screen reader support)
// TODO: Add support for combining multiple chart types in a single chart
// TODO: Implement data labeling options for charts

#endif // CHART_GENERATION_H