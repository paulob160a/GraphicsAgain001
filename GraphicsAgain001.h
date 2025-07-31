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
//#include "GraphicsAgainUtilities.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/

#define MAX_LOADSTRING                     (128)

#define GDI_PLUS_WINDOW_MENU_STATE         ((BOOL)FALSE)

#define CANVAS_SIZE_ZERO_X                 ((double)64.0)
#define CANVAS_SIZE_ZERO_Y                 ((double)64.0)
#define CANVAS_SIZE_WIDTH                  ((double)1440.0)
#define CANVAS_SIZE_HEIGHT                 ((double)768.0)

#define GDI_PLUS_WINDOW_RECTANGLE_X_LEFT   ((LONG)CANVAS_SIZE_ZERO_X)
#define GDI_PLUS_WINDOW_RECTANGLE_Y_TOP    ((LONG)CANVAS_SIZE_ZERO_Y)
#define GDI_PLUS_WINDOW_RECTANGLE_X_BOTTOM ((LONG)CANVAS_SIZE_WIDTH)
#define GDI_PLUS_WINDOW_RECTANGLE_Y_BOTTOM ((LONG)CANVAS_SIZE_HEIGHT)

#define WINDOW_CLASS_NAME                  L"GraphicsAgain001"
#define WINDOW_CAPTION                     L"GraphicsAgain001"

#define GRAPHICS_AGAIN_TITLE               L"GraphicsAgain001"
#define GRAPHICS_AGAIN_CLASS               L"GraphicsAgain001"

#define GRAPHICS_AGAIN_ARGUMENT_0           (0)
#define GRAPHICS_AGAIN_ARGUMENT_1           (GRAPHICS_AGAIN_ARGUMENT_0 + 1)
#define GRAPHICS_AGAIN_ARGUMENT_2           (GRAPHICS_AGAIN_ARGUMENT_1 + 1)
#define GRAPHICS_AGAIN_ARGUMENT_3           (GRAPHICS_AGAIN_ARGUMENT_2 + 1)
#define GRAPHICS_AGAIN_ARGUMENTS            (GRAPHICS_AGAIN_ARGUMENT_3 + 1)

#define GRAPHICS_AGAIN_SERVER_IP_ADDRESS    GRAPHICS_AGAIN_ARGUMENT_1
#define GRAPHICS_AGAIN_SERVER_PORT_NUMBER   GRAPHICS_AGAIN_ARGUMENT_2
#define GRAPHICS_AGAIN_ALPHABET_FILE        GRAPHICS_AGAIN_ARGUMENT_3

#define KEY_ESC_EXIT                        (0x1B)

#define GRAPHICS_CHAR_EOS                   (0x00)
#define GRAPHICS_WCHAR_EOS_LENGTH           (2)

// Test text strings : 
#define GRAPHICS_RECTANGLE_ONE_TEXT         L" Graphics "

/******************************************************************************/

// Ring Objects :
#define GRAPHICS_RING_OUTPUT_WSTR_LENGTH    (256)

#define GRAPHICS_OBJECT_CAPTION_WSTR        L"GUI Graphics Development"
#define GRAPHICS_OBJECT_TYPE_NONE_WSTR      L"GRAPHICS_OBJECT_TYPE_NONE"
#define GRAPHICS_OBJECT_TYPE_RECTANGLE_WSTR L"GRAPHICS_OBJECT_TYPE_RECTANGLE"
#define GRAPHICS_OBJECT_TYPE_UNKNOWN_WSTR   L"GRAPHICS_OBJECT_TYPE_UNKNOWN_STR"

#define GRAPHICS_OBJECT_TYPE_CODE           (0x0FEDC000)

