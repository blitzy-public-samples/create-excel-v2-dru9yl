#include "ChartGeneration.h"
#include "DataStore.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <mutex>

// Constructor implementation
ChartGeneration::ChartGeneration(DataStore* store) : dataStore(store) {}

// Create a new chart
std::string ChartGeneration::createChart(const std::string& dataRange, ChartType type, const std::string& title, const std::string& xAxisLabel, const std::string& yAxisLabel) {
    std::lock_guard<std::mutex> lock(chartMutex);

    // Validate input parameters
    if (dataRange.empty() || title.empty()) {
        throw std::runtime_error("Invalid input parameters for chart creation");
    }

    // Get data from the specified range
    auto data = getDataFromRange(dataRange);

    // Generate a chart object based on the data and chart type
    auto chartObject = generateChartObject(data, type);

    // Assign title and axis labels to the chart
    chartObject.setTitle(title);
    chartObject.setXAxisLabel(xAxisLabel);
    chartObject.setYAxisLabel(yAxisLabel);

    // Generate a unique identifier for the chart
    std::string chartId = "chart_" + std::to_string(std::hash<std::string>{}(title + dataRange));

    // Store the chart object in the DataStore
    dataStore->storeChartObject(chartId, chartObject);

    return chartId;
}

// Update an existing chart
bool ChartGeneration::updateChart(const std::string& chartId, const std::string& dataRange, const std::string& title, const std::string& xAxisLabel, const std::string& yAxisLabel) {
    std::lock_guard<std::mutex> lock(chartMutex);

    // Validate input parameters
    if (chartId.empty() || dataRange.empty()) {
        return false;
    }

    // Retrieve the existing chart object from the DataStore
    auto chartObject = dataStore->getChartObject(chartId);
    if (!chartObject) {
        return false;
    }

    // Get new data from the specified range
    auto newData = getDataFromRange(dataRange);

    // Update the chart object with new data and settings
    chartObject->updateData(newData);
    if (!title.empty()) chartObject->setTitle(title);
    if (!xAxisLabel.empty()) chartObject->setXAxisLabel(xAxisLabel);
    if (!yAxisLabel.empty()) chartObject->setYAxisLabel(yAxisLabel);

    // Store the updated chart object in the DataStore
    dataStore->storeChartObject(chartId, *chartObject);

    return true;
}

// Delete an existing chart
bool ChartGeneration::deleteChart(const std::string& chartId) {
    std::lock_guard<std::mutex> lock(chartMutex);

    // Validate the chart ID
    if (chartId.empty()) {
        return false;
    }

    // Remove the chart object from the DataStore
    return dataStore->removeChartObject(chartId);
}

// Retrieve data from the specified Excel range
std::vector<std::vector<double>> ChartGeneration::getDataFromRange(const std::string& range) {
    // Parse the range string to determine sheet and cell references
    // This is a simplified implementation and may need to be expanded
    auto cellRange = dataStore->getCellRange(range);

    std::vector<std::vector<double>> numericData;
    for (const auto& row : cellRange) {
        std::vector<double> numericRow;
        for (const auto& cell : row) {
            try {
                numericRow.push_back(std::stod(cell));
            } catch (const std::invalid_argument&) {
                // Handle non-numeric data
                numericRow.push_back(0.0);
            }
        }
        numericData.push_back(numericRow);
    }

    return numericData;
}

// Generate a chart object based on the provided data and chart type
ChartObject ChartGeneration::generateChartObject(const std::vector<std::vector<double>>& data, ChartType type) {
    ChartObject chartObject;
    chartObject.setType(type);

    // Process the data according to the chart type
    switch (type) {
        case ChartType::LINE:
        case ChartType::BAR:
        case ChartType::COLUMN:
            // For these types, assume first column is categories and rest are series
            for (size_t i = 1; i < data[0].size(); ++i) {
                std::vector<double> series;
                for (size_t j = 0; j < data.size(); ++j) {
                    series.push_back(data[j][i]);
                }
                chartObject.addSeries(series);
            }
            std::vector<std::string> categories;
            for (const auto& row : data) {
                categories.push_back(std::to_string(row[0]));
            }
            chartObject.setCategories(categories);
            break;
        case ChartType::PIE:
            // For pie charts, assume first column is categories and second is values
            if (data[0].size() >= 2) {
                std::vector<std::string> pieCategories;
                std::vector<double> pieValues;
                for (const auto& row : data) {
                    pieCategories.push_back(std::to_string(row[0]));
                    pieValues.push_back(row[1]);
                }
                chartObject.setCategories(pieCategories);
                chartObject.addSeries(pieValues);
            }
            break;
        // Add more chart types as needed
    }

    return chartObject;
}

// Human tasks:
// TODO: Implement support for additional chart types
// TODO: Add error handling for invalid data ranges
// TODO: Implement chart styling options
// TODO: Implement partial update support (e.g., update only data or only labels)
// TODO: Add support for updating chart type
// TODO: Implement undo/redo functionality for chart updates
// TODO: Implement cleanup of any resources associated with the chart
// TODO: Add logging for chart deletion events
// TODO: Implement error handling for non-numeric data
// TODO: Add support for handling date and time values in charts
// TODO: Implement support for different chart types
// TODO: Add data validation for each chart type
// TODO: Implement automatic color scheme generation