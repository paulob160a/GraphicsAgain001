/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* GraphicsAgainGUIObjects.h :                                                */
/* Paul O'Brien                                                               */
/* 18.08.25                                                                   */
/*                                                                            */
/* GUI object definition and operating constants                              */
/******************************************************************************/

#ifndef GRAPHICS_AGAIN_GUI_OBJECTS_H_
#define GRAPHICS_AGAIN_GUI_OBJECTS_H_

/******************************************************************************/

#include "resource.h"
#include <Windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "StrokeGraphicsTypes.h"
#include "StrokeGraphicsAlphabet.h"

/******************************************************************************/

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

#define RADIO_BUTTON_EXPIRY_MESSAGE (WM_USER)

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

#define LIGHT_GREEN_PEN        ((BYTE)0xFF), \
                               ((BYTE)0xB5), \
                               ((BYTE)0xFF), \
                               ((BYTE)0x86) // opaque LIGHT GREEN pen

#define LIGHT_GREEN_PEN_CODE   ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                               (((BYTE)0xB5)         << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                               (((BYTE)0xFF)         << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                               (((BYTE)0x86)         << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

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
                               
#define FLUSHED_RED_PEN        ((BYTE)0xFF), \
                               ((BYTE)0xFF), \
                               ((BYTE)0x58), \
                               ((BYTE)0x58) // opaque FLUSHED RED pen

#define FLUSHED_RED_PEN_CODE   ((GRAPHICS_UINT)((FULL_OPAQUE_CHANNEL << UNSIGNED_INT_SHIFT_BYTE_0) | \
                                                (((BYTE)0xFF)        << UNSIGNED_INT_SHIFT_BYTE_1) | \
                                                (((BYTE)0x58)        << UNSIGNED_INT_SHIFT_BYTE_2) | \
                                                (((BYTE)0x58)        << (UNSIGNED_INT_SHIFT_BYTE_1 + UNSIGNED_INT_SHIFT_BYTE_2))))

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

/* The stroke graph frame background is grey... */
#define FRAME_BACKGROUND_COLOUR_RED           (0xa5)
#define FRAME_BACKGROUND_COLOUR_GREEN         (0xa6)
#define FRAME_BACKGROUND_COLOUR_BLUE          (0xa8)
#define FRAME_BACKGROUND_COLOUR_A             (0xff)

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
/* GUI Object Size and Placement :                                            */
/* - all of the dimensions are initially scaled by the default screen size.   */
/*   This is recalculated as the window changes                               */
/******************************************************************************/

#define RECTANGLE_OBJECT_ONE_BASE_X        ((GRAPHICS_REAL)10.0)
#define RECTANGLE_OBJECT_ONE_BASE_Y        ((GRAPHICS_REAL)10.0)
                                           
#define RECTANGLE_OBJECT_ONE_LEFT_X        (RECTANGLE_OBJECT_ONE_BASE_X / canvasSize.right)
#define RECTANGLE_OBJECT_ONE_TOP_Y         (RECTANGLE_OBJECT_ONE_BASE_Y / canvasSize.bottom)
#define RECTANGLE_OBJECT_ONE_RIGHT_X       ((RECTANGLE_OBJECT_ONE_BASE_X + 20.0) / canvasSize.right)
#define RECTANGLE_OBJECT_ONE_BOTTOM_Y      ((RECTANGLE_OBJECT_ONE_BASE_Y + 20.0) / canvasSize.bottom)
#define RECTANGLE_OBJECT_ONE_LINEWIDTH     ((GRAPHICS_UINT)3)
                                           
#define RECTANGLE_OBJECT_TWO_BASE_X        ((GRAPHICS_REAL)200.0)
#define RECTANGLE_OBJECT_TWO_BASE_Y        ((GRAPHICS_REAL)50.0)
                                           