typedef enum graphicsObjectType_tTag
  {
  GRAPHICS_OBJECT_TYPE_NONE        = GRAPHICS_OBJECT_TYPE_CODE,
  GRAPHICS_OBJECT_TYPE_INDEX       = (GRAPHICS_OBJECT_TYPE_NONE        + 0x00000001),
  GRAPHICS_OBJECT_TYPE_RECTANGLE   = (GRAPHICS_OBJECT_TYPE_INDEX       + 0x00000001),
  GRAPHICS_OBJECT_TYPE_OCTAGON     = (GRAPHICS_OBJECT_TYPE_RECTANGLE   + 0x00000001),
  GRAPHICS_OBJECT_TYPE_HEXAGON     = (GRAPHICS_OBJECT_TYPE_OCTAGON     + 0x00000001),
  GRAPHICS_OBJECT_TYPE_RANDOM      = (GRAPHICS_OBJECT_TYPE_HEXAGON     + 0x00000001),
  GRAPHICS_OBJECT_TYPE_STATIC_TEXT = (GRAPHICS_OBJECT_TYPE_RANDOM      + 0x00000001),
  GRAPHICS_OBJECT_TYPE_COMPOSITE   = (GRAPHICS_OBJECT_TYPE_STATIC_TEXT + 0x00000001), // complex object tree root
  GRAPHICS_OBJECT_TYPE_UNKNOWN     = (GRAPHICS_OBJECT_TYPE_COMPOSITE   + 0x00000001),
  GRAPHICS_OBJECT_TYPES            = (GRAPHICS_OBJECT_TYPE_UNKNOWN     + 0x00000001)
  } graphicsObjectType_t, *graphicsObjectType_tPtr;

typedef struct canvasDescriptor_tTag
  {
  GRAPHICS_REAL left;
  GRAPHICS_REAL top;
  GRAPHICS_REAL right;
  GRAPHICS_REAL bottom;
  GRAPHICS_REAL width;
  GRAPHICS_REAL height;
  GRAPHICS_REAL centreX;
  GRAPHICS_REAL centreY;
  } canvasDescriptor_t, *canvasDescriptor_tPtr;

/******************************************************************************/
/* Colour Palette : the definitions and their enumerations must be kept in    */
/*                  lock-step. New colour definitions must be added to the    */
/*                  next numeral in the enumeration list                      */
/******************************************************************************/

// Have to appeal to the power of 'C' to extract the colour values!

#define SELECT_BYTE_0                                       ((GRAPHICS_UINT)0)
#define UNSIGNED_INT_MASK_BYTE_0                            ((GRAPHICS_UINT)0x000000ff)
#define UNSIGNED_INT_SHIFT_BYTE_0                           (0)
    
#define SELECT_BYTE_1                                       ((GRAPHICS_UINT)1)
#define UNSIGNED_INT_MASK_BYTE_1                            ((GRAPHICS_UINT)0x0000ff00)
#define UNSIGNED_INT_SHIFT_BYTE_1                           (8)
                         
#define SELECT_BYTE_2                                       ((GRAPHICS_UINT)2)
#define UNSIGNED_INT_MASK_BYTE_2                            ((GRAPHICS_UINT)0x00ff0000)
#define UNSIGNED_INT_SHIFT_BYTE_2                           (UNSIGNED_INT_SHIFT_BYTE_1 << 1)
                                                            
#define SELECT_BYTE_3                                       ((GRAPHICS_UINT)3)
#define UNSIGNED_INT_MASK_BYTE_3                            ((GRAPHICS_UINT)0xff000000)
#define UNSIGNED_INT_SHIFT_BYTE_3                           ((UNSIGNED_INT_SHIFT_BYTE_1 << 1) + UNSIGNED_INT_SHIFT_BYTE_1)

#define EXTRACT_COLOUR_FIELD_BYTE(colourField,selectedByte,requestedByte) { switch(selectedByte) \
                                                                              { \
                                                                              case SELECT_BYTE_0 : *requestedByte = (GRAPHICS_BYTE)((((GRAPHICS_UINT)colourField) & UNSIGNED_INT_MASK_BYTE_0) >> UNSIGNED_INT_SHIFT_BYTE_0); \
                                                                                                   break; \
                                                                              case SELECT_BYTE_1 : *requestedByte = (GRAPHICS_BYTE)((((GRAPHICS_UINT)colourField) & UNSIGNED_INT_MASK_BYTE_1) >> UNSIGNED_INT_SHIFT_BYTE_1); \
                                                                                                   break; \
                                                                              case SELECT_BYTE_2 : *requestedByte = (GRAPHICS_BYTE)((((GRAPHICS_UINT)colourField) & UNSIGNED_INT_MASK_BYTE_2) >> UNSIGNED_INT_SHIFT_BYTE_2); \
                                                                                                   break; \
                                                                              case SELECT_BYTE_3 : *requestedByte = (GRAPHICS_BYTE)((((GRAPHICS_UINT)colourField) & UNSIGNED_INT_MASK_BYTE_3) >> UNSIGNED_INT_SHIFT_BYTE_3); \
                                                                                                   break; \
                                                                              default            : *requestedByte = ((GRAPHICS_BYTE)0); \
                                                                                                   break; \
                                                                              } \
                                                                          }
