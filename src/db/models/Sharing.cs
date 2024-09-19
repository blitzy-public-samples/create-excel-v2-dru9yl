using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace ExcelV2.Models
{
    /// <summary>
    /// Represents a sharing relationship between a user and a workbook
    /// </summary>
    public class Sharing
    {
        /// <summary>
        /// Unique identifier for the sharing relationship
        /// </summary>
        [Key]
        public Guid Id { get; set; }

        /// <summary>
        /// ID of the workbook being shared
        /// </summary>
        [Required]
        public Guid WorkbookId { get; set; }

        /// <summary>
        /// Navigation property for the workbook
        /// </summary>
        [ForeignKey("WorkbookId")]
        public Workbook Workbook { get; set; }

        /// <summary>
        /// ID of the user the workbook is shared with
        /// </summary>
        [Required]
        public Guid UserId { get; set; }

        /// <summary>
        /// Navigation property for the user
        /// </summary>
        [ForeignKey("UserId")]
        public User User { get; set; }

        /// <summary>
        /// Permission level for this sharing relationship
        /// </summary>
        [Required]
        public SharingPermission Permission { get; set; }

        /// <summary>
        /// Date and time when the workbook was shared
        /// </summary>
        [Required]
        public DateTime SharedAt { get; set; }

        /// <summary>
        /// Optional expiration date and time for the sharing relationship
        /// </summary>
        public DateTime? ExpiresAt { get; set; }

        /// <summary>
        /// Initializes a new instance of the Sharing class
        /// </summary>
        public Sharing()
        {
            // Initialize Id with a new Guid
            Id = Guid.NewGuid();

            // Set SharedAt to the current UTC time
            SharedAt = DateTime.UtcNow;
        }
    }

    /// <summary>
    /// Enum representing different levels of sharing permissions
    /// </summary>
    public enum SharingPermission
    {
        ReadOnly,
        Comment,
        Edit
    }
}

// Human tasks:
// TODO: Implement a method to check if a sharing relationship has expired
// TODO: Add support for cascading delete when a workbook is deleted
// TODO: Implement a method to update sharing permissions
// TODO: Add support for sharing with groups of users
// TODO: Implement notifications for users when a workbook is shared with them
// TODO: Add support for sharing history and auditing
// TODO: Implement a method to revoke sharing access
// TODO: Add support for custom sharing messages
// TODO: Implement sharing analytics (e.g., tracking how often shared workbooks are accessed)
// TODO: Add support for sharing workbooks with external users (outside the organization)