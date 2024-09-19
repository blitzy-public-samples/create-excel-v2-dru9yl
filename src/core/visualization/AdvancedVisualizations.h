#ifndef ADVANCED_VISUALIZATIONS_H
#define ADVANCED_VISUALIZATIONS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <variant>
#include "src/core/visualization/Common.h"
#include "src/core/data/DataStore.h"

// Enum class representing different types of advanced visualizations
enum class VisualizationType {
    HEAT_MAP,
    TREEMAP
};

// Structure to hold options for heat maps
struct HeatMapOptions {
    Color minColor;
    Color maxColor;
    Color midColor;
    bool useCustomMidpoint;
    double customMidpoint;
};

// Enum class for treemap algorithms
enum class TreemapAlgorithm {
    SQUARIFIED,
    SLICE_AND_DICE,
    STRIP
};

// Structure to hold options for treemaps
struct TreemapOptions {
    std::vector<Color> colorPalette;
    bool showLabels;
    TreemapAlgorithm algorithm;
};

// Structure to hold information about an advanced visualization
struct VisualizationInfo {
    VisualizationType type;
    std::string range;
    std::variant<HeatMapOptions, TreemapOptions> options;
};

// Class for managing advanced visualizations in Excel
class AdvancedVisualizations {
private:
    DataStore* dataStore;
    std::unordered_map<std::string, VisualizationInfo> visualizations;
    mutable std::mutex visualizationMutex;

public:
    // Constructor
    explicit AdvancedVisualizations(DataStore* store);

    // Creates a new heat map based on the specified parameters
    std::string createHeatMap(const std::string& range, const HeatMapOptions& options);

    // Creates a new treemap based on the specified parameters
    std::string createTreemap(const std::string& range, const TreemapOptions& options);

    // Updates an existing advanced visualization with new options
    bool updateVisualization(const std::string& visualizationId, const std::variant<HeatMapOptions, TreemapOptions>& options);

    // Deletes an existing advanced visualization
    bool deleteVisualization(const std::string& visualizationId);

    // Retrieves information about a specific advanced visualization
    VisualizationInfo getVisualizationInfo(const std::string& visualizationId);

    // Lists all advanced visualizations in the current workbook
    std::vector<std::string> listVisualizations();
};

// Human tasks (to be implemented):
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement error handling and input validation for all public methods
// - Add support for additional advanced visualization types (e.g., network graphs, sunburst charts)
// - Implement customization options for each visualization type (e.g., label formatting, tooltips)
// - Add support for dynamic data updates in visualizations
// - Implement interactivity features for visualizations (e.g., drill-down, filtering)
// - Add export functionality for visualizations (e.g., as images or interactive web components)
// - Implement accessibility features for advanced visualizations (e.g., alternative text, keyboard navigation)
// - Add support for custom color palettes and themes across all visualization types
// - Implement performance optimizations for large datasets in visualizations

#endif // ADVANCED_VISUALIZATIONS_H