#define FULL_COLOUR_CHANNEL    ((BYTE)0xff)
#define NULL_COLOUR_CHANNEL    ((BYTE)0x00)
                               
#define FULL_OPAQUE_CHANNEL    ((BYTE)0xff)
#define NULL_OPAQUE_CHANNEL    ((BYTE)0x00)
                               
#define NO_COLOUR_PEN          ((BYTE)0x00), \
                               ((BYTE)0x00), \
                               ((BYTE)0x00), \
                               ((BYTE)0x00) // transparent pen

#define NO_COLOUR_PEN_CODE     ((GRAPHICS_UINT)((NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                 NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                 NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                 NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

#define DARK_GREEN_PEN         ((BYTE)0xFF), \
                               ((BYTE)0x16), \
                               ((BYTE)0x96), \
                               ((BYTE)0x19) // opaque DARK GREEN pen
                               
#define DARK_GREEN_PEN_CODE    ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (((BYTE)0x16)        << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (((BYTE)0x96)        << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (((BYTE)0x19)        << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define BRIGHT_YELLOW_PEN      ((BYTE)0xFF), \
                               ((BYTE)0xFF), \
                               ((BYTE)0xFF), \
                               ((BYTE)0x00) // opaque BRIGHT YELLOW pen

#define BRIGHT_YELLON_PEN_CODE ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (((BYTE)0xFF)        << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (((BYTE)0xFF)        << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (((BYTE)0x00)        << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

#define LIGHT_BLUE_PEN         ((BYTE)0xFF), \
                               ((BYTE)0x84), \
                               ((BYTE)0xBC), \
                               ((BYTE)0xEF) // opaque LIGHT BLUE pen
                               
#define LIGHT_BLUE_PEN_CODE    ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (((BYTE)0x84)        << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (((BYTE)0xBC)        << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (((BYTE)0xEF)        << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define BLUE_PEN               FULL_OPAQUE_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL  // opaque BLUE pen
                               
#define BLUE_PEN_CODE          ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (FULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define GREEN_PEN              FULL_OPAQUE_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL  // opaque GREEN pen 
                               
#define GREEN_PEN_CODE         ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define RED_PEN                FULL_OPAQUE_CHANNEL, \
                               FULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL  // opaque RED pen
                               
#define RED_PEN_CODE           ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

#define CYAN_PEN               FULL_OPAQUE_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL  // opaque CYAN pen
                               
#define CYAN_PEN_CODE          ((GRAPHICS_UINT) ((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                 (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                 (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                 (FULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define MAGENTA_PEN            FULL_OPAQUE_CHANNEL, \
                               FULL_COLOUR_CHANNEL, \
                               NULL_COLOUR_CHANNEL, \
                               FULL_COLOUR_CHANNEL  // opaque MAGENTA pen
                            
#define MAGENTA_PEN_CODE       ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (FULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define YELLOW_PEN              FULL_OPAQUE_CHANNEL, \
                                FULL_COLOUR_CHANNEL, \
                                FULL_COLOUR_CHANNEL, \
                                NULL_COLOUR_CHANNEL  // opaque YELLOW pen
                               
#define YELLOW_PEN_CODE        ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define BLACK_PEN_OPAQUE        FULL_OPAQUE_CHANNEL, \
                                NULL_COLOUR_CHANNEL, \
                                NULL_COLOUR_CHANNEL, \
                                NULL_COLOUR_CHANNEL  // opaque BLACK pen
                               
