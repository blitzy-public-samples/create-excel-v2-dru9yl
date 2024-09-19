using System;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace create_excel_v2_dru9yl.src.db.models
{
    /// <summary>
    /// Represents a single edit operation in an Excel workbook
    /// </summary>
    public class Edit
    {
        [Key]
        public Guid Id { get; set; }

        [Required]
        [ForeignKey("EditSession")]
        public Guid EditSessionId { get; set; }

        public virtual EditSession EditSession { get; set; }

        [Required]
        public string CellReference { get; set; }

        public string OldValue { get; set; }

        public string NewValue { get; set; }

        [Required]
        public DateTime Timestamp { get; set; }

        [Required]
        public EditType Type { get; set; }

        /// <summary>
        /// Initializes a new instance of the Edit class
        /// </summary>
        public Edit()
        {
            Id = Guid.NewGuid();
            Timestamp = DateTime.UtcNow;
        }
    }

    /// <summary>
    /// Enum representing different types of edits
    /// </summary>
    public enum EditType
    {
        CellValueChange,
        FormulaChange,
        FormatChange,
        RowInsert,
        RowDelete,
        ColumnInsert,
        ColumnDelete
    }
}

// Human tasks:
// TODO: Implement a method to revert an edit
// TODO: Add support for compound edits (e.g., paste operation affecting multiple cells)
// TODO: Implement a mechanism to track dependencies between edits
// TODO: Add support for edit comments or annotations
// TODO: Implement a method to calculate the impact of an edit on dependent cells
// TODO: Add support for edit conflict resolution in collaborative editing scenarios
// TODO: Implement a mechanism to compress or optimize stored edits
// TODO: Add support for edit notifications to relevant users
// TODO: Implement a method to generate a human-readable description of the edit
// TODO: Add support for tracking edit performance metrics (e.g., time taken to apply)