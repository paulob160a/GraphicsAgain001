/******************************************************************************/
/*                                                                            */
/* GraphicsAgain001.cpp :                                                     */
/* Paul O'Brien                                                               */
/* 17.05.25                                                                   */
/*                                                                            */
/* 2D GUI for radio test network client                                       */
/******************************************************************************/

#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "framework.h"
#include <objidl.h>
#include <gdiplus.h>
#include <time.h>
#include <shellapi.h>
#include "StrokeGraphicsListHandlers.h"
#include "GraphicsAgainCaption.h"
#include "GraphicsAgainUtilities.h"
#include "ar19937.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/

// Global Variables:
HINSTANCE                    hInst                          = nullptr;                  // current instance
WCHAR                        appTitle[MAX_LOADSTRING]       = { GRAPHICS_AGAIN_TITLE }; // The title bar text
WCHAR                        appWindowClass[MAX_LOADSTRING] = { GRAPHICS_AGAIN_CLASS }; // the main window class name

char                         commandLineParameters[GRAPHICS_AGAIN_ARGUMENTS][MAX_LOADSTRING] = { "", "", "" };

GRAPHICS_WCHAR               alphabetFilePath[ALPHABET_FILEPATH_MAXIMUM] = { L"" };

alphabetFileDescriptor_t     alphabetFileDescriptor                      = {
                                                                            ((GRAPHICS_ULONG)0),
                                                                            ((GRAPHICS_ULONG)0),
                                                                            STROKE_GRAPHICS_MODIFICATION_FLAG_NONE,
                                                                           &alphabetFilePath[0],
                                                                           &alphabetLineW[0]
                                                                           };

alphabetCharacters_tPtr      characterReference                          = nullptr;

strokeGraphPointBase_t       strokeGraphPointBase                        = {
                                                                           ((GRAPHICS_UINT)0),
                                                                           ((GRAPHICS_UINT)0),
                                                                           nullptr,
                                                                           BRIGHT_YELLOW_PEN,
                                                                           LIGHT_BLUE_PEN,
                                                                           RED_PEN,
                                                                           STROKE_LINE_WIDTH
                                                                           };

                                                         
  GRAPHICS_CHAR                strokeTextHeadline[MAX_LOADSTRING]        = GRAPHICS_STROKE_TEXT_HEADLINE;
                                                                         
  strokeTextStringDescriptor_t strokeTextHeadlineCharacters              = {
                                                                           &strokeTextHeadline[0]
                                                                           };

/******************************************************************************/

// Forward declarations of functions included in this code module:
ATOM             RegisterGraphicsClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, RECT *requiredWindowSize, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

/******************************************************************************/

