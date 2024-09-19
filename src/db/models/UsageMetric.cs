using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ExcelV2.Models
{
    /// <summary>
    /// Represents a usage metric for a specific feature in Excel
    /// </summary>
    public class UsageMetric
    {
        /// <summary>
        /// Unique identifier for the usage metric
        /// </summary>
        [Key]
        public Guid Id { get; set; }

        /// <summary>
        /// Foreign key for the User
        /// </summary>
        [Required]
        public Guid UserId { get; set; }

        /// <summary>
        /// Navigation property for the User
        /// </summary>
        [ForeignKey("UserId")]
        public User User { get; set; }

        /// <summary>
        /// Foreign key for the Feature
        /// </summary>
        [Required]
        public Guid FeatureId { get; set; }

        /// <summary>
        /// Navigation property for the Feature
        /// </summary>
        [ForeignKey("FeatureId")]
        public Feature Feature { get; set; }

        /// <summary>
        /// Timestamp when the feature was used
        /// </summary>
        [Required]
        public DateTime Timestamp { get; set; }

        /// <summary>
        /// Duration of feature usage in seconds
        /// </summary>
        [Required]
        public int Duration { get; set; }

        /// <summary>
        /// Additional context information about the feature usage
        /// </summary>
        public string Context { get; set; }

        /// <summary>
        /// Initializes a new instance of the UsageMetric class
        /// </summary>
        public UsageMetric()
        {
            Id = Guid.NewGuid();
            Timestamp = DateTime.UtcNow;
        }
    }
}

// Human tasks:
// TODO: Implement a method to aggregate usage metrics by feature
// TODO: Add support for tracking user sessions across multiple features
// TODO: Implement a mechanism to anonymize usage data for privacy compliance
// TODO: Add support for custom metadata fields for different feature types
// TODO: Implement a method to calculate feature adoption rates
// TODO: Add support for tracking feature usage trends over time
// TODO: Implement a mechanism to detect and flag unusual usage patterns
// TODO: Add support for exporting usage metrics in various formats
// TODO: Implement a method to correlate usage metrics with user demographics
// TODO: Add support for real-time usage metric streaming for live dashboards