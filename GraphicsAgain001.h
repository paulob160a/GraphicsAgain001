/******************************************************************************/
/*                                                                            */
/* GraphicsAgain001.h :                                                       */
/* Paul O'Brien                                                               */
/* 17.05.25                                                                   */
/******************************************************************************/

#ifndef _GRAPHICS_AGAIN_001_H_
#define _GRAPHICS_AGAIN_001_H_

/******************************************************************************/

#include "resource.h"
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "StrokeGraphicsTypes.h"
#include "StrokeGraphicsAlphabet.h"
#include "GraphicsAgainGUIObjects.h"
//#include "GraphicsAgainUtilities.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/

#define MAX_LOADSTRING                     (128)

#define GDI_PLUS_WINDOW_MENU_STATE         ((BOOL)FALSE)

#define CANVAS_SIZE_ZERO_X                 ((double)64.0)
#define CANVAS_SIZE_ZERO_Y                 ((double)64.0)
#define CANVAS_SIZE_WIDTH                  ((double)1680.0)
#define CANVAS_SIZE_HEIGHT                 ((double)840.0)

#define GDI_PLUS_WINDOW_RECTANGLE_X_LEFT   ((LONG)CANVAS_SIZE_ZERO_X)
#define GDI_PLUS_WINDOW_RECTANGLE_Y_TOP    ((LONG)CANVAS_SIZE_ZERO_Y)
#define GDI_PLUS_WINDOW_RECTANGLE_X_BOTTOM ((LONG)CANVAS_SIZE_WIDTH)
#define GDI_PLUS_WINDOW_RECTANGLE_Y_BOTTOM ((LONG)CANVAS_SIZE_HEIGHT)

#define WINDOW_CLASS_NAME                  L"GraphicsAgain001"
#define WINDOW_CAPTION                     L"GraphicsAgain001"

#define GRAPHICS_AGAIN_TITLE               L"GraphicsAgain001"
#define GRAPHICS_AGAIN_CLASS               L"GraphicsAgain001"

#define GRAPHICS_AGAIN_ARGUMENT_0          (0)
#define GRAPHICS_AGAIN_ARGUMENT_1          (GRAPHICS_AGAIN_ARGUMENT_0 + 1)
#define GRAPHICS_AGAIN_ARGUMENT_2          (GRAPHICS_AGAIN_ARGUMENT_1 + 1)
#define GRAPHICS_AGAIN_ARGUMENT_3          (GRAPHICS_AGAIN_ARGUMENT_2 + 1)
#define GRAPHICS_AGAIN_ARGUMENTS           (GRAPHICS_AGAIN_ARGUMENT_3 + 1)

#define GRAPHICS_AGAIN_SERVER_IP_ADDRESS   GRAPHICS_AGAIN_ARGUMENT_1
#define GRAPHICS_AGAIN_SERVER_PORT_NUMBER  GRAPHICS_AGAIN_ARGUMENT_2
#define GRAPHICS_AGAIN_ALPHABET_FILE       GRAPHICS_AGAIN_ARGUMENT_3

#define KEY_ESC_EXIT                       (0x1B)

#define GRAPHICS_CHAR_EOS                  (0x00)
#define GRAPHICS_WCHAR_EOS_LENGTH          (2)

// Test text strings : 
#define GRAPHICS_RECTANGLE_ONE_TEXT        L" Graphics "

/******************************************************************************/
/* The drawing object database containers and definitions :                   */
/* - NOTES :                                                                  */
/*      (i) the drawing surface (canvas )is normalised to :                   */
/*           - minimum X == ((GRAPHICS_REAL)0.0)                              */
/*           - minimum Y == ((GRAPHICS_REAL)0.0)                              */
/*           - maximum X == ((GRAPHICS_REAL)1.0)                              */
/*           - maximum Y == ((GRAPHICS_REAL)1.0)                              */
/*     (ii) the nominal drawing surface (screen) is full HD :                 */
/*          - minimum X == ((GRAPHICS_REAL)0.0)                               */
/*          - minimum Y == ((GRAPHICS_REAL)0.0)                               */
/*          - maximum X == ((GRAPHICS_REAL)1920)                              */
/*          - maximum Y == ((GRAPHICS_REAL)1280)                              */
/*                                                                            */
/*          - thus all X-axis coordinates are expressed as X / maximumX       */
/*                 all Y-axis coordinates are expressed as Y / maximumY       */
/*          - in this way, all coordinates are written as if the target       */
/*            screen was (0,0),(1920,1080) but scaled to a screen-free set    */
/*            of virtual normalised coordinates (0.0,0.0),(1.0,1.0)           */
/*                                                                            */
/*            further, the screen X- and Y- dimensions can be passed on the   */
/*            command-line to adjust for different screen sizes               */
/*                                                                            */
/*    (iii) negative values are not allowed!                                  */
/******************************************************************************/