#define RECTANGLE_OBJECT_TWO_LEFT_X        (RECTANGLE_OBJECT_TWO_BASE_X / canvasSize.right)
#define RECTANGLE_OBJECT_TWO_TOP_Y         (RECTANGLE_OBJECT_TWO_BASE_Y / canvasSize.bottom)
#define RECTANGLE_OBJECT_TWO_RIGHT_X       ((RECTANGLE_OBJECT_TWO_BASE_X + 50.0) / canvasSize.right)
#define RECTANGLE_OBJECT_TWO_BOTTOM_Y      ((RECTANGLE_OBJECT_TWO_BASE_Y + 50.0) / canvasSize.bottom)
#define RECTANGLE_OBJECT_TWO_LINEWIDTH     ((GRAPHICS_UINT)2)
                                         
#define RECTANGLE_OBJECT_THREE_BASE_X      ((GRAPHICS_REAL)225.0)
#define RECTANGLE_OBJECT_THREE_BASE_Y      ((GRAPHICS_REAL)25.0)
                                           
#define RECTANGLE_OBJECT_THREE_LEFT_X      (RECTANGLE_OBJECT_THREE_BASE_X / canvasSize.right)
#define RECTANGLE_OBJECT_THREE_TOP_Y       (RECTANGLE_OBJECT_THREE_BASE_Y / canvasSize.bottom)
#define RECTANGLE_OBJECT_THREE_RIGHT_X     ((RECTANGLE_OBJECT_THREE_BASE_X + 50.0) / canvasSize.right)
#define RECTANGLE_OBJECT_THREE_BOTTOM_Y    ((RECTANGLE_OBJECT_THREE_BASE_Y + 50.0) / canvasSize.bottom)
#define RECTANGLE_OBJECT_THREE_LINEWIDTH   ((GRAPHICS_UINT)1)
                                           
#define HEADLINE_BASE_X                    ((GRAPHICS_REAL)50.0)
#define HEADLINE_BASE_Y                    ((GRAPHICS_REAL)100.0)
                                           
#define HEADLINE_TEXT_ANCHOR_X             (HEADLINE_BASE_X / canvasSize.right)
#define HEADLINE_TEXT_ANCHOR_Y             (HEADLINE_BASE_Y / canvasSize.bottom)
                                           
#define HEADLINE_CHARACTER_WIDTH           ((GRAPHICS_REAL)(15.0 / canvasSize.right))
#define HEADLINE_CHARACTER_DEPTH           ((GRAPHICS_REAL)(20.0 / canvasSize.bottom))
                                           
#define HEADLINE_TEXT_WIDTH                (HEADLINE_CHARACTER_WIDTH * ((GRAPHICS_REAL)wcslen(GRAPHICS_STROKE_TEXT_HEADLINE))) + (HEADLINE_TEXT_DEPTH_SPACING * ((GRAPHICS_REAL)wcslen(GRAPHICS_STROKE_TEXT_HEADLINE))) // multiple characters 'x'
#define HEADLINE_TEXT_DEPTH                HEADLINE_CHARACTER_DEPTH // single character 'y'
#define HEADLINE_TEXT_WIDTH_SPACING        ((GRAPHICS_REAL)1.0)     // inter-character empty space
#define HEADLINE_TEXT_DEPTH_SPACING        ((GRAPHICS_REAL)1.0)     // inter-character empty space
                                           
#define HEADLINE_TEXT_WIDTH_X              (HEADLINE_TEXT_WIDTH / canvasSize.right)        
#define HEADLINE_TEXT_DEPTH_Y              HEADLINE_TEXT_DEPTH        
#define HEADLINE_TEXT_WIDTH_SPACING_X      (HEADLINE_TEXT_WIDTH_SPACING / canvasSize.right)
#define HEADLINE_TEXT_DEPTH_SPACING_Y      (HEADLINE_TEXT_WIDTH_SPACING / canvasSize.bottom)
                                           
