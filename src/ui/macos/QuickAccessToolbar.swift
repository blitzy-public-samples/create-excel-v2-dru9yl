import AppKit
import ExcelCore

class QuickAccessToolbar: NSView {
    // MARK: - Properties
    private var stackView: NSStackView
    private var defaultItems: [ToolbarItem] = []
    private var customItems: [ToolbarItem] = []
    private var currentWorkbook: ExcelCore.Workbook

    // MARK: - Initialization
    init(workbook: ExcelCore.Workbook) {
        self.currentWorkbook = workbook
        self.stackView = NSStackView()
        
        super.init(frame: .zero)
        
        setupUI()
        loadDefaultItems()
        loadCustomItems()
        configureEventHandlers()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    // MARK: - UI Setup
    private func setupUI() {
        // Create and configure the NSStackView
        stackView = NSStackView(frame: bounds)
        stackView.orientation = .horizontal
        stackView.spacing = 4
        stackView.distribution = .fillEqually
        
        // Add default and custom items to the stackView
        for item in defaultItems + customItems {
            stackView.addArrangedSubview(item.button)
        }
        
        // Apply styling to match Excel's theme
        stackView.wantsLayer = true
        stackView.layer?.backgroundColor = NSColor.controlBackgroundColor.cgColor
        
        addSubview(stackView)
        
        // Set up constraints
        stackView.translatesAutoresizingMaskIntoConstraints = false
        NSLayoutConstraint.activate([
            stackView.topAnchor.constraint(equalTo: topAnchor),
            stackView.leadingAnchor.constraint(equalTo: leadingAnchor),
            stackView.trailingAnchor.constraint(equalTo: trailingAnchor),
            stackView.bottomAnchor.constraint(equalTo: bottomAnchor)
        ])
    }

    // MARK: - Loading Items
    private func loadDefaultItems() {
        // Create ToolbarItem instances for default actions
        let saveItem = ToolbarItem(title: "Save", image: NSImage(named: "save_icon"), action: #selector(handleItemClick(_:)))
        let undoItem = ToolbarItem(title: "Undo", image: NSImage(named: "undo_icon"), action: #selector(handleItemClick(_:)))
        let redoItem = ToolbarItem(title: "Redo", image: NSImage(named: "redo_icon"), action: #selector(handleItemClick(_:)))
        
        // Add default items to the defaultItems array
        defaultItems = [saveItem, undoItem, redoItem]
    }

    private func loadCustomItems() {
        // Retrieve custom toolbar items from user settings
        let userSettings = ExcelCore.UserSettings.shared
        let customItemsData = userSettings.customToolbarItems
        
        // Create ToolbarItem instances for each custom item
        for itemData in customItemsData {
            if let item = ToolbarItem(from: itemData) {
                customItems.append(item)
            }
        }
    }

    // MARK: - Event Handling
    private func configureEventHandlers() {
        for item in defaultItems + customItems {
            item.button.target = self
            item.button.action = #selector(handleItemClick(_:))
        }
    }

    @objc private func handleItemClick(_ sender: NSButton) {
        // Identify the clicked toolbar item
        guard let item = (defaultItems + customItems).first(where: { $0.button == sender }) else {
            return
        }
        
        // Execute the associated command
        do {
            try ExcelCore.Command.execute(item.command, workbook: currentWorkbook)
        } catch {
            // Handle error (e.g., show an alert)
            let alert = NSAlert()
            alert.messageText = "Error"
            alert.informativeText = "Failed to execute command: \(error.localizedDescription)"
            alert.alertStyle = .warning
            alert.addButton(withTitle: "OK")
            alert.runModal()
        }
        
        // Update the UI state if necessary
        updateUIState()
    }

    private func updateUIState() {
        // Update the state of toggleable items
        for item in defaultItems + customItems {
            if item.isToggleable {
                item.button.state = item.isEnabled ? .on : .off
            }
        }
    }

    // MARK: - Customization
    @objc private func customizeToolbar() {
        // Create and show the customization window
        let customizationWindow = QuickAccessToolbarCustomizationWindow(toolbar: self)
        customizationWindow.showWindow(nil)
    }
}

// MARK: - Human Tasks
/*
 TODO: Implement a customization dialog for adding/removing Quick Access Toolbar items
 TODO: Add support for keyboard shortcuts for Quick Access Toolbar items
 TODO: Implement proper state management for toggleable toolbar items
 TODO: Add support for grouping related toolbar items
 TODO: Implement tooltips with extended information for toolbar items
 TODO: Add support for displaying recently used commands in the Quick Access Toolbar
 TODO: Implement synchronization of Quick Access Toolbar settings across devices
 TODO: Add support for exporting and importing Quick Access Toolbar configurations
 TODO: Implement accessibility features for VoiceOver
 TODO: Add support for Touch Bar integration for Quick Access Toolbar items
 TODO: Ensure proper scaling of toolbar for different screen resolutions
 TODO: Implement customizable toolbar positioning (left/right alignment)
 TODO: Determine the most useful default items for the Quick Access Toolbar
 TODO: Implement localization for default item labels and tooltips
 TODO: Implement a user interface for managing custom Quick Access Toolbar items
 TODO: Ensure proper serialization and deserialization of custom toolbar items
 TODO: Implement proper error handling for failed command executions
 TODO: Add support for command parameters in toolbar items
 TODO: Design and implement a user-friendly customization interface
 TODO: Ensure proper handling of drag-and-drop operations for item reordering
*/