int APIENTRY wWinMain(_In_     HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_     LPWSTR    lpCmdLine,
                      _In_     int       nCmdShow)
  {
/******************************************************************************/

  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  GdiplusStartupInput           gdiPlusStartupInput               = { 0 };
  ULONG_PTR                     gdiplusToken                      = NULL;
                                                                  
  __time32_t                    theCurrentTime                    = 0;
                                                                  
  HACCEL                        hAccelTable                       = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICSAGAIN001));
  MSG                           msg                               = { 0 };
                                                                  
  RECT                          requiredWindowSize                = {
                                                                    GDI_PLUS_WINDOW_RECTANGLE_X_LEFT,
                                                                    GDI_PLUS_WINDOW_RECTANGLE_Y_TOP,
                                                                    GDI_PLUS_WINDOW_RECTANGLE_X_BOTTOM,
                                                                    GDI_PLUS_WINDOW_RECTANGLE_Y_BOTTOM
                                                                    };
                                                                  
    RECT                        adjustedWindowSize                = {
                                                                    ((LONG)0),
                                                                    ((LONG)0),
                                                                    ((LONG)0),
                                                                    ((LONG)0)
                                                                    },
                                deltaWindowSize                   = {
                                                                    ((LONG)0),
                                                                    ((LONG)0),
                                                                    ((LONG)0),
                                                                    ((LONG)0)
                                                                    };
                                                              
  GRAPHICS_WCHAR_PTR           *commandLineList                   = nullptr;
  GRAPHICS_INT                  commandLineArgumentNumber         = ((GRAPHICS_INT)0),
                                listIndex                         = ((GRAPHICS_INT)0);
  GRAPHICS_WCHAR_PTR            commandLineArguments              = nullptr;
  GRAPHICS_CHAR_PTR             ipAddress                         = nullptr,
                                portNumber                        = nullptr;
                                                                  
  GRAPHICS_UINT                 stringLength                      = ((GRAPHICS_UINT)0),
                                characterIndex                    = ((GRAPHICS_UINT)0);
                                                                  
  size_t                        commandStringLength               = ((size_t)0);
  errno_t                       commandStringError                = NO_ERROR;
  GRAPHICS_INT                  commandStringIndex                = ((GRAPHICS_INT)0);
                                                                  
  graphicsError_t               graphicsErrorState                = GRAPHICS_NO_ERROR;
                                                                  
                                                                  
  singlePoint_t                characterFrameDimensions           = { 
                                                                    ((GRAPHICS_REAL)0.0),
                                                                    ((GRAPHICS_REAL)0.0) 
                                                                    };

  strokeGraphPointBase_t       strokeGraphicBase                  = {
                                                                     ((GRAPHICS_UINT)0),
                                                                     ((GRAPHICS_UINT)0),
                                                                     nullptr,
                                                                     BRIGHT_YELLOW_PEN,
                                                                     LIGHT_BLUE_PEN,
                                                                     RED_PEN,
                                                                     STROKE_LINE_WIDTH
                                                                     };