// Basic objects :
// ALL drawing object structures start with the graphics object type - 
// to ease identification during parsing this common type can decode 
// the object type allowing easy selection of the correct graphics object 
// pointer type
#pragma pack(push, 1)
typedef struct commonObject_tTag
  {
  graphicsObjectType_t objectType;
  graphicsObjectType_t nextDrawingObjectType; // composite objects are simply chained
  GRAPHICS_VOID_PTR    nextDrawingObject;
  } commonObject_t, *commonObject_tPtr;
#pragma pack(pop)

#define GRAPHICS_SINGLE_POINT_COORDINATES (2)

typedef struct octagonDimensions_tTag
  {
  GRAPHICS_REAL leftX;     
  GRAPHICS_REAL topY;      
  GRAPHICS_REAL rightX;    
  GRAPHICS_REAL bottomY;   
  GRAPHICS_REAL midLeftX;  
  GRAPHICS_REAL midTopY;   
  GRAPHICS_REAL midRightX; 
  GRAPHICS_REAL midBottomY;
  } octagonDimensions_t, *octagonDimensions_tPtr;

typedef struct hexagonDimensions_tTag
  {
  GRAPHICS_REAL leftX;     
  GRAPHICS_REAL topY;      
  GRAPHICS_REAL rightX;    
  GRAPHICS_REAL bottomY;   
  GRAPHICS_REAL midLeftX;  
  GRAPHICS_REAL midTopY;   
  GRAPHICS_REAL midRightX; 
  GRAPHICS_REAL midBottomY;
  GRAPHICS_REAL innerMidLeft;
  GRAPHICS_REAL innerMidTopY;
  GRAPHICS_REAL innerMidRightX;
  GRAPHICS_REAL innerMidBottomY;
  GRAPHICS_REAL outerMidLeft;
  GRAPHICS_REAL outerMidTopY;
  GRAPHICS_REAL outerMidRightX;
  GRAPHICS_REAL outerMidBottomY;
  } hexagonDimensions_t, *hexagonDimensions_tPtr;

typedef struct radioButtonDimensions_tTag
  {
  } radioButtonDimensions_t, *radioButtonDimensions_tPtr;

typedef struct randomDimensions_tTag
  {
  GRAPHICS_UINT    numberOfPoints;
  singlePoint_tPtr singlePoint;
  } randomDimensions_t, *randomDimensionsPtr;

// The common structure type allows for a more complex object detection boundary.
// Nominally the points are interleaved rectangles: 
//    (i) rectangle [x1 rectangle] 
//   (ii) octagon   [x2 rectangle]
//  (iii) hexagon   [x3 rectangle]
// but could describe a boundary composed of three (duplicated points in 
// the octagon structure) to sixteen
typedef union commonObjectDimensions_tTag
  {
  rectangleDimensions_t rectangularBoundary;
  octagonDimensions_t   octagonBoundary;
  hexagonDimensions_t   hexagonBoundary;
  } commonObjectDimensions_t, *commonObjectDimensions_tPtr;

// Normalised canvas dimensions
#define GUI_CANVAS_GUI_NOMINAL_LEFT_X   ((GRAPHICS_REAL)0.0)
#define GUI_CANVAS_GUI_NOMINAL_TOP_Y    ((GRAPHICS_REAL)0.0)
#define GUI_CANVAS_GUI_NOMINAL_RIGHT_X  ((GRAPHICS_REAL)1.0)
#define GUI_CANVAS_GUI_NOMINAL_BOTTOM_Y ((GRAPHICS_REAL)1.0)

// Default screen/canvas dimensions
#define GUI_SCREEN_GUI_DEFAULT_LEFT_X   ((GRAPHICS_REAL)0.0)
#define GUI_SCREEN_GUI_DEFAULT_TOP_Y    ((GRAPHICS_REAL)0.0)
#define GUI_SCREEN_GUI_DEFAULT_RIGHT_X  ((GRAPHICS_REAL)1920.0)
#define GUI_SCREEN_GUI_DEFAULT_BOTTOM_Y ((GRAPHICS_REAL)1280.0)

