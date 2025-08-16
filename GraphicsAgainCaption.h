/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/
/*                                                                            */
/* GraphicsAgainCaption.h :                                                   */
/* Paul O'Brien                                                               */
/* 24.07.25                                                                   */
/*                                                                            */
/* Test harness prompts and error reports                                     */
/******************************************************************************/

#ifndef GRAPHICS_AGAIN_CAPTION_H_
#define GRAPHICS_AGAIN_CAPTION_H_

/******************************************************************************/

#include "StrokeGraphicsTypes.h"
#include "GraphicsAgainUtilities.h"
#include "GraphicsAgain001.h"

/******************************************************************************/

#define COMMAND_LINE_ERROR_STATEMENT                         L"Error : usage is \n\n GraphicsAgain.exe \n <Harness IP_Addess> \n <Harness Port> \n <AlphabetFile>"
#define COMMAND_LINE_ERROR_CAPTION                           L"GraphicsAgain : "
#define COMMAND_LINE_ERROR_BEHAVIOUR                          (MB_OK)

#define GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELD_NUMBER          (4)
#define GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELD_BREAKS          (3)
#define GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELDS_MINIMUM_LENGTH (1)
#define GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELDS_MAXIMUM_LENGTH (3)
#define GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELD_BREAK           L"."
#define GRAPHICS_SERVER_IP_ADDRESS_SUB_FIELD_BREAK_CHAR      '.'
#define GRAPHICS_SERVER_IP_ADDRESS_MINIMUM_LENGTH            ((GRAPHICS_UINT)strlen("0.0.0.0"))                      // sample decimal-notation ip-address
#define GRAPHICS_SERVER_IP_ADDRESS_MAXIMUM_LENGTH            ((GRAPHICS_UINT)strlen("000.000.000.000"))
                                                             
#define GRAPHICS_SERVER_PORT_NUMBER_MINIMUM_LENGTH           (1)
#define GRAPHICS_SERVER_PORT_NUMBER_MAXIMUM_LENGTH           (5)
                                                             
#define GRAPHICS_ALPHABET_FILE_LENGTH                        (MAX_LOADSTRING)

#define GRAPHICS_STROKE_TEXT_HEADLINE                         L"RFM 9x Test Harness" // "ABCDEFGHIJKLMNOPQRSTUVWXYZ" // "!\"""#$%^&*()-=_+[]{}~#\\;:'@`,.<>/?" // "abcdefghijklmnopqrstuvwxyz" //

/******************************************************************************/

extern alphabetFileDescriptor_t alphabetFileDescriptor;

/******************************************************************************/

#endif

/******************************************************************************/
/* (c) PulsingCore Software 2025                                              */
/******************************************************************************/