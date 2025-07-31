/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsAlphabet.h :                                                 */
/* Paul O'Brien                                                               */
/* 22.06.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <math.h>
#include <errno.h>
#include "StrokeGraphicsTypes.h"
#include "StrokeGraphicsAlphabet.h"
#include "StrokeGraphicsListHandlers.h"
#include "GraphicsAgain001.h"

/******************************************************************************/

using namespace std;

/******************************************************************************/

GRAPHICS_WCHAR alphabetLineW[ALPHABET_NUMBER_LINE_LENGTH] = L"";

/******************************************************************************/
/* readAlphabetFromFile() :                                                   */
/* - load an alphabet from a file. This must be done before any editing       */ 
/*   starts!                                                                  */
/******************************************************************************/

graphicsError_t readAlphabetFromFile(const alphabetFileDescriptor_tPtr alphabetFileDescriptor,
                                           alphabetCharacters_tPtr     characterListRoot,
                                           singlePoint_tPtr            characterFrameDimensions)
  {
/******************************************************************************/

  FILE                      *alphabetFile                                         = nullptr;
                                                                                 
  errno_t                    fileError                                            = NO_ERROR;
                           
  GRAPHICS_WCHAR             alphabetNumberWString[ALPHABET_NUMBER_STRING_LENGTH] = L"",
                             pointNumberWString[ALPHABET_NUMBER_STRING_LENGTH]    = L"",
                             lineSymbol                                           = 0x00;

  GRAPHICS_UINT              segmentPoints[LINE_SEGMENT_POINTS]                    = { 
                                                                                     ((GRAPHICS_UINT)0), 
                                                                                     ((GRAPHICS_UINT)0),
                                                                                     ((GRAPHICS_UINT)0),
                                                                                     ((GRAPHICS_UINT)0)
                                                                                     },
                             fileDimensionX                                       = ((GRAPHICS_UINT)0),
                             fileDimensionY                                       = ((GRAPHICS_UINT)0);
                           
  GRAPHICS_UINT              characterSegmentCounter                              = ((GRAPHICS_UINT)0),
                             symbolIndex                                          = ((GRAPHICS_UINT)0);

  // DEBUG
  GRAPHICS_INT               characterGraphMidRow                                 = ((GRAPHICS_INT)0),
                             characterGraphMidColumn                              = ((GRAPHICS_INT)0),
                             charactersFound                                      = ((GRAPHICS_INT)-1),
                             segmentsFound                                        = ((GRAPHICS_INT)0);
  // DEBUG
                           
  GRAPHICS_BOOLEAN           characterScanningError                               = false,
                             pointScanningError                                   = false,
                             segmentScanningError                                 = false;
                           
  graphicsError_t            objectError                                          = GRAPHICS_NO_ERROR;

  alphabetFileParseStates_t  fileParseState                                       = ALPHABET_FILE_STATE_DIMENSIONS_BEGIN;
  
/******************************************************************************/

  if ((characterListRoot != nullptr) && (alphabetFileDescriptor->alphabetFilePath != nullptr))
    {
    // Try to open the file
    if ((fileError = _wfopen_s(&alphabetFile, alphabetFileDescriptor->alphabetFilePath, L"rb")) == NULL)
      {
      if (alphabetFile != nullptr)
        {
        // Fetch (any) all the line segments for each stroke graphics character

        //graphicsError_t addLineSegmentToCharacterList(const GRAPHICS_UINT           characterSelected,
        //                                      const newLineSegment_tPtr     newLineSegment,
        //                                            alphabetCharacters_tPtr alphabetCharacters)

        // The stroke graphics alphabet file is laid out line-wise : 
        //  - one line containing a character starting symbol '{'
        //  - '1..n' lines containing :
        //       - segment starting symbol '['
        //       - 1 { six-digit positive integer } 4
        //       - segment starting symbol ']'
        //  - one line containing a character ending symbol   '}'
        while (fileParseState != ALPHABET_PARSE_FILE_STATE_IDLE)
          {
          switch(fileParseState)
            {
            case ALPHABET_FILE_STATE_DIMENSIONS_BEGIN       : if (fgetws(&alphabetFileDescriptor->alphabetLineW[0], ALPHABET_NUMBER_LINE_LENGTH, alphabetFile) == NULL)
                                                                 {
                                                                 characterScanningError = true;
                                                                 fileParseState         = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }
                                                               else
                                                                 {
                                                                 // Try to read a dimensions start character
                                                                 if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                          GRAPHICS_FILE_SYMBOL_TYPE_SYMBOL, 
                                                                                          GRAPHICS_CHARACTER_START_DIMENSIONS_SYMBOL, 
                                                                                          GRAPHICS_CHARACTER_SYMBOL_WIDTH,
                                                                                          nullptr, // no symbol to return
                                                                                         &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                   {
                                                                   lineSymbol     = 0x00;
                                                                   fileParseState = ALPHABET_FILE_STATE_DIMENSION_X;
                                                                   }
                                                                 else
                                                                   {
                                                                   segmentScanningError = true;
                                                                   fileParseState       = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                   }
                                                                 }
                                                              break;

            case ALPHABET_FILE_STATE_DIMENSION_X            : if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                       GRAPHICS_FILE_SYMBOL_TYPE_HEXADECIMAL, 
                                                                                       GRAPHICS_SEGMENT_NULL_SYMBOL, 
                                                                                       GRAPHICS_POINT_VALUE_WIDTH,
                                                                                      &fileDimensionX,
                                                                                      &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                  {
                                                                  lineSymbol     = 0x00;
                                                                  fileParseState = ALPHABET_FILE_STATE_DIMENSION_Y;
                                                                  }
                                                                else
                                                                  {
                                                                  segmentScanningError = true;
                                                                  fileParseState       = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                  }
                                                               break;

            case ALPHABET_FILE_STATE_DIMENSION_Y             : if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_HEXADECIMAL, 
                                                                                        GRAPHICS_SEGMENT_NULL_SYMBOL, 
                                                                                        GRAPHICS_POINT_VALUE_WIDTH,
                                                                                       &fileDimensionY,
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                 {
                                                                 lineSymbol     = 0x00;
                                                                 fileParseState = ALPHABET_FILE_STATE_DIMENSIONS_END;
                                                                 }
                                                               else
                                                                 {
                                                                 segmentScanningError = true;
                                                                 fileParseState       = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }
                                                               break;

            case ALPHABET_FILE_STATE_DIMENSIONS_END         :  // Try to read a dimensions start character
                                                               if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_SYMBOL, 
                                                                                        GRAPHICS_CHARACTER_END_DIMENSIONS_SYMBOL, 
                                                                                        GRAPHICS_CHARACTER_SYMBOL_WIDTH,
                                                                                        nullptr, // no symbol to return
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                 {
#ifdef STROKE_GRAPHICS_APPLICATION                               // For reference keep the original functionality of this code paragraph
                                                                 // as it was used in "StrokeGraphics"
                                                                 // Ready the character frame dimensions for the calling function
                                                                 characterFrameDimensions->xAxisPoint = (GRAPHICS_REAL)fileDimensionX;
                                                                 characterFrameDimensions->yAxisPoint = (GRAPHICS_REAL)fileDimensionY;

                                                                 // Convert from relative segment points to absolute
                                                                 characterGraphMidRow                 = (GRAPHICS_INT)(fileDimensionX >> 1);
                                                                 characterGraphMidColumn              = (GRAPHICS_INT)(fileDimensionY >> 1);
#else
                                                                // Ready the character frame dimensions for the calling function around the 
                                                                // point { 0, 0 }. This produces "agnostic" segment dimensioning
                                                                characterFrameDimensions->xAxisPoint = ((GRAPHICS_REAL)0);
                                                                characterFrameDimensions->yAxisPoint = ((GRAPHICS_REAL)0);

                                                                // Convert from relative segment points to absolute - relative to { 0, 0 }
                                                                characterGraphMidRow                 = (GRAPHICS_INT)(0);
                                                                characterGraphMidColumn              = (GRAPHICS_INT)(0);
#endif

                                                                 lineSymbol     = 0x00;
                                                                 fileParseState = ALPHABET_PARSE_FILE_STATE_CHARACTER_BEGINS;
                                                                 }
                                                               else
                                                                 {
                                                                 segmentScanningError = true;
                                                                 fileParseState       = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }
                                                               break;

            case ALPHABET_PARSE_FILE_STATE_CHARACTER_BEGINS : if (fgetws(&alphabetFileDescriptor->alphabetLineW[0], ALPHABET_NUMBER_LINE_LENGTH, alphabetFile) == NULL)
                                                                 {
                                                                 characterScanningError = true;
                                                                 fileParseState         = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }
                                                               else
                                                                 {
                                                                 // Try to read a character start symbol
                                                                 characterSegmentCounter = ((GRAPHICS_UINT)0);
                                                                 
                                                                 if (swscanf_s(&alphabetFileDescriptor->alphabetLineW[0], L"%wc", &lineSymbol, GRAPHICS_CHARACTER_SYMBOL_WIDTH) == ((GRAPHICS_INT)1))
                                                                   {
                                                                   if (lineSymbol != GRAPHICS_CHARACTER_START_SYMBOL)
                                                                     {
                                                                     characterScanningError = true;
                                                                     fileParseState         = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                     }
                                                                   else
                                                                     {
                                                                     if (fgetws(&alphabetFileDescriptor->alphabetLineW[0], ALPHABET_NUMBER_LINE_LENGTH, alphabetFile) == NULL)
                                                                       {
                                                                       characterScanningError = true;
                                                                       fileParseState         = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                       }
                                                                     else
                                                                       {
                                                                       // DEBUG
                                                                       charactersFound = charactersFound + ((GRAPHICS_INT)1);
                                                                       segmentsFound   = ((GRAPHICS_INT)0);
                                                                       // DEBUG

                                                                       lineSymbol     = 0x00;
                                                                       symbolIndex    = ((GRAPHICS_UINT)0);
                                                                       fileParseState = ALPHABET_PARSE_FILE_STATE_SEGMENT_BEGINS;
                                                                       }
                                                                     }
                                                                   }
                                                                  else
                                                                    {
                                                                    characterScanningError = true;
                                                                    fileParseState         = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                    }
                                                                  }
                                                               break;

            case ALPHABET_PARSE_FILE_STATE_SEGMENT_BEGINS    : // Try to read a segment start symbol

                                                               // TEST
#ifdef _DEBUG_SEGMENT_POINT_VALUES_
                                                               if (charactersFound != 0x30)
                                                                 {
                                                                 wcscpy_s(alphabetFileDescriptor->alphabetLineW, ALPHABET_NUMBER_LINE_LENGTH, L"[123456 789aBc DeF098 765432 ]");
                                                                 }
#endif
                                                               // TEST 
                                                               if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_SYMBOL, 
                                                                                        GRAPHICS_SEGMENT_START_SYMBOL, 
                                                                                        GRAPHICS_CHARACTER_SYMBOL_WIDTH,
                                                                                        nullptr, // no symbol to return
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                 {
                                                                 lineSymbol     = 0x00;
                                                                 fileParseState = ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_0_X;
                                                                 segmentsFound  = segmentsFound + ((GRAPHICS_INT)1);
                                                                 }
                                                               else
                                                                 {
                                                                 segmentScanningError = true;
                                                                 fileParseState       = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }

                                                               break;

            case ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_0_X : // Try to read point 0,x
                                                               if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_HEXADECIMAL, 
                                                                                        GRAPHICS_SEGMENT_NULL_SYMBOL, 
                                                                                        GRAPHICS_POINT_VALUE_WIDTH,
                                                                                       &segmentPoints[LINE_SEGMENT_POINT_ORIGIN_X],
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                 {
                                                                 // Relative to absolute
                                                                 segmentPoints[LINE_SEGMENT_POINT_ORIGIN_X] = (GRAPHICS_UINT)(((GRAPHICS_INT)segmentPoints[LINE_SEGMENT_POINT_ORIGIN_X]) + characterGraphMidRow);
                                                                 
                                                                 lineSymbol     = 0x00;
                                                                 fileParseState = ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_0_Y;
                                                                 }
                                                               else
                                                                 {
                                                                 pointScanningError = true;
                                                                 fileParseState     = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }
                                                               break;

            case ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_0_Y : // Try to read point 0,y
                                                               if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_HEXADECIMAL, 
                                                                                        GRAPHICS_SEGMENT_NULL_SYMBOL, 
                                                                                        GRAPHICS_POINT_VALUE_WIDTH,
                                                                                       &segmentPoints[LINE_SEGMENT_POINT_ORIGIN_Y],
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                 {
                                                                 // Relative to absolute
                                                                 segmentPoints[LINE_SEGMENT_POINT_ORIGIN_Y] = (GRAPHICS_UINT)(((GRAPHICS_INT)segmentPoints[LINE_SEGMENT_POINT_ORIGIN_Y]) + characterGraphMidColumn);

                                                                 lineSymbol     = 0x00;
                                                                 fileParseState = ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_1_X;
                                                                 }
                                                               else
                                                                 {
                                                                 pointScanningError = true;
                                                                 fileParseState     = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }
                                                               break;

            case ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_1_X : // Try to read point 1,x
                                                               if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_HEXADECIMAL, 
                                                                                        GRAPHICS_SEGMENT_NULL_SYMBOL, 
                                                                                        GRAPHICS_POINT_VALUE_WIDTH,
                                                                                       &segmentPoints[LINE_SEGMENT_POINT_DESTINATION_X],
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                 {
                                                                 // Relative to absolute
                                                                 segmentPoints[LINE_SEGMENT_POINT_DESTINATION_X] = (GRAPHICS_UINT)(((GRAPHICS_INT)segmentPoints[LINE_SEGMENT_POINT_DESTINATION_X]) + characterGraphMidRow);

                                                                 lineSymbol     = 0x00;
                                                                 fileParseState = ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_1_Y;
                                                                 }
                                                               else
                                                                 {
                                                                 pointScanningError = true;
                                                                 fileParseState     = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }
                                                               break;

            case ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_1_Y : // Try to read point 1,y
                                                               if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_HEXADECIMAL, 
                                                                                        GRAPHICS_SEGMENT_NULL_SYMBOL, 
                                                                                        GRAPHICS_POINT_VALUE_WIDTH,
                                                                                       &segmentPoints[LINE_SEGMENT_POINT_DESTINATION_Y],
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                 {
                                                                 segmentPoints[LINE_SEGMENT_POINT_DESTINATION_Y] = (GRAPHICS_UINT)(((GRAPHICS_INT)segmentPoints[LINE_SEGMENT_POINT_DESTINATION_Y]) + characterGraphMidColumn);

                                                                 lineSymbol     = 0x00;
                                                                 fileParseState = ALPHABET_PARSE_FILE_STATE_SEGMENT_ENDS;
                                                                 }
                                                               else
                                                                 {
                                                                 pointScanningError = true;
                                                                 fileParseState     = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }
                                                               break;

            case ALPHABET_PARSE_FILE_STATE_SEGMENT_ENDS      :  // Try to read a segment end symbol
                                                               if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_SYMBOL, 
                                                                                        GRAPHICS_SEGMENT_END_SYMBOL, 
                                                                                        GRAPHICS_CHARACTER_SYMBOL_WIDTH,
                                                                                        nullptr, // no symbol to return
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                 {
                                                                 // Add the new line segment to the character stroke graphic
                                                                 lineSegment_t newSegment           = { 
                                                                                                      ((GRAPHICS_UINT)0), 
                                                                                                      ((GRAPHICS_UINT)0),
                                                                                                      ((GRAPHICS_UINT)0),
                                                                                                      ((GRAPHICS_UINT)0) 
                                                                                                      };

                                                                 newSegment.lineSegmentOriginX      = segmentPoints[LINE_SEGMENT_POINT_ORIGIN_X];
                                                                 newSegment.lineSegmentOriginY      = segmentPoints[LINE_SEGMENT_POINT_ORIGIN_Y];
                                                                 newSegment.lineSegmentDestinationX = segmentPoints[LINE_SEGMENT_POINT_DESTINATION_X];
                                                                 newSegment.lineSegmentDestinationY = segmentPoints[LINE_SEGMENT_POINT_DESTINATION_Y];

                                                                 objectError = addLineSegmentToCharacterList(charactersFound,
                                                                                                             (const lineSegment_tPtr)&newSegment,
                                                                                                             characterListRoot);

                                                                 // This is a fudge! When x0, y0, x1 and y1 are all zero i.e. a useless point, an error is 
                                                                 // generated and no segment is added to the character stroke graphic list
                                                                 if ((objectError == GRAPHICS_NO_ERROR) || (objectError == GRAPHICS_OBJECT_INSTANTIATION_ERROR))
                                                                   {
                                                                   if (fgetws(&alphabetFileDescriptor->alphabetLineW[0], ALPHABET_NUMBER_LINE_LENGTH, alphabetFile) == NULL)
                                                                     {
                                                                     characterScanningError = true;
                                                                     fileParseState         = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                     }
                                                                   else
                                                                     {
                                                                     lineSymbol              = 0x00;
                                                                     symbolIndex             = ((GRAPHICS_UINT)0);
                                                                     characterSegmentCounter = characterSegmentCounter + ((GRAPHICS_UINT)1);
                                                                     fileParseState          = ALPHABET_PARSE_FILE_STATE_CHARACTER_ENDS;
                                                                     }
                                                                   }
                                                                 else
                                                                   {
                                                                   segmentScanningError = true;
                                                                   fileParseState       = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                   }
                                                                 }
                                                               else
                                                                 {
                                                                 segmentScanningError = true;
                                                                 fileParseState       = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                 }

                                                               break;

            case ALPHABET_PARSE_FILE_STATE_CHARACTER_ENDS    : if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_SYMBOL, 
                                                                                        GRAPHICS_CHARACTER_END_SYMBOL, 
                                                                                        GRAPHICS_CHARACTER_SYMBOL_WIDTH,
                                                                                        nullptr, // no symbol to return
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                 {
                                                                 // After parsing the last character, exit
                                                                 if (charactersFound == (ALPHABET_SIZE - 1))
                                                                   {
                                                                   fileParseState = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                   objectError    = GRAPHICS_NO_ERROR;
                                                                   }
                                                                 else
                                                                   {
                                                                   fileParseState = ALPHABET_PARSE_FILE_STATE_CHARACTER_BEGINS;
                                                                   }
                                                                 }
                                                               else
                                                                 {
                                                                 symbolIndex = ((GRAPHICS_UINT)0);

                                                                 if ((advanceSymbolIndex(&alphabetFileDescriptor->alphabetLineW[0],
                                                                                        GRAPHICS_FILE_SYMBOL_TYPE_SYMBOL, 
                                                                                        GRAPHICS_SEGMENT_START_SYMBOL, 
                                                                                        GRAPHICS_CHARACTER_SYMBOL_WIDTH,
                                                                                        nullptr, // no symbol to return
                                                                                       &symbolIndex)) == GRAPHICS_NO_ERROR)
                                                                   {
                                                                   fileParseState = ALPHABET_PARSE_FILE_STATE_SEGMENT_POINT_0_X;
                                                                   }
                                                                 else
                                                                   {
                                                                   segmentScanningError = true;
                                                                   fileParseState       = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                                   }
                                                                 }

                                                               break;

            case ALPHABET_PARSE_FILE_STATE_FILE_ENDS         : fileParseState = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                               break;
          
            default                                          : characterScanningError = true;
                                                               fileParseState         = ALPHABET_PARSE_FILE_STATE_IDLE;
                                                               break;
            }
          }
      
        fclose(alphabetFile);
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_FILE_ERROR;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of readAlphabetFromFile                                            */

/******************************************************************************/
/* advanceSymbolIndex() :                                                     */
/******************************************************************************/

graphicsError_t advanceSymbolIndex(const GRAPHICS_WCHAR_PTR     segmentLine,
                                   const graphicsSymbolsTypes_t symbolType, 
                                   const GRAPHICS_WCHAR         symbol,
                                   const GRAPHICS_UINT          symbolWidth,
                                         GRAPHICS_VOID_PTR      decodedSymbol,
                                         GRAPHICS_UINT_PTR      symbolIndex)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((symbolIndex != nullptr) && (segmentLine != nullptr) && (wcslen(segmentLine) > ((size_t)0)) && (wcslen(segmentLine) < ((size_t)ALPHABET_NUMBER_LINE_LENGTH)))
    {
    switch(symbolType)
      {
      case GRAPHICS_FILE_SYMBOL_TYPE_SYMBOL      : while ((*(segmentLine + *symbolIndex)) != symbol)
                                                     {
                                                     GRAPHICS_WCHAR thisSymbol = *(segmentLine + *symbolIndex);

                                                     if ((thisSymbol == GRAPHICS_SEGMENT_END_OF_LINE_SYMBOL) || (thisSymbol == WEOF))
                                                       {
                                                       objectError = GRAPHICS_OBJECT_SYMBOL_ERROR;
                                                       break;
                                                       }
                                                     else
                                                       {
                                                       if (symbol == GRAPHICS_SEGMENT_ANY_SYMBOL)
                                                         {
                                                         *symbolIndex = *symbolIndex + ((GRAPHICS_UINT)1);
                                                          break;
                                                         }
                                                       else
                                                         {
                                                         *symbolIndex = *symbolIndex + ((GRAPHICS_UINT)1);
                                                         }
                                                       }
                                                     }
      
                                                   if (objectError == GRAPHICS_NO_ERROR)
                                                     {
                                                     if ((*(segmentLine + *symbolIndex)) == symbol)
                                                       {
                                                       *symbolIndex = *symbolIndex + ((GRAPHICS_UINT)1);
                                                       }
                                                     }
      
                                                   break;

      case GRAPHICS_FILE_SYMBOL_TYPE_DECIMAL     : if (((GRAPHICS_UINT_PTR)decodedSymbol) != nullptr)
                                                     {
                                                     GRAPHICS_UINT  thisSymbolIndex   = ((GRAPHICS_UINT)0),
                                                                    localSymbolIndex  = ((GRAPHICS_UINT)0), // the global "symbolIndex" needs to be synced after the mumber 
                                                                                                            // has been successfully scanned and built
                                                                    symbolAccumulator = ((GRAPHICS_UINT)0),
                                                                    symbolDecade      = ((GRAPHICS_UINT)0),
                                                                    symbolWeight      = DECIMAL_CHARACTER_LEADING_WEIGHT;

                                                     GRAPHICS_WCHAR thisSymbol        = *(segmentLine + *symbolIndex);

                                                     while (isnum(thisSymbol) != GRAPHICS_NO_ERROR)
                                                       {
                                                       if ((thisSymbol == GRAPHICS_SEGMENT_END_OF_LINE_SYMBOL) || (thisSymbol == WEOF))
                                                         {
                                                         objectError = GRAPHICS_OBJECT_SYMBOL_ERROR;
                                                         break;
                                                         }
                                                       else
                                                         {
                                                         *symbolIndex = *symbolIndex + ((GRAPHICS_UINT)1);
                                                         thisSymbol   = *(segmentLine + *symbolIndex);
                                                         }
                                                       }

                                                     if (objectError == GRAPHICS_NO_ERROR)
                                                       {
                                                       // Convert the (tested as decimal!) character to a decimal number
                                                       symbolDecade      = ((GRAPHICS_UINT)thisSymbol) - ((GRAPHICS_UINT)ASCII_NUMERIC_0);

                                                       symbolAccumulator = symbolAccumulator + (symbolDecade * symbolWeight); 
                                                       symbolWeight      = symbolWeight / DECIMAL_CHARACTER_WEIGHT_SHIFT;

                                                       localSymbolIndex  = *symbolIndex + ((GRAPHICS_UINT)1);

                                                       // The first decimal character has been found. Now we need (GRAPHICS_POINT_VALUE_WIDTH - 1) more...
                                                       for (thisSymbolIndex = ((GRAPHICS_UINT)0); thisSymbolIndex < (GRAPHICS_POINT_VALUE_WIDTH - 1); thisSymbolIndex++)
                                                         {
                                                         thisSymbol = *(segmentLine + thisSymbolIndex + localSymbolIndex);
                                                         
                                                         if (isnum(thisSymbol) == GRAPHICS_NO_ERROR)
                                                           {
                                                           symbolDecade      = ((GRAPHICS_UINT)thisSymbol) - ((GRAPHICS_UINT)ASCII_NUMERIC_0);
                                                       
                                                           symbolAccumulator = symbolAccumulator + (symbolDecade * symbolWeight); 
                                                           symbolWeight      = symbolWeight / DECIMAL_CHARACTER_WEIGHT_SHIFT;
                                                           }
                                                         else
                                                           {
                                                           objectError = GRAPHICS_OBJECT_SYMBOL_ERROR;
                                                           break;
                                                           }
                                                         }

                                                       if (objectError == GRAPHICS_NO_ERROR)
                                                         {
                                                         *((GRAPHICS_UINT_PTR)decodedSymbol) = symbolAccumulator;

                                                         *symbolIndex                        = localSymbolIndex + thisSymbolIndex;
                                                         }
                                                       else
                                                         {
                                                         objectError = GRAPHICS_OBJECT_SYMBOL_ERROR;
                                                         }
                                                       }
        }
                                                   break;

      case GRAPHICS_FILE_SYMBOL_TYPE_HEXADECIMAL : if (((GRAPHICS_UINT_PTR)decodedSymbol) != nullptr)
                                                     {
                                                     GRAPHICS_UINT  thisSymbolIndex   = ((GRAPHICS_UINT)0),
                                                                    localSymbolIndex  = ((GRAPHICS_UINT)0), // the global "symbolIndex" needs to be synced after the mumber 
                                                                                                            // has been successfully scanned and built
                                                                    symbolAccumulator = ((GRAPHICS_UINT)0),
                                                                    symbolHexade      = ((GRAPHICS_UINT)0),
                                                                    symbolWeight      = HEXADECIMAL_CHARACTER_LEADING_WEIGHT;

                                                     GRAPHICS_WCHAR thisSymbol        = *(segmentLine + *symbolIndex);

                                                     while ((isnum(thisSymbol) != GRAPHICS_NO_ERROR)                                    && 
                                                            (!((thisSymbol     >= ASCII_NUMERIC_A) && (thisSymbol <= ASCII_NUMERIC_F))) &&  
                                                            (!((thisSymbol     >= ASCII_NUMERIC_a) && (thisSymbol <= ASCII_NUMERIC_f))))
                                                       {
                                                       if ((thisSymbol == GRAPHICS_SEGMENT_END_OF_LINE_SYMBOL) || (thisSymbol == WEOF))
                                                         {
                                                         objectError = GRAPHICS_OBJECT_SYMBOL_ERROR;
                                                         break;
                                                         }
                                                       else
                                                         {
                                                         *symbolIndex = *symbolIndex + ((GRAPHICS_UINT)1);
                                                         thisSymbol   = *(segmentLine + *symbolIndex);
                                                         }
                                                       }

                                                     if (objectError == GRAPHICS_NO_ERROR)
                                                       {
                                                       // Convert the (tested as hexadecimal!) character to a decimal number
                                                       if (isnum(thisSymbol) == GRAPHICS_NO_ERROR)
                                                         {
                                                         symbolHexade = ((GRAPHICS_UINT)thisSymbol) - ((GRAPHICS_UINT)ASCII_NUMERIC_0);
                                                         }
                                                       else
                                                         {
                                                         if (iswupper(thisSymbol))
                                                           {
                                                           symbolHexade = ((GRAPHICS_UINT)thisSymbol) - ((GRAPHICS_UINT)ASCII_NUMERIC_A) + ASCII_HEXANUMERIC_OFFSET;
                                                           }
                                                         else
                                                           {
                                                           if (iswlower(thisSymbol))
                                                             {
                                                             symbolHexade = ((GRAPHICS_UINT)thisSymbol) - ((GRAPHICS_UINT)ASCII_NUMERIC_a) + ASCII_HEXANUMERIC_OFFSET;
                                                             }
                                                           else
                                                             {
                                                             objectError = GRAPHICS_OBJECT_SYMBOL_ERROR;
                                                             }
                                                           }
                                                         }

                                                       if (objectError == GRAPHICS_NO_ERROR)
                                                         {
                                                         symbolAccumulator = symbolAccumulator + (symbolHexade * symbolWeight); 
                                                         symbolWeight      = symbolWeight / HEXADECIMAL_CHARACTER_WEIGHT_SHIFT;
                                                         
                                                         localSymbolIndex  = *symbolIndex + ((GRAPHICS_UINT)1);
                                                         
                                                         // The first hexadecimal character has been found. Now we need (GRAPHICS_POINT_VALUE_WIDTH - 1) more...
                                                         for (thisSymbolIndex = ((GRAPHICS_UINT)0); thisSymbolIndex < (GRAPHICS_POINT_VALUE_WIDTH - 1); thisSymbolIndex++)
                                                           {
                                                           thisSymbol = *(segmentLine + thisSymbolIndex + localSymbolIndex);
                                                           
                                                           if (isnum(thisSymbol) == GRAPHICS_NO_ERROR)
                                                             {
                                                             symbolHexade = ((GRAPHICS_UINT)thisSymbol) - ((GRAPHICS_UINT)ASCII_NUMERIC_0);
                                                             }
                                                           else
                                                             {
                                                             if (iswupper(thisSymbol))
                                                               {
                                                               symbolHexade = ((GRAPHICS_UINT)thisSymbol) - ((GRAPHICS_UINT)ASCII_NUMERIC_A) + ASCII_HEXANUMERIC_OFFSET;
                                                               }
                                                             else
                                                               {
                                                               if (iswlower(thisSymbol))
                                                                 {
                                                                 symbolHexade = ((GRAPHICS_UINT)thisSymbol) - ((GRAPHICS_UINT)ASCII_NUMERIC_a) + ASCII_HEXANUMERIC_OFFSET;
                                                                 }
                                                               else
                                                                 {
                                                                 objectError = GRAPHICS_OBJECT_SYMBOL_ERROR;
                                                                 break;
                                                                 }
                                                               }
                                                             }

                                                           if (objectError == GRAPHICS_NO_ERROR)
                                                             {
                                                             symbolAccumulator = symbolAccumulator + (symbolHexade * symbolWeight); 
                                                             symbolWeight      = symbolWeight / HEXADECIMAL_CHARACTER_WEIGHT_SHIFT;
                                                             }
                                                           }
                                                         }

                                                       if (objectError == GRAPHICS_NO_ERROR)
                                                         {
                                                         *((GRAPHICS_UINT_PTR)decodedSymbol) = symbolAccumulator;
                                                         *symbolIndex                        = localSymbolIndex + thisSymbolIndex;
                                                         }
                                                       else
                                                         {
                                                         objectError = GRAPHICS_OBJECT_SYMBOL_ERROR;
                                                         }
                                                       }
                                                     }
                                                   break;

      default                                    : objectError = GRAPHICS_OBJECT_SYMBOL_ERROR;
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
  } /* end of advanceSymbolIndex                                              */

/******************************************************************************/
/* findNextWideTokenFile() :                                                  */
/* - scans a wide-character file for a requested token, ignoring 'white-      */
/*   space' characters as it goes. At each comparison the file pointer will   */
/*   increment                                                                */
/******************************************************************************/

graphicsError_t findNextWideTokenFile(FILE                  **wideCharacterFile, 
                                      GRAPHICS_WIDE_INT       wideCharacterTokenRequested,
                                      GRAPHICS_WIDE_INT_PTR   wideCharacterTokenFound)
  {
/******************************************************************************/

  GRAPHICS_WIDE_INT latestWideToken = ((GRAPHICS_WIDE_INT)0);

  graphicsError_t   objectError     = GRAPHICS_NO_ERROR;

/******************************************************************************/

  while ((latestWideToken = fgetwc(*wideCharacterFile)) != WEOF)
    {
    if (wideCharacterTokenRequested == GRAPHICS_SEGMENT_ANY_SYMBOL)
      {
      // Return ANY token found
      *wideCharacterTokenFound = latestWideToken;
       break;
      }
    else
      { // Return the requested token
      if (latestWideToken == wideCharacterTokenRequested)
        {
        *wideCharacterTokenFound = latestWideToken;
         break;
        }
      }
    }
         
  if (latestWideToken == WEOF)
    {
    objectError = GRAPHICS_OBJECT_END_OF_FILE_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of findNextWideTokenFile                                           */

/******************************************************************************/
/* isnum() :                                                                  */
/******************************************************************************/

graphicsError_t isnum(GRAPHICS_INT character)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (((character & ALPHABET_ASCII_CHARACTER_MASK) < (GRAPHICS_INT)ASCII_NUMERIC_0) ||
      ((character & ALPHABET_ASCII_CHARACTER_MASK) > (GRAPHICS_INT)ASCII_NUMERIC_9))
    {
    objectError = GRAPHICS_OBJECT_CHARACTER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of isnum                                                           */

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/