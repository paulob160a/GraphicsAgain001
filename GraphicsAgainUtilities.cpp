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
#include "StrokeGraphicsListHandlers.h"
#include "GraphicsAgainUtilities.h"
#include "GraphicsAgainCaption.h"
#include "StrokeGraphicsAlphabet.h"
#include "GraphicsAgain001.h"

/******************************************************************************/

#include "GraphicsAgain001.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/

// The screen coordinates are initially set to full HD
globalScreenCoordinates_t  globalScreenCoordinates          = {
                                                              GUI_SCREEN_GUI_DEFAULT_LEFT_X,
                                                              GUI_SCREEN_GUI_DEFAULT_TOP_Y,
                                                              GUI_SCREEN_GUI_DEFAULT_RIGHT_X,
                                                              GUI_SCREEN_GUI_DEFAULT_BOTTOM_Y
                                                              };
                                                            
canvasDescriptor_t         canvasSize                       = { CANVAS_SIZE_ZERO_X, CANVAS_SIZE_ZERO_Y, CANVAS_SIZE_WIDTH, CANVAS_SIZE_HEIGHT },
                           localCanvasSize                  = { CANVAS_SIZE_ZERO_X, CANVAS_SIZE_ZERO_Y, CANVAS_SIZE_WIDTH, CANVAS_SIZE_HEIGHT };
                                                            
guiObjectHoldingRing_tPtr  guiObjectHoldingRingBaseLink     = nullptr,
                           guiObjectHoldingRingCurrent      = nullptr,
                           guiObjectHoldingRingFreePtr      = nullptr; 

bool                       mouseOverObject                  = false; // flag when the mouse pointer is in an objects' perimeter
bool                       objectPositionTestFlag           = true; // flag the position test is enabled for a single traverse 
                                                                     // of the holding ring
GRAPHICS_SHORT             mouseXPosition                   = ((GRAPHICS_SHORT)0);
GRAPHICS_SHORT             mouseYPosition                   = ((GRAPHICS_SHORT)0);

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
  } /* end of buildCharacterStrokeGraph                                       */

/******************************************************************************/
/* deleteCharacterStrokeGraph() :                                             */
/******************************************************************************/

