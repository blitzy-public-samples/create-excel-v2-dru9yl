#ifndef FILE_IO_H
#define FILE_IO_H

#include <string>
#include <mutex>
#include "src/core/data/DataStore.h"

class FileIO {
public:
    /**
     * @brief Initializes the FileIO with a reference to the DataStore
     * @param store Pointer to the DataStore object
     */
    FileIO(DataStore* store);

    /**
     * @brief Saves the current workbook to a file
     * @param filePath Path to save the workbook
     * @return True if the save was successful, false otherwise
     */
    bool saveWorkbook(const std::string& filePath);

    /**
     * @brief Loads a workbook from a file
     * @param filePath Path to load the workbook from
     * @return True if the load was successful, false otherwise
     */
    bool loadWorkbook(const std::string& filePath);

    /**
     * @brief Exports a specific sheet to a CSV file
     * @param sheetName Name of the sheet to export
     * @param filePath Path to save the CSV file
     * @return True if the export was successful, false otherwise
     */
    bool exportToCsv(const std::string& sheetName, const std::string& filePath);

    /**
     * @brief Imports a CSV file into a new sheet
     * @param filePath Path of the CSV file to import
     * @param sheetName Name of the new sheet to create
     * @return True if the import was successful, false otherwise
     */
    bool importFromCsv(const std::string& filePath, const std::string& sheetName);

private:
    DataStore* dataStore;
    mutable std::mutex ioMutex;
};

// Human tasks:
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement error handling and reporting mechanism for file operations
// - Add support for different Excel file formats (e.g., XLSX, XLSM, XLS)
// - Implement progress reporting for long-running file operations
// - Add support for partial file loading and saving (e.g., specific sheets or ranges)
// - Implement file recovery mechanisms for interrupted save operations
// - Add support for encrypted file operations
// - Implement validation of file integrity after save and before load operations

#endif // FILE_IO_H