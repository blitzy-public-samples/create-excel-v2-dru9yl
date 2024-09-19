import AppKit
import ExcelCore

class Sidebar: NSView {
    private var stackView: NSStackView
    private var currentWorkbook: ExcelCore.Workbook
    private var sidebarPanes: [String: NSView]
    
    init(workbook: ExcelCore.Workbook) {
        // Initialize properties
        self.currentWorkbook = workbook
        self.stackView = NSStackView()
        self.sidebarPanes = [:]
        
        // Call superclass initializer
        super.init(frame: NSRect.zero)
        
        // Set up the UI
        setupUI()
        
        // Create and add sidebar panes
        createSidebarPanes()
        
        // Set up the layout of the sidebar
        self.addSubview(stackView)
        stackView.translatesAutoresizingMaskIntoConstraints = false
        NSLayoutConstraint.activate([
            stackView.topAnchor.constraint(equalTo: self.topAnchor),
            stackView.leadingAnchor.constraint(equalTo: self.leadingAnchor),
            stackView.trailingAnchor.constraint(equalTo: self.trailingAnchor),
            stackView.bottomAnchor.constraint(equalTo: self.bottomAnchor)
        ])
        
        // Configure event handlers
        NotificationCenter.default.addObserver(self, selector: #selector(updateSidebar), name: NSNotification.Name("WorkbookStateChanged"), object: nil)
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    private func setupUI() {
        // Create and configure the NSStackView
        stackView = NSStackView(frame: self.bounds)
        stackView.orientation = .vertical
        stackView.spacing = 8
        stackView.distribution = .fillEqually
        
        // Apply styling to match Excel's theme
        self.wantsLayer = true
        self.layer?.backgroundColor = NSColor(calibratedWhite: 0.95, alpha: 1.0).cgColor
    }
    
    private func createSidebarPanes() {
        // Create instances of different sidebar panes
        let formatPane = FormatPane()
        let reviewPane = ReviewPane()
        // Add more panes as needed
        
        // Add created panes to the sidebarPanes dictionary
        sidebarPanes["Format"] = formatPane
        sidebarPanes["Review"] = reviewPane
        
        // Create toggle buttons for each pane
        for (paneName, pane) in sidebarPanes {
            let toggleButton = NSButton(title: paneName, target: self, action: #selector(paneToggleClicked(_:)))
            toggleButton.setButtonType(.toggle)
            stackView.addArrangedSubview(toggleButton)
        }
    }
    
    @objc private func paneToggleClicked(_ sender: NSButton) {
        toggleSidebarPane(paneName: sender.title)
    }
    
    private func toggleSidebarPane(paneName: String) {
        guard let pane = sidebarPanes[paneName] else { return }
        
        // If visible, hide the current pane
        if pane.isHidden == false {
            pane.isHidden = true
            return
        }
        
        // If hidden, show the requested pane and hide others
        for (name, view) in sidebarPanes {
            view.isHidden = (name != paneName)
        }
        
        // Update toggle button states
        for subview in stackView.arrangedSubviews {
            if let button = subview as? NSButton {
                button.state = button.title == paneName ? .on : .off
            }
        }
    }
    
    @objc private func updateSidebar() {
        guard let activeWorksheet = currentWorkbook.activeWorksheet,
              let selectedRange = activeWorksheet.selectedRange else { return }
        
        // Update each sidebar pane with current context
        for (_, pane) in sidebarPanes {
            if let updatable = pane as? SidebarPaneUpdatable {
                updatable.update(worksheet: activeWorksheet, selectedRange: selectedRange)
            }
        }
        
        // Refresh the visibility of panes based on current context
        // This is a placeholder for context-sensitive pane visibility logic
        // Implement your own logic here based on the current workbook state
    }
}

// Protocol for updatable sidebar panes
protocol SidebarPaneUpdatable {
    func update(worksheet: ExcelCore.Worksheet, selectedRange: ExcelCore.Range)
}

// MARK: - Human Tasks
/*
 TODO: Implement a customization feature for users to add/remove sidebar panes
 TODO: Add support for resizing the sidebar and individual panes
 TODO: Implement drag-and-drop functionality for rearranging sidebar panes
 TODO: Add support for collapsible sections within sidebar panes
 TODO: Implement a search functionality within the sidebar
 TODO: Add support for pinning frequently used panes
 TODO: Implement context-sensitive help for sidebar pane features
 TODO: Add support for custom user-created panes
 TODO: Implement accessibility features for VoiceOver
 TODO: Add support for Touch Bar integration for sidebar functionality
 */