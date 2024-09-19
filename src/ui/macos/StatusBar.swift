import AppKit
import ExcelCore

class StatusBar: NSView {
    // MARK: - Properties
    private var cellInfoLabel: NSTextField
    private var sheetInfoLabel: NSTextField
    private var calculationModeLabel: NSTextField
    private var progressIndicator: NSProgressIndicator
    private var currentWorkbook: ExcelCore.Workbook

    // MARK: - Initialization
    init(workbook: ExcelCore.Workbook) {
        self.currentWorkbook = workbook
        
        // Initialize properties
        self.cellInfoLabel = NSTextField()
        self.sheetInfoLabel = NSTextField()
        self.calculationModeLabel = NSTextField()
        self.progressIndicator = NSProgressIndicator()
        
        // Call super.init with a zero frame, we'll set up constraints later
        super.init(frame: .zero)
        
        setupUI()
        configureEventHandlers()
    }
    
    required init?(coder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

    // MARK: - UI Setup
    private func setupUI() {
        // Create and configure cellInfoLabel
        cellInfoLabel.isEditable = false
        cellInfoLabel.isBordered = false
        cellInfoLabel.backgroundColor = .clear
        addSubview(cellInfoLabel)

        // Create and configure sheetInfoLabel
        sheetInfoLabel.isEditable = false
        sheetInfoLabel.isBordered = false
        sheetInfoLabel.backgroundColor = .clear
        addSubview(sheetInfoLabel)

        // Create and configure calculationModeLabel
        calculationModeLabel.isEditable = false
        calculationModeLabel.isBordered = false
        calculationModeLabel.backgroundColor = .clear
        addSubview(calculationModeLabel)

        // Create and configure progressIndicator
        progressIndicator.style = .bar
        progressIndicator.isIndeterminate = false
        progressIndicator.isHidden = true
        addSubview(progressIndicator)

        // Set up Auto Layout constraints
        cellInfoLabel.translatesAutoresizingMaskIntoConstraints = false
        sheetInfoLabel.translatesAutoresizingMaskIntoConstraints = false
        calculationModeLabel.translatesAutoresizingMaskIntoConstraints = false
        progressIndicator.translatesAutoresizingMaskIntoConstraints = false

        NSLayoutConstraint.activate([
            cellInfoLabel.leadingAnchor.constraint(equalTo: leadingAnchor, constant: 8),
            cellInfoLabel.centerYAnchor.constraint(equalTo: centerYAnchor),

            sheetInfoLabel.centerXAnchor.constraint(equalTo: centerXAnchor),
            sheetInfoLabel.centerYAnchor.constraint(equalTo: centerYAnchor),

            calculationModeLabel.trailingAnchor.constraint(equalTo: trailingAnchor, constant: -8),
            calculationModeLabel.centerYAnchor.constraint(equalTo: centerYAnchor),

            progressIndicator.leadingAnchor.constraint(equalTo: leadingAnchor),
            progressIndicator.trailingAnchor.constraint(equalTo: trailingAnchor),
            progressIndicator.bottomAnchor.constraint(equalTo: bottomAnchor),
            progressIndicator.heightAnchor.constraint(equalToConstant: 2)
        ])

        // Apply styling to match Excel's theme
        applyExcelThemeStyling()
    }

    private func applyExcelThemeStyling() {
        // Apply Excel-like styling (customize as needed)
        backgroundColor = NSColor(calibratedWhite: 0.95, alpha: 1.0)
        cellInfoLabel.textColor = .darkGray
        sheetInfoLabel.textColor = .darkGray
        calculationModeLabel.textColor = .darkGray
    }

    private func configureEventHandlers() {
        // Add event handlers for workbook changes, active sheet changes, etc.
        // This is a placeholder and should be implemented based on ExcelCore's API
    }

    // MARK: - Status Bar Updates
    func updateCellInfo() {
        guard let activeWorksheet = currentWorkbook.activeWorksheet,
              let activeCell = activeWorksheet.activeCell else {
            cellInfoLabel.stringValue = ""
            return
        }

        let cellAddress = activeCell.address
        let cellValue = activeCell.value
        let cellFormula = activeCell.formula

        cellInfoLabel.stringValue = "\(cellAddress): \(cellValue) (\(cellFormula))"
    }

    func updateSheetInfo() {
        guard let activeWorksheet = currentWorkbook.activeWorksheet else {
            sheetInfoLabel.stringValue = ""
            return
        }

        let sheetName = activeWorksheet.name
        let sheetIndex = currentWorkbook.worksheets.firstIndex(of: activeWorksheet) ?? 0
        let totalSheets = currentWorkbook.worksheets.count

        sheetInfoLabel.stringValue = "Sheet \(sheetIndex + 1) of \(totalSheets): \(sheetName)"
    }

    func updateCalculationMode() {
        let calculationMode = currentWorkbook.calculationMode

        switch calculationMode {
        case .automatic:
            calculationModeLabel.stringValue = "Auto"
        case .manual:
            calculationModeLabel.stringValue = "Manual"
        case .automaticExceptTables:
            calculationModeLabel.stringValue = "Auto (Except Tables)"
        }
    }

    func showProgress(progress: Double) {
        progressIndicator.isHidden = false
        progressIndicator.doubleValue = progress

        // Update layout to accommodate progress indicator
        NSLayoutConstraint.activate([
            heightAnchor.constraint(equalToConstant: 24) // Increase height to show progress indicator
        ])
    }
}

// MARK: - Human Tasks
/*
 TODO: Implement the following tasks:
 1. Ensure proper scaling of status bar for different screen resolutions
 2. Implement customizable status bar layout
 3. Implement support for displaying multi-cell selection information
 4. Add option to customize displayed cell information
 5. Add support for displaying custom sheet properties
 6. Implement a dropdown menu for quick sheet navigation
 7. Implement a clickable element to toggle calculation modes
 8. Add visual indicators for different calculation modes
 9. Implement smooth progress indicator animations
 10. Add support for displaying progress text alongside the indicator
 11. Implement a customization dialog for status bar content and layout
 12. Add support for displaying macro recording status in the status bar
 13. Implement zoom control functionality in the status bar
 14. Add support for displaying network status for cloud-connected workbooks
 15. Implement a notification system for displaying temporary messages in the status bar
 16. Add support for displaying file size and last saved time in the status bar
 17. Implement accessibility features for VoiceOver
 18. Add support for displaying keyboard shortcuts based on current context
 19. Implement a mini chart view in the status bar for selected data
 20. Add support for Touch Bar integration for status bar functionality
 */