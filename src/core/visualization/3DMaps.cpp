#include "3DMaps.h"
#include "DataStore.h"
#include <algorithm>
#include <stdexcept>
#include <mutex>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Constructor
ThreeDMaps::ThreeDMaps(DataStore* store) : dataStore(store) {}

// Create a new 3D map
std::string ThreeDMaps::create3DMap(const std::string& dataRange, const MapOptions& options) {
    std::lock_guard<std::mutex> lock(mapMutex);

    // Validate input parameters
    if (dataRange.empty()) {
        throw std::runtime_error("Data range cannot be empty");
    }

    // Get geographical data from the specified range
    std::vector<GeoPoint> geoData = getGeoData(dataRange);

    // Generate 3D map mesh based on the geographical data
    Mesh mapMesh = generateMapMesh(geoData, options);

    // Apply data values to the map (e.g., color coding, height mapping)
    applyDataToMap(mapMesh, geoData, options);

    // Create a MapInfo object with the generated map data
    MapInfo mapInfo{mapMesh, options, geoData};

    // Generate a unique identifier for the map
    std::string mapId = generateUniqueMapId();

    // Store the MapInfo in the maps unordered_map
    maps[mapId] = std::move(mapInfo);

    return mapId;
}

// Update an existing 3D map
bool ThreeDMaps::update3DMap(const std::string& mapId, const std::string& dataRange, const MapOptions& options) {
    std::lock_guard<std::mutex> lock(mapMutex);

    // Validate the map ID and input parameters
    auto it = maps.find(mapId);
    if (it == maps.end()) {
        return false;
    }

    if (dataRange.empty()) {
        throw std::runtime_error("Data range cannot be empty");
    }

    // Retrieve the existing MapInfo
    MapInfo& mapInfo = it->second;

    // Get new geographical data from the specified range
    std::vector<GeoPoint> newGeoData = getGeoData(dataRange);

    // Apply new data values to the existing map
    applyDataToMap(mapInfo.mesh, newGeoData, options);

    // Update the MapInfo object with new options and data
    mapInfo.options = options;
    mapInfo.geoData = std::move(newGeoData);

    return true;
}

// Delete an existing 3D map
bool ThreeDMaps::delete3DMap(const std::string& mapId) {
    std::lock_guard<std::mutex> lock(mapMutex);

    // Validate the map ID
    auto it = maps.find(mapId);
    if (it == maps.end()) {
        return false;
    }

    // Remove the map from the maps unordered_map
    maps.erase(it);

    // Note: Any additional cleanup of resources should be implemented here

    return true;
}

// Retrieve and process geographical data from the specified Excel range
std::vector<GeoPoint> ThreeDMaps::getGeoData(const std::string& range) {
    // Parse the range string to determine sheet and cell references
    // (Implement range parsing logic here)

    // Call DataStore::getCellRange to retrieve the data
    auto cellData = dataStore->getCellRange(range);

    std::vector<GeoPoint> geoPoints;

    // Process the data to extract latitude, longitude, and associated values
    for (const auto& row : cellData) {
        if (row.size() >= 3) {
            double lat = std::stod(row[0]);
            double lon = std::stod(row[1]);
            double value = std::stod(row[2]);
            geoPoints.emplace_back(GeoPoint{lat, lon, value});
        }
    }

    return geoPoints;
}

// Generate a 3D mesh for the map based on geographical data
Mesh ThreeDMaps::generateMapMesh(const std::vector<GeoPoint>& geoData, const MapOptions& options) {
    Mesh mesh;

    // Create a base mesh (e.g., sphere or plane) based on the map projection
    if (options.projection == MapProjection::Spherical) {
        generateSphericalMesh(mesh, options.resolution);
    } else {
        generatePlanarMesh(mesh, options.resolution);
    }

    // Apply geographical data to deform the mesh (e.g., elevation data)
    applyElevationData(mesh, geoData);

    // Generate UV coordinates for texture mapping
    generateUVCoordinates(mesh);

    return mesh;
}

// Apply data values to the map (e.g., color coding, height mapping)
void ThreeDMaps::applyDataToMap(Mesh& mesh, const std::vector<GeoPoint>& geoData, const MapOptions& options) {
    // Implement data application logic here
    // This could involve setting vertex colors, adjusting heights, etc.
}

// Generate a unique map ID
std::string ThreeDMaps::generateUniqueMapId() {
    // Implement a method to generate a unique ID (e.g., UUID)
    static int counter = 0;
    return "map_" + std::to_string(++counter);
}

// Helper function to generate a spherical mesh
void ThreeDMaps::generateSphericalMesh(Mesh& mesh, int resolution) {
    // Implement spherical mesh generation
}

// Helper function to generate a planar mesh
void ThreeDMaps::generatePlanarMesh(Mesh& mesh, int resolution) {
    // Implement planar mesh generation
}

// Helper function to apply elevation data to the mesh
void ThreeDMaps::applyElevationData(Mesh& mesh, const std::vector<GeoPoint>& geoData) {
    // Implement elevation data application
}

// Helper function to generate UV coordinates for the mesh
void ThreeDMaps::generateUVCoordinates(Mesh& mesh) {
    // Implement UV coordinate generation
}

// Human tasks:
// TODO: Implement support for different map projections
// TODO: Add error handling for invalid geographical data
// TODO: Implement optimization for large datasets
// TODO: Implement partial update support (e.g., update only data or only options)
// TODO: Add support for animating transitions between old and new data
// TODO: Implement undo/redo functionality for map updates
// TODO: Implement cleanup of any external resources associated with the map
// TODO: Add logging for map deletion events
// TODO: Add support for different geographical data formats (e.g., addresses, country names)
// TODO: Optimize mesh generation for performance with large datasets
// TODO: Add level-of-detail (LOD) support for efficient rendering at different zoom levels