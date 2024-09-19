#ifndef THREEDMAPS_H
#define THREEDMAPS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <glm/glm.hpp>

// Forward declaration
class DataStore;
class Mesh;

// Enum class representing different map projection types
enum class MapProjection {
    MERCATOR,
    EQUIRECTANGULAR,
    ORTHOGRAPHIC,
    WINKEL_TRIPEL
};

// Structure to represent a geographical point with associated data
struct GeoPoint {
    double latitude;
    double longitude;
    double value;
    std::string label;
};

// Structure to hold options for 3D map creation and rendering
struct MapOptions {
    MapProjection projection;
    glm::vec3 baseColor;
    glm::vec3 highlightColor;
    bool showLabels;
    bool useHeightMapping;
    double heightScale;
};

// Structure to hold information about a 3D map
struct MapInfo {
    std::string dataRange;
    MapOptions options;
    std::vector<GeoPoint> geoData;
    Mesh mapMesh;
};

class ThreeDMaps {
public:
    // Constructor: Initializes the 3DMaps with a reference to the DataStore
    ThreeDMaps(DataStore* store);

    // Creates a new 3D map based on the specified parameters
    std::string create3DMap(const std::string& dataRange, const MapOptions& options);

    // Updates an existing 3D map with new data or options
    bool update3DMap(const std::string& mapId, const std::string& dataRange, const MapOptions& options);

    // Deletes an existing 3D map
    bool delete3DMap(const std::string& mapId);

    // Retrieves information about a specific 3D map
    MapInfo getMapInfo(const std::string& mapId) const;

    // Lists all 3D maps in the current workbook
    std::vector<std::string> listMaps() const;

private:
    DataStore* dataStore;
    std::unordered_map<std::string, MapInfo> maps;
    mutable std::mutex mapMutex;
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples and best practices
// TODO: Implement error handling and input validation for all public methods
// TODO: Add support for custom map styles and themes
// TODO: Implement interactivity features (e.g., zoom, pan, rotate) for 3D maps
// TODO: Add support for time-based data visualization (e.g., animated maps showing data changes over time)
// TODO: Implement data filtering and selection capabilities for 3D maps
// TODO: Add export functionality for 3D maps (e.g., as 3D models or interactive web components)
// TODO: Implement accessibility features for 3D maps (e.g., alternative text, keyboard navigation)
// TODO: Add support for overlaying additional data layers on the 3D map
// TODO: Implement performance optimizations for rendering large 3D maps with complex datasets

#endif // THREEDMAPS_H