#include "AdvancedVisualizations.h"
#include "DataStore.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>
#include <mutex>

AdvancedVisualizations::AdvancedVisualizations(DataStore* store) : dataStore(store) {}

std::string AdvancedVisualizations::createHeatMap(const std::string& range, const HeatMapOptions& options) {
    std::lock_guard<std::mutex> lock(visualizationMutex);

    // Validate input parameters
    if (range.empty() || !dataStore) {
        throw std::runtime_error("Invalid input parameters for heat map creation");
    }

    // Get data from the specified range
    auto data = getDataFromRange(range);

    // Calculate heat map colors based on the data and options
    auto colors = calculateHeatMapColors(data, options);

    // Apply heat map to the range
    applyHeatMap(range, colors);

    // Generate a unique identifier for the heat map
    std::string id = generateUniqueId();

    // Store the VisualizationInfo in the visualizations map
    visualizations[id] = VisualizationInfo{VisualizationType::HeatMap, range, options};

    return id;
}

std::string AdvancedVisualizations::createTreemap(const std::string& range, const TreemapOptions& options) {
    std::lock_guard<std::mutex> lock(visualizationMutex);

    // Validate input parameters
    if (range.empty() || !dataStore) {
        throw std::runtime_error("Invalid input parameters for treemap creation");
    }

    // Get data from the specified range
    auto data = getDataFromRange(range);

    // Calculate treemap layout based on the data and options
    auto layout = calculateTreemapLayout(data, options);

    // Apply treemap to the worksheet
    applyTreemap(layout);

    // Generate a unique identifier for the treemap
    std::string id = generateUniqueId();

    // Store the VisualizationInfo in the visualizations map
    visualizations[id] = VisualizationInfo{VisualizationType::Treemap, range, options};

    return id;
}

bool AdvancedVisualizations::updateVisualization(const std::string& visualizationId, const VisualizationOptions& options) {
    std::lock_guard<std::mutex> lock(visualizationMutex);

    // Validate the visualization ID
    auto it = visualizations.find(visualizationId);
    if (it == visualizations.end()) {
        return false;
    }

    // Retrieve the existing VisualizationInfo
    VisualizationInfo& info = it->second;

    // Update the options in the VisualizationInfo
    info.options = options;

    // Based on the visualization type, call the appropriate creation function with the updated options
    switch (info.type) {
        case VisualizationType::HeatMap:
            createHeatMap(info.range, std::get<HeatMapOptions>(options));
            break;
        case VisualizationType::Treemap:
            createTreemap(info.range, std::get<TreemapOptions>(options));
            break;
        default:
            return false;
    }

    return true;
}

bool AdvancedVisualizations::deleteVisualization(const std::string& visualizationId) {
    std::lock_guard<std::mutex> lock(visualizationMutex);

    // Validate the visualization ID
    auto it = visualizations.find(visualizationId);
    if (it == visualizations.end()) {
        return false;
    }

    // Clear the visualization from the worksheet
    clearVisualization(it->second);

    // Remove the visualization from the visualizations map
    visualizations.erase(it);

    return true;
}

std::vector<std::vector<double>> AdvancedVisualizations::getDataFromRange(const std::string& range) {
    // Parse the range string to determine sheet and cell references
    auto [sheet, startCell, endCell] = parseRange(range);

    // Call DataStore::getCellRange to retrieve the data
    auto cellData = dataStore->getCellRange(sheet, startCell, endCell);

    // Convert the cell data to numeric values
    std::vector<std::vector<double>> numericData;
    for (const auto& row : cellData) {
        std::vector<double> numericRow;
        for (const auto& cell : row) {
            numericRow.push_back(convertToNumeric(cell));
        }
        numericData.push_back(numericRow);
    }

    return numericData;
}

// Helper functions (implementations not shown for brevity)
std::vector<std::vector<Color>> AdvancedVisualizations::calculateHeatMapColors(const std::vector<std::vector<double>>& data, const HeatMapOptions& options) {
    // Implementation not shown
}

void AdvancedVisualizations::applyHeatMap(const std::string& range, const std::vector<std::vector<Color>>& colors) {
    // Implementation not shown
}

std::vector<TreemapRect> AdvancedVisualizations::calculateTreemapLayout(const std::vector<std::vector<double>>& data, const TreemapOptions& options) {
    // Implementation not shown
}

void AdvancedVisualizations::applyTreemap(const std::vector<TreemapRect>& layout) {
    // Implementation not shown
}

std::string AdvancedVisualizations::generateUniqueId() {
    // Implementation not shown
}

std::tuple<std::string, CellReference, CellReference> AdvancedVisualizations::parseRange(const std::string& range) {
    // Implementation not shown
}

double AdvancedVisualizations::convertToNumeric(const CellValue& value) {
    // Implementation not shown
}

void AdvancedVisualizations::clearVisualization(const VisualizationInfo& info) {
    // Implementation not shown
}

// Human tasks:
// TODO: Implement support for custom color scales in heat maps
// TODO: Add error handling for invalid data ranges
// TODO: Implement options for different color interpolation methods
// TODO: Implement support for hierarchical data in treemaps
// TODO: Add error handling for invalid data structures
// TODO: Implement options for different treemap algorithms (e.g., squarified, slice-and-dice)
// TODO: Implement partial update support (e.g., update only specific options)
// TODO: Add support for updating the range of an existing visualization
// TODO: Implement undo/redo functionality for visualization updates
// TODO: Implement cleanup of any resources associated with the visualization
// TODO: Add logging for visualization deletion events
// TODO: Implement error handling for non-numeric data
// TODO: Add support for handling date and time values in visualizations