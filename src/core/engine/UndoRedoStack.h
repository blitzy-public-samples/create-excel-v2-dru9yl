#ifndef UNDO_REDO_STACK_H
#define UNDO_REDO_STACK_H

#include <vector>
#include <memory>
#include <mutex>

// Abstract base class for actions that can be undone and redone
class Action {
public:
    virtual ~Action() = default;

    // Pure virtual function to undo the action
    virtual void undo() = 0;

    // Pure virtual function to redo the action
    virtual void redo() = 0;
};

// Manages undo and redo operations for the Excel engine
class UndoRedoStack {
private:
    std::vector<std::unique_ptr<Action>> undoStack;
    std::vector<std::unique_ptr<Action>> redoStack;
    size_t maxStackSize;
    mutable std::mutex stackMutex;

public:
    // Initializes the UndoRedoStack with a specified maximum stack size
    explicit UndoRedoStack(size_t maxSize = 100) : maxStackSize(maxSize) {}

    // Pushes a new action onto the undo stack
    void pushAction(std::unique_ptr<Action> action);

    // Undoes the most recent action
    bool undo();

    // Redoes the most recently undone action
    bool redo();

    // Clears both undo and redo stacks
    void clear();

    // Checks if there are actions that can be undone
    bool canUndo() const;

    // Checks if there are actions that can be redone
    bool canRedo() const;
};

// Human tasks:
// - Add comprehensive documentation for each method, including usage examples and best practices
// - Implement a mechanism to group related actions for compound undo/redo operations
// - Consider adding a method to get the current undo/redo stack sizes
// - Implement a way to serialize and deserialize the undo/redo stacks for saving and loading workbook state
// - Add support for action descriptions to provide user-friendly undo/redo menu items
// - Consider implementing an event system to notify observers of undo/redo operations
// - Add support for limiting the undo/redo history based on time or memory usage in addition to action count

#endif // UNDO_REDO_STACK_H