#define BLACK_PEN_CODE         ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (NULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (NULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))
                               
#define WHITE_PEN_OPAQUE        FULL_OPAQUE_CHANNEL, \
                                FULL_COLOUR_CHANNEL, \
                                FULL_COLOUR_CHANNEL, \
                                FULL_COLOUR_CHANNEL  // opaque WHITE pen
                               
#define WHITE_PEN_OPAQUE_CODE   ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                 (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                 (FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                 (FULL_OPAQUE_CHANNEL << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

typedef enum lineSegmentColours_tTag
  {
  LINE_SEGMENT_COLOUR_BLUE = 0,
  LINE_SEGMENT_COLOUR_GREEN,
  LINE_SEGMENT_COLOUR_RED,
  LINE_SEGMENT_COLOUR_CYAN,
  LINE_SEGMENT_COLOUR_MAGENTA,
  LINE_SEGMENT_COLOUR_YELLOW,
  LINE_SEGMENT_COLOUR_BLACK_OPAQUE,
  LINE_SEGMENT_COLOUR_WHITE_OPAQUE,
  LINE_SEGMENT_COLOUR_DARK_GREEN,
  LINE_SEGMENT_COLOUR_BRIGHT_YELLOW,
  LINE_SEGMENT_COLOUR_LIGHT_BLUE,
  LINE_SEGMENT_COLOURS
  } lineSegmentColours_t, *lineSegmentColours_tPtr;

#pragma pack(push,1)
typedef struct colourPalette_tTag
  {
  BYTE colourOpacity;
  BYTE colourRedComponent;
  BYTE colourGreenComponent;
  BYTE colourBlueComponent;
  } colourPalette_t, *colourPalette_tPtr;
#pragma pack(pop)

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

typedef struct rectangleDimensions_tTag
  {
  GRAPHICS_REAL leftX;
  GRAPHICS_REAL topY;
  GRAPHICS_REAL rightX;
  GRAPHICS_REAL bottomY;
  GRAPHICS_UINT lineWidth;
  } rectangleDimensions_t, *rectangleDimensions_tPtr;

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

#define RECTANGLE_OBJECT_ONE_BASE_X      ((GRAPHICS_REAL)10.0)
#define RECTANGLE_OBJECT_ONE_BASE_Y      ((GRAPHICS_REAL)10.0)
                                        
#define RECTANGLE_OBJECT_ONE_LEFT_X      (RECTANGLE_OBJECT_ONE_BASE_X / globalScreenCoordinates.screenRightX)
#define RECTANGLE_OBJECT_ONE_TOP_Y       (RECTANGLE_OBJECT_ONE_BASE_Y / globalScreenCoordinates.screenBottomY)
#define RECTANGLE_OBJECT_ONE_RIGHT_X     ((RECTANGLE_OBJECT_ONE_BASE_X + 20.0) / globalScreenCoordinates.screenRightX)
#define RECTANGLE_OBJECT_ONE_BOTTOM_Y    ((RECTANGLE_OBJECT_ONE_BASE_Y + 20.0) / globalScreenCoordinates.screenBottomY)
#define RECTANGLE_OBJECT_ONE_LINEWIDTH   ((GRAPHICS_UINT)3)
                                         
#define RECTANGLE_OBJECT_TWO_BASE_X      ((GRAPHICS_REAL)200.0)
#define RECTANGLE_OBJECT_TWO_BASE_Y      ((GRAPHICS_REAL)50.0)
                                         
#define RECTANGLE_OBJECT_TWO_LEFT_X      (RECTANGLE_OBJECT_TWO_BASE_X / globalScreenCoordinates.screenRightX)
#define RECTANGLE_OBJECT_TWO_TOP_Y       (RECTANGLE_OBJECT_TWO_BASE_Y / globalScreenCoordinates.screenBottomY)
#define RECTANGLE_OBJECT_TWO_RIGHT_X     ((RECTANGLE_OBJECT_TWO_BASE_X + 50.0) / globalScreenCoordinates.screenRightX)
#define RECTANGLE_OBJECT_TWO_BOTTOM_Y    ((RECTANGLE_OBJECT_TWO_BASE_Y + 50.0) / globalScreenCoordinates.screenBottomY)
#define RECTANGLE_OBJECT_TWO_LINEWIDTH   ((GRAPHICS_UINT)2)
                                         