typedef struct globalScreensCoordinates_tTag
  {
  GRAPHICS_REAL screenLeftX;
  GRAPHICS_REAL screenTopY;
  GRAPHICS_REAL screenRightX;
  GRAPHICS_REAL screenBottomY;
  } globalScreenCoordinates_t, *globalScreenCoordinates_tPtr;
  
// Define how to apply new object numbering in the instantiation functions
typedef enum guiObjectNumberingControl_tTag
  {
  GRAPHICS_OBJECT_NUMBERING_NONE = 0,    // do not alter the object number
  GRAPHICS_OBJECT_NUMBERING_IMMEDIATE,   // use the number passed to the function
  GRAPHICS_OBJECT_NUMBERING_INCREMENTAL, // add one to the existing object number
  GRAPHICS_OBJECT_NUMBERING_CUMULATIVE,  // add the number passed to the function to the object number
  GRAPHICS_OBJECT_NUMBERING_RESET,       // set the object number to zero
  GRAPHICS_OBJECT_NUMBERING_CONTROLS
  } guiObjectNumberingControl_t;

typedef enum graphicsActiveType_tTag
  {
  GRAPHICS_ACTIVE_TYPE_NONE = 0,
  GRAPHICS_ACTIVE_TYPE_NOT_ACTIVE,
  GRAPHICS_ACTIVE_TYPE_STATIC,
  GRAPHICS_ACTIVE_TYPE_DYNAMIC,
  GRAPHICS_ACTIVE_TYPES
  } graphicsActiveType_t;

// Definition of active objects and handling functions. This is intended to push 
// item detection and control up to the holding ring objects
#pragma pack(push,1)
typedef struct graphicsActiveObject_tTag
  {
  graphicsActiveType_t       activeType;
  commonObjectDimensions_t   activeRegion;
  graphicsError_t          (*activityHandler)(GRAPHICS_VOID_PTR handlingMode);
  } graphicsActiveObject_t, *graphicsActiveObject_tPtr;
#pragma pack(pop)

// Object holding ring link object :
#pragma pack(push,1)
typedef struct guiObjectHoldingRing_tTag
  {
  GRAPHICS_VOID_PTR                 linkObject;
  graphicsObjectType_t              linkObjectType;
  graphicsActiveObject_t            activeObject;
  GRAPHICS_INT                      numberOfRingLinkObjects;
  struct guiObjectHoldingRing_tTag *lastLink;
  struct guiObjectHoldingRing_tTag *nextLink;
  } guiObjectHoldingRing_t, *guiObjectHoldingRing_tPtr;
#pragma pack(pop)

// The MINIMAL space division of a character frame
#define STROKE_FRAME_X_MINIMUM_POINTS_UINT ((GRAPHICS_UINT)3)
#define STROKE_FRAME_Y_MINIMUM_POINTS_UINT ((GRAPHICS_UINT)3) 

// The NOMINAL space division of a character frame
#define STROKE_FRAME_X_AXIS_POINTS         ((GRAPHICS_REAL)16.0) // 'x' = 0.0 --> point 0, 'x' = 1.0 --> point 15
#define STROKE_FRAME_Y_AXIS_POINTS         ((GRAPHICS_REAL)16.0) // 'y' = 0.0 --> point 0, 'y' = 1.0 --> point 15

// The normalised stroke frame coordinates
#define STROKE_FRAME_LEFT_X_NORMAL         ((GRAPHICS_REAL)0.0)
#define STROKE_FRAME_TOP_Y_NORMAL          ((GRAPHICS_REAL)0.0)
#define STROKE_FRAME_RIGHT_X_NORMAL        ((GRAPHICS_REAL)1.0)
#define STROKE_FRAME_BOTTOM_Y_NORMAL       ((GRAPHICS_REAL)1.0)
#define STROKE_FRAME_DELTA_X_NORMAL        (STROKE_FRAME_RIGHT_X_NORMAL  - STROKE_FRAME_LEFT_X_NORMAL)
#define STROKE_FRAME_DELTA_Y_NORMAL        (STROKE_FRAME_BOTTOM_Y_NORMAL - STROKE_FRAME_TOP_Y_NORMAL)

// The NOMINAL stroke frame coordinates
#define STROKE_FRAME_WIDTH                 ((GRAPHICS_REAL)0.6)
#define STROKE_FRAME_DEPTH                 ((GRAPHICS_REAL)0.8)
                                          
