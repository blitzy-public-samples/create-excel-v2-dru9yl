using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using ExcelCore.Models;
using ExcelCore.Interfaces;

namespace ExcelCore.Repositories
{
    public class UserRepository : IUserRepository
    {
        private readonly DbContext context;

        public UserRepository(DbContext context)
        {
            // Initialize the context property with the provided DbContext
            this.context = context;
        }

        public async Task<User> GetByIdAsync(Guid id)
        {
            // Call context.Users.FindAsync with the provided id
            return await context.Set<User>().FindAsync(id);
            // Return the result
        }

        public async Task<IEnumerable<User>> GetAllAsync()
        {
            // Call context.Users.ToListAsync
            return await context.Set<User>().ToListAsync();
            // Return the result
        }

        public async Task<User> AddAsync(User user)
        {
            // Call context.Users.AddAsync with the provided user
            await context.Set<User>().AddAsync(user);
            // Call context.SaveChangesAsync to persist the changes
            await context.SaveChangesAsync();
            // Return the added user
            return user;
        }

        public async Task<bool> UpdateAsync(User user)
        {
            // Call context.Users.Update with the provided user
            context.Set<User>().Update(user);
            // Call context.SaveChangesAsync to persist the changes
            int affectedRows = await context.SaveChangesAsync();
            // Return true if SaveChangesAsync affected any rows, false otherwise
            return affectedRows > 0;
        }

        public async Task<bool> DeleteAsync(Guid id)
        {
            // Retrieve the user with the provided id
            var user = await context.Set<User>().FindAsync(id);
            if (user == null)
            {
                return false;
            }
            // If found, call context.Users.Remove with the user
            context.Set<User>().Remove(user);
            // Call context.SaveChangesAsync to persist the changes
            int affectedRows = await context.SaveChangesAsync();
            // Return true if SaveChangesAsync affected any rows, false otherwise
            return affectedRows > 0;
        }
    }
}

// Human tasks:
// - Implement error handling for database connection issues
// - Add logging for user retrieval attempts
// - Implement pagination for large user sets
// - Add sorting options for the returned user collection
// - Implement validation for user data before adding to the database
// - Add conflict resolution for duplicate usernames or emails
// - Implement optimistic concurrency handling
// - Add partial update support to allow updating specific fields
// - Implement soft delete functionality instead of hard delete
// - Add cascading delete for related user data
// - Implement unit tests for all repository methods
// - Add support for transaction management across multiple operations
// - Implement caching mechanism for frequently accessed users
// - Add logging and telemetry for performance monitoring
// - Implement batch operations for bulk user management
// - Add support for complex queries, such as searching users by various criteria
// - Implement data consistency checks before persistence operations
// - Add support for user-related aggregate queries (e.g., count of active users)
// - Implement proper disposal of DbContext to prevent resource leaks
// - Add support for asynchronous enumeration of large result sets