#define RECTANGLE_OBJECT_THREE_BASE_X    ((GRAPHICS_REAL)225.0)
#define RECTANGLE_OBJECT_THREE_BASE_Y    ((GRAPHICS_REAL)25.0)
                                         
#define RECTANGLE_OBJECT_THREE_LEFT_X    (RECTANGLE_OBJECT_THREE_BASE_X / globalScreenCoordinates.screenRightX)
#define RECTANGLE_OBJECT_THREE_TOP_Y     (RECTANGLE_OBJECT_THREE_BASE_Y / globalScreenCoordinates.screenBottomY)
#define RECTANGLE_OBJECT_THREE_RIGHT_X   ((RECTANGLE_OBJECT_THREE_BASE_X + 50.0) / globalScreenCoordinates.screenRightX)
#define RECTANGLE_OBJECT_THREE_BOTTOM_Y  ((RECTANGLE_OBJECT_THREE_BASE_Y + 50.0) / globalScreenCoordinates.screenBottomY)
#define RECTANGLE_OBJECT_THREE_LINEWIDTH ((GRAPHICS_UINT)1)

#pragma pack(push,1)
typedef struct rectangleObject_tTag
  {
  graphicsObjectType_t   objectType;
  graphicsObjectType_t   nextDrawingObjectType; // composite objects are simply chained
  GRAPHICS_VOID_PTR      nextDrawingObject;
  objectColour_t         rectangleColour;
  rectangleDimensions_t  rectangleDimensions;
  } rectangleObject_t,  *rectangleObject_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct staticTextObject_tTag
  {
  graphicsObjectType_t   objectType;
  graphicsObjectType_t   nextDrawingObjectType;
  GRAPHICS_VOID_PTR      nextDrawingObject;
  objectColour_t         staticTextColour;
  rectangleDimensions_t  staticTextDimensions;
  GRAPHICS_WCHAR_PTR     staticText;
  } staticTextObject_t, *staticTextObject_tPtr;
#pragma pack(pop)

// The MINIMAL space division of a character frame
#define STROKE_FRAME_X_MINIMUM_POINTS_UINT ((GRAPHICS_UINT)3)
#define STROKE_FRAME_Y_MINIMUM_POINTS_UINT ((GRAPHICS_UINT)3) 

// The NOMINAL space division of a character frame
#define STROKE_FRAME_X_AXIS_POINTS         ((GRAPHICS_REAL)16.0) // 'x' = 0.0 --> point 0, 'x' = 1.0 --> point 15
#define STROKE_FRAME_Y_AXIS_POINTS         ((GRAPHICS_REAL)16.0) // 'y' = 0.0 --> point 0, 'y' = 1.0 --> point 15

// The normalised stroke frame
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

#define MIDDLE_FRAME_COLOUR                WHITE_PEN_OPAQUE
#define MIDDLE_FRAME_COLOUR_CODE           ((GRAPHICS_UINT)WHITE_PEN_OPAQUE_CODE)

#define DESCENT_FRAME_COLOUR               YELLOW_PEN
#define DESCENT_FRAME_COLOUR_CODE          ((GRAPHICS_UINT)YELLOW_PEN_CODE)

#define GRAPHICS_CHAR_LOWER                 (0)
#define GRAPHICS_CHAR_UPPER                 (GRAPHICS_CHAR_LOWER + 1)
#define GRAPHICS_CHAR_LENGTH                (GRAPHICS_CHAR_UPPER + 1)
#define GRAPHICS_WORD_LENGTH                (16)

/******************************************************************************/

extern WCHAR                     objectOutput[GRAPHICS_RING_OUTPUT_WSTR_LENGTH];
extern WCHAR                     captionPanel[GRAPHICS_RING_OUTPUT_WSTR_LENGTH];

