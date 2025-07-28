/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* GraphicsUtilities.cpp :                                                    */
/* Paul O'Brien                                                               */
/* 24.07.25                                                                   */
/*                                                                            */
/* Radio test-harness support utilities                                       */
/******************************************************************************/

#include <Windows.h>
#include "framework.h"
#include <objidl.h>
#include <gdiplus.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include "GraphicsAgainUtilities.h"
#include "GraphicsAgainCaption.h"
#include "GraphicsAgain001.h"

/******************************************************************************/

#include "GraphicsAgain001.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/

// The screen coordinates are initially set to full HD
globalScreenCoordinates_t globalScreenCoordinates             = {
                                                                GUI_SCREEN_GUI_DEFAULT_LEFT_X,
                                                                GUI_SCREEN_GUI_DEFAULT_TOP_Y,
                                                                GUI_SCREEN_GUI_DEFAULT_RIGHT_X,
                                                                GUI_SCREEN_GUI_DEFAULT_BOTTOM_Y
                                                                };
                                                              
canvasDescriptor_t        canvasSize                          = { CANVAS_SIZE_ZERO_X, CANVAS_SIZE_ZERO_Y, CANVAS_SIZE_WIDTH, CANVAS_SIZE_HEIGHT };
                                                              
guiObjectHoldingRing_tPtr guiObjectHoldingRingBaseLink        = nullptr,
                          guiObjectHoldingRingCurrent         = nullptr,
                          guiObjectHoldingRingFreePtr         = nullptr; 

// Define the first rectangle
#pragma pack(push,1)                                          
rectangleObject_tPtr      rectangleObjectOne                  = nullptr;
#pragma pack(pop)                                           
                                                            
rectangleDimensions_t     rectangleObjectOneDimensions        = {
                                                                RECTANGLE_OBJECT_ONE_LEFT_X,  
                                                                RECTANGLE_OBJECT_ONE_TOP_Y,   
                                                                RECTANGLE_OBJECT_ONE_RIGHT_X, 
                                                                RECTANGLE_OBJECT_ONE_BOTTOM_Y,
                                                                RECTANGLE_OBJECT_ONE_LINEWIDTH
                                                                };
objectColour_t            rectangleObjectOneColour            = {
                                                                BLUE_PEN
                                                                };
graphicsActiveObject_t    rectangleObjectOneActiveBehaviour   = { 
                                                                GRAPHICS_ACTIVE_TYPE_DYNAMIC,
                                                                { rectangleObjectOneDimensions },
                                                                rectangleObjectOneHandler 
                                                                };

// Define the first rectangle text...
#pragma pack(push,1)
staticTextObject_tPtr     rectangleOneText                    = nullptr;
#pragma pack(pop) 

rectangleDimensions_t     rectangleOneTextDimensions          = { // Use the same dimensions as the enclosing rectangle
                                                                RECTANGLE_OBJECT_ONE_LEFT_X,  
                                                                RECTANGLE_OBJECT_ONE_TOP_Y,   
                                                                RECTANGLE_OBJECT_ONE_RIGHT_X, 
                                                                RECTANGLE_OBJECT_ONE_BOTTOM_Y,
                                                                RECTANGLE_OBJECT_ONE_LINEWIDTH
                                                                };
objectColour_t            rectangleOneTextColour              = { // Use the same colour as the enclosing rectangle
                                                                BLUE_PEN
                                                                };
graphicsActiveObject_t    rectangleOneTextActiveBehaviour     = { 
                                                                GRAPHICS_ACTIVE_TYPE_STATIC,
                                                                { rectangleOneTextDimensions },
                                                                rectangleObjectOneHandler // use the same handling function
                                                                };

// Define the second rectangle...
#pragma pack(push,1)
rectangleObject_tPtr      rectangleObjectTwo                  = nullptr;
#pragma pack(pop)                                             
                                                              
rectangleDimensions_t     rectangleObjectTwoDimensions        = {
                                                                RECTANGLE_OBJECT_TWO_LEFT_X,
                                                                RECTANGLE_OBJECT_TWO_TOP_Y,
                                                                RECTANGLE_OBJECT_TWO_RIGHT_X,
                                                                RECTANGLE_OBJECT_TWO_BOTTOM_Y,
                                                                RECTANGLE_OBJECT_TWO_LINEWIDTH
                                                                };
objectColour_t            rectangleObjectTwoColour            = {
                                                                GREEN_PEN
                                                                };
graphicsActiveObject_t    rectangleObjectTwoActiveBehaviour   = { 
                                                                GRAPHICS_ACTIVE_TYPE_DYNAMIC,
                                                                { rectangleObjectTwoDimensions },
                                                                rectangleObjectTwoHandler
                                                                };
                                                              
#pragma pack(push,1)                                          
rectangleObject_tPtr      rectangleObjectThree                = nullptr;
#pragma pack(pop)                                             
                                                              
rectangleDimensions_t     rectangleObjectThreeDimensions      = {
                                                                RECTANGLE_OBJECT_THREE_LEFT_X,
                                                                RECTANGLE_OBJECT_THREE_TOP_Y,
                                                                RECTANGLE_OBJECT_THREE_RIGHT_X,
                                                                RECTANGLE_OBJECT_THREE_BOTTOM_Y,
                                                                RECTANGLE_OBJECT_THREE_LINEWIDTH
                                                                };
objectColour_t            rectangleObjectThreeColour          = {
                                                                CYAN_PEN
                                                                };
graphicsActiveObject_t    rectangleObjectThreeActiveBehaviour = { 
                                                                GRAPHICS_ACTIVE_TYPE_DYNAMIC,
                                                                { rectangleObjectThreeDimensions },
                                                                rectangleObjectThreeHandler
                                                                };

WCHAR objectOutput[GRAPHICS_RING_OUTPUT_WSTR_LENGTH]        = { L"" };
WCHAR captionPanel[GRAPHICS_RING_OUTPUT_WSTR_LENGTH]        = { GRAPHICS_OBJECT_CAPTION_WSTR };

strokeFrame_t  characterFrame                               = { 
                                                                {
                                                                STROKE_FRAME_LEFT_X,
                                                                STROKE_FRAME_TOP_Y,
                                                                STROKE_FRAME_RIGHT_X,
                                                                STROKE_FRAME_BOTTOM_Y 
                                                                },
                                                                {
                                                                STROKE_FRAME_COLOUR
                                                                },
                                                                STROKE_FRAME_COLOUR_CODE,
                                                                {
                                                                MIDDLE_FRAME_COLOUR,
                                                                },
                                                                MIDDLE_FRAME_COLOUR_CODE,
                                                                {
                                                                DESCENT_FRAME_COLOUR
                                                                },
                                                                DESCENT_FRAME_COLOUR_CODE,
                                                              nullptr,
                                                              STROKE_FRAME_X_AXIS_POINTS,
                                                              STROKE_FRAME_Y_AXIS_POINTS
                                                              };