/******************************************************************************/

  // Initialize GDI+.
  GdiplusStartup(&gdiplusToken, &gdiPlusStartupInput, NULL);

  // Processing the command-line arguments for required number and format 
  // conversion
  commandLineList = CommandLineToArgvW(GetCommandLineW(), &commandLineArgumentNumber);

  if (commandLineList == nullptr)
    {
    exit(0);
    }
  else
    {
    commandLineArguments = (LPWSTR)calloc(((size_t)(commandLineArgumentNumber + 1)), (MAX_LOADSTRING * ((size_t)(sizeof(WCHAR)))));

    // First copy the all the command-line arguments as wide-character strings
    if (commandLineArguments != nullptr)
      {
      for(listIndex = 0; listIndex < commandLineArgumentNumber; listIndex++) 
        {
        wcscpy_s((commandLineArguments + (listIndex * MAX_LOADSTRING)), ((rsize_t)MAX_LOADSTRING), *(commandLineList + listIndex));
        }
      }
    }

  // Now convert the parameter wide-character command-line arguments to 'char' form 
  // - but not the file path and name parameter 0
  if (listIndex == GRAPHICS_AGAIN_ARGUMENTS)
    {
    for (commandStringIndex = GRAPHICS_AGAIN_ARGUMENT_1; commandStringIndex < GRAPHICS_AGAIN_ARGUMENTS; commandStringIndex++)
      {
      if ((commandStringError = wcstombs_s(&commandStringLength,
                                            commandLineParameters[commandStringIndex],
                                            MAX_LOADSTRING,
                                            (commandLineArguments + (commandStringIndex * MAX_LOADSTRING)),
                                            MAX_LOADSTRING)) != NO_ERROR)
        {
        MessageBoxW(NULL, COMMAND_LINE_ERROR_STATEMENT, COMMAND_LINE_ERROR_CAPTION, COMMAND_LINE_ERROR_BEHAVIOUR);
        exit(0);
        }
      }
    }
  else
    {
    MessageBoxW(NULL, COMMAND_LINE_ERROR_STATEMENT, COMMAND_LINE_ERROR_CAPTION, COMMAND_LINE_ERROR_BEHAVIOUR);
    exit(0);
    }

  // GRAPHICS_AGAIN_ALPHABET_FILE     
  if ((graphicsErrorState = parseIpAddress(&ipAddress,
                                           &stringLength,
                                           &commandLineParameters[GRAPHICS_AGAIN_SERVER_IP_ADDRESS][0])) != GRAPHICS_NO_ERROR)
    {
    MessageBoxW(NULL, COMMAND_LINE_ERROR_STATEMENT, COMMAND_LINE_ERROR_CAPTION, COMMAND_LINE_ERROR_BEHAVIOUR);
    exit(0);
    }

  if ((graphicsErrorState = parsePortNumber(&portNumber,
                                            &stringLength,
                                            &commandLineParameters[GRAPHICS_AGAIN_SERVER_PORT_NUMBER][0])) != GRAPHICS_NO_ERROR)
    {
    MessageBoxW(NULL, COMMAND_LINE_ERROR_STATEMENT, COMMAND_LINE_ERROR_CAPTION, COMMAND_LINE_ERROR_BEHAVIOUR);
    exit(0);
    }

  // Try to open an alphabet file using the wide-character filename
  wcscpy_s(alphabetFileDescriptor.alphabetFilePath, ((rsize_t)MAX_LOADSTRING), commandLineList[GRAPHICS_AGAIN_ALPHABET_FILE]);

    // Build the character list as a set of line segments per character
  graphicsErrorState = buildCharacterList( ALPHABET_SIZE,
                                          &characterListRoot);

  if ((graphicsErrorState = readAlphabetFromFile(&alphabetFileDescriptor,
                                                  characterListRoot,
                                                 &characterFrameDimensions)) != GRAPHICS_NO_ERROR)
    {
    MessageBoxW(NULL, COMMAND_LINE_ERROR_STATEMENT, COMMAND_LINE_ERROR_CAPTION, COMMAND_LINE_ERROR_BEHAVIOUR);
    exit(0);
    }

  // Compute the character extents i.e. the invisible enclosing 'box'. To "print" stroke graphics text in 
  // this application the reference point will be the point : { x == 0, y == 0 }. Alphabets are generated 
  // referenced to this point in the range :
  //
  // -(gridSizeX / 2) .. +(gridSizeX / 2), -(gridSizeY / 2) .. + (gridSizeY / 2)
  //
  // This gives a common "centre-of-gravity" for all the characters from which spacing can be derived
  //
  // Thus, at loading the alphabet stroke extents are referenced to { 0, 0 } and not the grid-size used 
  // at generation

  strokeGraphicBase.graphRowNumber    = ((GRAPHICS_UINT)0);
  strokeGraphicBase.graphColumnNumber = ((GRAPHICS_UINT)0);

  for (characterIndex = ALPHABET_SIZE_START; characterIndex < ALPHABET_SIZE; characterIndex++)
    {
    graphicsErrorState = computeCharacterExtents((const GRAPHICS_UINT)characterIndex,
                                                 (const alphabetCharacters_tPtr)characterListRoot,
                                                 (const strokeGraphPointBase_tPtr)&strokeGraphicBase);

    if (!((graphicsErrorState == GRAPHICS_NO_ERROR) || (graphicsErrorState == GRAPHICS_OBJECT_INSTANTIATION_ERROR)))
      {
      MessageBoxW(NULL, COMMAND_LINE_ERROR_STATEMENT, COMMAND_LINE_ERROR_CAPTION, COMMAND_LINE_ERROR_BEHAVIOUR);
      exit(0);
      }
    }

#if (0)
  // Point at a character...
  graphicsErrorState = fetchCharacterReference ( ((GRAPHICS_UINT)'0'),
                                                 (const alphabetCharacters_tPtr)characterListRoot,
                                                &characterReference);
#endif

  /******************************************************************************/
  /* Initialise and instantiate the very first holding ring object              */
  /******************************************************************************/

  graphicsErrorState = instantiateObjectHoldingRingObject(&guiObjectHoldingRingBaseLink,
                                                           GRAPHICS_OBJECT_NUMBERING_IMMEDIATE,
                                                           1);

  //guiObjectHoldingRingBaseLink->numberOfRingLinkObjects = 1;

  // For now add two rectangles as proof-of-concept and development hints
  graphicsErrorState = instantiateRectangleObject(&rectangleObjectOne,
                                                   (const rectangleDimensions_t)rectangleObjectOneDimensions,
                                                   (const objectColour_t)rectangleObjectOneColour);
  