#define HEADLINE_TEXT_LINEWIDTH            ((GRAPHICS_REAL)2.0)

#define RADIO_BUTTON_ONE_PUSH_RADIUS       ((GRAPHICS_REAL)20.0)
#define RADIO_BUTTON_ONE_PUSH_SEGMENTS     ((GRAPHICS_REAL)0.0)
#define RADIO_BUTTON_ONE_PUSH_LINE_WIDTH   ((GRAPHICS_REAL)3.0)
#define RADIO_BUTTON_ONE_PUSH_LINE_COLOUR  BLUE_PEN

#define RADIO_BUTTON_ONE_DETENT_OUT_COLOUR   LIGHT_GREEN_PEN
#define RADIO_BUTTON_ONE_DETENT_IN_COLOUR    FLUSHED_RED_PEN
#define RADIO_BUTTON_ONE_DETENT_FLASH_PERIOD ((GRAPHICS_UINT)500) // 500 milliseconds
#define RADIO_BUTTON_ONE_DETENT_TYPE         GRAPHICS_DETENT_TYPE_PUSH
#define RADIO_BUTTON_ONE_DETENT_TIMEOUT      ((GRAPHICS_UINT)500) // 500 milliseconds 

// Ring Objects :
#define GRAPHICS_RING_OUTPUT_WSTR_LENGTH      (256)
                                             
#define GRAPHICS_OBJECT_CAPTION_WSTR          L"GUI Graphics Development"
#define GRAPHICS_OBJECT_TYPE_NONE_WSTR        L"GRAPHICS_OBJECT_TYPE_NONE"
#define GRAPHICS_OBJECT_TYPE_RECTANGLE_WSTR   L"GRAPHICS_OBJECT_TYPE_RECTANGLE"
#define GRAPHICS_OBJECT_TYPE_STATIC_TEXT_WSTR L"GRAPHICS_OBJECT_TYPE_STATIC_TEXT"
#define GRAPHICS_OBJECT_TYPE_UNKNOWN_WSTR     L"GRAPHICS_OBJECT_TYPE_UNKNOWN_STR"

#define GRAPHICS_OBJECT_TYPE_CODE             (0x0FEDC000)

typedef enum graphicsObjectType_tTag
  {
  GRAPHICS_OBJECT_TYPE_NONE         = GRAPHICS_OBJECT_TYPE_CODE,
  GRAPHICS_OBJECT_TYPE_INDEX        = (GRAPHICS_OBJECT_TYPE_NONE         + 0x00000001),
  GRAPHICS_OBJECT_TYPE_RECTANGLE    = (GRAPHICS_OBJECT_TYPE_INDEX        + 0x00000001),
  GRAPHICS_OBJECT_TYPE_OCTAGON      = (GRAPHICS_OBJECT_TYPE_RECTANGLE    + 0x00000001),
  GRAPHICS_OBJECT_TYPE_HEXAGON      = (GRAPHICS_OBJECT_TYPE_OCTAGON      + 0x00000001),
  GRAPHICS_OBJECT_TYPE_RANDOM       = (GRAPHICS_OBJECT_TYPE_HEXAGON      + 0x00000001),
  GRAPHICS_OBJECT_TYPE_RADIO_BUTTON = (GRAPHICS_OBJECT_TYPE_RANDOM       + 0x00000001),
  GRAPHICS_OBJECT_TYPE_CIRCULAR     = (GRAPHICS_OBJECT_TYPE_RADIO_BUTTON + 0x00000001),
  GRAPHICS_OBJECT_TYPE_STATIC_TEXT  = (GRAPHICS_OBJECT_TYPE_CIRCULAR     + 0x00000001),
  GRAPHICS_OBJECT_TYPE_DETENT       = (GRAPHICS_OBJECT_TYPE_STATIC_TEXT  + 0x00000001),
  GRAPHICS_OBJECT_TYPE_COMPOSITE    = (GRAPHICS_OBJECT_TYPE_DETENT       + 0x00000001), // complex object tree root
  GRAPHICS_OBJECT_TYPE_UNKNOWN      = (GRAPHICS_OBJECT_TYPE_COMPOSITE    + 0x00000001),
  GRAPHICS_OBJECT_TYPES             = (GRAPHICS_OBJECT_TYPE_UNKNOWN      + 0x00000001)
  } graphicsObjectType_t, *graphicsObjectType_tPtr;

