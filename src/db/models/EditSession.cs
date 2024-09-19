using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Collections.Generic;

namespace create_excel_v2_dru9yl.src.db.models
{
    /// <summary>
    /// Represents an editing session for a workbook in the Excel application
    /// </summary>
    public class EditSession
    {
        /// <summary>
        /// Unique identifier for the edit session
        /// </summary>
        [Key]
        public Guid Id { get; set; }

        /// <summary>
        /// Foreign key to the associated Workbook
        /// </summary>
        [Required]
        public Guid WorkbookId { get; set; }

        /// <summary>
        /// Navigation property for the associated Workbook
        /// </summary>
        [ForeignKey("WorkbookId")]
        public Workbook Workbook { get; set; }

        /// <summary>
        /// Foreign key to the associated User
        /// </summary>
        [Required]
        public Guid UserId { get; set; }

        /// <summary>
        /// Navigation property for the associated User
        /// </summary>
        [ForeignKey("UserId")]
        public User User { get; set; }

        /// <summary>
        /// Start time of the edit session
        /// </summary>
        [Required]
        public DateTime StartTime { get; set; }

        /// <summary>
        /// End time of the edit session (null if session is ongoing)
        /// </summary>
        public DateTime? EndTime { get; set; }

        /// <summary>
        /// Collection of edits made during this session
        /// </summary>
        public ICollection<Edit> Edits { get; set; }

        /// <summary>
        /// Initializes a new instance of the EditSession class
        /// </summary>
        public EditSession()
        {
            // Initialize Id with a new Guid
            Id = Guid.NewGuid();

            // Set StartTime to the current UTC time
            StartTime = DateTime.UtcNow;

            // Initialize Edits as an empty collection
            Edits = new List<Edit>();
        }
    }
}

// Human tasks:
// TODO: Implement a method to calculate the duration of the edit session
// TODO: Add support for tracking idle time within a session
// TODO: Implement a method to merge overlapping edit sessions
// TODO: Add support for session continuation after brief disconnections
// TODO: Implement a mechanism to detect and handle abandoned sessions
// TODO: Add support for session metadata (e.g., client information, IP address)
// TODO: Implement a method to summarize changes made during the session
// TODO: Add support for collaborative editing sessions with multiple users
// TODO: Implement session recovery mechanisms in case of unexpected termination
// TODO: Add support for session replay functionality