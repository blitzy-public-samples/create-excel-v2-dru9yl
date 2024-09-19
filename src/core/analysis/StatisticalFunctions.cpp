#include "StatisticalFunctions.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <vector>

// Human tasks:
// TODO: Add input validation for NaN and infinity values
// TODO: Implement overflow protection for large datasets
double StatisticalFunctions::mean(const std::vector<double>& data) {
    // Check if the input vector is empty
    if (data.empty()) {
        throw std::invalid_argument("Cannot calculate mean of an empty dataset");
    }

    // Calculate the sum of all elements using std::accumulate
    double sum = std::accumulate(data.begin(), data.end(), 0.0);

    // Divide the sum by the number of elements
    return sum / data.size();
}

// Human tasks:
// TODO: Implement a more efficient algorithm for large datasets
// TODO: Add support for weighted median calculation
double StatisticalFunctions::median(std::vector<double> data) {
    // Check if the input vector is empty
    if (data.empty()) {
        throw std::invalid_argument("Cannot calculate median of an empty dataset");
    }

    // Sort the input vector
    std::sort(data.begin(), data.end());

    size_t size = data.size();
    if (size % 2 == 0) {
        // If the vector size is even, return the average of the two middle elements
        return (data[size/2 - 1] + data[size/2]) / 2.0;
    } else {
        // If the vector size is odd, return the middle element
        return data[size/2];
    }
}

// Human tasks:
// TODO: Implement both population and sample standard deviation options
// TODO: Optimize for numerical stability in large datasets
double StatisticalFunctions::standardDeviation(const std::vector<double>& data) {
    // Check if the input vector has less than two elements
    if (data.size() < 2) {
        throw std::invalid_argument("Cannot calculate standard deviation with less than two elements");
    }

    // Calculate the mean of the dataset
    double mean_value = mean(data);

    // Calculate the sum of squared differences from the mean
    double sum_squared_diff = 0.0;
    for (const double& value : data) {
        sum_squared_diff += std::pow(value - mean_value, 2);
    }

    // Divide the sum by (n-1) for sample standard deviation
    double variance = sum_squared_diff / (data.size() - 1);

    // Take the square root of the result
    return std::sqrt(variance);
}

// Human tasks:
// TODO: Implement error handling for datasets with zero standard deviation
// TODO: Add support for Spearman's rank correlation
double StatisticalFunctions::correlation(const std::vector<double>& x, const std::vector<double>& y) {
    // Check if input vectors have the same size and at least two elements
    if (x.size() != y.size() || x.size() < 2) {
        throw std::invalid_argument("Input vectors must have the same size and at least two elements");
    }

    // Calculate means of both datasets
    double mean_x = mean(x);
    double mean_y = mean(y);

    // Calculate the sum of products of differences from means
    double sum_product_diff = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        sum_product_diff += (x[i] - mean_x) * (y[i] - mean_y);
    }

    // Calculate standard deviations of both datasets
    double std_dev_x = standardDeviation(x);
    double std_dev_y = standardDeviation(y);

    // Divide the sum of products by the product of standard deviations
    return sum_product_diff / (std_dev_x * std_dev_y * x.size());
}

// Human tasks:
// TODO: Implement calculation of R-squared value
// TODO: Add support for multiple linear regression
std::pair<double, double> StatisticalFunctions::linearRegression(const std::vector<double>& x, const std::vector<double>& y) {
    // Check if input vectors have the same size and at least two elements
    if (x.size() != y.size() || x.size() < 2) {
        throw std::invalid_argument("Input vectors must have the same size and at least two elements");
    }

    // Calculate means of both datasets
    double mean_x = mean(x);
    double mean_y = mean(y);

    // Calculate the sum of products of differences from means
    double sum_product_diff = 0.0;
    double sum_squared_diff_x = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double diff_x = x[i] - mean_x;
        sum_product_diff += diff_x * (y[i] - mean_y);
        sum_squared_diff_x += diff_x * diff_x;
    }

    // Calculate the slope (b1) of the regression line
    double slope = sum_product_diff / sum_squared_diff_x;

    // Calculate the intercept (b0) of the regression line
    double intercept = mean_y - slope * mean_x;

    // Return the slope and intercept as a pair
    return std::make_pair(slope, intercept);
}