#pragma pack(push,1)
typedef struct rectangleDimensions_tTag
  {
  GRAPHICS_REAL leftX;
  GRAPHICS_REAL topY;
  GRAPHICS_REAL rightX;
  GRAPHICS_REAL bottomY;
  GRAPHICS_UINT lineWidth;
  } rectangleDimensions_t, *rectangleDimensions_tPtr;

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

// Circular objects (continuous or segmented boundary)
typedef enum circularObjectType_tTag
  {
  GRAPHICS_CIRCULAR_OBJECT_TYPE_CONTINUOUS = 0,
  GRAPHICS_CIRCULAR_OBJECT_TYPE_SEGMENTED,
  GRAPHICS_CIRCULAR_OBJECT_TYPES
  } circularObjectType_t, *circularObjectType_tPtr;

// Circular objects (continuous or segmented boundary)
#pragma pack(push,1)
typedef struct circularObjectDescriptor_tTag
  {
  circularObjectType_t circularType;
  GRAPHICS_REAL        circularRadius;
  GRAPHICS_REAL        circularSegments;
  GRAPHICS_REAL        circularLineWidth;
  objectColour_t       circularColour;
  } circularObjectDescriptor_t, *circularObjectDescriptor_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct circularObject_tTag
  {
  graphicsObjectType_t          objectType;
  graphicsObjectType_t          nextDrawingObjectType; // composite objects are simply chained
  GRAPHICS_VOID_PTR             nextDrawingObject;
  circularObjectDescriptor_tPtr circularDefinition;
  } circularObject_t, *circularObject_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct staticTextObject_tTag
  {
  graphicsObjectType_t            objectType;
  graphicsObjectType_t            nextDrawingObjectType;
  GRAPHICS_VOID_PTR               nextDrawingObject;
  strokeTextStringDescriptor_tPtr staticText;
  } staticTextObject_t, *staticTextObject_tPtr;
#pragma pack(pop)

// A radiobutton is a higher-level graphics construct composed of a chain of more primitive constructs.
// The only mandatory feature is the button's circular outline. If a rectangular outline is selected it 
// will be automatically placed outside of the circle. If accompanying text is selected it will be 
// automatically scaled and placed according to the requested selection, which in absentia defaults to 
// inside the button
typedef enum radioButtonTextPlacement_tTag
  {
  GRAPHICS_RADIO_BUTTON_TEXT_PLACEMENT_UNDEFINED = 0,
  GRAPHICS_RADIO_BUTTON_TEXT_PLACEMENT_CENTRE,
  GRAPHICS_RADIO_BUTTON_TEXT_PLACEMENT_BELOW,
  GRAPHICS_RADIO_BUTTON_TEXT_PLACEMENT_ABOVE,
  GRAPHICS_RADIO_BUTTON_TEXT_PLACEMENTS
  } radioButtonTextPlacement_t, *radioButtonTextPlacement_tPtr;

// The "detent" object is a set of instructions and a set of flags to control 
// how the detent responds when 'in' (pressed) and 'out' (depressed). How and 
// what object the detent applies to is object-dependent. It is defined as an 
// object to allow it to be chained and operate over all the other objects in 
// the chain if required
typedef void            (CALLBACK *DETENT_TIMEOUT_DETECT)(HWND hWnd, GRAPHICS_UINT message, GRAPHICS_ULONGLONG timerId, GRAPHICS_ULONG timeOutPeriod);   // flag detent timer expired and timer deletion
typedef graphicsError_t (*DETENT_CREATE_TIMER)(HWND hWnd, GRAPHICS_ULONGLONG timerId, GRAPHICS_UINT timeOutPeriod, DETENT_TIMEOUT_DETECT timeOutDetect); // create and start detent timer
typedef graphicsError_t (*DETENT_DELETE_TIMER)(HWND hWnd, GRAPHICS_ULONGLONG timerId);                                                                   // delete timer