extern canvasDescriptor_t        canvasSize;

extern alphabetCharacters_tPtr   characterReference;

extern strokeGraphPointBase_t    strokeGraphPointBase;

extern bool                      mouseOverObject;
extern bool                      objectPositionTestFlag;
extern GRAPHICS_SHORT            mouseXPosition;
extern GRAPHICS_SHORT            mouseYPosition;

extern guiObjectHoldingRing_tPtr guiObjectHoldingRingBaseLink;
extern guiObjectHoldingRing_tPtr guiObjectHoldingRingCurrent;
extern guiObjectHoldingRing_tPtr guiObjectHoldingRingFreePtr;

// Rectangle 1 : 
#pragma pack(push,1)
extern rectangleObject_tPtr      rectangleObjectOne;
#pragma pack(pop)
extern rectangleDimensions_t     rectangleObjectOneDimensions;
extern objectColour_t            rectangleObjectOneColour;
#pragma pack(push,1)
extern graphicsActiveObject_t    rectangleObjectOneActiveBehaviour;
#pragma pack(pop)

// Rectangle 1 text : 
#pragma pack(push,1)
extern staticTextObject_tPtr     rectangleOneText  ;               
#pragma pack(pop)
extern rectangleDimensions_t     rectangleOneTextDimensions;                   
extern objectColour_t            rectangleOneTextColour;
#pragma pack(push,1)
extern graphicsActiveObject_t    rectangleOneTextActiveBehaviour;
#pragma pack(pop)

// Rectangle 2 : 
#pragma pack(push,1)
extern rectangleObject_tPtr      rectangleObjectTwo;
#pragma pack(pop)
extern rectangleDimensions_t     rectangleObjectTwoDimensions;
extern objectColour_t            rectangleObjectTwoColour;
#pragma pack(push,1)
extern graphicsActiveObject_t    rectangleObjectTwoActiveBehaviour;
#pragma pack(pop)

// Rectangle 3 : 
#pragma pack(push,1)
extern rectangleObject_tPtr      rectangleObjectThree;
#pragma pack(pop)
extern rectangleDimensions_t     rectangleObjectThreeDimensions;
extern objectColour_t            rectangleObjectThreeColour;

/******************************************************************************/

extern graphicsError_t instantiateObjectHoldingRingObject(      guiObjectHoldingRing_tPtr   *objectHoldingRingObject,
                                                          const guiObjectNumberingControl_t  guiObjectNumberingControl,
                                                          const GRAPHICS_INT                 guiObjectNumber);
extern graphicsError_t instantiateStaticTextObject(      staticTextObject_tPtr *staticTextObject,
                                                   const GRAPHICS_WCHAR_PTR     staticText,
                                                   const rectangleDimensions_t  staticTextDimensions,
                                                   const objectColour_t         staticTextColour);
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
extern graphicsError_t computeCompositeObjectBoundary(guiObjectHoldingRing_tPtr objectHoldingRingRoot);

extern graphicsError_t guiPrintStaticText(HDC                 hdc,
                                          commonObject_tPtr   graphicsObject,
                                          canvasDescriptor_t *canvasSize);
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

extern graphicsError_t printHoldingRingObject(guiObjectHoldingRing_tPtr ringObjectBase);
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

extern graphicsError_t drawStrokeText(      HDC                             hdc,
                                      const strokeTextStringDescriptor_tPtr strokeTextHeadlineCharacters,
                                            alphabetCharacters_tPtr         characterList,
                                      const strokeFrame_tPtr                characterFrame,
                                      const canvasDescriptor_tPtr           canvasSize,
                                      const strokeGraphPointBase_tPtr       strokeGraphBase);

extern graphicsError_t drawStrokeCharacter(      HDC                          hdc,
                                           const GRAPHICS_UINT                strokeCharacter,
                                                 alphabetCharacters_tPtr      characterList,
                                           const strokeFrame_tPtr             characterFrame,
                                           const canvasDescriptor_tPtr        canvasSize,
                                           const strokeGraphPointBase_tPtr    strokeGraphBase);

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
