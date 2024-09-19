#include "DataBarsIconSets.h"
#include "DataStore.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <mutex>

DataBarsIconSets::DataBarsIconSets(DataStore* store) : dataStore(store) {}

std::string DataBarsIconSets::createDataBar(const std::string& range, const DataBarOptions& options) {
    std::lock_guard<std::mutex> lock(elementMutex);

    // Validate input parameters
    if (range.empty() || !dataStore) {
        throw std::runtime_error("Invalid input parameters for createDataBar");
    }

    // Get data from the specified range
    std::vector<double> data = getDataFromRange(range);

    // Calculate data bar widths based on the data and options
    std::vector<double> widths = calculateDataBarWidths(data, options);

    // Apply data bars to the range
    applyDataBars(range, widths, options);

    // Generate a unique identifier for the data bar
    std::string elementId = generateUniqueId();

    // Store the VisualElementInfo in the visualElements map
    visualElements[elementId] = VisualElementInfo{range, VisualElementType::DataBar, options};

    return elementId;
}

std::string DataBarsIconSets::createIconSet(const std::string& range, const IconSetOptions& options) {
    std::lock_guard<std::mutex> lock(elementMutex);

    // Validate input parameters
    if (range.empty() || !dataStore) {
        throw std::runtime_error("Invalid input parameters for createIconSet");
    }

    // Get data from the specified range
    std::vector<double> data = getDataFromRange(range);

    // Calculate icon indices based on the data and options
    std::vector<int> iconIndices = calculateIconIndices(data, options);

    // Apply icon set to the range
    applyIconSet(range, iconIndices, options);

    // Generate a unique identifier for the icon set
    std::string elementId = generateUniqueId();

    // Store the VisualElementInfo in the visualElements map
    visualElements[elementId] = VisualElementInfo{range, VisualElementType::IconSet, options};

    return elementId;
}

bool DataBarsIconSets::updateVisualElement(const std::string& elementId, const VisualElementOptions& options) {
    std::lock_guard<std::mutex> lock(elementMutex);

    // Validate the element ID
    auto it = visualElements.find(elementId);
    if (it == visualElements.end()) {
        return false;
    }

    VisualElementInfo& elementInfo = it->second;

    // Update the options in the VisualElementInfo
    if (elementInfo.type == VisualElementType::DataBar) {
        DataBarOptions& dataBarOptions = std::get<DataBarOptions>(elementInfo.options);
        dataBarOptions = std::get<DataBarOptions>(options);
        createDataBar(elementInfo.range, dataBarOptions);
    } else if (elementInfo.type == VisualElementType::IconSet) {
        IconSetOptions& iconSetOptions = std::get<IconSetOptions>(elementInfo.options);
        iconSetOptions = std::get<IconSetOptions>(options);
        createIconSet(elementInfo.range, iconSetOptions);
    } else {
        return false;
    }

    return true;
}

bool DataBarsIconSets::deleteVisualElement(const std::string& elementId) {
    std::lock_guard<std::mutex> lock(elementMutex);

    // Validate the element ID
    auto it = visualElements.find(elementId);
    if (it == visualElements.end()) {
        return false;
    }

    const VisualElementInfo& elementInfo = it->second;

    // Clear the visual element from its range in the worksheet
    clearVisualElement(elementInfo.range, elementInfo.type);

    // Remove the visual element from the visualElements map
    visualElements.erase(it);

    return true;
}

std::vector<double> DataBarsIconSets::getDataFromRange(const std::string& range) {
    // Parse the range string to determine sheet and cell references
    SheetRange sheetRange = parseRange(range);

    // Call DataStore::getCellRange to retrieve the data
    std::vector<CellData> cellData = dataStore->getCellRange(sheetRange);

    // Convert the cell data to numeric values
    std::vector<double> numericData;
    numericData.reserve(cellData.size());

    std::transform(cellData.begin(), cellData.end(), std::back_inserter(numericData),
                   [](const CellData& cell) { return cell.getNumericValue(); });

    return numericData;
}

// Helper functions (implementation details omitted for brevity)
std::vector<double> DataBarsIconSets::calculateDataBarWidths(const std::vector<double>& data, const DataBarOptions& options) {
    // Implementation details omitted
    return std::vector<double>();
}

void DataBarsIconSets::applyDataBars(const std::string& range, const std::vector<double>& widths, const DataBarOptions& options) {
    // Implementation details omitted
}

std::vector<int> DataBarsIconSets::calculateIconIndices(const std::vector<double>& data, const IconSetOptions& options) {
    // Implementation details omitted
    return std::vector<int>();
}

void DataBarsIconSets::applyIconSet(const std::string& range, const std::vector<int>& iconIndices, const IconSetOptions& options) {
    // Implementation details omitted
}

std::string DataBarsIconSets::generateUniqueId() {
    // Implementation details omitted
    return "";
}

void DataBarsIconSets::clearVisualElement(const std::string& range, VisualElementType type) {
    // Implementation details omitted
}

SheetRange DataBarsIconSets::parseRange(const std::string& range) {
    // Implementation details omitted
    return SheetRange();
}

// Human tasks (commented):
/*
TODO: Implement support for custom color scales in data bars
TODO: Add error handling for invalid data ranges
TODO: Implement data bar direction (left-to-right or right-to-left)
TODO: Implement support for custom icon sets
TODO: Add error handling for invalid data ranges
TODO: Implement reverse order option for icon sets
TODO: Implement partial update support (e.g., update only specific options)
TODO: Add support for updating the range of an existing visual element
TODO: Implement undo/redo functionality for visual element updates
TODO: Implement cleanup of any resources associated with the visual element
TODO: Add logging for visual element deletion events
TODO: Implement error handling for non-numeric data
TODO: Add support for handling date and time values in visual elements
*/