typedef enum detentResponseType_tTag
  {
  GRAPHICS_DETENT_TYPE_UNINITIALISED = 0,
  GRAPHICS_DETENT_TYPE_NONE,              // detent off - the object to which the detent applies will respond to
                                          // a keypress but graphically nothing will change
  GRAPHICS_DETENT_TYPE_PUSH,              // acknowledge a momentary button push graphically with a delayed colour change (flash)
                                          // EVEN if the keypress is held
  GRAPHICS_DETENT_TYPE_PUSH_HOLD_RELEASE, // acknowledge a held button press graphically by changing colour from 'out' to 'in' on
                                          // keydown and holding before changing colour from 'in' to 'out' on keyup
  GRAPHICS_DETENT_TYPES
  } detentResponseType_t, *detentResponseType_tPtr;

typedef enum detentObjectInOutFlags_tTag
  {
  GRAPHICS_DETENT_FLAG_UNINITIALISED = 0,
  GRAPHICS_DETENT_FLAG_OUT,
  GRAPHICS_DETENT_FLAG_IN,
  GRAPHICS_DETENT_FLAGS
  } detentObjectInOutFlags_t, *detentObjectInOutFlags_tPtr;

#pragma pack(push,1)
typedef struct detentObjectDescriptor_tTag
  {
  objectColour_t           detentOutColour;
  objectColour_t           detentInColour;
  detentObjectInOutFlags_t detentStateOne;       // the two flags mediate the 'in'/'out' states
  detentObjectInOutFlags_t detentStateTwo;
  GRAPHICS_UINT            detentNoneFlashDelay; // when the detent is off, make the object 'flash' on a press
  detentResponseType_t     detentResponse;
  GRAPHICS_ULONGLONG       detentTimerId;
  GRAPHICS_UINT            detentTimeoutPeriod;
  DETENT_TIMEOUT_DETECT    detentTimeoutDetect;
  DETENT_CREATE_TIMER      detentTimerCreate;
  DETENT_DELETE_TIMER      detentTimerDelete;
  } detentObjectDescriptor_t, *detentObjectDescriptor_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct detentObject_tTag
  {
  graphicsObjectType_t        objectType;
  graphicsObjectType_t        nextDrawingObjectType; // composite objects are simply chained
  GRAPHICS_VOID_PTR           nextDrawingObject;
  detentObjectDescriptor_tPtr detentDefinition;
  } detentObject_t, *detentObject_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct radioButtonDescriptor_tTag
  {
  singlePoint_t                   radioButtonCentreAnchor;
  circularObject_tPtr             radioButtonPush;
  bool                            radioButtonOutline;
  bool                            radioButtonText;
  strokeTextStringDescriptor_tPtr radioButtonStaticText;
  radioButtonTextPlacement_t      radioButtonTextPlacement;
  bool                            radioButtonDetent;
  detentObject_tPtr               radioButtonDetentControl;
  } radioButtonDescriptor_t, *radioButtonDescriptor_tPtr;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct radioButtonObject_tTag
  {
  graphicsObjectType_t       objectType;
  graphicsObjectType_t       nextDrawingObjectType; // composite objects are simply chained
  GRAPHICS_VOID_PTR          nextDrawingObject;
  radioButtonDescriptor_tPtr radioButtonDefinition;
  } radioButtonObject_t, *radioButtonObject_tPtr;
#pragma pack(pop)

/******************************************************************************/