#define STROKE_FRAME_LEFT_X                ((GRAPHICS_REAL)0.05)
#define STROKE_FRAME_TOP_Y                 ((GRAPHICS_REAL)0.05)
#define STROKE_FRAME_RIGHT_X               (STROKE_FRAME_LEFT_X + STROKE_FRAME_WIDTH)
#define STROKE_FRAME_BOTTOM_Y              (STROKE_FRAME_TOP_Y  + STROKE_FRAME_DEPTH)

#define STROKE_FRAME_DETECT_ODD_EVEN       ((GRAPHICS_UINT)0x00000001)

#define STROKE_FRAME_COLOUR                BLUE_PEN // DO NOT ENCLOSE THIS MACRO IN BRACKETS '('..')' OR '{'..'}'
#define STROKE_FRAME_COLOUR_CODE           ((GRAPHICS_UINT)BLUE_PEN_CODE)

#define DESCENT_ROW_PROPORTION             ((GRAPHICS_REAL)0.8) // used to derive the row number where a characters'
                                                                // line segments are considered part of a descender

#define MIDDLE_FRAME_COLOUR                WHITE_PEN_OPAQUE
#define MIDDLE_FRAME_COLOUR_CODE           ((GRAPHICS_UINT)WHITE_PEN_OPAQUE_CODE)

#define DESCENT_FRAME_COLOUR               YELLOW_PEN
#define DESCENT_FRAME_COLOUR_CODE          ((GRAPHICS_UINT)YELLOW_PEN_CODE)

#define GRAPHICS_CHAR_LOWER                 (0)
#define GRAPHICS_CHAR_UPPER                 (GRAPHICS_CHAR_LOWER + 1)
#define GRAPHICS_CHAR_LENGTH                (GRAPHICS_CHAR_UPPER + 1)
#define GRAPHICS_WORD_LENGTH                (16)

/******************************************************************************/

extern globalScreenCoordinates_t    globalScreenCoordinates;

extern WCHAR                        objectOutput[GRAPHICS_RING_OUTPUT_WSTR_LENGTH];
extern WCHAR                        captionPanel[GRAPHICS_RING_OUTPUT_WSTR_LENGTH];
                                    
extern canvasDescriptor_t           canvasSize;
extern canvasDescriptor_t           localCanvasSize;
                                    
extern alphabetCharacters_tPtr      characterReference;
                                    
extern strokeGraphPointBase_t       strokeGraphPointBase;
                                    
extern bool                         mouseOverObject;
extern bool                         objectPositionTestFlag;
extern GRAPHICS_SHORT               mouseXPosition;
extern GRAPHICS_SHORT               mouseYPosition;
                                    
extern guiObjectHoldingRing_tPtr    guiObjectHoldingRingBaseLink;
extern guiObjectHoldingRing_tPtr    guiObjectHoldingRingCurrent;
extern guiObjectHoldingRing_tPtr    guiObjectHoldingRingFreePtr;

extern graphicsActiveObject_t       rectangleObjectOneActiveBehaviour;
extern graphicsActiveObject_t       rectangleOneTextActiveBehaviour;
extern graphicsActiveObject_t       rectangleObjectTwoActiveBehaviour; 

extern graphicsActiveObject_t       radioButtonOneActiveBehaviour;

/******************************************************************************/

extern graphicsError_t instantiateObjectHoldingRingObject(      guiObjectHoldingRing_tPtr   *objectHoldingRingObject,
                                                          const guiObjectNumberingControl_t  guiObjectNumberingControl,
                                                          const GRAPHICS_INT                 guiObjectNumber);
extern graphicsError_t instantiateStaticTextObject(      staticTextObject_tPtr           *staticTextObject,
                                                   const strokeTextStringDescriptor_tPtr  strokeTextObject);
extern graphicsError_t instantiateRectangleObject(      rectangleObject_tPtr  *rectangleObject,
                                                  const rectangleDimensions_t  rectangleDimensions,
                                                  const objectColour_t         rectangleColour);

extern graphicsError_t applyObjectNumberingControl(const GRAPHICS_INT                newNumber,
                                                         GRAPHICS_INT_PTR            objectNumber,
                                                   const guiObjectNumberingControl_t numberingControl);

extern graphicsError_t addNewHoldingRingObject(      guiObjectHoldingRing_tPtr   *newObjectHoldingRingObject,
                                               const guiObjectNumberingControl_t  guiObjectNumberingControl,
                                               const GRAPHICS_INT                 guiObjectNumber);
