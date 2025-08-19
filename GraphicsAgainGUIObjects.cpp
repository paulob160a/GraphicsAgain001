/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* GraphicsAgainGUIObjects.c :                                                */
/* Paul O'Brien                                                               */
/* 18.08.25                                                                   */
/*                                                                            */
/* GUI object definition and operating constants                              */
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

objectColour_t               headlineColour                   = {
                                                                BLACK_PEN_OPAQUE
                                                                };

strokeTextStringDescriptor_t headlineString                   = {
                                                                (GRAPHICS_WCHAR_PTR)&GRAPHICS_STROKE_TEXT_HEADLINE[0],
                                                                  {
                                                                  HEADLINE_TEXT_ANCHOR_X,
                                                                  HEADLINE_TEXT_ANCHOR_Y
                                                                  },
                                                                  HEADLINE_CHARACTER_WIDTH,
                                                                  HEADLINE_CHARACTER_DEPTH,
                                                                  {
                                                                    {
                                                                    HEADLINE_TEXT_ANCHOR_X,
                                                                    HEADLINE_TEXT_ANCHOR_Y
                                                                    },
                                                                    {
                                                                    HEADLINE_TEXT_WIDTH_X,        
                                                                    HEADLINE_TEXT_DEPTH_Y
                                                                    }
                                                                  },
                                                                  {
                                                                  HEADLINE_TEXT_WIDTH_SPACING_X,
                                                                  HEADLINE_TEXT_DEPTH_SPACING_Y
                                                                  },
                                                                HEADLINE_TEXT_LINEWIDTH,
                                                                headlineColour,
                                                                true
                                                                };