// Headline Text : 
extern strokeTextStringDescriptor_t headlineString;
extern objectColour_t               headlineColour;

// Rectangle coordinates for a string's boundary
extern  characterExtentsReal_t      strokeTextStringBoundary;
// Rectangle 1 : 

extern rectangleObject_tPtr         rectangleObjectOne;
               
extern rectangleDimensions_t        rectangleObjectOneDimensions;
extern objectColour_t               rectangleObjectOneColour;
                                                   
// Rectangle 1 Text :               
               
extern staticTextObject_tPtr        rectangleOneText;               
               
extern rectangleDimensions_t        rectangleOneTextDimensions;                   
extern objectColour_t               rectangleOneTextColour;
                                  
// Rectangle 2 :                    
               
extern rectangleObject_tPtr         rectangleObjectTwo;
                 
extern rectangleDimensions_t        rectangleObjectTwoDimensions;
extern objectColour_t               rectangleObjectTwoColour;
                        
// Rectangle 3 :                    
               
extern rectangleObject_tPtr         rectangleObjectThree;
            
extern rectangleDimensions_t        rectangleObjectThreeDimensions;
extern objectColour_t               rectangleObjectThreeColour;

// Radiobutton One :
extern radioButtonObject_tPtr       radioButtonOne;
extern circularObject_tPtr          radioButtonOnePush;
extern circularObjectDescriptor_t   radioButtonOnePushDefinition;
extern detentObject_tPtr            radioButtonOneDetent;
extern detentObjectDescriptor_t     radioButtonOnePushDetent;
extern radioButtonDescriptor_t      radioButtonOneDefinition;

/******************************************************************************/

extern graphicsError_t drawStrokeText(      HDC                             hdc,
                                      const strokeTextStringDescriptor_tPtr strokeTextHeadlineCharacters,
                                            alphabetCharacters_tPtr         characterList,
                                      const strokeFrame_tPtr                characterFrame,
                                      const canvasDescriptor_tPtr           canvasSize,
                                      const strokeGraphPointBase_tPtr       strokeGraphBase,
                                            characterExtentsReal_tPtr       strokeTextStringBoundary);

extern graphicsError_t drawStrokeCharacter(      HDC                          hdc,
                                           const GRAPHICS_UINT                strokeCharacter,
                                                 alphabetCharacters_tPtr      characterList,
                                           const strokeFrame_tPtr             characterFrame,
                                           const canvasDescriptor_tPtr        canvasSize,
                                           const strokeGraphPointBase_tPtr    strokeGraphBase);

extern graphicsError_t instantiateCircularObject(      circularObject_tPtr           *circularObject,
                                                 const circularObjectDescriptor_tPtr  circularObjectDefinition);

extern graphicsError_t instantiateRadioButtonObject(      radioButtonObject_tPtr        *radioButtonObject,
                                                    const radioButtonDescriptor_tPtr     radioButtonDescriptor,
                                                          circularObject_tPtr           *radioButtonPush,
                                                    const circularObjectDescriptor_tPtr  radioButtonPushDefinition,
                                                          detentObject_tPtr             *radioButtonPushDetent,
                                                    const detentObjectDescriptor_tPtr    radioButtonPushDetentDefinition);

extern graphicsError_t createAndStartHoldTimer(HWND hWnd, GRAPHICS_ULONGLONG timerId, GRAPHICS_UINT timeOutPeriod, DETENT_TIMEOUT_DETECT timeOutDetect);
extern graphicsError_t stopAndDeleteHoldTimer(HWND hWnd, GRAPHICS_ULONGLONG timerId);

extern VOID CALLBACK   detentHoldExpiryTimeout(HWND               hWnd,   
                                               GRAPHICS_UINT      message,
                                               GRAPHICS_ULONGLONG idTimer,
                                               GRAPHICS_ULONG     dwTime);

/******************************************************************************/

#endif

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