extern graphicsError_t addGuiObjectToHoldingRing(guiObjectHoldingRing_tPtr  insertionIndex,
                                                 GRAPHICS_VOID_PTR          newObject,
                                                 graphicsActiveObject_tPtr  activeBehaviour,
                                                 graphicsObjectType_t       graphicsObjectType);

extern graphicsError_t addGuiObjectToGuiChain(GRAPHICS_VOID_PTR newGuiObject,
                                              GRAPHICS_VOID_PTR lastGuiObject);


extern graphicsError_t computeStrokeTextBoundary(const strokeTextStringDescriptor_tPtr strokeTextCharacters,
                                                 const canvasDescriptor_tPtr           canvasSize,
                                                       characterExtentsReal_tPtr       strokeTextStringBoundary);

extern graphicsError_t computeCompositeObjectBoundary(      guiObjectHoldingRing_tPtr objectHoldingRingRoot,
                                                      const alphabetCharacters_tPtr   characterList,
                                                      const canvasDescriptor_tPtr     canvasSize);

extern graphicsError_t guiDrawRectangle(HDC                 hdc,
                                        commonObject_tPtr   graphicsObject,
                                        canvasDescriptor_t *canvasSize);

extern graphicsError_t incrementallyTraverseHoldingRing(guiObjectHoldingRing_tPtr *objectHoldingRingCurrentIndex,
                                                        guiObjectHoldingRing_tPtr *objectHoldingRingNextIndex);

extern bool            testHoldingRingObjectType(graphicsObjectType_t objectType);

extern graphicsError_t detectMouseOverObject(const guiObjectHoldingRing_tPtr  guiObjectHoldingRing,
                                             const GRAPHICS_SHORT             xPosition,
                                             const GRAPHICS_SHORT             yPosition,
                                             const canvasDescriptor_t        *canvasSize,
                                                   bool                      *mouseOverObject);

extern graphicsError_t rectangleObjectOneHandler(GRAPHICS_VOID_PTR handlingMode);
extern graphicsError_t rectangleObjectTwoHandler(GRAPHICS_VOID_PTR handlingMode);
extern graphicsError_t rectangleObjectThreeHandler(GRAPHICS_VOID_PTR handlingMode);

extern graphicsError_t radioButtonOneHandler(GRAPHICS_VOID_PTR handlingMode);

extern graphicsError_t traverseHoldingRingObject(guiObjectHoldingRing_tPtr ringObjectBase);
extern graphicsError_t printHoldingRingObject(guiObjectHoldingRing_tPtr ringObjectBase,
                                              GRAPHICS_WCHAR_PTR        objectOutput,
                                              GRAPHICS_WCHAR_PTR        captionPanel);
extern graphicsError_t printMessageBox(WCHAR *message, WCHAR *caption);

extern graphicsError_t setCanvasCoordinates(canvasDescriptor_t *canvasSize,
                                            GRAPHICS_REAL       left,
                                            GRAPHICS_REAL       top,
                                            GRAPHICS_REAL       right,
                                            GRAPHICS_REAL       bottom,
                                            GRAPHICS_REAL       width,
                                            GRAPHICS_REAL       height);

extern graphicsError_t buildCharacterStrokeGraph(strokeFrame_tPtr          characterFrame,
                                                 strokeGraphPointBase_tPtr strokeGraphBase);

extern graphicsError_t deleteCharacterStrokeGraph(strokeGraphPointBase_tPtr  strokeGraphBase);

extern VOID            OnPaint(HDC                        hdc,
                               canvasDescriptor_t        *canvasSize,
                               guiObjectHoldingRing_tPtr  guiObjectHoldingRingBaseLink,
                               alphabetCharacters_tPtr    characterList,
                               strokeFrame_tPtr           characterFrame,
                               GRAPHICS_SHORT_PTR         mouseXPosition,
                               GRAPHICS_SHORT_PTR         mouseYPosition,
                               strokeGraphPointBase_tPtr  strokeGraphBase);

extern graphicsError_t drawLineSegment(      HDC                       hdc,
                                       const lineSegment_tPtr          newLineSegment,
                                       const lineSegmentMode_t         newLineSegmentMode,
                                       const strokeFrame_tPtr          characterFrame,
                                       const canvasDescriptor_tPtr     canvasSize,
                                       const strokeGraphPointBase_tPtr strokeGraphPoints);

/******************************************************************************/

#endif

/******************************************************************************/
