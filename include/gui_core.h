

#ifndef GUI_CORE_H
#define GUI_CORE_H

#include <stdint.h>
#include <stdbool.h>

#ifndef DISPLAY_COLUMNS
#define DISPLAY_COLUMNS 128
#endif
#ifndef DISPLAY_PAGES
#define DISPLAY_PAGES   8
#endif

#define DISPLAY_BUFFER_SIZE (DISPLAY_COLUMNS*DISPLAY_PAGES)

#define VERTICAL_DISPLAY_COLUMNS  (DISPLAY_PAGES * 8)
#define VERTICAL_DISPLAY_PAGES  (DISPLAY_COLUMNS / 8)

extern const uint8_t font_ASCII[][5];
extern const uint8_t selectionMarkerImage[];

typedef enum menuObjectValueType_t
{
  INT,
  FLOAT,
  BOOLEAN,
  UINT_LIST
}menuObjectValueType_t;

typedef struct guiObject_t
{
  char *objectLabel;

  menuObjectValueType_t objectValueType;
  int32_t   objectDisplayedIntValue;
  float     objectDisplayedfloatValue;
  bool      objectDisplayedBoolValue;
  uint8_t   listIndex;

  void *objectSourceValuePointer;

  void (*valueUpFunction)(void);
  void (*valueDnFunction)(void);
  void (*valueSaveFunction)(void);
  void (*objectEnterFunction)(void);

  uint8_t objectPagePosition;
  uint8_t objectColumnPosition;

}guiObject_t;

void setGuiDisplayBuffer(uint8_t *displayDataBuffer);
void setGuiDisplayBufferVertical(uint8_t *displayDataBuffer);

typedef enum VbuffRotation_t
{
  ROTATION_NONE,  // 0
  ROTATION_LEFT,  // +90
  ROTATION_RIGHT  // -90
}VbuffRotation_t;

extern VbuffRotation_t guiRotation;

void rotateVerticalBuffer(VbuffRotation_t rotation);

void setRenderFunction(void(*renderFunctionPointer)(void));
guiObject_t createNewGuiObject(void);
void clearDisplayBuffer(void);
void clearDisplayVerticalBuffer(void);

void renderGui(void);
void renderObjectLabel(guiObject_t *object, uint8_t page, uint8_t column);
void renderObjectLabelVertical(guiObject_t *object, uint8_t page, uint8_t column);
void renderObjectValue(guiObject_t *object, uint8_t page, uint8_t column);
void renderObjectValueVertical(guiObject_t *object, uint8_t page, uint8_t column);
void reloadObjectDisplayedValue(guiObject_t *object);
void renderInt(int variable, uint8_t page, uint8_t column);
void renderIntVertical(int variable, uint8_t page, uint8_t column);
void renderFloat(float variable, uint8_t page, uint8_t column);
void renderFloatVertical(float variable, uint8_t page, uint8_t column);
void renderBoolean(bool variable, uint8_t page, uint8_t column);
void renderBooleanVertical(bool variable, uint8_t page, uint8_t column);

void renderText(char *inputText, uint8_t page, uint8_t column);
void renderTextVertical(char *inputText, uint8_t page, uint8_t column);
void renderTextOnCenter(char *inputText, uint8_t page);
void renderTextOnCenterVertical(char *inputText, uint8_t page);

void renderSelectionMarker(uint8_t page, uint8_t column);
void renderSelectionMarkerVertical(uint8_t page, uint8_t column);
uint16_t getDisplayBufferPosition(uint8_t page, uint8_t column);
uint16_t getVerticalDisplayBufferPosition(uint8_t page, uint8_t column);
uint16_t create_text_buff( unsigned char* text_buffer, char* buff);
void blankFunction(void);

#endif