/* #pragma pack(push,1)
extern staticTextObject_tPtr     rectangleOneText  ;               
#pragma pack(pop)
extern rectangleDimensions_t     rectangleOneTextDimensions;                   
extern objectColour_t            rectangleOneTextColour;
#pragma pack(push,1)
extern graphicsActiveObject_t    rectangleOneTextActiveBehaviour;
#pragma pack(pop) */

  graphicsErrorState = instantiateStaticTextObject(&rectangleOneText,
                                                   (const GRAPHICS_WCHAR_PTR)    GRAPHICS_RECTANGLE_ONE_TEXT,
                                                   (const rectangleDimensions_t) rectangleOneTextDimensions,
                                                   (const objectColour_t)        rectangleOneTextColour);


  // Add the text object to the first rectangle object
  graphicsErrorState = addGuiObjectToGuiChain((GRAPHICS_VOID_PTR)rectangleOneText,
                                              (GRAPHICS_VOID_PTR)rectangleObjectOne);

  // Add this to the first holding ring object element
  guiObjectHoldingRingFreePtr = guiObjectHoldingRingBaseLink;

  graphicsErrorState = addGuiObjectToHoldingRing(guiObjectHoldingRingFreePtr,
                                                 (GRAPHICS_VOID_PTR)rectangleObjectOne,
                                                 &rectangleObjectOneActiveBehaviour,
                                                 GRAPHICS_OBJECT_TYPE_RECTANGLE);

  guiObjectHoldingRingCurrent = guiObjectHoldingRingBaseLink;

  graphicsErrorState = instantiateRectangleObject(&rectangleObjectTwo,
                                                  (const rectangleDimensions_t)rectangleObjectTwoDimensions,
                                                  (const objectColour_t)rectangleObjectTwoColour); 

  // DEVELOPMENT_START
  graphicsErrorState = instantiateRectangleObject(&rectangleObjectThree,
                                                  (const rectangleDimensions_t)rectangleObjectThreeDimensions,
                                                  (const objectColour_t)rectangleObjectThreeColour); 

  // The new chain starts at two...
  graphicsErrorState = addGuiObjectToGuiChain((GRAPHICS_VOID_PTR)rectangleObjectThree,
                                              (GRAPHICS_VOID_PTR)rectangleObjectTwo);
  // DEVELOPMENT_END

  // Instantiate a second holding ring object element attached to the first one
  graphicsErrorState = addNewHoldingRingObject(&guiObjectHoldingRingCurrent,
                                                GRAPHICS_OBJECT_NUMBERING_INCREMENTAL,
                                                0);

  graphicsErrorState = addGuiObjectToHoldingRing(guiObjectHoldingRingCurrent,
                                                 (GRAPHICS_VOID_PTR)rectangleObjectTwo,
                                                 &rectangleObjectTwoActiveBehaviour,
                                                 GRAPHICS_OBJECT_TYPE_RECTANGLE);

  graphicsErrorState = computeCompositeObjectBoundary(guiObjectHoldingRingCurrent);

  graphicsErrorState = printHoldingRingObject(guiObjectHoldingRingBaseLink);

/******************************************************************************/

  // Initialise the Mersenne Twister random number generator
  _time32(&theCurrentTime);
  init_genrand((GRAPHICS_ULONG)theCurrentTime);

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, appTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_GRAPHICSAGAIN001, appWindowClass, MAX_LOADSTRING);

  RegisterGraphicsClass(hInstance);

  // Correct the frame to our requirements!
  adjustedWindowSize = requiredWindowSize;

  AdjustWindowRect(&requiredWindowSize, WS_CAPTION, GDI_PLUS_WINDOW_MENU_STATE);

  deltaWindowSize.left   = adjustedWindowSize.left   - requiredWindowSize.left;
  deltaWindowSize.right  = requiredWindowSize.right  - adjustedWindowSize.right;
  deltaWindowSize.top    = adjustedWindowSize.top    - requiredWindowSize.top;
  deltaWindowSize.bottom = requiredWindowSize.bottom - adjustedWindowSize.bottom;

  requiredWindowSize.left   = deltaWindowSize.left   + adjustedWindowSize.left;
  requiredWindowSize.right  = deltaWindowSize.right  + adjustedWindowSize.right;
  requiredWindowSize.top    = deltaWindowSize.top    + adjustedWindowSize.top;
  requiredWindowSize.bottom = deltaWindowSize.bottom + adjustedWindowSize.bottom;

  AdjustWindowRect(&requiredWindowSize, WS_CAPTION, GDI_PLUS_WINDOW_MENU_STATE);

  // Perform application initialization:
  if (!InitInstance(hInstance, &requiredWindowSize, nCmdShow))
    {
    return(FALSE);
    }

  // Main message loop :
  while (GetMessage(&msg, nullptr, 0, 0))
    {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
      {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      }
    }

  GdiplusShutdown(gdiplusToken);

  LocalFree(commandLineList);

  free(commandLineArguments);

