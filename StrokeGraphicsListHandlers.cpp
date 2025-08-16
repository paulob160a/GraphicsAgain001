/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* StrokeGraphicsListHandlers.cpp :                                           */
/* Paul O'Brien                                                               */
/* 09.06.25                                                                   */
/*                                                                            */
/* 2D GUI to create stroke/vector graphic alphanumeric set                    */
/******************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "StrokeGraphicsAlphabet.h"
#include "GraphicsAgainCaption.h"
#include "StrokeGraphicsListHandlers.h"

/******************************************************************************/

alphabetCharacters_tPtr characterListRoot = nullptr;
alphabetCharacters_tPtr characterCache    = nullptr;

/******************************************************************************/
/* buildCharacterList() :                                                     */
/******************************************************************************/

graphicsError_t buildCharacterList(const GRAPHICS_UINT            characterListLength,
                                         alphabetCharacters_tPtr *alphabetCharacters)
  {
/******************************************************************************/

  GRAPHICS_UINT           listIndex     = ((GRAPHICS_UINT)0);

  alphabetCharacters_tPtr newCharacter  = nullptr,
                          lastCharacter = nullptr;

  lineSegment_tPtr        newSegment    = nullptr;
                                        
  graphicsError_t         objectError   = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((characterListLength >= ALPHABET_MINIMUM_SIZE) && ( characterListLength <= ALPHABET_SIZE) && 
      (alphabetCharacters  != nullptr)               && (*alphabetCharacters  == nullptr))
    {
    for (listIndex = ((GRAPHICS_UINT)0); listIndex < characterListLength; listIndex++)
      {
      if ((newCharacter = (alphabetCharacters_tPtr)calloc((size_t(1)), ((size_t)sizeof(alphabetCharacters_t)))) != nullptr)
        {
        // The first line segment descriptor is instantiated here
        if ((newSegment = (lineSegment_tPtr)calloc(((size_t)1), sizeof(lineSegment_t))) != nullptr)
          {         
          newCharacter->characterNumber       = listIndex;
          newCharacter->characterState        = ALPHABET_CHARACTER_STATE_NONE;
          newCharacter->characterLineSegments = newSegment;
          newCharacter->numberOfLineSegments  = ((GRAPHICS_UINT)0);
          newCharacter->lineSegmentIndex      = ((GRAPHICS_UINT)0); // used to step through the line indices during modification/editing mode
          newCharacter->lastCharacter         = nullptr;
          newCharacter->nextCharacter         = nullptr;
          
          if (lastCharacter == nullptr)
            {
             lastCharacter      = newCharacter;
            *alphabetCharacters = lastCharacter;
            }
          else
            {
            lastCharacter->nextCharacter = newCharacter;
            newCharacter->lastCharacter  = lastCharacter;
            lastCharacter                = newCharacter;
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
          break;
          }
        }
      else
        {
        objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
        break;
        }
      }

    // Complete the loop
    if (objectError == GRAPHICS_NO_ERROR)
      {
       newCharacter->nextCharacter          = *alphabetCharacters;
       (*alphabetCharacters)->lastCharacter =  newCharacter;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of buildCharacterList                                              */

/******************************************************************************/
/* addLineSegmentToCharacterList() :                                          */
/******************************************************************************/

graphicsError_t addLineSegmentToCharacterList(const GRAPHICS_UINT           characterSelected,
                                              const lineSegment_tPtr        newLineSegment,
                                                    alphabetCharacters_tPtr alphabetCharacters)
  {
/******************************************************************************/

  GRAPHICS_UINT    characterIndex    = ((GRAPHICS_UINT)0);

  lineSegment_tPtr newSegment        = nullptr,
                   characterSegments = nullptr;

  graphicsError_t  objectError       = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((characterSelected < ALPHABET_SIZE) && (newLineSegment != nullptr) && (alphabetCharacters != nullptr))
    {
    if ((newSegment = (lineSegment_tPtr)calloc(((size_t)1), sizeof(lineSegment_t))) != nullptr)
      {
      // Select the current character
      for (characterIndex = ((GRAPHICS_UINT)0); characterIndex < characterSelected; characterIndex++)
        {
        alphabetCharacters = alphabetCharacters->nextCharacter;
        }

      characterSegments = alphabetCharacters->characterLineSegments;

      while (characterSegments->nextLineSegment != nullptr)
        {
        characterSegments = characterSegments->nextLineSegment;
        }

      // The 'current segment' always points to an instantiated line segment descriptor, but there 
      // is no reason to save points ( origin(x,y) == destination(x,y) )
      if ((newLineSegment->lineSegmentOriginX != newLineSegment->lineSegmentDestinationX) || 
          (newLineSegment->lineSegmentOriginY != newLineSegment->lineSegmentDestinationY))
        {
        // Segment is good, save the latest number of segments
        alphabetCharacters->numberOfLineSegments   = alphabetCharacters->numberOfLineSegments + ((GRAPHICS_UINT)1);

        characterSegments->lineSegmentOriginX      = newLineSegment->lineSegmentOriginX;
        characterSegments->lineSegmentOriginY      = newLineSegment->lineSegmentOriginY;
        characterSegments->lineSegmentDestinationX = newLineSegment->lineSegmentDestinationX;
        characterSegments->lineSegmentDestinationY = newLineSegment->lineSegmentDestinationY;
       
        characterSegments->nextLineSegment         = newSegment;
        newSegment->lastLineSegment                = characterSegments;
        }
      else
        {
        objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
        free(newSegment);
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
  } /* end of addLineSegmentToCharacterList                                   */

/******************************************************************************/
/* fetchCharacterReference() :                                                */
/******************************************************************************/

graphicsError_t fetchCharacterReference(const GRAPHICS_UINT            characterSelected,
                                        const alphabetCharacters_tPtr  alphabetCharacters,
                                              alphabetCharacters_tPtr *characterReference)
  {
/******************************************************************************/

  GRAPHICS_UINT   characterIndex = ((GRAPHICS_UINT)0);

  graphicsError_t objectError    = GRAPHICS_NO_ERROR;

/******************************************************************************/

    if ((characterSelected   < ALPHABET_SIZE) && (alphabetCharacters != nullptr) &&
        (characterReference != nullptr))
      {
      *characterReference = alphabetCharacters;

       // Select the requested character reference
      for (characterIndex = ((GRAPHICS_UINT)0); characterIndex < characterSelected; characterIndex++)
        {
        *characterReference = (*characterReference)->nextCharacter;
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
      }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of fetchCharacterReference                                         */

/******************************************************************************/
/* fetchCharacterExtantSegment() :                                            */
/******************************************************************************/

graphicsError_t fetchCharacterExtantSegment(const GRAPHICS_INT             segmentNumber,
                                            const alphabetCharacters_tPtr  alphabetCharacterReference,
                                                  lineSegment_tPtr         *newLineSegment)
  {
/******************************************************************************/

  lineSegment_tPtr segmentReference = nullptr;
  GRAPHICS_UINT    segmentIndex     = ((GRAPHICS_INT)-1); // the first occupied segment index will increment to zero
                                    
  graphicsError_t  objectError      = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((newLineSegment != nullptr) && (alphabetCharacterReference != nullptr))
    {
    segmentReference = alphabetCharacterReference->characterLineSegments;

    // Check if there are any extant line segments - there is always an empty 
    // segment structure at the end of the list that points to nothing
    if (segmentReference->nextLineSegment != nullptr)
      {
      while (segmentReference->nextLineSegment != nullptr)
        {
        // Prepare to return the reference to the segment contents
        *newLineSegment = segmentReference;

         segmentIndex   = segmentIndex + ((GRAPHICS_INT)1);

        // The loop will stop if the list is too short i.e. the segment at index 'n' 
        // doesn't exist. There is no such thing as "too long"!
        if (segmentIndex == segmentNumber)
          {
          break;
          }
        else
          {
          segmentReference = segmentReference->nextLineSegment;
          }
        }

      // If the list is too short...
      if (segmentIndex != segmentNumber)
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
  } /* fetchCharacterExtantSegment                                            */

/******************************************************************************/
/* cloneAndNormaliseCharacterSegments() :                                     */
/* - converts a characters' line segments to normalised 'REAL' numbers        */
/******************************************************************************/

graphicsError_t cloneAndNormaliseCharacterSegments(const alphabetCharacters_tPtr      characterReference,
                                                         alphabetCharactersReal_tPtr *normalisedReference)
  {
/******************************************************************************/

  GRAPHICS_UINT        newSegmentIndex     = ((GRAPHICS_UINT)0);
  lineSegment_tPtr     oldSegmentTrack     = nullptr;

  lineSegmentReal_tPtr newSegmentTrack     = nullptr,
                       newSegmentReference = nullptr,
                       nullSegment         = nullptr;

  GRAPHICS_INT64       extentLeftX         = ((GRAPHICS_INT)0),
                       extentTopY          = ((GRAPHICS_INT)0),
                       extentRightX        = ((GRAPHICS_INT)0),
                       extentBottomY       = ((GRAPHICS_INT)0);

  GRAPHICS_REAL        normaliseMaximumX   = ((GRAPHICS_REAL)0.0),
                       normaliseMaximumY   = ((GRAPHICS_REAL)0.0);
                       
  graphicsError_t      objectError         = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (characterReference != nullptr)
    {
    if (characterReference->numberOfLineSegments != ((GRAPHICS_UINT)0))
      {
      // Compute the normalising range from the character extents
      extentLeftX   = characterReference->characterExtents.topLeft.pointX;
      extentTopY    = characterReference->characterExtents.topLeft.pointY;
      extentRightX  = characterReference->characterExtents.bottomRight.pointX;
      extentBottomY = characterReference->characterExtents.bottomRight.pointY;

      // If 'x' or 'y' extents are equal no normalisation in that axis is 
      // necessary/possible
      if ((extentLeftX - extentRightX) != ((GRAPHICS_INT)0))
        {
        if (abs(extentLeftX) >= abs(extentRightX))
          {
          normaliseMaximumX = (GRAPHICS_REAL)abs(extentLeftX);
          }
        else
          {
          normaliseMaximumX = (GRAPHICS_REAL)abs(extentRightX);
          }
        }

      if ((extentTopY - extentBottomY) != ((GRAPHICS_INT)0))
        {
        if (abs(extentTopY) >= abs(extentBottomY))
          {
          normaliseMaximumY = (GRAPHICS_REAL)abs(extentTopY);
          }
        else
          {
          normaliseMaximumY = (GRAPHICS_REAL)abs(extentBottomY);
          }
        }

      if ((*normalisedReference = (alphabetCharactersReal_tPtr)calloc(((size_t)1), ((size_t)sizeof(alphabetCharactersReal_t)))) != nullptr)
        {
        // Make a partial "deep" copy of the character definition
        (*normalisedReference)->characterNumber      = characterReference->characterNumber;

        // Character extents are generated as signed 32-bit numbers but loaded as signed 64-bit numbers. Hence 
        // the signage gets lost! The serial casts should recover negative numbers
        (*normalisedReference)->characterExtents.topLeft.pointX     = (GRAPHICS_INT64)((GRAPHICS_INT)characterReference->characterExtents.topLeft.pointX);
        (*normalisedReference)->characterExtents.topLeft.pointY     = (GRAPHICS_INT64)((GRAPHICS_INT)characterReference->characterExtents.topLeft.pointY);
        (*normalisedReference)->characterExtents.bottomRight.pointX = (GRAPHICS_INT64)((GRAPHICS_INT)characterReference->characterExtents.bottomRight.pointX);
        (*normalisedReference)->characterExtents.bottomRight.pointY = (GRAPHICS_INT64)((GRAPHICS_INT)characterReference->characterExtents.bottomRight.pointY);

        (*normalisedReference)->lineSegmentIndex                    = ((GRAPHICS_INT)0);

        // Create an empty segment to terminate the list
        if ((nullSegment = (lineSegmentReal_tPtr)calloc(((size_t)1), ((size_t)sizeof(lineSegmentReal_t)))) != nullptr)
          {
          // Recreate and normalise the segments
          (*normalisedReference)->numberOfLineSegments  = ((GRAPHICS_UINT)0);
          (*normalisedReference)->characterLineSegments = nullSegment;
       
          oldSegmentTrack                               = characterReference->characterLineSegments;

          for (newSegmentIndex = ((GRAPHICS_UINT)0); newSegmentIndex < characterReference->numberOfLineSegments; newSegmentIndex++)
            {
            if ((newSegmentReference = (lineSegmentReal_tPtr)calloc(((size_t)1), ((size_t)sizeof(lineSegmentReal_t)))) != nullptr)
              {
              newSegmentTrack                              = (*normalisedReference)->characterLineSegments;

              // Populate the new segment; the pointers will be recalculated
              newSegmentReference->lineSegmentOriginX      = (GRAPHICS_REAL)oldSegmentTrack->lineSegmentOriginX;
              newSegmentReference->lineSegmentOriginY      = (GRAPHICS_REAL)oldSegmentTrack->lineSegmentOriginY;
              newSegmentReference->lineSegmentDestinationX = (GRAPHICS_REAL)oldSegmentTrack->lineSegmentDestinationX;
              newSegmentReference->lineSegmentDestinationY = (GRAPHICS_REAL)oldSegmentTrack->lineSegmentDestinationY;

              // Normalise the two line segment end-points
              if (normaliseMaximumX != ((GRAPHICS_REAL)0.0))
                {
                newSegmentReference->lineSegmentOriginX      = newSegmentReference->lineSegmentOriginX      / normaliseMaximumX;
                newSegmentReference->lineSegmentDestinationX = newSegmentReference->lineSegmentDestinationX / normaliseMaximumX;
                }
              
              if (normaliseMaximumY != ((GRAPHICS_REAL)0.0))
                {
                newSegmentReference->lineSegmentOriginY      = newSegmentReference->lineSegmentOriginY      / normaliseMaximumY;
                newSegmentReference->lineSegmentDestinationY = newSegmentReference->lineSegmentDestinationY / normaliseMaximumY;
                }

              (*normalisedReference)->characterLineSegments =  newSegmentReference;
              newSegmentReference->nextLineSegment          =  newSegmentTrack;
              newSegmentTrack->lastLineSegment              =  newSegmentReference;
                                                         
              oldSegmentTrack                               =  oldSegmentTrack->nextLineSegment;

              (*normalisedReference)->numberOfLineSegments  = (*normalisedReference)->numberOfLineSegments + ((GRAPHICS_UINT)1);       
              }
            else
              {
              objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
              break;
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
      if (characterReference->characterNumber == ALPHABET_ASCII_SPACE) 
        {
        // Creatre a very sparse "normalisedReference"
        if ((*normalisedReference = (alphabetCharactersReal_tPtr)calloc(((size_t)1), ((size_t)sizeof(alphabetCharactersReal_t)))) != nullptr)
          {
          (*normalisedReference)->characterNumber = ALPHABET_ASCII_SPACE;
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
  } /* end of cloneAndNormaliseCharacterSegments                              */

/******************************************************************************/
/* drawNormalisedStrokeCharacter() :                                          */
/******************************************************************************/

graphicsError_t drawNormalisedStrokeCharacter(      HDC                             hdc,
                                              const strokeTextStringDescriptor_tPtr strokeTextStringCharacters,
                                              const alphabetCharactersReal_tPtr     normalisedReference,
                                                    strokeCharacterTrack_tPtr       characterTrack,
                                              const canvasDescriptor_tPtr           canvasSize,
                                              const objectColour_tPtr               strokeTextStringColour,
                                              const strokeGraphPointBase_tPtr       strokeGraphBase)
  {
/******************************************************************************/

  Graphics             graphics(hdc);

  GRAPHICS_REAL        deltaX                   = ((GRAPHICS_REAL)0.0),
                       deltaY                   = ((GRAPHICS_REAL)0.0);
                                                
  GRAPHICS_REAL        extentTopX               = ((GRAPHICS_REAL)0.0),
                       extentTopY               = ((GRAPHICS_REAL)0.0),
                       extentBottomX            = ((GRAPHICS_REAL)0.0),
                       extentBottomY            = ((GRAPHICS_REAL)0.0),
                       extentRangeX             = ((GRAPHICS_REAL)0.0),
                       extentRangeY             = ((GRAPHICS_REAL)0.0);
                                                
  GRAPHICS_REAL        characterResizeX         = ((GRAPHICS_REAL)0.0), // resize the character using the requested width and 
                       characterResizeY         = ((GRAPHICS_REAL)0.0), // depth as modifiers to the extents
                       characterTextWidth       = ((GRAPHICS_REAL)0.0),
                       characterDescenderOffset = ((GRAPHICS_REAL)0.0); // move the character 'y' coordinates down to account
                                                                        // for descenders, based on the priginal 'row' and
                                                                        // column values used in "StrokeGraphics"

  GRAPHICS_FLOAT       endPointX                = ((GRAPHICS_FLOAT)0.0),
                       endPointY                = ((GRAPHICS_FLOAT)0.0),
                       startPointX              = ((GRAPHICS_FLOAT)0.0),
                       startPointY              = ((GRAPHICS_FLOAT)0.0);
                                                
  lineSegmentReal_tPtr segmentTrack             = nullptr;
                                                
  graphicsError_t      objectError              = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((strokeTextStringCharacters  != nullptr)   && (normalisedReference != nullptr) && 
      (characterTrack              != nullptr)   && (canvasSize          != nullptr) &&
      (strokeTextStringColour      != nullptr))
    {
    if (normalisedReference->numberOfLineSegments != ((GRAPHICS_UINT)0))
      {
      // Select the text colour
      Pen pen(Color((BYTE)strokeTextStringColour->opacity,
                    (BYTE)strokeTextStringColour->red,
                    (BYTE)strokeTextStringColour->green,
                    (BYTE)strokeTextStringColour->blue));
      
      pen.SetWidth((REAL)strokeTextStringCharacters->strokeLineWidth);
      
      pen.SetStartCap(LineCapRound);
      pen.SetEndCap(LineCapRound);
      
      // Compress or expand the character dimensions if they are greater/lesser than the allocated space
      deltaX           = ((GRAPHICS_REAL)(canvasSize->right))  - ((GRAPHICS_FLOAT)(canvasSize->left));
      deltaY           = ((GRAPHICS_REAL)(canvasSize->bottom)) - ((GRAPHICS_FLOAT)(canvasSize->top));
      
      // At this point the character extent values are still related to the number of rows and columns 
      // used in their original definition in "StrokeGraphics"...
      extentTopX       = (GRAPHICS_REAL)((GRAPHICS_INT64)normalisedReference->characterExtents.topLeft.pointX);
      extentTopY       = (GRAPHICS_REAL)((GRAPHICS_INT64)normalisedReference->characterExtents.topLeft.pointY);
      extentBottomX    = (GRAPHICS_REAL)((GRAPHICS_INT64)normalisedReference->characterExtents.bottomRight.pointX);
      extentBottomY    = (GRAPHICS_REAL)((GRAPHICS_INT64)normalisedReference->characterExtents.bottomRight.pointY);
      
      // Check and correct for descender dimensions
      characterDescenderOffset = strokeGraphBase->descenderRatio * strokeGraphBase->graphColumnNumber; // 'column' and 'row' are back-to-front...
      
      // The descender row is an integer number for detection but a real number for correction...
      // NOTE : this value is calculated once for the character being drawn and is reset once 
      //        at the next function invocation
      if (((GRAPHICS_UINT)extentBottomY) > ((GRAPHICS_UINT)characterDescenderOffset))
        {
        characterDescenderOffset = extentBottomY - characterDescenderOffset - ((GRAPHICS_REAL)1.0);
        }
      else
        {
        characterDescenderOffset = ((GRAPHICS_REAL)0.0);
        }
      
      extentRangeX     = extentBottomX - extentTopX;
      extentRangeY     = extentBottomY - extentTopY;
      
      characterResizeX = extentRangeX;
      characterResizeY = extentRangeY;
      
      if (characterResizeX > (strokeTextStringCharacters->strokeTextCharacterWidth * deltaX))
        {
        characterResizeX = (strokeTextStringCharacters->strokeTextCharacterWidth * deltaX) / characterResizeX;
        }
      else
        {
        characterResizeX = (strokeTextStringCharacters->strokeTextCharacterWidth * deltaX) / characterResizeX;
        }
      
      if (characterResizeY > (strokeTextStringCharacters->strokeTextCharacterDepth * deltaY))
        {
        characterResizeY = (strokeTextStringCharacters->strokeTextCharacterDepth * deltaY) / characterResizeY;
        }
      else
        {
        characterResizeY = (strokeTextStringCharacters->strokeTextCharacterDepth * deltaY) / characterResizeY;
        } 
      
      // Now draw the segments
      segmentTrack = normalisedReference->characterLineSegments;
      
      while (segmentTrack->nextLineSegment != nullptr)
        {
        startPointX = (GRAPHICS_FLOAT)((characterTrack->characterPlacementX * deltaX) + (segmentTrack->lineSegmentOriginX * (extentRangeX * characterResizeX)));
        startPointY = (GRAPHICS_FLOAT)(((strokeTextStringCharacters->strokeTextAnchor.yAxisPoint) * deltaY) + (segmentTrack->lineSegmentOriginY * (extentRangeY * characterResizeY)));
        startPointY = startPointY + (GRAPHICS_FLOAT)characterDescenderOffset;
      
        endPointX   = (GRAPHICS_FLOAT)((characterTrack->characterPlacementX * deltaX) + (segmentTrack->lineSegmentDestinationX * (extentRangeX * characterResizeX)));
        endPointY   = (GRAPHICS_FLOAT)(((strokeTextStringCharacters->strokeTextAnchor.yAxisPoint) * deltaY) + (segmentTrack->lineSegmentDestinationY * (extentRangeY * characterResizeY)));
        endPointY   = endPointY + (GRAPHICS_FLOAT)characterDescenderOffset;
      
        graphics.DrawLine((const Pen *)&pen, startPointX,  startPointY, endPointX, endPointY);
      
        segmentTrack = segmentTrack->nextLineSegment;
        }
      
      // Return the components of the running average of the sum of the largest 'resize in X' values
      characterTrack->characterWidthX.yAxisPoint = characterTrack->characterWidthX.yAxisPoint + ((GRAPHICS_REAL)1.0);
      
      characterTrack->characterWidthX.xAxisPoint = characterTrack->characterWidthX.xAxisPoint + characterResizeX;
      }
    else
      {
      if (normalisedReference->characterNumber == ALPHABET_ASCII_SPACE)
        {
        characterTrack->characterWidthX.yAxisPoint = characterTrack->characterWidthX.yAxisPoint + ((GRAPHICS_REAL)1.0);
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
  } /* end of drawNormalisedStrokeCharacter                                   */

/******************************************************************************/
/* computeCharacterExtents() :                                                */
/* - scans the characters' line segments and determines the maximum { x, y }  */
/*   coordinates at each "corner" of the character                            */
/******************************************************************************/

graphicsError_t computeCharacterExtents(const GRAPHICS_UINT             characterSelected,
                                        const alphabetCharacters_tPtr   alphabetCharacters,
                                        const strokeGraphPointBase_tPtr strokeGraphicBase)
  {
/******************************************************************************/

  alphabetCharacters_tPtr characterReference = nullptr;

  lineSegment_tPtr        nextSegment        = nullptr;

  graphicsError_t         objectError        = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((alphabetCharacters != nullptr) && (strokeGraphicBase != nullptr) && (characterSelected < ALPHABET_SIZE))
    {
    if ((objectError = fetchCharacterReference( characterSelected,
                                                alphabetCharacters,
                                               &characterReference)) == GRAPHICS_NO_ERROR)
      {
      characterReference->characterExtents.topLeft.pointX     = ((GRAPHICS_INT)0);
      characterReference->characterExtents.topLeft.pointY     = ((GRAPHICS_INT)0);
      characterReference->characterExtents.bottomRight.pointX = ((GRAPHICS_INT)0);
      characterReference->characterExtents.bottomRight.pointY = ((GRAPHICS_INT)0);

      nextSegment = characterReference->characterLineSegments;

      if (nextSegment->nextLineSegment == nullptr)     
        {
        // May or may not be an error per se - could be the character hasn't been built yet
        objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
        }
      else
        {
        characterReference->characterExtents.topLeft.pointX     = ((GRAPHICS_INT)strokeGraphicBase->graphRowNumber);
        characterReference->characterExtents.topLeft.pointY     = ((GRAPHICS_INT)strokeGraphicBase->graphColumnNumber);
        characterReference->characterExtents.bottomRight.pointX = ((GRAPHICS_INT)0);
        characterReference->characterExtents.bottomRight.pointY = ((GRAPHICS_INT)0);

        while (nextSegment->nextLineSegment != nullptr)
          {      
          // "Box" the character's extents
          // TopLeft X : 
          if (nextSegment->lineSegmentOriginX <= nextSegment->lineSegmentDestinationX)
            {
            if (nextSegment->lineSegmentOriginX <= characterReference->characterExtents.topLeft.pointX)
              {
              characterReference->characterExtents.topLeft.pointX = nextSegment->lineSegmentOriginX;
              }
            }
          else
            {
            if (nextSegment->lineSegmentDestinationX <= characterReference->characterExtents.topLeft.pointX)
              {
              characterReference->characterExtents.topLeft.pointX = nextSegment->lineSegmentDestinationX;
              }
            }
       
          // TopLeft Y : 
          if (nextSegment->lineSegmentOriginY <= nextSegment->lineSegmentDestinationY)
            {
            if (nextSegment->lineSegmentOriginY <= characterReference->characterExtents.topLeft.pointY)
              {
              characterReference->characterExtents.topLeft.pointY = nextSegment->lineSegmentOriginY;
              }
            }
          else
            {
            if (nextSegment->lineSegmentDestinationY <= characterReference->characterExtents.topLeft.pointY)
              {
              characterReference->characterExtents.topLeft.pointY = nextSegment->lineSegmentDestinationY;
              }
            }

          // BottomRight X : 
          if (nextSegment->lineSegmentOriginX >= nextSegment->lineSegmentDestinationX)
            {
            if (nextSegment->lineSegmentOriginX >= characterReference->characterExtents.bottomRight.pointX)
              {
              characterReference->characterExtents.bottomRight.pointX = nextSegment->lineSegmentOriginX;
              }
            }
          else
            {
            if (nextSegment->lineSegmentDestinationX >= characterReference->characterExtents.bottomRight.pointX)
              {
              characterReference->characterExtents.bottomRight.pointX = nextSegment->lineSegmentDestinationX;
              }
            }

          // BottomRight Y : 
          if (nextSegment->lineSegmentOriginY >= nextSegment->lineSegmentDestinationY)
            {
            if (nextSegment->lineSegmentOriginY >= characterReference->characterExtents.bottomRight.pointY)
              {
              characterReference->characterExtents.bottomRight.pointY = nextSegment->lineSegmentOriginY;
              }
            }
          else
            {
            if (nextSegment->lineSegmentDestinationY >= characterReference->characterExtents.bottomRight.pointY)
              {
              characterReference->characterExtents.bottomRight.pointY = nextSegment->lineSegmentDestinationY;
              }
            }

          nextSegment = nextSegment->nextLineSegment;
          }
        }
      }
    else
      {
      objectError = GRAPHICS_OBJECT_CHARACTER_ERROR;
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of computeCharacterExtents                                         */

/******************************************************************************/
/* deleteCharacter() :                                                        */
/******************************************************************************/

graphicsError_t deleteCharacter(alphabetCharacters_tPtr selectedCharacter)
  {
/******************************************************************************/

  lineSegment_tPtr thislineSegment = nullptr;
  graphicsError_t  objectError      = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((selectedCharacter != nullptr) && (selectedCharacter->characterLineSegments != nullptr))
    {
    if (selectedCharacter->numberOfLineSegments > ((GRAPHICS_UINT)0))
      {
      thislineSegment                     = selectedCharacter->characterLineSegments;
      selectedCharacter->lineSegmentIndex = (GRAPHICS_INT)(selectedCharacter->numberOfLineSegments - ((GRAPHICS_UINT)1));
      
      while ((selectedCharacter->lineSegmentIndex >= ((GRAPHICS_INT)0)) && (selectedCharacter->numberOfLineSegments > ((GRAPHICS_UINT)0)))
        {
        if (fetchCharacterExtantSegment(  selectedCharacter->lineSegmentIndex,
                                          selectedCharacter,
                                         &thislineSegment) == GRAPHICS_NO_ERROR)
          {
          if (deleteCharacterExtantSegment(selectedCharacter,
                                           thislineSegment) != GRAPHICS_NO_ERROR)
            {
            objectError = GRAPHICS_OBJECT_CHARACTER_ERROR;
            break;
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_CHARACTER_ERROR;
          break;
          }
        }
      }
    else
      {
      // The character in use is a pointer-based clone of a character in the 
      // full alphabet list
      delete(selectedCharacter);
      }
    }
  else
    {
    objectError = GRAPHICS_OBJECT_PARAMETER_ERROR;
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of deleteCharacter                                                 */

/******************************************************************************/
/* deleteCharacterExtantSegment() :                                           */
/******************************************************************************/

graphicsError_t deleteCharacterExtantSegment(alphabetCharacters_tPtr alphabetCharacters,
                                             lineSegment_tPtr        lineSegment)
  {
/******************************************************************************/

  graphicsError_t objectError = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((alphabetCharacters != nullptr) && (lineSegment != nullptr))
    {
    if (lineSegment->nextLineSegment != nullptr)
      {
      // Cut the segment out of the list
      if (alphabetCharacters->lineSegmentIndex == ((GRAPHICS_INT)0))
        { // The head of the segment list now points to the next segment descriptor
        alphabetCharacters->characterLineSegments                  = lineSegment->nextLineSegment;
        alphabetCharacters->characterLineSegments->lastLineSegment = nullptr;

        alphabetCharacters->numberOfLineSegments                   = alphabetCharacters->numberOfLineSegments - ((GRAPHICS_UINT)1);
        }
      else
        {
        lineSegment->lastLineSegment->nextLineSegment = lineSegment->nextLineSegment;
        lineSegment->nextLineSegment->lastLineSegment = lineSegment->lastLineSegment;

        alphabetCharacters->numberOfLineSegments      = alphabetCharacters->numberOfLineSegments - ((GRAPHICS_UINT)1);
        alphabetCharacters->lineSegmentIndex          = alphabetCharacters->lineSegmentIndex     - ((GRAPHICS_UINT)1);
        }

      free(lineSegment);
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
  } /* end of deleteCharacterExtantSegment                                    */

/******************************************************************************/
/* flipCharacterHorizontally() :                                              */
/******************************************************************************/

graphicsError_t flipCharacterHorizontally(alphabetCharacters_tPtr selectedCharacter)
  {
/******************************************************************************/

  lineSegment_tPtr lineSegment   = nullptr;
                                 
  GRAPHICS_INT64   extentTop     = ((GRAPHICS_INT)0),
                   extentBottom  = ((GRAPHICS_INT)0);
                   
  GRAPHICS_FLOAT   midLine       = ((GRAPHICS_FLOAT)0),
                   pointDistance = ((GRAPHICS_FLOAT)0),
                   pointY        = ((GRAPHICS_FLOAT)0);
                   
  graphicsError_t  objectError   = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (selectedCharacter != nullptr)
    {
    if (selectedCharacter->numberOfLineSegments > ((GRAPHICS_UINT)1))
      {
      selectedCharacter->lineSegmentIndex = selectedCharacter->numberOfLineSegments - ((GRAPHICS_UINT)1);

      extentTop    = selectedCharacter->characterExtents.topLeft.pointY;
      extentBottom = selectedCharacter->characterExtents.bottomRight.pointY;

      if (extentBottom != extentTop) // cannot flip a linear 'shape'!
        {
        if (extentBottom > (extentTop + ((GRAPHICS_INT)1))) // points are seperated by a mid-line
          {
          // A definite mid-line...for an even extent in 'x' the mid-line is an integer but 
          // for an odd extent it will be 'nnn.05'
          midLine     =  ((GRAPHICS_FLOAT)extentTop) + ((((GRAPHICS_FLOAT)extentBottom) - ((GRAPHICS_FLOAT)extentTop)) / ((GRAPHICS_FLOAT)2.0));

          lineSegment = selectedCharacter->characterLineSegments;
       
          while (lineSegment->nextLineSegment != nullptr)
            {
            pointY = ((GRAPHICS_FLOAT)lineSegment->lineSegmentOriginY);

            if (pointY < midLine)
              {
              pointDistance = midLine - pointY;

              lineSegment->lineSegmentOriginY = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointY > midLine)
                {
                pointDistance = pointY - midLine;

                lineSegment->lineSegmentOriginY = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentOriginY = (GRAPHICS_INT)midLine;
                }
              }

            pointY = ((GRAPHICS_FLOAT)lineSegment->lineSegmentDestinationY);

            if (pointY < midLine)
              {
              pointDistance = midLine - pointY;

              lineSegment->lineSegmentDestinationY = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointY > midLine)
                {
                pointDistance = pointY - midLine;

                lineSegment->lineSegmentDestinationY = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentDestinationY = (GRAPHICS_INT)midLine;
                }
              }

            lineSegment = lineSegment->nextLineSegment;
            }
          }
        else
          {
          if (extentBottom > extentTop) // all points are seperated by 0 or +1
            {
            lineSegment = selectedCharacter->characterLineSegments;

            while (lineSegment->nextLineSegment != nullptr)
              {
              if (lineSegment->lineSegmentOriginY == extentBottom)
                {
                lineSegment->lineSegmentOriginY = extentTop;
                }
              else
                {
                lineSegment->lineSegmentOriginY = extentBottom;
                }

              if (lineSegment->lineSegmentDestinationY == extentBottom)
                {
                lineSegment->lineSegmentDestinationY = extentTop;
                }
              else
                {
                lineSegment->lineSegmentDestinationY = extentBottom;
                }

              lineSegment = lineSegment->nextLineSegment;
              }
            }
          else
            {
            // What ?
            objectError = GRAPHICS_OBJECT_RANGE_ERROR;
            }
          }
        }
      }
    else
      {
      if (selectedCharacter->numberOfLineSegments == ((GRAPHICS_UINT)1))
        {
        // The image is a single line segment and the segment end-points are 
        // equal to the extents. Just mirror top-left and bottom-right
        extentTop  = selectedCharacter->characterExtents.topLeft.pointY;
        extentBottom = selectedCharacter->characterExtents.bottomRight.pointY;

        if (extentBottom != extentTop) // cannot flip a linear 'shape'!
          {
          if (extentBottom > (extentTop + ((GRAPHICS_INT)1))) // points are seperated by a mid-line
            {
            // A definite mid-line...for an even extent in 'x' the mid-line is an integer but 
            // for an odd extent it will be 'nnn.05'
            midLine     = ((GRAPHICS_FLOAT)extentTop) + ((((GRAPHICS_FLOAT)extentBottom) - ((GRAPHICS_FLOAT)extentTop)) / ((GRAPHICS_FLOAT)2.0));

            lineSegment = selectedCharacter->characterLineSegments;

            pointY = ((GRAPHICS_FLOAT)lineSegment->lineSegmentOriginY);

            if (pointY < midLine)
              {
              pointDistance = midLine - pointY;

              lineSegment->lineSegmentOriginY = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointY > midLine)
                {
                pointDistance = pointY - midLine;

                lineSegment->lineSegmentOriginY = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentOriginY = (GRAPHICS_INT)midLine;
                }
              }

            pointY = ((GRAPHICS_FLOAT)lineSegment->lineSegmentDestinationY);

            if (pointY < midLine)
              {
              pointDistance = midLine - pointY;

              lineSegment->lineSegmentDestinationY = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointY > midLine)
                {
                pointDistance = pointY - midLine;

                lineSegment->lineSegmentDestinationY = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentDestinationY = (GRAPHICS_INT)midLine;
                }
              }
            }
          else
            {
            if (extentBottom > extentTop) // all points are seperated by 0 or +1
              {
              lineSegment = selectedCharacter->characterLineSegments;
        
              if (lineSegment->lineSegmentOriginY == extentBottom)
                {
                lineSegment->lineSegmentOriginY = extentTop;
                }
              else
                {
                lineSegment->lineSegmentOriginY = extentBottom;
                }
        
              if (lineSegment->lineSegmentDestinationY == extentBottom)
                {
                lineSegment->lineSegmentDestinationY = extentTop;
                }
              else
                {
                lineSegment->lineSegmentDestinationY = extentBottom;
                }
              }
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
          }  
        }
      }
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of flipCharacterHorizontally                                       */

/******************************************************************************/
/* flipCharacterVertically :                                                  */
/* - note "left" and "right" imply that "left" coordinate values are smaller  */
/*   than "right" coordinate values                                           */
/******************************************************************************/

graphicsError_t flipCharacterVertically(alphabetCharacters_tPtr selectedCharacter)
  {
/******************************************************************************/

  lineSegment_tPtr lineSegment   = nullptr;
                                 
  GRAPHICS_INT64   extentLeft    = ((GRAPHICS_INT)0),
                   extentRight   = ((GRAPHICS_INT)0);
                   
  GRAPHICS_FLOAT   midLine       = ((GRAPHICS_FLOAT)0),
                   pointDistance = ((GRAPHICS_FLOAT)0),
                   pointX        = ((GRAPHICS_FLOAT)0);
                   
  graphicsError_t  objectError   = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if (selectedCharacter != nullptr)
    {
    if (selectedCharacter->numberOfLineSegments > ((GRAPHICS_UINT)1))
      {
      selectedCharacter->lineSegmentIndex = selectedCharacter->numberOfLineSegments - ((GRAPHICS_UINT)1);

      extentLeft  = selectedCharacter->characterExtents.topLeft.pointX;
      extentRight = selectedCharacter->characterExtents.bottomRight.pointX;

      if (extentRight != extentLeft) // cannot flip a linear 'shape'!
        {
        if (extentRight > (extentLeft + ((GRAPHICS_INT)1))) // points are seperated by a mid-line
          {
          // A definite mid-line...for an even extent in 'x' the mid-line is an integer but 
          // for an odd extent it will be 'nnn.05'
          midLine     = ((GRAPHICS_FLOAT)extentLeft) + ((((GRAPHICS_FLOAT)extentRight) - ((GRAPHICS_FLOAT)extentLeft)) / ((GRAPHICS_FLOAT)2.0));

          lineSegment = selectedCharacter->characterLineSegments;
       
          while (lineSegment->nextLineSegment != nullptr)
            {
            pointX = ((GRAPHICS_FLOAT)lineSegment->lineSegmentOriginX);

            if (pointX < midLine)
              {
              pointDistance = midLine - pointX;

              lineSegment->lineSegmentOriginX = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointX > midLine)
                {
                pointDistance = pointX - midLine;

                lineSegment->lineSegmentOriginX = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentOriginX = (GRAPHICS_INT)midLine;
                }
              }

            pointX = ((GRAPHICS_FLOAT)lineSegment->lineSegmentDestinationX);

            if (pointX < midLine)
              {
              pointDistance = midLine - pointX;

              lineSegment->lineSegmentDestinationX = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointX > midLine)
                {
                pointDistance = pointX - midLine;

                lineSegment->lineSegmentDestinationX = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentDestinationX = (GRAPHICS_INT)midLine;
                }
              }

            lineSegment = lineSegment->nextLineSegment;
            }
          }
        else
          {
          if (extentRight > extentLeft) // all points are seperated by 0 or +1
            {
            lineSegment = selectedCharacter->characterLineSegments;

            while (lineSegment->nextLineSegment != nullptr)
              {
              if (lineSegment->lineSegmentOriginX == extentRight)
                {
                lineSegment->lineSegmentOriginX = extentLeft;
                }
              else
                {
                lineSegment->lineSegmentOriginX = extentRight;
                }

              if (lineSegment->lineSegmentDestinationX == extentRight)
                {
                lineSegment->lineSegmentDestinationX = extentLeft;
                }
              else
                {
                lineSegment->lineSegmentDestinationX = extentRight;
                }

              lineSegment = lineSegment->nextLineSegment;
              }
            }
          else
            {
            // What ?
            objectError = GRAPHICS_OBJECT_RANGE_ERROR;
            }
          }
        }
      }
    else
      {
      if (selectedCharacter->numberOfLineSegments == ((GRAPHICS_UINT)1))
        {
        // The image is a single line segment and the segment end-points are 
        // equal to the extents. Just mirror top-left and bottom-right
        extentLeft  = selectedCharacter->characterExtents.topLeft.pointX;
        extentRight = selectedCharacter->characterExtents.bottomRight.pointX;

        if (extentRight != extentLeft) // cannot flip a linear 'shape'!
          {
          if (extentRight > (extentLeft + ((GRAPHICS_INT)1))) // points are seperated by a mid-line
            {
            // A definite mid-line...for an even extent in 'x' the mid-line is an integer but 
            // for an odd extent it will be 'nnn.05'
            midLine     = ((GRAPHICS_FLOAT)extentLeft) + ((((GRAPHICS_FLOAT)extentRight) - ((GRAPHICS_FLOAT)extentLeft)) / ((GRAPHICS_FLOAT)2.0));

            lineSegment = selectedCharacter->characterLineSegments;

            pointX = ((GRAPHICS_FLOAT)lineSegment->lineSegmentOriginX);

            if (pointX < midLine)
              {
              pointDistance = midLine - pointX;

              lineSegment->lineSegmentOriginX = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointX > midLine)
                {
                pointDistance = pointX - midLine;

                lineSegment->lineSegmentOriginX = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentOriginX = (GRAPHICS_INT)midLine;
                }
              }

            pointX = ((GRAPHICS_FLOAT)lineSegment->lineSegmentDestinationX);

            if (pointX < midLine)
              {
              pointDistance = midLine - pointX;

              lineSegment->lineSegmentDestinationX = ((GRAPHICS_INT)(midLine + pointDistance));
              }
            else
              {
              if (pointX > midLine)
                {
                pointDistance = pointX - midLine;

                lineSegment->lineSegmentDestinationX = ((GRAPHICS_INT)(midLine - pointDistance));
                }
              else
                {
                lineSegment->lineSegmentDestinationX = (GRAPHICS_INT)midLine;
                }
              }
            }
          else
            {
            if (extentRight > extentLeft) // all points are seperated by 0 or +1
              {
              lineSegment = selectedCharacter->characterLineSegments;
        
              if (lineSegment->lineSegmentOriginX == extentRight)
                {
                lineSegment->lineSegmentOriginX = extentLeft;
                }
              else
                {
                lineSegment->lineSegmentOriginX = extentRight;
                }
        
              if (lineSegment->lineSegmentDestinationX == extentRight)
                {
                lineSegment->lineSegmentDestinationX = extentLeft;
                }
              else
                {
                lineSegment->lineSegmentDestinationX = extentRight;
                }
              }
            }
          }
        else
          {
          objectError = GRAPHICS_OBJECT_INSTANTIATION_ERROR;
          }  
        }
      }
    }

/******************************************************************************/

  return(objectError);

/******************************************************************************/
  } /* end of flipCharacterVertically                                         */

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