characterExtentsReal_t  strokeTextStringBoundary              = {
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

// Define the first rectangle
                                      
rectangleObject_tPtr      rectangleObjectOne                  = nullptr;                                       
    
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
staticTextObject_tPtr     rectangleOneText                    = nullptr;

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
rectangleObject_tPtr      rectangleObjectTwo                  = nullptr;                                      
                                                              
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
                                                                                                      
rectangleObject_tPtr      rectangleObjectThree                = nullptr;                                           
                                                              
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

strokeFrame_t              characterFrame                   = { 
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

// Radiobutton One Start : 
radioButtonObject_tPtr     radioButtonOne                   = nullptr;

circularObject_tPtr        radioButtonOnePush               = nullptr;

detentObject_tPtr          radioButtonOneDetent             = nullptr;

circularObjectDescriptor_t radioButtonOnePushDefinition     = {
                                                              GRAPHICS_CIRCULAR_OBJECT_TYPE_CONTINUOUS,
                                                              RADIO_BUTTON_ONE_PUSH_RADIUS,
                                                              RADIO_BUTTON_ONE_PUSH_SEGMENTS,
                                                              RADIO_BUTTON_ONE_PUSH_LINE_WIDTH,
                                                              RADIO_BUTTON_ONE_PUSH_LINE_COLOUR
                                                              };

 detentObjectDescriptor_t radioButtonOnePushDetent         = {
                                                              RADIO_BUTTON_ONE_DETENT_OUT_COLOUR,
                                                              RADIO_BUTTON_ONE_DETENT_IN_COLOUR,
                                                              GRAPHICS_DETENT_FLAG_UNINITIALISED,
                                                              GRAPHICS_DETENT_FLAG_UNINITIALISED,
                                                              RADIO_BUTTON_ONE_DETENT_FLASH_PERIOD,
                                                              RADIO_BUTTON_ONE_DETENT_TYPE,
                                                              ((GRAPHICS_ULONGLONG)0),
                                                              RADIO_BUTTON_ONE_DETENT_TIMEOUT,
                                                              detentHoldExpiryTimeout,
                                                              createAndStartHoldTimer,
                                                              stopAndDeleteHoldTimer
                                                              };

radioButtonDescriptor_t    radioButtonOneDefinition        = { 
                                                               {
                                                               ((GRAPHICS_REAL)0.0),                           // button centre coordinate 'x'
                                                               ((GRAPHICS_REAL)0.0)                            // button centre coordinate 'y'
                                                               },                                              
                                                               nullptr,                                        // allocated dynamically at radiobutton instantiation
                                                               true,                                           // rectangular outline is required
                                                               true,                                           // button text is required
                                                               nullptr,                                        // static text definition goes here
                                                               GRAPHICS_RADIO_BUTTON_TEXT_PLACEMENT_UNDEFINED, // use default placement behaviour
                                                               true
                                                             };

graphicsActiveObject_t    radioButtonOneActiveBehaviour    = { 
                                                               GRAPHICS_ACTIVE_TYPE_DYNAMIC,
                                                               {
                                                               ((GRAPHICS_REAL)0.0),
                                                               ((GRAPHICS_REAL)0.0),
                                                               ((GRAPHICS_REAL)0.0),
                                                               ((GRAPHICS_REAL)0.0),
                                                               ((GRAPHICS_UINT)0.0)
                                                               },
                                                               radioButtonOneHandler
                                                             };
// Radiobutton One End : 

/******************************************************************************/
/* drawStrokeCharacter() :                                                    */
/******************************************************************************/

graphicsError_t drawStrokeCharacter(       HDC                          hdc,
                                     const GRAPHICS_UINT                strokeCharacter,
                                           alphabetCharacters_tPtr      characterList,
                                     const strokeFrame_tPtr             characterFrame,
                                     const canvasDescriptor_tPtr        canvasSize,
                                     const strokeGraphPointBase_tPtr    strokeGraphBase)
  {
/******************************************************************************/

  Graphics         graphics(hdc);

  GRAPHICS_UINT    segmentIndex    = ((GRAPHICS_UINT)0);
  lineSegment_tPtr nextLineSegment = nullptr;

  graphicsError_t  objectError     = GRAPHICS_NO_ERROR;

/******************************************************************************/

    objectError = buildCharacterStrokeGraph(characterFrame,
                                            strokeGraphBase);

    objectError = fetchCharacterReference( strokeCharacter,
                                           characterList,
                                          &characterReference);

    // One-by-one, point at the already extant line segments
    while ((objectError = fetchCharacterExtantSegment( segmentIndex,
                                                       characterReference,
                                                      &nextLineSegment)) == GRAPHICS_NO_ERROR)
      {
      // "strokeGraphBase" may be the key to compressing the character...
      strokeGraphBase->graphRowNumber    = 1024;
      strokeGraphBase->graphColumnNumber = 1025;

      objectError = drawLineSegment(hdc,
                                    nextLineSegment,
                                    LINE_SEGMENT_MODE_PASSIVE,
                                    (const strokeFrame_tPtr)characterFrame,
                                    (const canvasDescriptor_tPtr)canvasSize,
                                    (const strokeGraphPointBase_tPtr)strokeGraphBase);

      segmentIndex = segmentIndex + ((GRAPHICS_UINT)1);
      }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of drawStrokeCharacter                                             */

/******************************************************************************/
/* drawStrokeText() :                                                         */
/* - compute the screen positions of each stroke character in a string and    */
/*   draw the characters in the assigned colour. If enabled draw the string's */
/*   boundary rectangle                                                       */
/*   NOTE : this function also returns the top-left and bottom-right x- and   */
/*          y-coordinates of the string's boundary rectangle. This is a by-   */
/*          product of the drawing process and is principally used to draw    */
/*          the string's boundary rectangle, if it is enabled in the string's */
/*          describing structure. It is also accurate and updated at each     */
/*          screen redraw. Which means this function can be used "purely"(!)  */
/*          to compute this boundary IF the text drawing is switched off.     */
/*          Computing the boundary rectangle is quite simple given the        */
/*          information in the stroke text structure and is copied to the     */
/*          function "computeStrokeTextBoundary()"                            */
/******************************************************************************/

graphicsError_t drawStrokeText(      HDC                             hdc,
                               const strokeTextStringDescriptor_tPtr strokeTextCharacters,
                                     alphabetCharacters_tPtr         characterList,
                               const strokeFrame_tPtr                characterFrame,
                               const canvasDescriptor_tPtr           canvasSize,
                               const strokeGraphPointBase_tPtr       strokeGraphBase,
                                     characterExtentsReal_tPtr       strokeTextStringBoundary)
  {
/******************************************************************************/

  Graphics                    graphics(hdc);

  GRAPHICS_UINT               strokeCharacter          = ((GRAPHICS_UINT)0),
                              strokeCharacterIndex     = ((GRAPHICS_UINT)0);
                              
  strokeCharacterTrack_t      characterTrack           = {
                                                         ((GRAPHICS_REAL)0.0),
                                                         ((GRAPHICS_REAL)0.0),
                                                         { ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0) },
                                                         { ((GRAPHICS_REAL)0.0), ((GRAPHICS_REAL)0.0) }
                                                         };
                                                       
  size_t                      strokeStringSize         = ((size_t)0);

  alphabetCharacters_tPtr     strokeCharacterReference = nullptr;

  alphabetCharactersReal_tPtr normalisedReference      = nullptr;
                                            
  globalScreenCoordinates_t   screenCoordinates        = {
                                                         ((GRAPHICS_REAL)0.0),
                                                         ((GRAPHICS_REAL)0.0),
                                                         ((GRAPHICS_REAL)0.0),
                                                         ((GRAPHICS_REAL)0.0)
                                                         };

  staticTextObject_t          staticTextObject         = {
                                                         GRAPHICS_OBJECT_TYPE_STATIC_TEXT,
                                                         GRAPHICS_OBJECT_TYPE_NONE,
                                                         nullptr,
                                                         nullptr
                                                         };

  graphicsError_t             objectError              = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((strokeTextCharacters != nullptr) && (characterList                != nullptr) &&
      (characterFrame       != nullptr) && (canvasSize                   != nullptr) &&
      (strokeGraphBase      != nullptr) && (strokeGraphBase->graphPoints == nullptr))
    {
    if ((strokeStringSize = wcslen(strokeTextCharacters->strokeTextString)) != ((size_t)0))
      {
      if (buildCharacterStrokeGraph(characterFrame,
                                    strokeGraphBase) == GRAPHICS_NO_ERROR)
        {
        // Initialise the screen position for stroke text placement
        characterTrack.characterPlacementX = strokeTextCharacters->strokeTextAnchor.xAxisPoint;
        characterTrack.characterPlacementY = strokeTextCharacters->strokeTextAnchor.yAxisPoint;

        // Find the boundary containing the complete string; this requires building 
        // some of the parameters into the form required by "computeStrokeTextBoundary()"
        screenCoordinates.screenLeftX   = canvasSize->left;
        screenCoordinates.screenTopY    = canvasSize->top;
        screenCoordinates.screenRightX  = canvasSize->right;
        screenCoordinates.screenBottomY = canvasSize->bottom;

        staticTextObject.staticText     = strokeTextCharacters;

        for (strokeCharacterIndex = ((GRAPHICS_UINT)0); strokeCharacterIndex < strokeStringSize; strokeCharacterIndex++)
          {
          strokeCharacter = strokeTextCharacters->strokeTextString[strokeCharacterIndex];

          objectError = fetchCharacterReference( strokeCharacter,
                                                 characterList,
                                                &strokeCharacterReference);
          
          objectError = cloneAndNormaliseCharacterSegments( strokeCharacterReference,
                                                           &normalisedReference);

          objectError = drawNormalisedStrokeCharacter( hdc,
                                                       strokeTextCharacters,
                                                       normalisedReference,
                                                      &characterTrack,
                                                       canvasSize,
                                                       (const objectColour_tPtr)&strokeTextCharacters->strokeTextColour,
                                                       (const strokeGraphPointBase_tPtr)strokeGraphBase);

          // Move to the next character
          characterTrack.characterPlacementX = characterTrack.characterPlacementX + strokeTextCharacters->strokeTextCharacterWidth + strokeTextCharacters->strokeTextInterCharacterSpacing.xAxisPoint;

          // TRICKY : line-segments are originally defined in the "StrokeGraphics" application as "GRAPHICS_INT"
          //          which are 4-byte variables. Normalised line-segments are "GRAPHICS_REAL" which are "double" 
          //          and therefore 8-byte variables. The functions that fetch a character reference and fetch 
          //          line-segments originally used "GRAPHICS_INT" so the line-segment memory requirements did 
          //          not match. So "GRAPHICS_INT" is replaced by "GRAPHICS_INT64" which is an 8-byte variable.
          //          The "alphabetCharacters_t" and "alphabetCharactersReal_t" types should now correctly overlap.
          //          If this becomes a problem then both "fetch" functions and "deleteCharacter" will need to 
          //          have new "GRAPHICS_REAL" counterpart functions. Or maybe a "union" of the two types would 
          //          be more well-defined in terms of memory requirements, but require the respective functions 
          //          to recognise where to use the correct sub-structure. Or I could write a class in C++ and 
          //          have two member functions with appropriate signatures...
          deleteCharacter((alphabetCharacters_tPtr)&strokeCharacterReference);
          deleteCharacter((alphabetCharacters_tPtr)normalisedReference);
          }

        strokeTextStringBoundary->topLeft.pointX = strokeTextCharacters->strokeTextAnchor.xAxisPoint * (screenCoordinates.screenRightX  - screenCoordinates.screenLeftX);
        strokeTextStringBoundary->topLeft.pointY = (strokeTextCharacters->strokeTextAnchor.yAxisPoint - strokeTextCharacters->strokeTextInterCharacterSpacing.yAxisPoint) * (screenCoordinates.screenBottomY - screenCoordinates.screenTopY);

        characterTrack.characterPlacementX = characterTrack.characterPlacementX + strokeTextCharacters->strokeTextCharacterWidth + strokeTextCharacters->strokeTextInterCharacterSpacing.xAxisPoint;

        strokeTextStringBoundary->bottomRight.pointX = characterTrack.characterPlacementX * (screenCoordinates.screenRightX  - screenCoordinates.screenLeftX);
        strokeTextStringBoundary->bottomRight.pointY = strokeTextStringBoundary->topLeft.pointY + ((strokeTextCharacters->strokeTextCharacterDepth + (((GRAPHICS_REAL)2.0) * strokeTextCharacters->strokeTextInterCharacterSpacing.yAxisPoint)) * (screenCoordinates.screenBottomY - screenCoordinates.screenTopY));

        if (strokeTextCharacters->drawStrokeTextBoundary == true)
          {
          RectF frameF(((GRAPHICS_FLOAT)strokeTextStringBoundary->topLeft.pointX),
                       ((GRAPHICS_FLOAT)strokeTextStringBoundary->topLeft.pointY),
                       ((GRAPHICS_FLOAT)(strokeTextStringBoundary->bottomRight.pointX - strokeTextStringBoundary->topLeft.pointX)),
                       ((GRAPHICS_FLOAT)(strokeTextStringBoundary->bottomRight.pointY - strokeTextStringBoundary->topLeft.pointY)));
          
          // Select the text colour
          Pen pen(Color((BYTE)strokeGraphBase->passiveLineStrokeColour.opacity,
                        (BYTE)strokeGraphBase->passiveLineStrokeColour.red,
                        (BYTE)strokeGraphBase->passiveLineStrokeColour.green,
                        (BYTE)strokeGraphBase->passiveLineStrokeColour.blue));
          
          graphics.DrawRectangle(&pen, frameF);
          }
        }

      // Clear out all the allocated point structures
      deleteCharacterStrokeGraph(strokeGraphBase);

      // Clean "strokeGraphBase"
      strokeGraphBase->graphRowNumber    = ((GRAPHICS_UINT)0);
      strokeGraphBase->graphColumnNumber = ((GRAPHICS_UINT)0);
      strokeGraphBase->lineWidth         = ((GRAPHICS_REAL)0.0);
      strokeGraphBase->graphPoints       = nullptr;
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
  } /* end of drawStrokeText                                                  */

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

  if ((newLineSegment != nullptr) && (canvasSize != nullptr) && (characterFrame != nullptr) && (strokeGraphPoints != nullptr))
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
/* instantiateCircularObject() :                                              */
/******************************************************************************/

graphicsError_t instantiateCircularObject(      circularObject_tPtr           *circularObject,
                                          const circularObjectDescriptor_tPtr  circularObjectDefinition)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((*circularObject == nullptr) && (circularObjectDefinition != nullptr))
    {
    if ((*circularObject = ((circularObject_tPtr)calloc(((size_t)1), ((size_t)(sizeof(circularObject_t)))))) != nullptr)
      {
      (*circularObject)->objectType            = GRAPHICS_OBJECT_TYPE_CIRCULAR;
      (*circularObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_NONE;
      (*circularObject)->nextDrawingObject     = nullptr;
      (*circularObject)->circularDefinition    = circularObjectDefinition;
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
  } /* end of instantiateCircularObject                                       */

/******************************************************************************/
/* instantiateStaticTextObject() :                                            */
/******************************************************************************/

graphicsError_t instantiateStaticTextObject(      staticTextObject_tPtr           *staticTextObject,
                                            const strokeTextStringDescriptor_tPtr  strokeTextObject)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  // The object pointer cannot be active...
  if ((*staticTextObject == nullptr) && (strokeTextObject != nullptr))
    {
    if ((*staticTextObject = ((staticTextObject_tPtr)calloc(((size_t)1), ((size_t)(sizeof(staticTextObject)))))) != nullptr)
      {
      (*staticTextObject)->objectType            = GRAPHICS_OBJECT_TYPE_STATIC_TEXT;
      (*staticTextObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_NONE;
      (*staticTextObject)->nextDrawingObject     = nullptr;
      (*staticTextObject)->staticText            = strokeTextObject;
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
/* instantiateDetentObject() :                                                */
/******************************************************************************/

graphicsError_t instantiateDetentObject(      detentObject_tPtr           *detentObject,
                                        const detentObjectDescriptor_tPtr  detentObjectDefinition)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((*detentObject == nullptr) && (detentObjectDefinition != nullptr))
    {
    if ((*detentObject = ((detentObject_tPtr)calloc(((size_t)1), ((size_t)(sizeof(detentObject_t)))))) != nullptr)
      {
      (*detentObject)->objectType            = GRAPHICS_OBJECT_TYPE_DETENT;
      (*detentObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_NONE;
      (*detentObject)->nextDrawingObject     = nullptr;
      (*detentObject)->detentDefinition      = detentObjectDefinition;
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
  } /* end of instantiateDetentObject                                         */

/******************************************************************************/
/* instantiateRadioButtonObject() :                                           */
/******************************************************************************/

graphicsError_t instantiateRadioButtonObject(      radioButtonObject_tPtr        *radioButtonObject,
                                             const radioButtonDescriptor_tPtr     radioButtonDescriptor,
                                                   circularObject_tPtr           *radioButtonPush,
                                             const circularObjectDescriptor_tPtr  radioButtonPushDefinition,
                                                   detentObject_tPtr             *radioButtonPushDetent,
                                             const detentObjectDescriptor_tPtr    radioButtonPushDetentDefinition)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  // The object pointer cannot be active...
  if ((*radioButtonObject     == nullptr) && (radioButtonDescriptor           != nullptr) &&
      (*radioButtonPush       == nullptr) && (radioButtonPushDefinition       != nullptr) &&
      (*radioButtonPushDetent == nullptr) && (radioButtonPushDetentDefinition != nullptr))
    {
    if ((*radioButtonObject = ((radioButtonObject_tPtr)calloc(((size_t)1), ((size_t)(sizeof(radioButtonObject_t)))))) != nullptr)
      {
      (*radioButtonObject)->objectType            = GRAPHICS_OBJECT_TYPE_RADIO_BUTTON;
      (*radioButtonObject)->nextDrawingObjectType = GRAPHICS_OBJECT_TYPE_NONE;
      (*radioButtonObject)->nextDrawingObject     = nullptr;
      (*radioButtonObject)->radioButtonDefinition = radioButtonDescriptor;

      // Assemble the radiobutton components
      if ((objectError = instantiateCircularObject(radioButtonPush,
                                                   radioButtonPushDefinition)) == GRAPHICS_NO_ERROR)
        {
        radioButtonDescriptor->radioButtonPush = *radioButtonPush;

        if (radioButtonDescriptor->radioButtonDetent == true)
          {
          if ((objectError = instantiateDetentObject(radioButtonPushDetent,
                                                     radioButtonPushDetentDefinition)) == GRAPHICS_NO_ERROR)
            {
            radioButtonDescriptor->radioButtonDetentControl = *radioButtonPushDetent;
            }
          else
            {
            objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
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
  } /* end of instantiateRadioButtonObject                                    */

/******************************************************************************/
/* detentHoldExpiryTimeout() :                                                */
/******************************************************************************/

VOID CALLBACK detentHoldExpiryTimeout(HWND               hWnd,    // handle to window for timer messages 
                                      GRAPHICS_UINT      message, // WM_TIMER message 
                                      GRAPHICS_ULONGLONG idTimer, // timer identifier 
                                      GRAPHICS_ULONG     dwTime)  // current system time 
  {
/******************************************************************************/

  PostMessageA(hWnd,
               ((GRAPHICS_UINT)RADIO_BUTTON_EXPIRY_MESSAGE),
               ((GRAPHICS_ULONGLONG)0),
               ((GRAPHICS_LONGLONG)idTimer));

/******************************************************************************/
  } /* detentHoldExpiryTimeout                                                */

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/