bool           mouseOverObject                              = false; // flag when the mouse pointer is in an objects' perimeter
bool           objectPositionTestFlag                       = false; // flag the position test is enabled for a single traverse 
                                                                     // of the holding ring
GRAPHICS_SHORT mouseXPosition                               = ((GRAPHICS_SHORT)0);
GRAPHICS_SHORT mouseYPosition                               = ((GRAPHICS_SHORT)0);

/******************************************************************************/
/* parseIpAddress() :                                                         */
/* - parse an IP4 address in decimal and 'dot' format :                       */
/*   minimum legal address length : "0.0.0.0"                                 */
/*   maximum legal address length : "000.000.000.000"                         */
/******************************************************************************/

graphicsError_t parseIpAddress(GRAPHICS_CHAR_PTR *ipAddress,
                               GRAPHICS_UINT_PTR  ipAddressLength,
                               GRAPHICS_CHAR_PTR  commandLineArgument)
  {
/******************************************************************************/

  GRAPHICS_CHAR_PTR ipAddressT    = nullptr;
                                  
  GRAPHICS_CHAR     fieldBreak    = GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELD_BREAK_CHAR;

  GRAPHICS_UINT     subFieldIndex = ((GRAPHICS_UINT)0),
                    breakIndex    = ((GRAPHICS_UINT)0);

  graphicsError_t   objectError   = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((ipAddress != nullptr) && (ipAddressLength != nullptr) && (commandLineArgument != nullptr))
    {
    // If the 'IP-Address', 'Port' and 'AlphabetFile' arguments are legal, convert 
    // them to their native representations
    *ipAddressLength = (GRAPHICS_UINT)strlen(commandLineArgument); // find the end of the command-line 'IP-Address' parameter
    
    if (*ipAddressLength != ((GRAPHICS_UINT)0))
      {
      // Test the putative IP address for length and legal sub-fields
      if ((*ipAddressLength >= GRAPHICS_SERVER_IP_ADDRESS_MINIMUM_LENGTH) && (*ipAddressLength <= GRAPHICS_SERVER_IP_ADDRESS_MAXIMUM_LENGTH))
        {
        ipAddressT = commandLineArgument;
        
        for (breakIndex = ((GRAPHICS_UINT)0); breakIndex < GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELD_NUMBER; breakIndex++)
          {
          for (subFieldIndex = ((GRAPHICS_UINT)0); subFieldIndex < GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELDS_MAXIMUM_LENGTH; subFieldIndex++)
            {
            if (isdigit(*ipAddressT))
              {
              // Parsing 1 { decimal } 3
              ipAddressT = ipAddressT + 1;
              }
            else
              {
              break;
              }
            }
        
          // Test for at least one decimal before the next '.'
          if ((subFieldIndex != ((GRAPHICS_UINT)0)) && (((GRAPHICS_CHAR)*ipAddressT) == fieldBreak))
            {
            ipAddressT = ipAddressT + 1;
            }
          else
            {
            // Test for the end of the ip address string
            if  ((subFieldIndex != ((GRAPHICS_UINT)0)) && (breakIndex == GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELD_NUMBER - 1) && (((GRAPHICS_CHAR)*ipAddressT) == GRAPHICS_CHAR_EOS))
              {
              *ipAddress = commandLineArgument;
              }
            else
              {
              objectError = GRAPHICS_OBJECT_CHARACTER_ERROR;
              break;
              }
            }
          }
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of parseIpAddress                                                  */

/******************************************************************************/
/* parsePortNumber() :                                                        */
/******************************************************************************/

graphicsError_t parsePortNumber(GRAPHICS_CHAR_PTR *portNumber,
                                GRAPHICS_UINT_PTR  portNumberLength,
                                GRAPHICS_CHAR_PTR  commandLineArgument)
  {
/******************************************************************************/

  GRAPHICS_CHAR_PTR port        = nullptr;

  GRAPHICS_UINT     portIndex   = ((GRAPHICS_UINT)0);
                    
  graphicsError_t   objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((portNumber != nullptr) && (portNumberLength != nullptr) && (commandLineArgument != nullptr))
    {
    *portNumberLength = (GRAPHICS_UINT)strlen(commandLineArgument); // find the end of the command-line 'IP-Address' parameter

    if ((*portNumberLength >= GRAPHICS_SERVER_PORT_NUMBER_MINIMUM_LENGTH) && (*portNumberLength <= GRAPHICS_SERVER_PORT_NUMBER_MAXIMUM_LENGTH))
      {
      port = commandLineArgument;

      for (portIndex = ((GRAPHICS_UINT)0); portIndex <= GRAPHICS_SERVER_PORT_NUMBER_MAXIMUM_LENGTH; portIndex++)
        {
        if (isdigit(*port))
          {
          port = port + 1;
          }
        else
          {
          // Test for the end of the ip address string
          if ((portIndex != ((GRAPHICS_UINT)0)) && (((GRAPHICS_CHAR)*port) == GRAPHICS_CHAR_EOS))
            {
            *portNumber = commandLineArgument;
             break;
            }
          else
            {
            objectError = GRAPHICS_OBJECT_CHARACTER_ERROR;
            break;
            }
          }
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* parsePortNumber                                                        */

/******************************************************************************/
/* setCanvasCoordinates() :                                                   */
/******************************************************************************/

graphicsError_t setCanvasCoordinates(canvasDescriptor_t *canvasSize,
                                     GRAPHICS_REAL       left,
                                     GRAPHICS_REAL       top,
                                     GRAPHICS_REAL       right,
                                     GRAPHICS_REAL       bottom,
                                     GRAPHICS_REAL       width,
                                     GRAPHICS_REAL       height)
  {
/******************************************************************************/

  graphicsError_t canvasError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (canvasSize !=nullptr)
    {
    canvasSize->left    = left;
    canvasSize->right   = right;
    canvasSize->top     = top;
    canvasSize->bottom  = bottom;
   
    canvasSize->width   = width;
    canvasSize->centreX = (GRAPHICS_REAL)ceil((double)(width / ((GRAPHICS_REAL)2.0)));
   
    canvasSize->height  = height;
    canvasSize->centreY = (GRAPHICS_REAL)ceil((double)(height / ((GRAPHICS_REAL)2.0)));
    }
  else
    {
    canvasError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(canvasError);

/******************************************************************************/
  } /* end of setCanvasCoordinates                                            */

/******************************************************************************/
/* buildCharacterStrokeGraph() :                                              */
/* - create a grid of { 0..n, 0..m} points to describe the matrix of points/  */
/*   lines that will be drawn as the character-creation grid                  */
/*   NOTE : the row-/column-numbers are INCLUSIVE i.e. 0..N = (N + 1) rows/   */
/*          columns                                                           */
/******************************************************************************/

graphicsError_t buildCharacterStrokeGraph(strokeFrame_tPtr          characterFrame,
                                          strokeGraphPointBase_tPtr strokeGraphBase)
    {
/******************************************************************************/

  GRAPHICS_UINT             graphicLineIndexX                   = ((GRAPHICS_UINT)0),
                            graphicLineIndexY                   = ((GRAPHICS_UINT)0),
                            graphicLineLengthX                  = ((GRAPHICS_UINT)0),
                            graphicLineLengthY                  = ((GRAPHICS_UINT)0);
                           
  GRAPHICS_REAL             offsetDeltaX                        = ((GRAPHICS_REAL)0.0),
                            offsetDeltaY                        = ((GRAPHICS_REAL)0.0),
                            pageOffsetX                         = ((GRAPHICS_REAL)0.0),
                            pageOffsetY                         = ((GRAPHICS_REAL)0.0);

  GRAPHICS_UCHAR            characterCode[GRAPHICS_CHAR_LENGTH] = { ((GRAPHICS_UCHAR)ALPHABET_SIZE) };
  size_t                    characterCodeLength                 = ((size_t)0);
  errno_t                   characterError                      = ((errno_t)0);

  strokeGraphLinePoint_tPtr point                               = nullptr,
                            lastPoint                           = nullptr;

  graphicsError_t           objectError                         = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((characterFrame != nullptr) && (strokeGraphBase != nullptr) && (strokeGraphBase->graphPoints == nullptr))
    { // First, construct the points array that describes the character frame
      // How many points in the x- and y-axes ?
      if (((graphicLineLengthX = ((GRAPHICS_UINT)ceil(characterFrame->strokeXAxisPoints))) >= STROKE_FRAME_X_MINIMUM_POINTS_UINT) &&
          ((graphicLineLengthY = ((GRAPHICS_UINT)ceil(characterFrame->strokeYAxisPoints))) >= STROKE_FRAME_Y_MINIMUM_POINTS_UINT))
        {
        // Force the grid dimensions { x, y } to be even
        if ((graphicLineLengthX & STROKE_FRAME_DETECT_ODD_EVEN) == STROKE_FRAME_DETECT_ODD_EVEN)
          {
          graphicLineLengthX = graphicLineLengthX + ((GRAPHICS_UINT)1);
          }

        if ((graphicLineLengthY & STROKE_FRAME_DETECT_ODD_EVEN) == STROKE_FRAME_DETECT_ODD_EVEN)
          {
          graphicLineLengthY = graphicLineLengthY + ((GRAPHICS_UINT)1);
          }

        // The grid has a normalised page-plotting range that is added to each point coordinate
        offsetDeltaX = characterFrame->frameCoordinates.frameRightX  - characterFrame->frameCoordinates.frameLeftX;
        offsetDeltaY = characterFrame->frameCoordinates.frameBottomY - characterFrame->frameCoordinates.frameTopY;
        
        // Instantiate the first point
        if ((point = (strokeGraphLinePoint_tPtr)calloc(((size_t)1), sizeof(strokeGraphLinePoint_t))) != nullptr)
          {
          // Load the 'point' pointer into the base and initialise the row/column values
          strokeGraphBase->graphPoints       = point;
          strokeGraphBase->graphRowNumber    = ((GRAPHICS_UINT)0);
          strokeGraphBase->graphColumnNumber = ((GRAPHICS_UINT)0);

          // Build the basic x- and y-grid in the same format as the alphanumeric cells
          for (graphicLineIndexX = ((GRAPHICS_UINT)0); graphicLineIndexX <= graphicLineLengthX; graphicLineIndexX++)
            {
            if (objectError != GRAPHICS_NO_ERROR)
              {
              break;
              }
  
            for (graphicLineIndexY = ((GRAPHICS_UINT)0); graphicLineIndexY <= graphicLineLengthY; graphicLineIndexY++)
              {
              if (objectError != GRAPHICS_NO_ERROR)
                {
                break;
                }
  
              // Mark the line starts' and ends'
              if (graphicLineIndexX == ((GRAPHICS_UINT)0))
                {
                if (graphicLineIndexY == ((GRAPHICS_UINT)0))
                  {
                  strokeGraphBase->graphRowNumber    = ((GRAPHICS_UINT)0);
                  strokeGraphBase->graphColumnNumber = ((GRAPHICS_UINT)0);

                  point->pointMarker = GRAPH_LINE_POINT_MARKER_FIRST_LINE_START;
                  }
                else
                  {
                  if (graphicLineIndexY == graphicLineLengthY)
                    {
                    strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                    point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_END;
                    }
                  else
                    {
                    strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                    point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_MID;
                    }
                  }
                }
              else
                {
                if (graphicLineIndexX == graphicLineLengthX)
                  {
                  if (graphicLineIndexY == ((GRAPHICS_UINT)0))
                    {
                    strokeGraphBase->graphRowNumber    = strokeGraphBase->graphRowNumber + ((GRAPHICS_UINT)1);
                    strokeGraphBase->graphColumnNumber = ((GRAPHICS_UINT)0);

                    point->pointMarker = GRAPH_LINE_POINT_MARKER_LAST_LINE_START;
                    }
                  else
                    {
                    if (graphicLineIndexY == graphicLineLengthY)
                      {
                      strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                      point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_END;
                      }
                    else
                      {
                      strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                      point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_MID;
                      }
                    }
                  }
                else
                  {
                  if (graphicLineIndexY == ((GRAPHICS_UINT)0))
                    {
                    strokeGraphBase->graphRowNumber    = strokeGraphBase->graphRowNumber + ((GRAPHICS_UINT)1);
                    strokeGraphBase->graphColumnNumber = ((GRAPHICS_UINT)0);

                    point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_START;
                    }
                  else
                    {
                    if (graphicLineIndexY == graphicLineLengthY)
                      {
                      strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                      point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_END;
                      }
                    else
                      {
                      strokeGraphBase->graphColumnNumber = strokeGraphBase->graphColumnNumber + ((GRAPHICS_UINT)1);

                      point->pointMarker = GRAPH_LINE_POINT_MARKER_LINE_MID;
                      }
                    }
                  }
                }
              // NOTE : the point values are first normalised in the range { 0.0..1.0 }
              point->pointX  = (GRAPHICS_REAL)graphicLineIndexX / graphicLineLengthX;
              point->pointY  = (GRAPHICS_REAL)graphicLineIndexY / graphicLineLengthY;
  
              point->pointX = (point->pointX  * offsetDeltaX) + characterFrame->frameCoordinates.frameLeftX; 
              point->pointY = (point->pointY  * offsetDeltaY) + characterFrame->frameCoordinates.frameTopY;
  
              point->nextStrokeGraphLine = nullptr;
  
              if ((lastPoint = (strokeGraphLinePoint_tPtr)calloc(((size_t)1), sizeof(strokeGraphLinePoint_t))) != nullptr)
                {
                point->nextStrokeGraphLine = lastPoint;
                point                      = lastPoint;
                }
              else
                {
                objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
                }
              }
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
          }
        }
      else
        {
        objectError = GRAPHICS_OBJECT_RANGE_ERROR;
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
      }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
    } /* end of buildCharacterStrokeGraph                                     */

/******************************************************************************/
/* drawLineSegment() :                                                        */
/******************************************************************************/

graphicsError_t drawLineSegment(      HDC                       hdc,
                                const lineSegment_tPtr          newLineSegment,
                                const lineSegmentMode_t         newLineSegmentMode,
                                const strokeFrame_tPtr          characterFrame,
                                const canvasDescriptor_tPtr     canvasSize,
                                const strokeGraphPointBase_tPtr strokeGraphPoints)
  {
/******************************************************************************/

  Graphics        graphics(hdc);

  GRAPHICS_FLOAT  point0XCentre   = ((GRAPHICS_FLOAT)0.0),
                  point0YCentre   = ((GRAPHICS_FLOAT)0.0),
                  point1XCentre   = ((GRAPHICS_FLOAT)0.0),
                  point1YCentre   = ((GRAPHICS_FLOAT)0.0);

  GRAPHICS_REAL   xAxisPointDelta = ((GRAPHICS_REAL)0.0),
                  yAxisPointDelta = ((GRAPHICS_REAL)0.0);

  objectColour_t  lineColour      = { 0 };

  graphicsError_t objectError     = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((newLineSegment != nullptr) && (canvasSize != nullptr) && (strokeGraphPoints != nullptr))
    {
    // The grid has a normalised page-plotting range that is added to each point coordinate
    xAxisPointDelta = (characterFrame->frameCoordinates.frameRightX  - characterFrame->frameCoordinates.frameLeftX) / ((GRAPHICS_REAL)strokeGraphPoints->graphRowNumber);
    yAxisPointDelta = (characterFrame->frameCoordinates.frameBottomY - characterFrame->frameCoordinates.frameTopY)  / ((GRAPHICS_REAL)strokeGraphPoints->graphColumnNumber);

    // Find the line endpoints' centre coordinates
    point0XCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointX + (((GRAPHICS_REAL)newLineSegment->lineSegmentOriginX) * xAxisPointDelta));
    point0YCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointY + (((GRAPHICS_REAL)newLineSegment->lineSegmentOriginY) * yAxisPointDelta));

    point0XCentre = point0XCentre * ((GRAPHICS_FLOAT)(canvasSize->width));
    point0YCentre = point0YCentre * ((GRAPHICS_FLOAT)(canvasSize->height));

    point1XCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointX + (((GRAPHICS_REAL)newLineSegment->lineSegmentDestinationX) * xAxisPointDelta));
    point1YCentre = (GRAPHICS_FLOAT)(strokeGraphPoints->graphPoints->pointY + (((GRAPHICS_REAL)newLineSegment->lineSegmentDestinationY) * yAxisPointDelta));

    point1XCentre = point1XCentre * ((GRAPHICS_FLOAT)(canvasSize->width));
    point1YCentre = point1YCentre * ((GRAPHICS_FLOAT)(canvasSize->height));

    switch(newLineSegmentMode)
      {
      case LINE_SEGMENT_MODE_PASSIVE : lineColour = strokeGraphPoints->passiveLineStrokeColour;
                                       break;
      case LINE_SEGMENT_MODE_ACTIVE  : lineColour = strokeGraphPoints->activeLineStrokeColour;
                                       break;
      case LINE_SEGMENT_MODE_EDIT    : lineColour = strokeGraphPoints->editLineStrokeColour;
                                       break;
      default                        : objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
                                       [[fallthrough]]; // warning C26819 fallthrough is explicit
      case LINE_SEGMENT_MODE_NONE    : 
                                       break;
      }

    if (objectError == GRAPHICS_NO_ERROR)
      {
      Pen pen(Color((BYTE)((lineColour).opacity),
                    (BYTE)((lineColour).red),
                    (BYTE)((lineColour).green),
                    (BYTE)((lineColour).blue)));

      pen.SetWidth((REAL)strokeGraphPoints->lineWidth);

      pen.SetStartCap(LineCapRound);
      pen.SetEndCap(LineCapRound);

      graphics.DrawLine((const Pen *)&pen, point0XCentre, point0YCentre, point1XCentre, point1YCentre);
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of drawLineSegment                                                 */

/******************************************************************************/
/* instantiateObjectHoldingRingObject() :                                     */
/******************************************************************************/

graphicsError_t instantiateObjectHoldingRingObject(      guiObjectHoldingRing_tPtr   *objectHoldingRingObject,
                                                   const guiObjectNumberingControl_t  guiObjectNumberingControl,
                                                   const GRAPHICS_INT                 guiObjectNumber)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  // The object pointer cannot be active...
  if (*objectHoldingRingObject == nullptr)
    {
    // Create a new holding ring object and link it to itself
    if ((*objectHoldingRingObject = ((guiObjectHoldingRing_tPtr)calloc(((size_t)1), ((size_t)(sizeof(guiObjectHoldingRing_t)))))) != nullptr)
      {
      (*objectHoldingRingObject)->linkObject              = ((guiObjectHoldingRing_tPtr)&objectHoldingRingObject);
      (*objectHoldingRingObject)->activeObject            = { GRAPHICS_ACTIVE_TYPE_NONE, { ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0) }, nullptr };
      (*objectHoldingRingObject)->lastLink                = ((guiObjectHoldingRing_tTag *)&objectHoldingRingObject);
      (*objectHoldingRingObject)->nextLink                = ((guiObjectHoldingRing_tTag *)&objectHoldingRingObject);

      objectError = applyObjectNumberingControl( guiObjectNumber,
                                                &((*objectHoldingRingObject)->numberOfRingLinkObjects),
                                                 guiObjectNumberingControl);
      }
    else
      {
      objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
      }
    }
 else
   {
   objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
   }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of instantiateObjectHoldingRingObject                              */

/******************************************************************************/
/* addObjectHoldingRingObject() :                                             */
/* - this is a bit opaque(!) The parameter 'newObjectHoldingRingObject' is a  */
/*   pointer to the CURRENT populated end of the holding ring. A new holding  */
/*   ring object is instantiated and linked into the holding ring. On exit    */
/*   the parameter 'newObjectHoldingRingObject' points to this new object     */
/******************************************************************************/

graphicsError_t addNewHoldingRingObject(      guiObjectHoldingRing_tPtr   *newObjectHoldingRingObject,
                                        const guiObjectNumberingControl_t  guiObjectNumberingControl,
                                        const GRAPHICS_INT                 guiObjectNumber)
  {
/******************************************************************************/

  guiObjectHoldingRing_tPtr newObject   = nullptr;
  graphicsError_t           objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (*newObjectHoldingRingObject != nullptr)
    {
    if ((objectError = instantiateObjectHoldingRingObject(&newObject, guiObjectNumberingControl, guiObjectNumber)) == GRAPHICS_NO_ERROR)
      {
      (*newObjectHoldingRingObject)->nextLink = newObject;

      newObject->activeObject                 =  { GRAPHICS_ACTIVE_TYPE_NONE, { ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0) }, nullptr };
      newObject->nextLink                     =  nullptr;
      newObject->lastLink                     = *newObjectHoldingRingObject;
      newObject->linkObject                   =  nullptr;
      newObject->linkObjectType               =  GRAPHICS_OBJECT_TYPE_NONE;

      *newObjectHoldingRingObject             =  newObject;

      newObject->numberOfRingLinkObjects      = (*newObjectHoldingRingObject)->numberOfRingLinkObjects;

      objectError = applyObjectNumberingControl( guiObjectNumber,
                                                &((newObject)->numberOfRingLinkObjects),
                                                 guiObjectNumberingControl);
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of addObjectHoldingRingObject                                      */

/******************************************************************************/
/* addGuiObjectToHoldingRing() :                                              */
/* - this function populates an EXISTING holding ring object node             */
/******************************************************************************/

graphicsError_t addGuiObjectToHoldingRing(guiObjectHoldingRing_tPtr insertionIndex,
                                          GRAPHICS_VOID_PTR         newObject,
                                          graphicsActiveObject_tPtr activeBehaviour,
                                          graphicsObjectType_t      graphicsObjectType)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((newObject       != nullptr) && (insertionIndex     != nullptr) && 
      (activeBehaviour != nullptr) && (graphicsObjectType  < GRAPHICS_OBJECT_TYPES))
    {
    switch(graphicsObjectType)
      {
      case GRAPHICS_OBJECT_TYPE_RECTANGLE : insertionIndex->linkObject     =  (GRAPHICS_VOID_PTR)newObject;
                                            insertionIndex->linkObjectType =  graphicsObjectType;

                                            // The holding ring object can "choose" to inherit the GUI 
                                            // objects' active behaviour
                                            insertionIndex->activeObject   = *activeBehaviour;
                                            break;

      case GRAPHICS_OBJECT_TYPE_NONE      : // this may change!
      default                             : objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
                                            break;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of addGuiObjectToHoldingRing                                       */

/******************************************************************************/
/* instantiateStaticTextObject() :                                            */
/******************************************************************************/

graphicsError_t instantiateStaticTextObject(      staticTextObject_tPtr *staticTextObject,
                                            const GRAPHICS_WCHAR_PTR     staticText,
                                            const rectangleDimensions_t  staticTextDimensions,
                                            const objectColour_t         staticTextColour)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  // The object pointer cannot be active...
  if ((*staticTextObject == nullptr) && (staticText != nullptr))
    {
    if ((staticTextDimensions.leftX   >= GUI_CANVAS_GUI_NOMINAL_LEFT_X)  &&
        (staticTextDimensions.topY    >= GUI_CANVAS_GUI_NOMINAL_TOP_Y)   &&
        (staticTextDimensions.rightX  <= GUI_CANVAS_GUI_NOMINAL_RIGHT_X) &&
        (staticTextDimensions.bottomY <= GUI_CANVAS_GUI_NOMINAL_BOTTOM_Y))
      { 
      if ((*staticTextObject = ((staticTextObject_tPtr)calloc(((size_t)1), ((size_t)(sizeof(staticTextObject)))))) != nullptr)
        {
        if (((*staticTextObject)->staticText = ((GRAPHICS_WCHAR_PTR)calloc(((size_t)1), ((size_t)(wcslen(staticText) + GRAPHICS_WCHAR_EOS_LENGTH))))) != nullptr)
          {
          wcscpy_s((*staticTextObject)->staticText, ((rsize_t)(wcslen(staticText) + GRAPHICS_WCHAR_EOS_LENGTH)), staticText);

          (*staticTextObject)->objectType            = GRAPHICS_OBJECT_TYPE_STATIC_TEXT;
          (*staticTextObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_NONE;
          (*staticTextObject)->nextDrawingObject     = nullptr;
          (*staticTextObject)->staticTextDimensions  = staticTextDimensions;
          (*staticTextObject)->staticTextColour      = staticTextColour;
          }
        else
          {
          objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
          }
        }
      else
        {
        objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
        }
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of instantiateStaticTextObject                                     */

/******************************************************************************/
/* instantiateRectangleObject() :                                             */
/******************************************************************************/

graphicsError_t instantiateRectangleObject(      rectangleObject_tPtr  *rectangleObject,
                                           const rectangleDimensions_t  rectangleDimensions,
                                           const objectColour_t         rectangleColour)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  // The object pointer cannot be active...
  if (*rectangleObject == nullptr)
    {
    if ((rectangleDimensions.leftX   >= GUI_CANVAS_GUI_NOMINAL_LEFT_X)  &&
        (rectangleDimensions.topY    >= GUI_CANVAS_GUI_NOMINAL_TOP_Y)   &&
        (rectangleDimensions.rightX  <= GUI_CANVAS_GUI_NOMINAL_RIGHT_X) &&
        (rectangleDimensions.bottomY <= GUI_CANVAS_GUI_NOMINAL_BOTTOM_Y))
      {    
      if ((*rectangleObject = ((rectangleObject_tPtr)calloc(((size_t)1), ((size_t)(sizeof(rectangleObject_t)))))) != nullptr)
        {
        (*rectangleObject)->objectType            = GRAPHICS_OBJECT_TYPE_RECTANGLE;
        (*rectangleObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_NONE;
        (*rectangleObject)->nextDrawingObject     = nullptr;
        (*rectangleObject)->rectangleDimensions   = rectangleDimensions;
        (*rectangleObject)->rectangleColour       = rectangleColour;
        }
      else
        {
        objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
        }
     }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of instantiateRectangleObject                                      */

/******************************************************************************/
/* computeCompositeObjectBoundary() :                                         */
/* - all the gui objects in a chain are examined for their bounding points. A */
/*   new composite boundary is computed from this and held in the root. NOTE  */
/*   there is one chain per holding ring (composite) object and must be       */
/*   revisited if the chain is modified                                       */
/******************************************************************************/

graphicsError_t computeCompositeObjectBoundary(guiObjectHoldingRing_tPtr guObjectHoldingRingRoot)
  {
/******************************************************************************/

  commonObject_tPtr guiRingRoot = nullptr;
  graphicsError_t   objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((guObjectHoldingRingRoot != nullptr) && (guObjectHoldingRingRoot->linkObject != nullptr))
    {
    guiRingRoot = (commonObject_tPtr)guObjectHoldingRingRoot->linkObject;

    do
      { // Different composite object components may have a different boundary layout
      switch(((commonObject_tPtr)guiRingRoot)->objectType)
        {
        case GRAPHICS_OBJECT_TYPE_RECTANGLE : if (((rectangleObject_tPtr)guiRingRoot)->rectangleDimensions.leftX < guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.leftX)
                                                {
                                                guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.leftX = ((rectangleObject_tPtr)guiRingRoot)->rectangleDimensions.leftX;
                                                }

                                              if (((rectangleObject_tPtr)guiRingRoot)->rectangleDimensions.rightX > guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.rightX)
                                                {
                                                guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.rightX = ((rectangleObject_tPtr)guiRingRoot)->rectangleDimensions.rightX;
                                                }

                                              if (((rectangleObject_tPtr)guiRingRoot)->rectangleDimensions.topY < guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.topY)
                                                {
                                                guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.topY = ((rectangleObject_tPtr)guiRingRoot)->rectangleDimensions.topY;
                                                }

                                              if (((rectangleObject_tPtr)guiRingRoot)->rectangleDimensions.bottomY > guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.bottomY)
                                                {
                                                guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.bottomY = ((rectangleObject_tPtr)guiRingRoot)->rectangleDimensions.bottomY;
                                                }
                                              break;

        default                             : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                              break;
        }
      }
    while ((guiRingRoot = ((commonObject_tPtr)guiRingRoot->nextDrawingObject)) != nullptr);
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of computeCompositeObjectBoundary                                  */

/******************************************************************************/
/*  addGuiObjectToGuiChain() :                                                */
/* - adds a new drawing object to the end of a chain. The 'lastGuiObject'     */
/*   MUST be the last object at the end of the chain!                         */
/******************************************************************************/

graphicsError_t addGuiObjectToGuiChain(GRAPHICS_VOID_PTR newGuiObject,
                                       GRAPHICS_VOID_PTR lastGuiObject)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  // The last objects' link pointer must be null
  if ((newGuiObject != nullptr) && (lastGuiObject != nullptr) && ((graphicsObjectType_tPtr)(((commonObject_tPtr)lastGuiObject)->nextDrawingObject) == nullptr)) // (((commonObject_tPtr)lastGuiObject)->nextDrawingObject == nullptr))
    { // Check the end-of-chain object type is valid
    switch(((commonObject_tPtr)lastGuiObject)->objectType)
      { // Check the new object type is valid and if so link it in the gui chain
      case GRAPHICS_OBJECT_TYPE_RECTANGLE   : switch(((commonObject_tPtr)newGuiObject)->objectType)
                                                {
                                                case GRAPHICS_OBJECT_TYPE_RECTANGLE   : (((commonObject_tPtr)lastGuiObject)->nextDrawingObject)    = (rectangleObject_tPtr)newGuiObject;
                                                                                         ((commonObject_tPtr)lastGuiObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_RECTANGLE;
                                                                                        break;
                                            
                                                case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : (((commonObject_tPtr)lastGuiObject)->nextDrawingObject)    = (staticTextObject_tPtr)newGuiObject;
                                                                                         ((commonObject_tPtr)lastGuiObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_STATIC_TEXT;
                                                                                        break;

                                                default                               : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                                                                        break;
                                                }
                                              break;

      case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : switch(((commonObject_tPtr)newGuiObject)->objectType)
                                                {
                                                case GRAPHICS_OBJECT_TYPE_RECTANGLE   : (((commonObject_tPtr)lastGuiObject)->nextDrawingObject)    = (rectangleObject_tPtr)newGuiObject;
                                                                                         ((commonObject_tPtr)lastGuiObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_RECTANGLE;
                                                                                        break;

                                                case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : (((commonObject_tPtr)lastGuiObject)->nextDrawingObject)    = (staticTextObject_tPtr)newGuiObject;
                                                                                         ((commonObject_tPtr)lastGuiObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_STATIC_TEXT;
                                                                                        break;

                                                default                               : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                                                                        break;
                                                }
                                              break;

      default                               : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                              break;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of addGuiObjectToGuiChain                                          */

/******************************************************************************/
/* applyObjectNumberingControl() :                                            */
/******************************************************************************/

graphicsError_t applyObjectNumberingControl(const GRAPHICS_INT                newNumber,
                                                  GRAPHICS_INT_PTR            objectNumber,
                                            const guiObjectNumberingControl_t numberingControl)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (objectNumber != nullptr)
    {
    switch(numberingControl)
      {
      case  GRAPHICS_OBJECT_NUMBERING_IMMEDIATE   : *objectNumber =  newNumber;
                                                    break;
      case  GRAPHICS_OBJECT_NUMBERING_INCREMENTAL : *objectNumber = *objectNumber + 1;
                                                    break;
      case  GRAPHICS_OBJECT_NUMBERING_CUMULATIVE  : *objectNumber = *objectNumber + newNumber;
                                                    break;
      case  GRAPHICS_OBJECT_NUMBERING_RESET       : *objectNumber =  0;
                                                    break;
      case  GRAPHICS_OBJECT_NUMBERING_NONE        : 
      default                                     : break;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of applyObjectNumberingControl                                     */

/******************************************************************************/
/* incrementallyTraverseHoldingRing() :                                       */
/* - step along the object holding ring one element at a time i.e. confirm    */
/*   the CURRENT index as valid or not. If valid keep the current index       */
/*   unchanged and point to the next (possibly) element. If not valid return  */
/*   with an error                                                            */
/******************************************************************************/

graphicsError_t incrementallyTraverseHoldingRing(guiObjectHoldingRing_tPtr *objectHoldingRingCurrentIndex,
                                                 guiObjectHoldingRing_tPtr *objectHoldingRingNextIndex)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((objectHoldingRingCurrentIndex != nullptr) && (*objectHoldingRingCurrentIndex != nullptr))
    {
    // Check the index is pointing actually pointing at a valid object
    switch((*objectHoldingRingCurrentIndex)->linkObjectType)
      {
      case GRAPHICS_OBJECT_TYPE_NONE        : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                              break;
                                            
      case GRAPHICS_OBJECT_TYPE_RECTANGLE   : // The CURRENT index is(?) valid so save the next index
                                              *objectHoldingRingNextIndex = (*objectHoldingRingCurrentIndex)->nextLink;
                                            
                                              break;

      case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : // The CURRENT index is(?) valid so save the next index
                                              *objectHoldingRingNextIndex = (*objectHoldingRingCurrentIndex)->nextLink;

                                              break;

      case GRAPHICS_OBJECT_TYPE_UNKNOWN     :
      default                               : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                              break;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of incrementallyTraverseHoldingRing                                */

/******************************************************************************/
/* guiPrintStaticText() :                                                     */
/* - prints a text phrase constrained by an invisible rectangle               */
/******************************************************************************/

graphicsError_t guiPrintStaticText(HDC                 hdc,
                                   commonObject_tPtr   graphicsObject,
                                   canvasDescriptor_t *canvasSize)
  {
/******************************************************************************/

  Graphics        graphics(hdc);

  graphicsError_t objectError    = GRAPHICS_NO_ERROR;
                                 
  GRAPHICS_FLOAT  leftX          = 0.0f,
                  topY           = 0.0f,
                  rightX         = 0.0f,
                  bottomY        = 0.0f,
                  deltaX         = 0.0f,
                  deltaY         = 0.0f;

  RECT            staticTextBox  = { ((GRAPHICS_LONG)0), ((GRAPHICS_LONG)0), ((GRAPHICS_LONG)0), ((GRAPHICS_LONG)0) };

  GRAPHICS_UINT   lineWidthIndex = ((GRAPHICS_UINT)0);

/******************************************************************************/

  // Drawing is in 'canvas' coordinates. gui coordinates are normalised to 
  // ((double)0.0)...((double)1.0) in the x- and y-axes. The final number to 
  // use is gui((double)x.y) * canvas((double)x.y) cast to 'float'
  if ((graphicsObject != nullptr) && (canvasSize != nullptr))
    {
    if ((graphicsObjectType_t)graphicsObject->objectType == GRAPHICS_OBJECT_TYPE_STATIC_TEXT)
      {
      Pen  pen(Color( (BYTE)(((staticTextObject_tPtr)(graphicsObject))->staticTextColour.opacity),
                      (BYTE)(((staticTextObject_tPtr)(graphicsObject))->staticTextColour.red),
                      (BYTE)(((staticTextObject_tPtr)(graphicsObject))->staticTextColour.green),
                      (BYTE)(((staticTextObject_tPtr)(graphicsObject))->staticTextColour.blue)));
    
      leftX   = (GRAPHICS_FLOAT)(((staticTextObject_tPtr)(graphicsObject))->staticTextDimensions.leftX);
      topY    = (GRAPHICS_FLOAT)(((staticTextObject_tPtr)(graphicsObject))->staticTextDimensions.topY);
      rightX  = (GRAPHICS_FLOAT)(((staticTextObject_tPtr)(graphicsObject))->staticTextDimensions.rightX);
      bottomY = (GRAPHICS_FLOAT)(((staticTextObject_tPtr)(graphicsObject))->staticTextDimensions.bottomY);
    
      deltaX  = ((GRAPHICS_FLOAT)(canvasSize->right))  - ((GRAPHICS_FLOAT)(canvasSize->left));
      deltaY  = ((GRAPHICS_FLOAT)(canvasSize->bottom)) - ((GRAPHICS_FLOAT)(canvasSize->top));
    
      leftX   = ceilf(leftX   * deltaX);
      topY    = ceilf(topY    * deltaY);
      rightX  = ceilf(rightX  * deltaX);
      bottomY = ceilf(bottomY * deltaY);
      
      if (leftX < ((GRAPHICS_FLOAT)0.0))
        {
        leftX = ((GRAPHICS_FLOAT)0.0);
        }
    
      if (topY < ((GRAPHICS_FLOAT)0.0))
        {
        topY = ((GRAPHICS_FLOAT)0.0);
        }
    
      if (rightX > ((GRAPHICS_FLOAT)(canvasSize->right)))
        {
        rightX = ((GRAPHICS_FLOAT)(canvasSize->right));
        }
    
      if (bottomY > ((GRAPHICS_FLOAT)(canvasSize->bottom)))
        {
        bottomY = ((GRAPHICS_FLOAT)(canvasSize->bottom));
        }

      staticTextBox.left   = (GRAPHICS_LONG)leftX;
      staticTextBox.top    = (GRAPHICS_LONG)topY;
      staticTextBox.right  = (GRAPHICS_LONG)rightX;
      staticTextBox.bottom = (GRAPHICS_LONG)bottomY;

  //   DrawText( hdc,
  //             ((staticTextObject_tPtr)graphicsObject)->staticText,
  //             (GRAPHICS_INT)wcslen(((staticTextObject_tPtr)graphicsObject)->staticText), // CAPTION_TEXT_LENGTH(captionText),
  //            &staticTextBox,
  //             DT_CENTER);
      }
    else
      {
      objectError = GRAPHICS_OBJECT_TYPE_ERROR;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of guiPrintStaticText                                              */




/******************************************************************************/
/* guiDrawRectangle() :                                                       */
/******************************************************************************/

graphicsError_t guiDrawRectangle(HDC                 hdc,
                                 commonObject_tPtr   graphicsObject,
                                 canvasDescriptor_t *canvasSize)
  {
/******************************************************************************/

  Graphics        graphics(hdc);

  graphicsError_t objectError    = GRAPHICS_NO_ERROR;
                                 
  GRAPHICS_FLOAT  leftX          = 0.0f,
                  leftXo         = 0.0f,
                  topY           = 0.0f,
                  topYo          = 0.0f,
                  rightX         = 0.0f,
                  rightXo        = 0.0f,
                  bottomY        = 0.0f,
                  bottomYo       = 0.0f,
                  deltaX         = 0.0f,
                  deltaY         = 0.0f;

  GRAPHICS_UINT   lineWidthIndex = ((GRAPHICS_UINT)0);

/******************************************************************************/

  // Drawing is in 'canvas' coordinates. gui coordinates are normalised to 
  // ((double)0.0)...((double)1.0) in the x- and y-axes. The final number to 
  // use is gui((double)x.y) * canvas((double)x.y) cast to 'float'
  if ((graphicsObject != nullptr) && (canvasSize != nullptr))
    {
    if ((graphicsObjectType_t)graphicsObject->objectType == GRAPHICS_OBJECT_TYPE_RECTANGLE)
      {
      Pen  pen(Color( (BYTE)(((rectangleObject_tPtr)(graphicsObject))->rectangleColour.opacity),
                      (BYTE)(((rectangleObject_tPtr)(graphicsObject))->rectangleColour.red),
                      (BYTE)(((rectangleObject_tPtr)(graphicsObject))->rectangleColour.green),
                      (BYTE)(((rectangleObject_tPtr)(graphicsObject))->rectangleColour.blue)));
    
      leftXo   = (GRAPHICS_FLOAT)(((rectangleObject_tPtr)(graphicsObject))->rectangleDimensions.leftX);
      topYo    = (GRAPHICS_FLOAT)(((rectangleObject_tPtr)(graphicsObject))->rectangleDimensions.topY);
      rightXo  = (GRAPHICS_FLOAT)(((rectangleObject_tPtr)(graphicsObject))->rectangleDimensions.rightX);
      bottomYo = (GRAPHICS_FLOAT)(((rectangleObject_tPtr)(graphicsObject))->rectangleDimensions.bottomY);
    
      deltaX  = ((GRAPHICS_FLOAT)(canvasSize->right))  - ((GRAPHICS_FLOAT)(canvasSize->left));
      deltaY  = ((GRAPHICS_FLOAT)(canvasSize->bottom)) - ((GRAPHICS_FLOAT)(canvasSize->top));
    
      // Draw a set of nested rectangles to build the requested linewidth
      for (lineWidthIndex = ((GRAPHICS_UINT)0); lineWidthIndex < (GRAPHICS_UINT)(((rectangleObject_tPtr)(graphicsObject))->rectangleDimensions.lineWidth); lineWidthIndex++)
        {
        GRAPHICS_FLOAT lineWidthX = ((GRAPHICS_FLOAT)lineWidthIndex) / deltaX,
                       lineWidthY = ((GRAPHICS_FLOAT)lineWidthIndex) / deltaY;
    
        leftX   = ceilf((leftXo   - lineWidthX) * deltaX);
        topY    = ceilf((topYo    - lineWidthY) * deltaY);
        rightX  = ceilf((rightXo  + lineWidthX) * deltaX);
        bottomY = ceilf((bottomYo + lineWidthY) * deltaY);
        
        if (leftX < ((GRAPHICS_FLOAT)0.0))
          {
          leftX = ((GRAPHICS_FLOAT)0.0);
          }
    
        if (topY < ((GRAPHICS_FLOAT)0.0))
          {
          topY = ((GRAPHICS_FLOAT)0.0);
          }
    
        if (rightX > ((GRAPHICS_FLOAT)(canvasSize->right)))
          {
          rightX = ((GRAPHICS_FLOAT)(canvasSize->right));
          }
    
        if (bottomY > ((GRAPHICS_FLOAT)(canvasSize->bottom)))
          {
          bottomY = ((GRAPHICS_FLOAT)(canvasSize->bottom));
          }
    
        graphics.DrawLine((const Pen *)&pen, leftX,  topY,    rightX, topY);
        graphics.DrawLine((const Pen *)&pen, leftX,  bottomY, rightX, bottomY);
        graphics.DrawLine((const Pen *)&pen, leftX,  topY,    leftX,  bottomY);
        graphics.DrawLine((const Pen *)&pen, rightX, topY,    rightX, bottomY);
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_TYPE_ERROR;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of GuiDrawRectangle                                                */

/******************************************************************************/
/* Active object handling functions :                                         */
/******************************************************************************/
/* rectangleObjectOneHandler() :                                              */
/******************************************************************************/

graphicsError_t rectangleObjectOneHandler(GRAPHICS_VOID_PTR handlingMode)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/
/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of rectangleObjectOneHandler                                       */

/******************************************************************************/
/* rectangleObjectTwoHandler() :                                              */
/******************************************************************************/

graphicsError_t rectangleObjectTwoHandler(GRAPHICS_VOID_PTR handlingMode)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/
/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of rectangleObjectTwoHandler                                       */

/******************************************************************************/
/* rectangleObjectThreeHandler() :                                            */
/******************************************************************************/

graphicsError_t rectangleObjectThreeHandler(GRAPHICS_VOID_PTR handlingMode)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/
/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of rectangleObjectThreeHandler                                    */

/******************************************************************************/
/* printHoldingRingObject() :                                                 */
/******************************************************************************/

graphicsError_t printHoldingRingObject(guiObjectHoldingRing_tPtr ringObjectBase)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (ringObjectBase != nullptr)
    {
    do
      {
      switch(ringObjectBase->linkObjectType)
        {
        case GRAPHICS_OBJECT_TYPE_NONE      : wcscpy_s(objectOutput, GRAPHICS_OBJECT_TYPE_NONE_WSTR);
                                              objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                              break;

        case GRAPHICS_OBJECT_TYPE_RECTANGLE : wcscpy_s(objectOutput, GRAPHICS_OBJECT_TYPE_RECTANGLE_WSTR);
                                              break;

        case GRAPHICS_OBJECT_TYPE_UNKNOWN   :
        default                             : wcscpy_s(objectOutput, GRAPHICS_OBJECT_TYPE_UNKNOWN_WSTR);
                                              objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                              break;
        }

      objectError = printMessageBox(&objectOutput[0], &captionPanel[0]);

      ringObjectBase = ringObjectBase->nextLink;
      }
    while (ringObjectBase != nullptr);
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of printHoldingRingObject                                         */

/******************************************************************************/
/* printMessageBox() :                                                        */
/******************************************************************************/

graphicsError_t printMessageBox(WCHAR *message, WCHAR *caption)
  {
/******************************************************************************/

  GRAPHICS_INT    msgboxID                                       = 0;

  graphicsError_t objectError                                    = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((message != nullptr) && (caption != nullptr))
    {
    msgboxID = MessageBox( NULL,
                           message,
                           caption,
                           MB_ICONINFORMATION | MB_OK);
    }
    
/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of printMessageBox                                                 */

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/

