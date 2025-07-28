/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* GraphicsAgainUtilities.h :                                                 */
/* Paul O'Brien                                                               */
/* 24.07.25                                                                   */
/*                                                                            */
/* Radio test-harness support utilities                                       */
/******************************************************************************/

#ifndef GRAPHICS_UTILITIES_H_
#define GRAPHICS_UTILITIES_H_

/******************************************************************************/

#include "StrokeGraphicsTypes.h"

/******************************************************************************/

// Analagous to the stroke cell line types
typedef enum graphLinePointMarkers_tTag
  {
  GRAPH_LINE_POINT_MARKER_NONE = 0,
  GRAPH_LINE_POINT_MARKER_LINE_START,
  GRAPH_LINE_POINT_MARKER_LINE_END,
  GRAPH_LINE_POINT_MARKER_LINE_MID,
  GRAPH_LINE_POINT_MARKER_FIRST_LINE_START,
  GRAPH_LINE_POINT_MARKER_LAST_LINE_START,
  GRAPH_LINE_POINT_MARKERS
  } graphLinePointMarkers_t, *graphLinePointMarkers_tPtr;

typedef enum lineSegmentMode_tTag
  {
  LINE_SEGMENT_MODE_NONE = 0,
  LINE_SEGMENT_MODE_PASSIVE,
  LINE_SEGMENT_MODE_ACTIVE,
  LINE_SEGMENT_MODE_EDIT,
  LINE_SEGMENT_MODES
  } lineSegmentMode_t, *lineSegmentMode_tPtr;

#define CURSOR_LINE_WIDTH   ((GRAPHICS_REAL)3)
#define STROKE_LINE_WIDTH   ((GRAPHICS_REAL)5)

#pragma pack(push,1)
typedef struct strokeFrameCoordinates_tTag
  {
  GRAPHICS_REAL frameLeftX;
  GRAPHICS_REAL frameTopY;
  GRAPHICS_REAL frameRightX;
  GRAPHICS_REAL frameBottomY;
  } strokeFrameCoordinates_t, *strokeFrameCoordinates_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct objectColour_tTag
  {
  BYTE opacity;
  BYTE red;
  BYTE green;
  BYTE blue;
  } objectColour_t, *objectColour_tPtr;
#pragma pack(pop)

typedef enum strokeCellPointType_tTag
  {
  STROKE_CELL_POINT_TYPE_NONE = 0,
  STROKE_CELL_POINT_TYPE_LINE_START,
  STROKE_CELL_POINT_TYPE_LINE_END,
  STROKE_CELL_POINT_TYPE_LINE_MID,
  STROKE_CELL_POINT_TYPE_FIRST_LINE_START,
  STROKE_CELL_POINT_TYPE_LAST_LINE_START,
  STROKE_CELL_POINT_TYPES
  } strokeCellPointType_t, *strokeCellPointType_tPtr;

// Stroke cell points SHALL be stored as row-wise i.e. { p(0,0) .. p(0,[M-1]) } .. { p([N-1],0) .. p([N-1],[M-1]) }
#pragma pack(push,1)
typedef struct strokeCellPoint_tTag
  {
  GRAPHICS_REAL          xAxisPoint;
  GRAPHICS_REAL          yAxisPoint;
  strokeCellPointType_t  pointType;
  strokeCellPoint_tTag  *nextStrokeCellPoint;
  strokeCellPoint_tTag  *lastStrokeCellPoint;
  } strokeCellPoint_t, *strokeCellPoint_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct strokeCell_tTag
  {
  strokeCellPoint_tPtr  strokeCellPoint;
  GRAPHICS_REAL         strokeXAxisPoints; // the number of stroke lines in 'x'
  GRAPHICS_REAL         strokeYAxisPoints; // the number of stroke lines in 'y'
  strokeCell_tTag      *nextStrokeCell;
  strokeCell_tTag      *lastStrokeCell;
  } strokeCell_t, *strokeCell_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct strokeFrame_tTag
  {
  const strokeFrameCoordinates_t frameCoordinates;  // the frame as drawn on the canvas in x- and y-axis multipliers from 0.0..1.0
        objectColour_t           strokeColour;
        GRAPHICS_UINT            strokeColourCode;
        objectColour_t           middleColour;
        GRAPHICS_UINT            middleColourCode;
        objectColour_t           descentColour;
        GRAPHICS_UINT            descentColourCode;
        strokeCell_tPtr          strokeCellArray;
        GRAPHICS_REAL            strokeXAxisPoints; // the number of stroke lines in 'x'
        GRAPHICS_REAL            strokeYAxisPoints; // the number of stroke lines in 'y'
  } strokeFrame_t, *strokeFrame_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct strokeGraphLinePoint_tTag
  {
  GRAPHICS_REAL              pointX;
  GRAPHICS_REAL              pointY;
  graphLinePointMarkers_t    pointMarker;
  strokeGraphLinePoint_tTag *nextStrokeGraphLine;
  strokeGraphLinePoint_tTag *lastStrokeGraphLine;
  } strokeGraphLinePoint_t, *strokeGraphLinePoint_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct strokeGraphPointBase_tTag
  {
  GRAPHICS_UINT             graphRowNumber;
  GRAPHICS_UINT             graphColumnNumber;
  strokeGraphLinePoint_tPtr graphPoints;
  objectColour_t            activeLineStrokeColour;
  objectColour_t            passiveLineStrokeColour;
  objectColour_t            editLineStrokeColour;
  GRAPHICS_REAL             lineWidth;
  } strokeGraphPointBase_t, *strokeGraphPointBase_tPtr;
#pragma pack(pop)

/******************************************************************************/

extern strokeFrame_t characterFrame;

/******************************************************************************/

extern graphicsError_t parseIpAddress(GRAPHICS_CHAR_PTR *ipAddress,
                                      GRAPHICS_UINT_PTR  ipAddressLength,
                                      GRAPHICS_CHAR_PTR  commandLineArgument);

extern graphicsError_t parsePortNumber(GRAPHICS_CHAR_PTR *portNumber,
                                       GRAPHICS_UINT_PTR  portNumberLength,
                                       GRAPHICS_CHAR_PTR  commandLineArgument);

/******************************************************************************/

#endif

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
