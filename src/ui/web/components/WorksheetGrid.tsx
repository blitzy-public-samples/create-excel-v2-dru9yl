import React, { useState, useEffect, useCallback, useMemo } from 'react';
import styled from 'styled-components';
import { ExcelCore } from 'ExcelCore';

// Styled components for the grid
const GridContainer = styled.div`
  position: relative;
  overflow: auto;
  width: 100%;
  height: 100%;
`;

const ColumnHeader = styled.div`
  position: sticky;
  top: 0;
  background-color: #f1f1f1;
  z-index: 1;
`;

const RowHeader = styled.div`
  position: sticky;
  left: 0;
  background-color: #f1f1f1;
  z-index: 1;
`;

const CellContainer = styled.div`
  position: absolute;
`;

// Cell component for rendering individual cells
const Cell: React.FC<{
  rowIndex: number;
  colIndex: number;
  value: string;
  onChange: (rowIndex: number, colIndex: number, value: string) => void;
}> = ({ rowIndex, colIndex, value, onChange }) => {
  const handleChange = (e: React.ChangeEvent<HTMLInputElement>) => {
    onChange(rowIndex, colIndex, e.target.value);
  };

  return (
    <input
      type="text"
      value={value}
      onChange={handleChange}
      style={{ width: '100%', height: '100%', border: '1px solid #ccc' }}
    />
  );
};

// WorksheetGrid component
export const WorksheetGrid: React.FC<{
  worksheet: ExcelCore.Worksheet;
  onCellChange: (rowIndex: number, colIndex: number, value: string) => void;
}> = ({ worksheet, onCellChange }) => {
  // State for visible cells
  const [visibleCells, setVisibleCells] = useState({
    startRow: 0,
    endRow: 0,
    startCol: 0,
    endCol: 0,
  });

  // Calculate visible cells based on scroll position and container size
  const calculateVisibleCells = useCallback(() => {
    const container = document.querySelector('.grid-container');
    if (!container) return;

    const { scrollTop, scrollLeft, clientHeight, clientWidth } = container;
    const rowHeight = 25; // Assuming fixed row height
    const colWidth = 100; // Assuming fixed column width

    const startRow = Math.floor(scrollTop / rowHeight);
    const endRow = Math.min(
      Math.ceil((scrollTop + clientHeight) / rowHeight),
      worksheet.rowCount
    );
    const startCol = Math.floor(scrollLeft / colWidth);
    const endCol = Math.min(
      Math.ceil((scrollLeft + clientWidth) / colWidth),
      worksheet.columnCount
    );

    setVisibleCells({ startRow, endRow, startCol, endCol });
  }, [worksheet]);

  // Effect to calculate visible cells on mount and window resize
  useEffect(() => {
    calculateVisibleCells();
    window.addEventListener('resize', calculateVisibleCells);
    return () => window.removeEventListener('resize', calculateVisibleCells);
  }, [calculateVisibleCells]);

  // Handle scroll event
  const handleScroll = useCallback(() => {
    calculateVisibleCells();
  }, [calculateVisibleCells]);

  // Handle cell change
  const handleCellChange = useCallback(
    (rowIndex: number, colIndex: number, value: string) => {
      worksheet.setCellValue(rowIndex, colIndex, value);
      onCellChange(rowIndex, colIndex, value);
    },
    [worksheet, onCellChange]
  );

  // Render visible cells
  const renderCells = useMemo(() => {
    const cells = [];
    for (let row = visibleCells.startRow; row <= visibleCells.endRow; row++) {
      for (let col = visibleCells.startCol; col <= visibleCells.endCol; col++) {
        const value = worksheet.getCellValue(row, col);
        cells.push(
          <CellContainer
            key={`${row}-${col}`}
            style={{
              top: `${row * 25}px`,
              left: `${col * 100}px`,
              width: '100px',
              height: '25px',
            }}
          >
            <Cell
              rowIndex={row}
              colIndex={col}
              value={value}
              onChange={handleCellChange}
            />
          </CellContainer>
        );
      }
    }
    return cells;
  }, [visibleCells, worksheet, handleCellChange]);

  // Render column headers
  const renderColumnHeaders = useMemo(() => {
    const headers = [];
    for (let col = visibleCells.startCol; col <= visibleCells.endCol; col++) {
      headers.push(
        <ColumnHeader
          key={`col-${col}`}
          style={{
            left: `${col * 100}px`,
            width: '100px',
            height: '25px',
          }}
        >
          {ExcelCore.columnIndexToLetter(col)}
        </ColumnHeader>
      );
    }
    return headers;
  }, [visibleCells]);

  // Render row headers
  const renderRowHeaders = useMemo(() => {
    const headers = [];
    for (let row = visibleCells.startRow; row <= visibleCells.endRow; row++) {
      headers.push(
        <RowHeader
          key={`row-${row}`}
          style={{
            top: `${row * 25}px`,
            width: '50px',
            height: '25px',
          }}
        >
          {row + 1}
        </RowHeader>
      );
    }
    return headers;
  }, [visibleCells]);

  return (
    <GridContainer className="grid-container" onScroll={handleScroll}>
      {renderColumnHeaders}
      {renderRowHeaders}
      {renderCells}
    </GridContainer>
  );
};

// Human tasks:
// - Implement virtualization for efficient rendering of large worksheets
// - Add support for cell selection and multi-cell selection
// - Implement cell editing functionality
// - Add support for copy, cut, and paste operations
// - Implement drag-and-drop functionality for cell ranges
// - Add support for freezing rows and columns
// - Implement auto-fill functionality for cells
// - Add support for inserting and deleting rows and columns
// - Implement cell comment functionality
// - Add support for conditional formatting
// - Implement data validation for cells
// - Add support for cell merging and splitting
// - Implement accessibility features for screen readers
// - Add support for touch devices and gesture-based interactions