#ifndef DATA_BARS_ICON_SETS_H
#define DATA_BARS_ICON_SETS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <variant>
#include "src/core/data/DataStore.h"
#include "src/core/visualization/Common.h"

// Enum class representing different types of visual elements
enum class VisualElementType {
    DATA_BAR,
    ICON_SET
};

// Enum class representing different types of icon sets
enum class IconSetType {
    THREE_ARROWS,
    FOUR_ARROWS,
    FIVE_ARROWS,
    THREE_TRAFFIC_LIGHTS,
    FOUR_TRAFFIC_LIGHTS,
    FIVE_QUARTERS,
    FIVE_RATINGS
};

// Structure to hold options for data bars
struct DataBarOptions {
    Color fillColor;
    Color borderColor;
    bool showBorder;
    bool gradientFill;
    double minValue;
    double maxValue;
    bool usePercentage;
};

// Structure to hold options for icon sets
struct IconSetOptions {
    IconSetType iconSet;
    std::vector<double> thresholds;
    bool reverseOrder;
    bool showIconOnly;
};

// Structure to hold information about a visual element
struct VisualElementInfo {
    VisualElementType type;
    std::string range;
    std::variant<DataBarOptions, IconSetOptions> options;
};

class DataBarsIconSets {
public:
    // Constructor: Initializes the DataBarsIconSets with a reference to the DataStore
    explicit DataBarsIconSets(DataStore* store);

    // Creates a new data bar based on the specified parameters
    std::string createDataBar(const std::string& range, const DataBarOptions& options);

    // Creates a new icon set based on the specified parameters
    std::string createIconSet(const std::string& range, const IconSetOptions& options);

    // Updates an existing data bar or icon set with new options
    bool updateVisualElement(const std::string& elementId, const std::variant<DataBarOptions, IconSetOptions>& options);

    // Deletes an existing data bar or icon set
    bool deleteVisualElement(const std::string& elementId);

    // Retrieves information about a specific visual element
    VisualElementInfo getVisualElementInfo(const std::string& elementId);

    // Lists all visual elements in the current workbook
    std::vector<std::string> listVisualElements();

private:
    DataStore* dataStore;
    std::unordered_map<std::string, VisualElementInfo> visualElements;
    mutable std::mutex elementMutex;
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples and best practices
// TODO: Implement error handling and input validation for all public methods
// TODO: Add support for custom color scales in data bars
// TODO: Implement custom icon set creation and management
// TODO: Add support for conditional formatting rules in combination with data bars and icon sets
// TODO: Implement data bar and icon set styles (e.g., solid fill, gradient fill, different icon styles)
// TODO: Add support for negative value handling in data bars
// TODO: Implement axis position customization for data bars
// TODO: Add support for value-specific icon assignment in icon sets
// TODO: Implement accessibility features for data bars and icon sets (e.g., alternative text descriptions)

#endif // DATA_BARS_ICON_SETS_H