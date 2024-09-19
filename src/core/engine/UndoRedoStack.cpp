#include "UndoRedoStack.h"
#include <stdexcept>
#include <algorithm>
#include <mutex>

// Constructor: Initialize the UndoRedoStack with a specified maximum stack size
UndoRedoStack::UndoRedoStack(size_t maxSize) : maxStackSize(maxSize) {
    // Undo and redo stacks are initialized as empty vectors by default
}

// Push a new action onto the undo stack
void UndoRedoStack::pushAction(std::unique_ptr<Action> action) {
    std::lock_guard<std::mutex> lock(stackMutex);
    
    // Clear the redo stack as a new action invalidates previous redos
    redoStack.clear();
    
    // Push the new action onto the undo stack
    undoStack.push_back(std::move(action));
    
    // If undo stack size exceeds maxStackSize, remove the oldest action
    if (undoStack.size() > maxStackSize) {
        undoStack.erase(undoStack.begin());
    }
}

// Undo the most recent action
bool UndoRedoStack::undo() {
    std::lock_guard<std::mutex> lock(stackMutex);
    
    if (undoStack.empty()) {
        return false;
    }
    
    // Pop the most recent action from the undo stack
    std::unique_ptr<Action> action = std::move(undoStack.back());
    undoStack.pop_back();
    
    // Execute the action's undo operation
    action->undo();
    
    // Push the action onto the redo stack
    redoStack.push_back(std::move(action));
    
    return true;
}

// Redo the most recently undone action
bool UndoRedoStack::redo() {
    std::lock_guard<std::mutex> lock(stackMutex);
    
    if (redoStack.empty()) {
        return false;
    }
    
    // Pop the most recent action from the redo stack
    std::unique_ptr<Action> action = std::move(redoStack.back());
    redoStack.pop_back();
    
    // Execute the action's redo operation
    action->redo();
    
    // Push the action onto the undo stack
    undoStack.push_back(std::move(action));
    
    return true;
}

// Clear both undo and redo stacks
void UndoRedoStack::clear() {
    std::lock_guard<std::mutex> lock(stackMutex);
    
    undoStack.clear();
    redoStack.clear();
}

// Human tasks (commented):
// TODO: Implement a mechanism to group related actions for compound undo/redo operations
// TODO: Add support for action compression to reduce memory usage
// TODO: Implement error handling for failed undo operations
// TODO: Add logging for undo operations to aid in debugging
// TODO: Implement error handling for failed redo operations
// TODO: Add logging for redo operations to aid in debugging
// TODO: Implement a mechanism to notify observers when stacks are cleared
// TODO: Add an option to selectively clear undo or redo stack