/******************************************************************************/

  return((int)msg.wParam);

/******************************************************************************/
  } /* end of WinMain                                                         */

/******************************************************************************/
/*                                                                            */
/* RegisterGraphicsClass() :                                                  */
/*                                                                            */
/*  - registers the window class.                                             */
/******************************************************************************/

ATOM RegisterGraphicsClass(HINSTANCE hInstance)
  {
/******************************************************************************/

  WNDCLASSEXW wcex    = { 0 };

/******************************************************************************/

  wcex.cbSize         = sizeof(WNDCLASSEX);

  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc    = WndProc;
  wcex.cbClsExtra     = 0;
  wcex.cbWndExtra     = 0;
  wcex.hInstance      = hInstance;
  wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICSAGAIN001));
  wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GRAPHICSAGAIN001);
  wcex.lpszClassName  = appWindowClass;
  wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

/******************************************************************************/

  return(RegisterClassExW(&wcex));

/******************************************************************************/
  } /* end of RegisterGraphicsClass                                           */

/******************************************************************************/
/*                                                                            */
/* InitInstance(HINSTANCE, RECT *, int) :                                     */
/*                                                                            */ 
/* - saves the instance handle and creates the main window                    */
/******************************************************************************/

BOOL InitInstance(HINSTANCE hInstance, RECT *requiredWindowSize, int nCmdShow)
  {
/******************************************************************************/

  HINSTANCE hInst = hInstance; // Store instance handle in our global variable

  HWND      hWnd = CreateWindowW(WINDOW_CLASS_NAME,          // window class name
                                 WINDOW_CAPTION,             // window caption
                                 WS_OVERLAPPEDWINDOW,        // window style
                                 requiredWindowSize->left,   // initial x position
                                 requiredWindowSize->top,    // initial y position
                                 requiredWindowSize->right,  // initial x size
                                 requiredWindowSize->bottom, // initial y size
                                 nullptr,
                                 nullptr,
                                 hInstance,
                                 nullptr);

/******************************************************************************/

  if (!hWnd)
    {
    return(FALSE);
    }

/******************************************************************************/

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

/******************************************************************************/

  return(TRUE);

/******************************************************************************/
  } /* end of InitInstance                                                    */

/******************************************************************************/
/*                                                                            */
/* WndProc(HWND, UINT, WPARAM, LPARAM) :                                      */
/* - processes messages for the main window.                                  */
//                                                                            */
//  WM_COMMAND  - process the application menu                                */
//  WM_PAINT    - Paint the main window                                       */
//  WM_DESTROY  - post a quit message and return                              */
/******************************************************************************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
/******************************************************************************/

  PAINTSTRUCT ps            = { 0 };
  HDC         hdc           = 0,
              swapHdc       = 0;
  RECT        thisRectangle = { 0 };

