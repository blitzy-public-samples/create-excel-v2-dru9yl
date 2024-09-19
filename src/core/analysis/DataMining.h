#ifndef DATA_MINING_H
#define DATA_MINING_H

#include <string>
#include <vector>
#include <mutex>
#include "src/core/data/DataStore.h"

class DataMining {
public:
    // Constructor: Initializes the DataMining with a reference to the DataStore
    DataMining(DataStore* store);

    // Performs k-means clustering on the given data range
    bool clusterAnalysis(const std::string& dataRange, int k, const std::string& outputRange);

    // Performs association rule mining on the given data range
    bool associationRuleMining(const std::string& dataRange, double minSupport, double minConfidence, const std::string& outputRange);

    // Builds a decision tree classifier on the given training data
    bool decisionTreeClassification(const std::string& trainingDataRange, const std::string& targetColumnRange, const std::string& outputRange);

    // Builds a Naive Bayes classifier on the given training data
    bool naiveBayesClassification(const std::string& trainingDataRange, const std::string& targetColumnRange, const std::string& outputRange);

    // Performs linear regression on the given data range
    bool linearRegression(const std::string& dataRange, const std::string& outputRange);

    // Performs Principal Component Analysis (PCA) on the given data range
    bool principalComponentAnalysis(const std::string& dataRange, int numComponents, const std::string& outputRange);

private:
    DataStore* dataStore;
    mutable std::mutex miningMutex;
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples and best practices
// TODO: Implement error handling and input validation for all public methods
// TODO: Add support for more advanced clustering algorithms (e.g., DBSCAN, hierarchical clustering)
// TODO: Implement feature selection and feature engineering capabilities
// TODO: Add support for time series analysis and forecasting
// TODO: Implement cross-validation and model evaluation metrics
// TODO: Add support for ensemble methods (e.g., Random Forests, Gradient Boosting)
// TODO: Implement data preprocessing methods (e.g., normalization, handling missing values)
// TODO: Add support for exporting models and importing pre-trained models
// TODO: Implement visualization methods for data mining results

#endif // DATA_MINING_H