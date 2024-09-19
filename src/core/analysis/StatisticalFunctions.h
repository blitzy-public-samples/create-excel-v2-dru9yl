#ifndef STATISTICAL_FUNCTIONS_H
#define STATISTICAL_FUNCTIONS_H

#include <vector>
#include <utility>

// Statistical functions for Excel's data analysis

// Calculates the arithmetic mean of a dataset
double mean(const std::vector<double>& data);

// Calculates the median of a dataset
double median(std::vector<double> data);

// Calculates the standard deviation of a dataset
double standardDeviation(const std::vector<double>& data);

// Calculates the Pearson correlation coefficient between two datasets
double correlation(const std::vector<double>& x, const std::vector<double>& y);

// Performs simple linear regression on two datasets
std::pair<double, double> linearRegression(const std::vector<double>& x, const std::vector<double>& y);

// Calculates the variance of a dataset
double variance(const std::vector<double>& data);

// Calculates the covariance between two datasets
double covariance(const std::vector<double>& x, const std::vector<double>& y);

// Calculates the skewness of a dataset
double skewness(const std::vector<double>& data);

// Calculates the kurtosis of a dataset
double kurtosis(const std::vector<double>& data);

// Human tasks:
// TODO: Add comprehensive documentation for each function, including mathematical formulas and usage examples
// TODO: Implement error handling for edge cases (e.g., empty vectors, division by zero)
// TODO: Add overloads for functions to support different data types (e.g., int, float)
// TODO: Implement versions of functions that work with Excel cell ranges directly
// TODO: Add more advanced statistical functions (e.g., ANOVA, t-tests, chi-square tests)
// TODO: Optimize functions for large datasets, possibly using parallel processing
// TODO: Implement confidence interval calculations for relevant functions
// TODO: Add support for handling missing data points in datasets
// TODO: Implement functions for probability distributions (e.g., normal, binomial, Poisson)

#endif // STATISTICAL_FUNCTIONS_H