/******************************************************************************/

    switch (message)
      {
      case WM_COMMAND:
        {
        int wmId = LOWORD(wParam);

        // Parse the menu selections:
        switch (wmId)
          {
          case IDM_ABOUT:
              DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
              break;
          case IDM_EXIT:
              DestroyWindow(hWnd);
              break;
          default:
              return(DefWindowProc(hWnd, message, wParam, lParam));
          }
        }
      break;

      case WM_MOUSEMOVE :
        *(&mouseXPosition) = GET_X_LPARAM(lParam);
        *(&mouseYPosition) = GET_Y_LPARAM(lParam); 

        break;

        //InvalidateRect(hWnd, NULL, TRUE);
        
        //return(0);

      //break;

      case WM_MBUTTONDOWN : objectPositionTestFlag = true;
      
                            InvalidateRect(hWnd, NULL, TRUE);
        
                            return(0);
                            break;
   /*   case WM_MOUSEWHEEL : switch(LOWORD(wParam))
        {
        case MK_LBUTTON :
                          break;
        case MK_MBUTTON : InvalidateRect(hWnd, NULL, TRUE);
        
                          return(0);
                          break;
        case MK_RBUTTON :
                          break;
        default         :
                          break;
        } */

      break;

      case WM_KEYDOWN : switch(wParam)
        {
        case KEY_ESC_EXIT :
          PostMessage(hWnd, WM_CLOSE, 0, 0);
          [[fallthrough]]; // warning C26819 fallthrough is explicit
        default : 
                  break;
        };

      InvalidateRect(hWnd, NULL, TRUE);

      return(0);

      case WM_PAINT:
        {
        // Dynamically resize the window
        GetWindowRect(hWnd, &thisRectangle);

        setCanvasCoordinates(&canvasSize,
                              (GRAPHICS_REAL)thisRectangle.left,
                              (GRAPHICS_REAL)thisRectangle.top,
                              (GRAPHICS_REAL)thisRectangle.right,
                              (GRAPHICS_REAL)thisRectangle.bottom,
                              (GRAPHICS_REAL)(thisRectangle.right  - thisRectangle.left),
                              (GRAPHICS_REAL)(thisRectangle.bottom - thisRectangle.top));

        hdc     = BeginPaint(hWnd, &ps);   // the ultimate destination for the new image
        swapHdc = CreateCompatibleDC(hdc); // the hidden drawing plane for the new image

        OnPaint( hdc,
                &canvasSize,
                 guiObjectHoldingRingBaseLink,
                 characterListRoot,
                &characterFrame,
                &mouseXPosition,
                &mouseYPosition,
                &strokeGraphPointBase);

        DeleteDC(swapHdc);
        DeleteDC(hdc);

        EndPaint(hWnd, &ps);
        }
      break;

      case WM_CLOSE:
        DestroyWindow(hWnd);
        return(0);

      case WM_DESTROY:
        PostQuitMessage(0);
        return(0);
      break;

      default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
      }

/******************************************************************************/

  return(0);

/******************************************************************************/
  } /* end of WndProc                                                         */

/******************************************************************************/
/* OnPaint() :                                                                */
/* - the business end for drawing and reacting to input                       */
/******************************************************************************/

