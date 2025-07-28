/******************************************************************************/
/* GraphicsObjectDetection.c :                                                */
/* 22.05.25                                                                   */
/* POB                                                                        */
/******************************************************************************/

#include <stdbool.h>
#include "GraphicsAgain001.h"

/******************************************************************************/

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

/******************************************************************************/
/* detectMouseOverObject() :                                                  */
/* - travcerse the hokding ring objects and detect when the mouse position    */
/*   intersects an object boundary. Scale the mouse position by the canvas x- */
/*   and y-axis height and width in normalised units                          */
/* - NOTE : 'guiObjectHoldingRing' is intended to be the base element in the  */
/*          holding ring but can be ANY element node                          */
/******************************************************************************/

graphicsError_t detectMouseOverObject(const guiObjectHoldingRing_tPtr  guiObjectHoldingRing,
                                      const GRAPHICS_SHORT             xPosition,
                                      const GRAPHICS_SHORT             yPosition,
                                      const canvasDescriptor_t        *canvasSize,
                                            bool                      *mouseOverObject)
  {
/******************************************************************************/

  guiObjectHoldingRing_tPtr ringPointer     = (guiObjectHoldingRing_tPtr)guiObjectHoldingRing;

  GRAPHICS_SHORT            boundaryLeftX   = ((GRAPHICS_SHORT)0);
  GRAPHICS_SHORT            boundaryTopY    = ((GRAPHICS_SHORT)0);
  GRAPHICS_SHORT            boundaryRightX  = ((GRAPHICS_SHORT)0);
  GRAPHICS_SHORT            boundaryBottomY = ((GRAPHICS_SHORT)0);

  graphicsError_t           objectError     = GRAPHICS_NO_ERROR;

/******************************************************************************/

  if ((guiObjectHoldingRing != NULL) &&
      (canvasSize           != NULL) && (mouseOverObject != NULL))
    { // test for a known or legal object
    if (testHoldingRingObjectType(ringPointer->linkObjectType) == true)
      {
      *mouseOverObject = false;

      // Is the ring element in use or just a holding node ?
      switch(ringPointer->linkObjectType)
        {
        case GRAPHICS_OBJECT_TYPE_RECTANGLE : // Scale the boundaries - this assumes the canvas will never exceed dX = 0..32767, dY = 0..32767
                                              boundaryLeftX   = (GRAPHICS_SHORT)(ringPointer->activeObject.activeRegion.rectangularBoundary.leftX   * canvasSize->width);
                                              boundaryTopY    = (GRAPHICS_SHORT)(ringPointer->activeObject.activeRegion.rectangularBoundary.topY    * canvasSize->height);
                                              boundaryRightX  = (GRAPHICS_SHORT)(ringPointer->activeObject.activeRegion.rectangularBoundary.rightX  * canvasSize->width);
                                              boundaryBottomY = (GRAPHICS_SHORT)(ringPointer->activeObject.activeRegion.rectangularBoundary.bottomY * canvasSize->height);

                                              // Test to see if the mouse is inside this boundary
                                              if ((xPosition > boundaryLeftX) && (xPosition < boundaryRightX))
                                                {
                                                if ((yPosition > boundaryTopY) && (yPosition < boundaryBottomY))
                                                  {
                                                  *mouseOverObject = true;
                                                  }
                                                }

                                              break;
        
        case GRAPHICS_OBJECT_TYPE_NONE      : break;
                                            
        default                             : objectError = GRAPHICS_OBJECT_TYPE_ERROR;
                                              break;
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
  } /* end of detectMouseOverObject                                           */

/******************************************************************************/
/* testHoldingRingObjectType() :                                              */
/* - all holding ring objects are object-typed by a simple coded value. This  */
/*   is an extra level of checking that a pointer nominally passed as a legal */
/*   object pointer is properly recognised                                    */
/******************************************************************************/

bool testHoldingRingObjectType(graphicsObjectType_t objectType)
  {
/******************************************************************************/

  bool legalObjectError = false;

/******************************************************************************/

  switch(objectType)
    {
    case GRAPHICS_OBJECT_TYPE_NONE        : [[fallthrough]]; // warning C26819 fallthrough is explicit
    case GRAPHICS_OBJECT_TYPE_INDEX       : [[fallthrough]]; // warning C26819 fallthrough is explicit
    case GRAPHICS_OBJECT_TYPE_RECTANGLE   : [[fallthrough]]; // warning C26819 fallthrough is explicit
    case GRAPHICS_OBJECT_TYPE_OCTAGON     : [[fallthrough]]; // warning C26819 fallthrough is explicit
    case GRAPHICS_OBJECT_TYPE_HEXAGON     : [[fallthrough]]; // warning C26819 fallthrough is explicit
    case GRAPHICS_OBJECT_TYPE_RANDOM      : [[fallthrough]]; // warning C26819 fallthrough is explicit
    case GRAPHICS_OBJECT_TYPE_STATIC_TEXT : [[fallthrough]]; // warning C26819 fallthrough is explicit
    case GRAPHICS_OBJECT_TYPE_COMPOSITE   : [[fallthrough]]; // warning C26819 fallthrough is explicit
    case GRAPHICS_OBJECT_TYPE_UNKNOWN     : break;
    default                               : legalObjectError = true;
                                            break;
    }

/******************************************************************************/

  return(legalObjectError);

/******************************************************************************/
  } /* end of testHoldingRingObjectType                                       */

/******************************************************************************/