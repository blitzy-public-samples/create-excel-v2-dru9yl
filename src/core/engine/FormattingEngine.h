#ifndef FORMATTING_ENGINE_H
#define FORMATTING_ENGINE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

// Forward declarations
class ConditionalFormat;

// Enum for horizontal alignment
enum class HorizontalAlignment {
    Left,
    Center,
    Right,
    Justify
};

// Enum for vertical alignment
enum class VerticalAlignment {
    Top,
    Middle,
    Bottom
};

// Simple color class
class Color {
public:
    unsigned char r, g, b, a;
    Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255)
        : r(r), g(g), b(b), a(a) {}
};

// CellFormat class to represent the formatting properties of a cell
class CellFormat {
public:
    std::string fontName;
    double fontSize;
    bool isBold;
    bool isItalic;
    bool isUnderlined;
    Color textColor;
    Color backgroundColor;
    HorizontalAlignment horizontalAlignment;
    VerticalAlignment verticalAlignment;
    std::string numberFormat;

    // Constructor with default values
    CellFormat() : fontName("Arial"), fontSize(11), isBold(false), isItalic(false),
                   isUnderlined(false), textColor(0, 0, 0), backgroundColor(255, 255, 255),
                   horizontalAlignment(HorizontalAlignment::Left),
                   verticalAlignment(VerticalAlignment::Top), numberFormat("General") {}
};

// FormattingEngine class to manage cell formatting
class FormattingEngine {
private:
    std::unordered_map<std::string, CellFormat> cellFormats;
    mutable std::mutex formatMutex;

public:
    // Constructor
    FormattingEngine() = default;

    // Sets the format for a specific cell or range of cells
    void setCellFormat(const std::string& cellRange, const CellFormat& format);

    // Retrieves the format for a specific cell
    CellFormat getCellFormat(const std::string& cellReference) const;

    // Clears the format for a specific cell or range of cells
    void clearCellFormat(const std::string& cellRange);

    // Applies conditional formatting to a range of cells
    void applyConditionalFormatting(const std::string& cellRange, const ConditionalFormat& conditionalFormat);

    // Removes conditional formatting from a range of cells
    void removeConditionalFormatting(const std::string& cellRange);
};

// Human tasks:
// TODO: Add comprehensive documentation for each method, including usage examples and best practices
// TODO: Implement support for named styles that can be applied to multiple cells
// TODO: Add methods for bulk formatting operations to improve performance
// TODO: Implement a mechanism to export and import cell formats
// TODO: Add support for gradients and other advanced fill options in cell backgrounds
// TODO: Implement a notification system for format changes to update UI
// TODO: Add support for format inheritance (e.g., from column or row styles)

#endif // FORMATTING_ENGINE_H