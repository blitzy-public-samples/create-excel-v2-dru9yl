#ifndef CLOUDSYNC_H
#define CLOUDSYNC_H

#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include "src/core/data/DataStore.h"

class CloudSync {
public:
    // Constructor: Initializes the CloudSync with a reference to the DataStore and cloud provider settings
    CloudSync(DataStore* store, const std::string& provider, const std::string& token, int intervalSeconds = 300);

    // Starts the synchronization process
    void startSync();

    // Stops the synchronization process
    void stopSync();

    // Checks if synchronization is currently active
    bool isSyncActive() const;

    // Sets the synchronization interval
    void setSyncInterval(int intervalSeconds);

    // Gets the current synchronization interval
    int getSyncInterval() const;

    // Sets the cloud storage provider
    void setCloudProvider(const std::string& provider);

    // Gets the current cloud storage provider
    std::string getCloudProvider() const;

private:
    DataStore* dataStore;
    std::string cloudProvider;
    std::string authToken;
    std::chrono::seconds syncInterval;
    std::thread syncThread;
    mutable std::mutex syncMutex;
    bool isSyncing;

    // Add any private helper methods here
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples and best practices
// TODO: Implement error handling and reporting mechanism for sync operations
// TODO: Add support for multiple cloud storage providers (e.g., OneDrive, Google Drive, Dropbox)
// TODO: Implement a callback mechanism for sync status updates
// TODO: Add support for selective sync of specific sheets or ranges
// TODO: Implement a conflict resolution strategy for simultaneous edits
// TODO: Add support for offline mode and sync queue management
// TODO: Implement data encryption for secure cloud storage

#endif // CLOUDSYNC_H