VOID OnPaint(HDC                        hdc,
             canvasDescriptor_t        *canvasSize,
             guiObjectHoldingRing_tPtr  guiObjectHoldingRingBaseLink,
             alphabetCharacters_tPtr    characterList,
             strokeFrame_tPtr           characterFrame,
             GRAPHICS_SHORT_PTR         mouseXPosition,
             GRAPHICS_SHORT_PTR         mouseYPosition,
             strokeGraphPointBase_tPtr  strokeGraphBase)
  {           
/******************************************************************************/
                   
  Graphics         graphics(hdc);
                   
  GRAPHICS_UINT    segmentIndex    = ((GRAPHICS_UINT)0);

  lineSegment_tPtr nextLineSegment = nullptr;

  graphicsError_t  objectError     = GRAPHICS_NO_ERROR;

/******************************************************************************/

  guiObjectHoldingRing_tPtr objectHoldingRingCurrentIndex = guiObjectHoldingRingBaseLink,
                            objectHoldingRingNextIndex    = nullptr;

/******************************************************************************/

  // Traverse the object holding ring : '...CurrentIndex' and '...NextIndex' 
  // swap each iteration so the current element is always '...CureentIndex' and 
  // the next element is always in '...NextIndex'
  do
    {
    objectError = incrementallyTraverseHoldingRing(&objectHoldingRingCurrentIndex,
                                                   &objectHoldingRingNextIndex);

    // Only proceed on no errors...
    if ((objectHoldingRingCurrentIndex != nullptr) && (objectError == GRAPHICS_NO_ERROR))
      {
      commonObject_tPtr guiObject = ((commonObject_tPtr)objectHoldingRingCurrentIndex->linkObject);

      while (guiObject != nullptr)
        {
        switch(((graphicsObjectType_t)(guiObject->objectType)))
          {
          case GRAPHICS_OBJECT_TYPE_RECTANGLE   : objectError = guiDrawRectangle(hdc,
                                                                                 guiObject,
                                                                                 canvasSize);
                                                  break;
        
          case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : objectError = guiPrintStaticText(hdc,
                                                                                   guiObject,
                                                                                   canvasSize);
                                                  break;

          case GRAPHICS_OBJECT_TYPE_NONE        : // currently "do nothing" for deviant cases - TBC!
          case GRAPHICS_OBJECT_TYPE_UNKNOWN     : 
          default                               :
                                                  break;
          }

        guiObject = (commonObject_tPtr)guiObject->nextDrawingObject;
        }
      }

    // TEST :
#if (0)
    // - draw a character from the loaded alphabet file
    objectError = buildCharacterStrokeGraph(characterFrame,
                                            strokeGraphBase);

    objectError = fetchCharacterReference( ((GRAPHICS_UINT)'A'),
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


    objectError = normaliseCharacterSegments( characterReference,
                                             &normalisedReference);
#else
    {
    GRAPHICS_UINT strokeCharacter = ((GRAPHICS_UINT)'g');

#if (0)
    objectError = drawStrokeCharacter(hdc,
                                      (const GRAPHICS_UINT)strokeCharacter,
                                      characterList,
                                      (const strokeFrame_tPtr)characterFrame,
                                      (const canvasDescriptor_tPtr)canvasSize,
                                      (const strokeGraphPointBase_tPtr)strokeGraphBase);

     strokeGraphBase->graphPoints = nullptr;
#endif

     objectError = drawStrokeText(hdc,
                                  (const strokeTextStringDescriptor_tPtr)&strokeTextHeadlineCharacters,
                                  characterList,
                                  (const strokeFrame_tPtr)characterFrame,
                                  (const canvasDescriptor_tPtr)canvasSize,
                                  (const strokeGraphPointBase_tPtr)strokeGraphBase);
    }
#endif
    // TEST :

    // On the flag, test to see if the mouse is in the (composite) objects' detection boundary
    if (objectPositionTestFlag == true)
      {
      objectError = detectMouseOverObject( (const guiObjectHoldingRing_tPtr)objectHoldingRingCurrentIndex,
                                           (const GRAPHICS_SHORT)*mouseXPosition,
                                           (const GRAPHICS_SHORT)*mouseYPosition,
                                           (const canvasDescriptor_tPtr)canvasSize,
                                          &mouseOverObject);
      }
    
    objectHoldingRingCurrentIndex = objectHoldingRingNextIndex;
    } //...but bail out on any errors
  while ((objectHoldingRingCurrentIndex != nullptr) && ((objectError == GRAPHICS_NO_ERROR) || (objectError == GRAPHICS_OBJECT_INSTANTIATION_ERROR)));

  objectPositionTestFlag = false;

/******************************************************************************/
  } /* end of OnPaint                                                         */

/******************************************************************************/
/* About() :                                                                  */
/* - message handler for about box.                                           */
/******************************************************************************/

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
  {
/******************************************************************************/

  UNREFERENCED_PARAMETER(lParam);

/******************************************************************************/

  switch (message)
    {
    case WM_INITDIALOG:
      return((INT_PTR)TRUE);
    
    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
        EndDialog(hDlg, LOWORD(wParam));
        return((INT_PTR)TRUE);
        }
    break;
    }

/******************************************************************************/

  return((INT_PTR)FALSE);

/******************************************************************************/
  } /* end of About                                                           */

/******************************************************************************/
