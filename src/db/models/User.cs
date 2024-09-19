using System;
using System.ComponentModel.DataAnnotations;
using System.Collections.Generic;

namespace ExcelV2.Models
{
    /// <summary>
    /// Represents a user in the Excel application
    /// </summary>
    public class User
    {
        /// <summary>
        /// Unique identifier for the user
        /// </summary>
        public Guid Id { get; set; }

        /// <summary>
        /// Username of the user
        /// </summary>
        [Required]
        [StringLength(50, MinimumLength = 3)]
        public string Username { get; set; }

        /// <summary>
        /// Email address of the user
        /// </summary>
        [Required]
        [EmailAddress]
        public string Email { get; set; }

        /// <summary>
        /// Hashed password of the user
        /// </summary>
        [Required]
        public string PasswordHash { get; set; }

        /// <summary>
        /// Date and time when the user account was created
        /// </summary>
        public DateTime CreatedAt { get; set; }

        /// <summary>
        /// Date and time of the user's last login
        /// </summary>
        public DateTime LastLoginAt { get; set; }

        /// <summary>
        /// Indicates whether the user account is active
        /// </summary>
        public bool IsActive { get; set; }

        /// <summary>
        /// Collection of workbooks associated with the user
        /// </summary>
        public ICollection<Workbook> Workbooks { get; set; }

        /// <summary>
        /// Initializes a new instance of the User class
        /// </summary>
        public User()
        {
            // Initialize Id with a new Guid
            Id = Guid.NewGuid();

            // Set CreatedAt to the current UTC time
            CreatedAt = DateTime.UtcNow;

            // Set IsActive to true
            IsActive = true;

            // Initialize Workbooks collection
            Workbooks = new List<Workbook>();
        }
    }
}

// Human tasks:
// TODO: Implement password hashing mechanism for the PasswordHash property
// TODO: Add validation for username uniqueness
// TODO: Implement a method for checking user permissions
// TODO: Add support for user roles and permissions
// TODO: Implement a method for user account deactivation
// TODO: Add support for multi-factor authentication
// TODO: Implement password reset functionality
// TODO: Add logging for user actions
// TODO: Implement user profile picture support
// TODO: Add support for user preferences and settings