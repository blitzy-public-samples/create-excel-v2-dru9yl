using System;
using System.ComponentModel.DataAnnotations;
using System.Collections.Generic;

namespace ExcelV2.Models
{
    /// <summary>
    /// Represents a feature in the Excel application
    /// </summary>
    public class Feature
    {
        /// <summary>
        /// Unique identifier for the feature
        /// </summary>
        [Required]
        public Guid Id { get; set; }

        /// <summary>
        /// Name of the feature
        /// </summary>
        [Required]
        [StringLength(100)]
        public string Name { get; set; }

        /// <summary>
        /// Description of the feature
        /// </summary>
        [StringLength(500)]
        public string Description { get; set; }

        /// <summary>
        /// Category of the feature
        /// </summary>
        [Required]
        [StringLength(50)]
        public string Category { get; set; }

        /// <summary>
        /// Indicates whether the feature is enabled
        /// </summary>
        public bool IsEnabled { get; set; }

        /// <summary>
        /// Date and time when the feature was created
        /// </summary>
        [Required]
        public DateTime CreatedAt { get; set; }

        /// <summary>
        /// Date and time when the feature was last updated
        /// </summary>
        public DateTime? LastUpdatedAt { get; set; }

        /// <summary>
        /// Collection of usage metrics associated with this feature
        /// </summary>
        public ICollection<UsageMetric> UsageMetrics { get; set; }

        /// <summary>
        /// Initializes a new instance of the Feature class
        /// </summary>
        public Feature()
        {
            // Initialize Id with a new Guid
            Id = Guid.NewGuid();

            // Set CreatedAt to the current UTC time
            CreatedAt = DateTime.UtcNow;

            // Set IsEnabled to true by default
            IsEnabled = true;

            // Initialize UsageMetrics as an empty collection
            UsageMetrics = new List<UsageMetric>();
        }
    }
}

// Human tasks:
// TODO: Implement a method to toggle feature enablement
// TODO: Add support for feature versioning
// TODO: Implement a mechanism for feature dependencies
// TODO: Add support for feature flags and A/B testing
// TODO: Implement a method to calculate feature usage statistics
// TODO: Add support for feature-specific configuration options
// TODO: Implement a mechanism for feature deprecation and sunset dates
// TODO: Add support for feature documentation and user guides
// TODO: Implement a method to track feature performance metrics
// TODO: Add support for feature access control based on user roles or licenses