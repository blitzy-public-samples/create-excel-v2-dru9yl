import React, { useState, useEffect, useCallback } from 'react';
import styled from 'styled-components';
import { ExcelCore } from 'ExcelCore';

// Styled components for the status bar
const StatusBarContainer = styled.div`
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 4px 8px;
  background-color: #f3f3f3;
  border-top: 1px solid #d1d1d1;
`;

const StatusSection = styled.div`
  display: flex;
  align-items: center;
`;

const StatusItem = styled.span`
  margin-right: 12px;
  font-size: 12px;
  color: #333;
`;

const ZoomControl = styled.input`
  width: 60px;
`;

interface StatusBarProps {
  workbook: ExcelCore.Workbook;
  onZoomChange: (newZoom: number) => void;
}

const StatusBar: React.FC<StatusBarProps> = ({ workbook, onZoomChange }) => {
  const [cellInfo, setCellInfo] = useState('');
  const [sheetInfo, setSheetInfo] = useState('');
  const [zoomLevel, setZoomLevel] = useState(100);

  // Function to update status bar information
  const updateStatusInfo = useCallback(() => {
    if (workbook) {
      const activeCell = workbook.getActiveCell();
      const activeSheet = workbook.getActiveSheet();
      
      if (activeCell) {
        setCellInfo(`${activeCell.getAddress()}: ${activeCell.getValue()}`);
      }
      
      if (activeSheet) {
        setSheetInfo(`Sheet: ${activeSheet.getName()}`);
      }
    }
  }, [workbook]);

  // Update status info when workbook changes
  useEffect(() => {
    updateStatusInfo();
    // Set up listener for workbook changes
    const unsubscribe = workbook.on('change', updateStatusInfo);
    return () => unsubscribe();
  }, [workbook, updateStatusInfo]);

  // Handle zoom level changes
  const handleZoomChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const newZoom = parseInt(event.target.value, 10);
    setZoomLevel(newZoom);
    onZoomChange(newZoom);
  };

  return (
    <StatusBarContainer>
      <StatusSection>
        <StatusItem>{cellInfo}</StatusItem>
        <StatusItem>{sheetInfo}</StatusItem>
      </StatusSection>
      <StatusSection>
        <StatusItem>
          Zoom: 
          <ZoomControl
            type="number"
            min="10"
            max="400"
            value={zoomLevel}
            onChange={handleZoomChange}
          />
          %
        </StatusItem>
      </StatusSection>
    </StatusBarContainer>
  );
};

export default StatusBar;

// Human tasks:
// TODO: Implement accessibility features for screen readers
// TODO: Add support for customizable status bar items
// TODO: Implement context menu for status bar items
// TODO: Add support for displaying file size and last saved time
// TODO: Implement status indicators for network connectivity and sync status
// TODO: Add support for displaying calculation mode (auto/manual)
// TODO: Implement quick actions for common tasks (e.g., sheet navigation)
// TODO: Add support for displaying extension-specific status information
// TODO: Implement localization for status bar text
// TODO: Add unit and integration tests for the StatusBar component
// TODO: Implement real-time updates for cell and sheet information
// TODO: Add support for displaying macro recording status
// TODO: Implement custom status items based on user preferences
// TODO: Implement efficient update mechanism to avoid unnecessary re-renders
// TODO: Add error handling for cases where workbook data is unavailable
// TODO: Implement smooth transitions for zoom level changes
// TODO: Add validation for minimum and maximum zoom levels