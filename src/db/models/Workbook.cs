using System;
using System.ComponentModel.DataAnnotations;
using System.Collections.Generic;

namespace ExcelV2.Models
{
    /// <summary>
    /// Represents a workbook in the Excel application
    /// </summary>
    public class Workbook
    {
        /// <summary>
        /// Unique identifier for the workbook
        /// </summary>
        [Required]
        public Guid Id { get; set; }

        /// <summary>
        /// Name of the workbook
        /// </summary>
        [Required]
        [StringLength(255)]
        public string Name { get; set; }

        /// <summary>
        /// Identifier of the workbook owner
        /// </summary>
        [Required]
        public Guid OwnerId { get; set; }

        /// <summary>
        /// Navigation property for the workbook owner
        /// </summary>
        public User Owner { get; set; }

        /// <summary>
        /// Date and time when the workbook was created
        /// </summary>
        [Required]
        public DateTime CreatedAt { get; set; }

        /// <summary>
        /// Date and time when the workbook was last modified
        /// </summary>
        [Required]
        public DateTime LastModifiedAt { get; set; }

        /// <summary>
        /// Indicates whether the workbook is shared with other users
        /// </summary>
        public bool IsShared { get; set; }

        /// <summary>
        /// Collection of worksheets in the workbook
        /// </summary>
        public ICollection<Worksheet> Worksheets { get; set; }

        /// <summary>
        /// Collection of permissions for the workbook
        /// </summary>
        public ICollection<WorkbookPermission> Permissions { get; set; }

        /// <summary>
        /// Initializes a new instance of the Workbook class
        /// </summary>
        public Workbook()
        {
            // Initialize Id with a new Guid
            Id = Guid.NewGuid();

            // Set CreatedAt and LastModifiedAt to the current UTC time
            CreatedAt = DateTime.UtcNow;
            LastModifiedAt = DateTime.UtcNow;

            // Initialize Worksheets as an empty collection
            Worksheets = new List<Worksheet>();

            // Initialize Permissions as an empty collection
            Permissions = new List<WorkbookPermission>();
        }
    }
}

// Human Tasks:
// TODO: Implement versioning system for workbooks
// TODO: Add support for workbook templates
// TODO: Implement a method for creating a copy of a workbook
// TODO: Add support for workbook metadata (e.g., tags, categories)
// TODO: Implement auto-save functionality
// TODO: Add support for workbook comments and annotations
// TODO: Implement workbook sharing and collaboration features
// TODO: Add support for workbook encryption
// TODO: Implement workbook export to different formats (e.g., PDF, CSV)
// TODO: Add support for workbook recovery (from crashes or data corruption)