import React, { useState, useEffect, useCallback } from 'react';
import styled from 'styled-components';
import { ExcelCore } from 'ExcelCore';
import SidebarPane from './SidebarPane';

// Styled components for the sidebar
const SidebarContainer = styled.div`
  width: 300px;
  height: 100%;
  background-color: #f3f3f3;
  border-left: 1px solid #d1d1d1;
`;

const PaneSelector = styled.div`
  display: flex;
  justify-content: space-around;
  padding: 10px 0;
  border-bottom: 1px solid #d1d1d1;
`;

const PaneButton = styled.button<{ active: boolean }>`
  background-color: ${props => props.active ? '#ffffff' : 'transparent'};
  border: none;
  padding: 5px 10px;
  cursor: pointer;
  font-weight: ${props => props.active ? 'bold' : 'normal'};
`;

interface SidebarProps {
  workbook: ExcelCore.Workbook;
  activePaneId: string;
  onPaneChange: (paneId: string) => void;
}

const Sidebar: React.FC<SidebarProps> = ({ workbook, activePaneId, onPaneChange }) => {
  const [availablePanes, setAvailablePanes] = useState<string[]>([]);

  // Load available panes based on workbook context
  const loadAvailablePanes = useCallback(() => {
    // TODO: Implement logic to determine relevant panes based on workbook content
    const panes = ['formatting', 'dataValidation', 'review']; // Example panes
    setAvailablePanes(panes);
  }, [workbook]);

  useEffect(() => {
    loadAvailablePanes();
  }, [loadAvailablePanes]);

  // Handle pane change
  const handlePaneChange = (paneId: string) => {
    onPaneChange(paneId);
  };

  return (
    <SidebarContainer>
      <PaneSelector>
        {availablePanes.map(paneId => (
          <PaneButton
            key={paneId}
            active={paneId === activePaneId}
            onClick={() => handlePaneChange(paneId)}
          >
            {paneId}
          </PaneButton>
        ))}
      </PaneSelector>
      <SidebarPane paneId={activePaneId} workbook={workbook} />
    </SidebarContainer>
  );
};

export default Sidebar;

// Human tasks:
// TODO: Implement smooth transitions when switching between panes
// TODO: Add support for resizable sidebar width
// TODO: Implement keyboard navigation for sidebar panes
// TODO: Implement drag-and-drop functionality for rearranging sidebar panes
// TODO: Add support for collapsible sections within panes
// TODO: Implement a search functionality within the sidebar
// TODO: Add support for pinning frequently used panes
// TODO: Implement context-sensitive help for sidebar features
// TODO: Add support for custom user-created panes
// TODO: Implement accessibility features for screen readers
// TODO: Add support for theming and custom color schemes in the sidebar
// TODO: Implement performance optimizations for rendering large pane contents
// TODO: Add unit and integration tests for the Sidebar and SidebarPane components