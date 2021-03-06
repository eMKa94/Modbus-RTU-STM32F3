#include "gui_core.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint8_t *guiDisplayBuffer;
uint8_t *guiDisplayBufferVertical;
void (*currentRendererFunction)(void) = &blankFunction;

VbuffRotation_t guiRotation = ROTATION_NONE;

void setGuiDisplayBuffer(uint8_t *displayDataBuffer)
{
    guiDisplayBuffer = displayDataBuffer;
}

void setGuiDisplayBufferVertical(uint8_t *displayDataBuffer)
{
    guiDisplayBufferVertical = displayDataBuffer;
}

void rotateBufferLeft(void)
{
    for (uint8_t pageNumber = 0 ; pageNumber < VERTICAL_DISPLAY_PAGES ; pageNumber++)
    {
        for (uint8_t columnNumber = 0 ; columnNumber < VERTICAL_DISPLAY_COLUMNS ; columnNumber ++)
        {
            uint8_t horPageNumber = (columnNumber / 8);
            uint8_t horColumnOffset = (pageNumber * 8);
            uint8_t currentVerticalBufferByte = guiDisplayBufferVertical[getVerticalDisplayBufferPosition(pageNumber, columnNumber)];
            for (uint8_t bitNumber = 0 ; bitNumber < 8 ; bitNumber++)
            {
                uint16_t horBufferPosition = getDisplayBufferPosition(7 - horPageNumber, horColumnOffset + bitNumber);
                if(currentVerticalBufferByte & (1 << bitNumber ))
                {
                    guiDisplayBuffer[horBufferPosition] |= (1 << (7 - (columnNumber%8)));
                }
                else
                {
                    guiDisplayBuffer[horBufferPosition] &= ~(1 << (7 - (columnNumber%8)));
                }
            }
        }
    }
}

void rotateBufferRight(void)
{
    for (uint8_t pageNumber = 0 ; pageNumber < VERTICAL_DISPLAY_PAGES ; pageNumber++)
    {
        for (uint8_t columnNumber = 0 ; columnNumber < VERTICAL_DISPLAY_COLUMNS ; columnNumber ++)
        {
            uint8_t horPageNumber = 7 - (columnNumber / 8);
            uint8_t horColumnOffset = 119 - (pageNumber * 8);
            uint8_t currentVerticalBufferByte = guiDisplayBufferVertical[getVerticalDisplayBufferPosition(pageNumber, columnNumber)];
            for (uint8_t bitNumber = 0 ; bitNumber < 8 ; bitNumber++)
            {
                uint16_t horBufferPosition = getDisplayBufferPosition(7 - horPageNumber, horColumnOffset + bitNumber);
                if(currentVerticalBufferByte & (1 << (7 - bitNumber) ))
                {
                    guiDisplayBuffer[horBufferPosition] |= (1 << ((columnNumber%8)));
                }
                else
                {
                    guiDisplayBuffer[horBufferPosition] &= ~(1 << ((columnNumber%8)));
                }
            }
        }
    }
}

void rotateVerticalBuffer(VbuffRotation_t rotation)
{
    switch(rotation)
    {
        case ROTATION_LEFT:
            rotateBufferLeft();
            break;

        case ROTATION_RIGHT:
            rotateBufferRight();
            break;

        default:
            break;
    }
}

void setRenderFunction(void(*renderFunctionPointer)(void))
{
    currentRendererFunction = renderFunctionPointer;
}

void clearDisplayBuffer(void)
{
    memset(guiDisplayBuffer, 0, DISPLAY_BUFFER_SIZE);
}

 void clearDisplayVerticalBuffer(void)
{
    memset(guiDisplayBufferVertical, 0, DISPLAY_BUFFER_SIZE);
}

void renderInt(int variable, uint8_t page, uint8_t column)
{

    char *textBuffer = malloc(10 * sizeof(uint8_t));
    sprintf(textBuffer, "%d", variable);
    renderText(textBuffer, page, column);
    free(textBuffer);
    
}

void renderIntVertical(int variable, uint8_t page, uint8_t column)
{

    char *textBuffer = malloc(10 * sizeof(uint8_t));
    sprintf(textBuffer, "%d", variable);
    renderTextVertical(textBuffer, page, column);
    free(textBuffer);
    
}

void renderFloat(float variable, uint8_t page, uint8_t column)
{

    char *textBuffer = malloc(5 * sizeof(float));
    sprintf(textBuffer, "%.2f", variable);
    renderText(textBuffer, page, column);
    free(textBuffer);

}

void renderFloatVertical(float variable, uint8_t page, uint8_t column)
{

    char *textBuffer = malloc(5 * sizeof(float));
    sprintf(textBuffer, "%.2f", variable);
    renderTextVertical(textBuffer, page, column);
    free(textBuffer);

}

void renderBoolean(bool variable, uint8_t page, uint8_t column)
{
    renderText(variable ? "Tak" : "Nie", page, column);
}

void renderBooleanVertical(bool variable, uint8_t page, uint8_t column)
{
    renderTextVertical(variable ? "Tak" : "Nie", page, column);
}

void renderText(char *inputText, uint8_t page, uint8_t column)
{
    uint8_t *textRenderBuffer = malloc(100 * sizeof(uint8_t));
    memcpy(guiDisplayBuffer+getDisplayBufferPosition(page, column), textRenderBuffer, create_text_buff(textRenderBuffer, inputText));
    free(textRenderBuffer);
}

