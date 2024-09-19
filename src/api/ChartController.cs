using Microsoft.AspNetCore.Mvc;
using ExcelCore;
using System.Threading.Tasks;
using System;

// Human tasks:
// TODO: Implement authentication and authorization for all endpoints
// TODO: Add support for different chart types (e.g., bar, line, pie)
// TODO: Implement caching mechanisms for frequently accessed charts
// TODO: Add support for chart customization options (colors, fonts, legends)
// TODO: Implement versioning and change tracking for chart modifications
// TODO: Add support for exporting charts as images
// TODO: Implement rate limiting and throttling for API endpoints
// TODO: Add comprehensive error handling and logging
// TODO: Implement pagination for listing charts in a worksheet
// TODO: Add support for dynamic data ranges for charts

[ApiController]
[Route("api/[controller]")]
public class ChartController : Controller
{
    private readonly ChartService chartService;

    public ChartController(ChartService chartService)
    {
        // Initialize the chartService property with the provided service
        this.chartService = chartService;
    }

    [HttpGet("{workbookId}/{worksheetId}/{chartId}")]
    public async Task<IActionResult> GetChart(Guid workbookId, Guid worksheetId, Guid chartId)
    {
        // Call chartService.GetChartAsync with the provided workbookId, worksheetId, and chartId
        var chart = await chartService.GetChartAsync(workbookId, worksheetId, chartId);

        // If chart is found, return Ok result with the chart data
        if (chart != null)
        {
            return Ok(chart);
        }

        // If chart is not found, return NotFound result
        return NotFound();

        // TODO: Implement proper error handling for database errors
        // TODO: Add logging for chart retrieval attempts
    }

    [HttpPost("{workbookId}/{worksheetId}")]
    public async Task<IActionResult> CreateChart(Guid workbookId, Guid worksheetId, [FromBody] ChartCreateModel model)
    {
        // Validate the input model
        if (!ModelState.IsValid)
        {
            // If model is invalid, return BadRequest result
            return BadRequest(ModelState);
        }

        // Call chartService.CreateChartAsync with the workbookId, worksheetId, and model data
        var createdChart = await chartService.CreateChartAsync(workbookId, worksheetId, model);

        // Return CreatedAtAction result with the new chart data
        return CreatedAtAction(nameof(GetChart), new { workbookId, worksheetId, chartId = createdChart.Id }, createdChart);

        // TODO: Implement input validation for the ChartCreateModel
        // TODO: Add support for creating charts from templates
    }

    [HttpPut("{workbookId}/{worksheetId}/{chartId}")]
    public async Task<IActionResult> UpdateChart(Guid workbookId, Guid worksheetId, Guid chartId, [FromBody] ChartUpdateModel model)
    {
        // Validate the input model
        if (!ModelState.IsValid)
        {
            // If model is invalid, return BadRequest result
            return BadRequest(ModelState);
        }

        // Call chartService.UpdateChartAsync with the workbookId, worksheetId, chartId, and model data
        var result = await chartService.UpdateChartAsync(workbookId, worksheetId, chartId, model);

        // If update is successful, return NoContent result
        if (result)
        {
            return NoContent();
        }

        // If chart is not found, return NotFound result
        return NotFound();

        // TODO: Implement partial update support (PATCH) for chart properties
        // TODO: Add validation for chart data range changes
    }

    [HttpDelete("{workbookId}/{worksheetId}/{chartId}")]
    public async Task<IActionResult> DeleteChart(Guid workbookId, Guid worksheetId, Guid chartId)
    {
        // Call chartService.DeleteChartAsync with the provided workbookId, worksheetId, and chartId
        var result = await chartService.DeleteChartAsync(workbookId, worksheetId, chartId);

        // If deletion is successful, return NoContent result
        if (result)
        {
            return NoContent();
        }

        // If chart is not found, return NotFound result
        return NotFound();

        // TODO: Implement undo functionality for chart deletion
        // TODO: Add support for bulk chart deletion
    }
}