graphicsError_t deleteCharacterStrokeGraph(strokeGraphPointBase_tPtr strokeGraphBase)
  {
/******************************************************************************/

  GRAPHICS_UINT             rowIndex    = ((GRAPHICS_UINT)0),
                            columnIndex = ((GRAPHICS_UINT)0);

  strokeGraphLinePoint_tPtr pointTrack  = nullptr,
                            thisPoint   = nullptr;

  graphicsError_t           objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (strokeGraphBase != nullptr)
    {
    if ((strokeGraphBase->graphRowNumber != ((GRAPHICS_UINT)0)) && (strokeGraphBase->graphColumnNumber != ((GRAPHICS_UINT)0)))
      {
      pointTrack = strokeGraphBase->graphPoints;

      for (rowIndex = ((GRAPHICS_UINT)0); rowIndex <= strokeGraphBase->graphRowNumber; rowIndex++)
        {
        for (columnIndex = ((GRAPHICS_UINT)0); columnIndex <= strokeGraphBase->graphColumnNumber ; columnIndex++)
          {
          if (pointTrack->nextStrokeGraphLine != nullptr)
            {
            thisPoint = pointTrack;
            pointTrack = pointTrack->nextStrokeGraphLine;

            delete(thisPoint);
            }
          }
        }
      }
    else
      {
      //objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of deleteCharacterStrokeGraph                                      */

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
      case GRAPHICS_OBJECT_TYPE_RADIO_BUTTON : [[fallthrough]]; // warning C26819 fallthrough is explicit
      case GRAPHICS_OBJECT_TYPE_STATIC_TEXT  : [[fallthrough]]; // warning C26819 fallthrough is explicit
      case GRAPHICS_OBJECT_TYPE_CIRCULAR     : [[fallthrough]]; // warning C26819 fallthrough is explicit
      case GRAPHICS_OBJECT_TYPE_RECTANGLE    : insertionIndex->linkObject     =  (GRAPHICS_VOID_PTR)newObject;
                                               insertionIndex->linkObjectType =  graphicsObjectType;
                                             
                                               // The holding ring object can "choose" to inherit the GUI 
                                               // objects' active behaviour
                                               insertionIndex->activeObject   = *activeBehaviour;
                                            
                                               insertionIndex->nextLink       =  nullptr;
                                               break;
                                            
      case GRAPHICS_OBJECT_TYPE_UNKNOWN      : [[fallthrough]]; // warning C26819 fallthrough is explicit
      case GRAPHICS_OBJECT_TYPE_NONE         : // This may change!
      default                                : objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
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
/* computeStrokeTextBoundary() :                                              */
/* - compute the virtual enclosing rectangle of a complete string of stroke   */
/*   text characters. The components of the calculation are defined in the    */
/*   stroke text structure                                                    */
/******************************************************************************/

graphicsError_t computeStrokeTextBoundary(const strokeTextStringDescriptor_tPtr strokeTextCharacters,
                                          const canvasDescriptor_tPtr           canvasSize,
                                                characterExtentsReal_tPtr       strokeTextStringBoundary)
  {
/******************************************************************************/

  GRAPHICS_UINT               strokeCharacterIndex     = ((GRAPHICS_UINT)0);
                              
  strokeCharacterTrack_t      characterTrack           = {
                                                         ((GRAPHICS_REAL)0.0),
                                                         ((GRAPHICS_REAL)0.0),
                                                         { ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0) },
                                                         { ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0) }
                                                         };
                                                       
  size_t                      strokeStringSize         = ((size_t)0);

  graphicsError_t             objectError              = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((strokeTextCharacters != nullptr) && (canvasSize!= nullptr))
    {
    if ((strokeStringSize = wcslen(strokeTextCharacters->strokeTextString)) != ((size_t)0))
      {
      // Initialise the screen position for stroke text placement
      characterTrack.characterPlacementX = strokeTextCharacters->strokeTextAnchor.xAxisPoint;
      characterTrack.characterPlacementY = strokeTextCharacters->strokeTextAnchor.yAxisPoint;

      for (strokeCharacterIndex = ((GRAPHICS_UINT)0); strokeCharacterIndex < strokeStringSize; strokeCharacterIndex++)
        {
        // Move to the next character
        characterTrack.characterPlacementX = characterTrack.characterPlacementX + strokeTextCharacters->strokeTextCharacterWidth + strokeTextCharacters->strokeTextInterCharacterSpacing.xAxisPoint;
        }

      // Return the unscaled "raw" top-left boundary points
      strokeTextStringBoundary->topLeftRaw.pointX = strokeTextCharacters->strokeTextAnchor.xAxisPoint;
      strokeTextStringBoundary->topLeftRaw.pointY = strokeTextCharacters->strokeTextAnchor.yAxisPoint - strokeTextCharacters->strokeTextInterCharacterSpacing.yAxisPoint;

      // Return the scaled "cooked" boundary points
      strokeTextStringBoundary->topLeft.pointX = strokeTextCharacters->strokeTextAnchor.xAxisPoint * (canvasSize->right - canvasSize->left);
      strokeTextStringBoundary->topLeft.pointY = (strokeTextCharacters->strokeTextAnchor.yAxisPoint - strokeTextCharacters->strokeTextInterCharacterSpacing.yAxisPoint) * (canvasSize->bottom - canvasSize->top);

      characterTrack.characterPlacementX = characterTrack.characterPlacementX + strokeTextCharacters->strokeTextCharacterWidth + strokeTextCharacters->strokeTextInterCharacterSpacing.xAxisPoint;

      strokeTextStringBoundary->bottomRight.pointX = characterTrack.characterPlacementX * (canvasSize->right  - canvasSize->left);
      strokeTextStringBoundary->bottomRight.pointY = strokeTextStringBoundary->topLeft.pointY + ((strokeTextCharacters->strokeTextCharacterDepth + (((GRAPHICS_REAL)2.0) * strokeTextCharacters->strokeTextInterCharacterSpacing.yAxisPoint)) * (canvasSize->bottom - canvasSize->top));

      // Return the unscaled "raw" botton-right boundary points
      strokeTextStringBoundary->bottomRightRaw.pointX = characterTrack.characterPlacementX;
      strokeTextStringBoundary->bottomRightRaw.pointY = strokeTextStringBoundary->topLeftRaw.pointY + ((strokeTextCharacters->strokeTextCharacterDepth + (((GRAPHICS_REAL)2.0) * strokeTextCharacters->strokeTextInterCharacterSpacing.yAxisPoint)));
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
  } /* end of computeStrokeTextBoundary                                       */

/******************************************************************************/
/* computeCompositeObjectBoundary() :                                         */
/* - all the gui objects in a chain are examined for their bounding points. A */
/*   new composite boundary is computed from this and held in the root. NOTE  */
/*   there is one chain per holding ring (composite) object and must be       */
/*   revisited if the chain is modified                                       */
/******************************************************************************/

graphicsError_t computeCompositeObjectBoundary(      guiObjectHoldingRing_tPtr guObjectHoldingRingRoot,
                                               const alphabetCharacters_tPtr   characterList,
                                               const canvasDescriptor_tPtr     canvasSize)
  {
/******************************************************************************/

  GRAPHICS_UINT           strokeCharacter          = ((GRAPHICS_UINT)0),
                          strokeCharacterIndex     = ((GRAPHICS_UINT)0);

  size_t                  staticTextLength         = ((size_t)0);
  GRAPHICS_UINT           boundX                   = ((GRAPHICS_UINT)0),
                          boundY                   = ((GRAPHICS_UINT)0),
                          spaceCount               = ((GRAPHICS_UINT)0),
                          spaceWidth               = ((GRAPHICS_UINT)0), // width of a space character
                          extentX                  = ((GRAPHICS_UINT)GRAPHICS_CHARACTER_MAXIMUM_TEST),
                          extentY                  = ((GRAPHICS_UINT)GRAPHICS_CHARACTER_MAXIMUM_TEST),
                          boundaryX                = ((GRAPHICS_UINT)0),
                          boundaryY                = ((GRAPHICS_UINT)0);

  GRAPHICS_REAL           topLeftX                 = ((GRAPHICS_REAL)0.0),
                          topLeftY                 = ((GRAPHICS_REAL)0.0),
                          bottomRightX             = ((GRAPHICS_REAL)0.0),
                          bottomRightY             = ((GRAPHICS_REAL)0.0);

  singlePoint_t           textSpacing              = { ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0) }; // inter-character gap width

  alphabetCharacters_tPtr strokeCharacterReference = nullptr;
  commonObject_tPtr       guiRingRoot              = nullptr;

  characterExtentsReal_t  textStringBoundary       = {
                                                       {
                                                       ((GRAPHICS_REAL)0.0),
                                                       ((GRAPHICS_REAL)0.0)
                                                       },
                                                       {
                                                       ((GRAPHICS_REAL)0.0),
                                                       ((GRAPHICS_REAL)0.0)
                                                       },
                                                       {
                                                       ((GRAPHICS_REAL)0.0),
                                                       ((GRAPHICS_REAL)0.0)
                                                       },
                                                       {
                                                       ((GRAPHICS_REAL)0.0),
                                                       ((GRAPHICS_REAL)0.0)
                                                       }
                                                     };

  graphicsError_t         objectError              = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((guObjectHoldingRingRoot != nullptr) && (guObjectHoldingRingRoot->linkObject != nullptr))
    {
    guiRingRoot = (commonObject_tPtr)guObjectHoldingRingRoot->linkObject;

    do
      { // Different composite object components may have a different boundary layout
      switch(((commonObject_tPtr)guiRingRoot)->objectType)
        {
        case GRAPHICS_OBJECT_TYPE_RECTANGLE   : if (((rectangleObject_tPtr)guiRingRoot)->rectangleDimensions.leftX < guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.leftX)
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

        case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : if ((((staticTextObject_tPtr)guiRingRoot)->staticText != nullptr) && (((staticTextObject_tPtr)guiRingRoot)->staticText->strokeTextString != nullptr))
                                                  {
                                                  if ((objectError = computeStrokeTextBoundary((const strokeTextStringDescriptor_tPtr)((staticTextObject_tPtr)guiRingRoot)->staticText,
                                                                                               (const canvasDescriptor_tPtr)canvasSize,
                                                                                                                            &textStringBoundary)) == GRAPHICS_NO_ERROR)
                                                    {
                                                    // Compare with the current boundaries and update them if necessary
                                                    if (textStringBoundary.topLeftRaw.pointX < guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.leftX)
                                                      {
                                                      guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.leftX = textStringBoundary.topLeftRaw.pointX;
                                                      }
                                                  
                                                    if (textStringBoundary.bottomRightRaw.pointX > guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.rightX)
                                                      {
                                                      guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.rightX = textStringBoundary.bottomRightRaw.pointX;
                                                      }
                                                  
                                                    if (textStringBoundary.topLeftRaw.pointY < guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.topY)
                                                      {
                                                      guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.topY = textStringBoundary.topLeftRaw.pointY;
                                                      }
                                                  
                                                    if (textStringBoundary.bottomRightRaw.pointY > guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.bottomY)
                                                      {
                                                      guObjectHoldingRingRoot->activeObject.activeRegion.rectangularBoundary.bottomY = textStringBoundary.bottomRightRaw.pointY;
                                                      }
                                                    }
                                                  else
                                                    {
                                                    objectError = GRAPHICS_OBJECT_FUNCTION_ERROR;
                                                    }
                                                  }
                                                else
                                                  {
                                                  objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
                                                  break;
                                                  }

                                                break;

        default                               : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
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
      case GRAPHICS_OBJECT_TYPE_NONE         : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                               break;
                                             
      case GRAPHICS_OBJECT_TYPE_RECTANGLE    : // The CURRENT index is(?) valid so save the next index
                                               *objectHoldingRingNextIndex = (*objectHoldingRingCurrentIndex)->nextLink;
                                             
                                               break;
                                            
      case GRAPHICS_OBJECT_TYPE_STATIC_TEXT  : // The CURRENT index is(?) valid so save the next index
                                               *objectHoldingRingNextIndex = (*objectHoldingRingCurrentIndex)->nextLink;
                                            
                                               break;

      case GRAPHICS_OBJECT_TYPE_RADIO_BUTTON :// The CURRENT index is(?) valid so save the next index
                                               *objectHoldingRingNextIndex = (*objectHoldingRingCurrentIndex)->nextLink;
                                            
                                               break;

      case GRAPHICS_OBJECT_TYPE_UNKNOWN      :
      default                                : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
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
    
      deltaX   = ((GRAPHICS_FLOAT)(canvasSize->right))  - ((GRAPHICS_FLOAT)(canvasSize->left));
      deltaY   = ((GRAPHICS_FLOAT)(canvasSize->bottom)) - ((GRAPHICS_FLOAT)(canvasSize->top));
    
      // Draw a set of nested rectangles to build the requested linewidth
      for (lineWidthIndex = ((GRAPHICS_UINT)0); lineWidthIndex < (GRAPHICS_UINT)(((rectangleObject_tPtr)(graphicsObject))->rectangleDimensions.lineWidth); lineWidthIndex++)
        {
        GRAPHICS_FLOAT lineWidthX = ((GRAPHICS_FLOAT)lineWidthIndex) / deltaX,
                       lineWidthY = ((GRAPHICS_FLOAT)lineWidthIndex) / deltaY;
    
        leftX   = ceilf((leftXo   - lineWidthX) * deltaX);
        topY    = ceilf((topYo    - lineWidthY) * deltaY);
        rightX  = ceilf((rightXo  + lineWidthX) * deltaX);
        bottomY = ceilf((bottomYo + lineWidthY) * deltaY);
    
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
  } /* end of rectangleObjectThreeHandler                                     */

/******************************************************************************/
/* radioButtonOneHandler() :                                                  */
/******************************************************************************/

graphicsError_t radioButtonOneHandler(GRAPHICS_VOID_PTR handlingMode)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/
/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of radioButtonOneHandler                                           */

/******************************************************************************/
/* traverseHoldingRingObject() :                                              */
/******************************************************************************/

graphicsError_t traverseHoldingRingObject(guiObjectHoldingRing_tPtr ringObjectBase)
  {
/******************************************************************************/

  commonObject_tPtr objectTrack = nullptr,
                    lastObject  = nullptr;

  graphicsError_t   objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((ringObjectBase != nullptr) && (objectOutput != nullptr) && (captionPanel != nullptr))
    {
    do
      {
      objectTrack = (commonObject_tPtr)ringObjectBase->linkObject;

      while(((GRAPHICS_VOID_PTR)objectTrack) != nullptr)
        {
        switch(objectTrack->objectType)
          {
          case GRAPHICS_OBJECT_TYPE_NONE        : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                                  break;
                                                
          case GRAPHICS_OBJECT_TYPE_RECTANGLE   : lastObject = (commonObject_tPtr)objectTrack;
                                                  break;
        
          case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : lastObject = (commonObject_tPtr)objectTrack;
                                                  break;
        
          case GRAPHICS_OBJECT_TYPE_UNKNOWN     :
          default                               : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                                  break;
          }

        if (objectError != GRAPHICS_NO_ERROR)
          {
          break;
          }

        objectTrack = (commonObject_tPtr)objectTrack->nextDrawingObject;

        switch(lastObject->objectType)
          {
          case GRAPHICS_OBJECT_TYPE_RECTANGLE   : delete((rectangleObject_tPtr)lastObject);
                                                  break;

          case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : delete((staticTextObject_tPtr)lastObject);
                                                  break;

          default                               : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                                  break;
          }
        }

      if (objectError != GRAPHICS_NO_ERROR)
        {
        break;
        }

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
  } /* end of traverseHoldingRingObject                                       */

/******************************************************************************/
/* createAndStartHoldTimer() :                                                */
/******************************************************************************/

graphicsError_t createAndStartHoldTimer(HWND hWnd, GRAPHICS_ULONGLONG timerId, GRAPHICS_UINT timeOutPeriod, DETENT_TIMEOUT_DETECT timeOutDetect)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((timeOutDetect != nullptr) && (timeOutPeriod >= USER_TIMER_MINIMUM))
    {
    if ((timerId = SetTimer(NULL, ((GRAPHICS_ULONGLONG)0), timeOutPeriod, timeOutDetect)) != ((GRAPHICS_ULONGLONG)0))
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
  } /* end of createAndStartHoldTimer                                         */

/******************************************************************************/
/* stopAndDeleteHoldTimer() :                                                 */
/******************************************************************************/

graphicsError_t stopAndDeleteHoldTimer(HWND hWnd, GRAPHICS_ULONGLONG timerId)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (timerId != ((GRAPHICS_ULONGLONG)0))
    {
    if (KillTimer(hWnd, timerId) != TRUE)
      {
      objectError = GRAPHICS_OBJECT_FUNCTION_ERROR;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of stopAndDeleteHoldTimer                                          */

/******************************************************************************/
/* printHoldingRingObject() :                                                 */
/******************************************************************************/

graphicsError_t printHoldingRingObject(guiObjectHoldingRing_tPtr ringObjectBase,
                                       GRAPHICS_WCHAR_PTR        objectOutput,
                                       GRAPHICS_WCHAR_PTR        captionPanel)
  {
/******************************************************************************/

  commonObject_tPtr objectTrack = nullptr;

  graphicsError_t   objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((ringObjectBase != nullptr) && (objectOutput != nullptr) && (captionPanel != nullptr))
    {
    do
      {
      objectTrack = (commonObject_tPtr)ringObjectBase->linkObject;

      while(((GRAPHICS_VOID_PTR)objectTrack) != nullptr)
        {
        switch(objectTrack->objectType)
          {
          case GRAPHICS_OBJECT_TYPE_NONE        : wcscpy_s(objectOutput, ((rsize_t)(wcslen(GRAPHICS_OBJECT_TYPE_NONE_WSTR)) + 1), GRAPHICS_OBJECT_TYPE_NONE_WSTR);
                                                  objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                                  break;
                                                
          case GRAPHICS_OBJECT_TYPE_RECTANGLE   : wcscpy_s(objectOutput, ((rsize_t)(wcslen(GRAPHICS_OBJECT_TYPE_RECTANGLE_WSTR)) + 1), GRAPHICS_OBJECT_TYPE_RECTANGLE_WSTR);
                                                  objectError = printMessageBox(&objectOutput[0], &captionPanel[0]);
                                                  break;
        
          case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : wcscpy_s(objectOutput, ((rsize_t)(wcslen(GRAPHICS_OBJECT_TYPE_STATIC_TEXT_WSTR)) + 1), GRAPHICS_OBJECT_TYPE_STATIC_TEXT_WSTR);
                                                  objectError = printMessageBox(&objectOutput[0], &captionPanel[0]);
                                                  break;
        
          case GRAPHICS_OBJECT_TYPE_UNKNOWN     :
          default                               : wcscpy_s(objectOutput, ((rsize_t)(wcslen(GRAPHICS_OBJECT_TYPE_UNKNOWN_WSTR)) + 1), GRAPHICS_OBJECT_TYPE_UNKNOWN_WSTR);
                                                  objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                                  break;
          }

        if (objectError != GRAPHICS_NO_ERROR)
          {
          break;
          }

        objectTrack = (commonObject_tPtr)objectTrack->nextDrawingObject;
        }

      if (objectError != GRAPHICS_NO_ERROR)
        {
        break;
        }

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
  } /* end of printHoldingRingObject                                          */

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