void renderTextVertical(char *inputText, uint8_t page, uint8_t column)
{

    uint8_t *textRenderBuffer = malloc(100 * sizeof(uint8_t));
    memcpy(guiDisplayBufferVertical+getVerticalDisplayBufferPosition(page, column), textRenderBuffer, create_text_buff(textRenderBuffer, inputText));
    free(textRenderBuffer);
}

void renderTextOnCenter(char *inputText, uint8_t page)
{
    uint8_t *textRenderBuffer = malloc(100 * sizeof(uint8_t));
    uint16_t renderedTextSize = create_text_buff(textRenderBuffer, inputText);
    memcpy(guiDisplayBuffer+getDisplayBufferPosition(page, (DISPLAY_COLUMNS - renderedTextSize)/2), textRenderBuffer, renderedTextSize);
    free(textRenderBuffer);
}

void renderTextOnCenterVertical(char *inputText, uint8_t page)
{
    
    uint8_t *textRenderBuffer = malloc(100 * sizeof(uint8_t));
    uint16_t renderedTextSize = create_text_buff(textRenderBuffer, inputText);
    memcpy(guiDisplayBufferVertical+getVerticalDisplayBufferPosition(page, (VERTICAL_DISPLAY_COLUMNS - renderedTextSize)/2), textRenderBuffer, renderedTextSize);
    free(textRenderBuffer);

}

void renderSelectionMarker(uint8_t page, uint8_t column)
{
    memcpy(guiDisplayBuffer+getDisplayBufferPosition(page, column), selectionMarkerImage, 5);
}

void renderSelectionMarkerVertical(uint8_t page, uint8_t column)
{
    memcpy(guiDisplayBufferVertical+getVerticalDisplayBufferPosition(page, column), selectionMarkerImage, 5);
}

uint16_t getDisplayBufferPosition(uint8_t page, uint8_t column)
{
    if (page > (DISPLAY_PAGES - 1))     page = (DISPLAY_PAGES - 1);
    if (column > (DISPLAY_COLUMNS-1))   column = (DISPLAY_COLUMNS - 1);
    return((page * DISPLAY_COLUMNS) + column);
}

uint16_t getVerticalDisplayBufferPosition(uint8_t page, uint8_t column)
{
    if (page > (VERTICAL_DISPLAY_PAGES - 1))     page = (VERTICAL_DISPLAY_PAGES - 1);
    if (column > (VERTICAL_DISPLAY_COLUMNS-1))   column = (VERTICAL_DISPLAY_COLUMNS - 1);
    return((page * VERTICAL_DISPLAY_COLUMNS) + column);
}

uint16_t create_text_buff( unsigned char* target_buffer, char* input_text)
{
	uint16_t byte_counter = 0;

	while(*input_text != 0x00)
	{
		int ch = *input_text ++;

		const uint8_t* font = &font_ASCII[ch - ' '][0];

		for (int i = 0 ; i < 5 ; i ++)
		{
			*target_buffer++ = *font++ ;
			byte_counter ++;
		}
		*target_buffer++ = 0x00 ;
		byte_counter ++;
	}
	return(byte_counter);
}

void renderGui(void)
{
    /* if (guiRotation == ROTATION_NONE) clearDisplayBuffer();
    else clearDisplayVerticalBuffer(); */
    clearDisplayBuffer();
    clearDisplayVerticalBuffer();
    currentRendererFunction();
}

void renderObjectLabel(guiObject_t *object, uint8_t page, uint8_t column)
{
    object->objectPagePosition = page;
    object->objectColumnPosition = column;
    renderText(object->objectLabel, page, column);
     
}

void renderObjectLabelVertical(guiObject_t *object, uint8_t page, uint8_t column)
{
    object->objectPagePosition = page;
    object->objectColumnPosition = column;
    renderTextVertical(object->objectLabel, page, column);
     
}

void renderObjectValue(guiObject_t *object, uint8_t page, uint8_t column)
{
    switch(object->objectValueType)
    {
        case INT:
            renderInt(object->objectDisplayedIntValue, page, column);
            break;
            
        case FLOAT:
            renderFloat(object->objectDisplayedfloatValue, page, column);
            break;
            
        case BOOLEAN:
            renderBoolean(object->objectDisplayedBoolValue, page, column);
            break;

        case UINT_LIST:
            
            break;

        default:
            break;
    }
}

void renderObjectValueVertical(guiObject_t *object, uint8_t page, uint8_t column)
{
    switch(object->objectValueType)
    {
        case INT:
            renderIntVertical(object->objectDisplayedIntValue, page, column);
            break;
            
        case FLOAT:
            renderFloatVertical(object->objectDisplayedfloatValue, page, column);
            break;
            
        case BOOLEAN:
            renderBooleanVertical(object->objectDisplayedBoolValue, page, column);
            break;

        case UINT_LIST:
            
            break;

        default:
            break;
    }
}

void reloadObjectDisplayedValue(guiObject_t *object)
{
    switch(object->objectValueType)
    {
        case INT:
            object->objectDisplayedIntValue = *(int16_t *)object->objectSourceValuePointer;
            break;
            
        case FLOAT:
            object->objectDisplayedfloatValue = *(float *)object->objectSourceValuePointer;
            break;

        case BOOLEAN:
            break;

        default:
            break;
    }
}

void blankFunction(void)
{
    
}



const uint8_t font_ASCII[][5] = {
  {0x00, 0x00, 0x00, 0x00, 0x00} // 20
  ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c
  ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
  ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
};

const uint8_t selectionMarkerImage[]=
{
    0x00, 0x7F, 0x3E, 0x1C, 0x08
};

