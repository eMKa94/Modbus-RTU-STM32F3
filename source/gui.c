/* 
    Zenex sp. z o. o.
    Maciej Kowalski

    mkowalski@zenex.pl
    maciej.qb@gmail.com

 */

#include "gui.h"
#include <string.h>

uint8_t IpaGuiBuffer[BOARD_LCD_BUFFER_SIZE];
uint8_t IpaGuiBufferVertical[BOARD_LCD_BUFFER_SIZE];

uint8_t navigationIndexTop = 0;

guiObject_t *(*guiCurrentObjectsList);

bool settingsObjectEdit = false;

uint32_t sampleList[5]=
{
    100,
    200,
    1000,
    2000,
    3000
};

extern float volatageRmsL1;
extern float volatageRmsL2;
extern float volatageRmsL3;

void (*EscButtonClickFunction)(void)    = &blankFunction;   
void (*DnButtonClickFunction)(void)     = &blankFunction;
void (*UpButtonClickFunction)(void)     = &blankFunction;
void (*OkButtonClickFunction)(void)     = &blankFunction;

void guiInit(void)
{

    setGuiDisplayBuffer(IpaGuiBuffer); // Set what will be modified by GUI
    setGuiDisplayBufferVertical(IpaGuiBufferVertical);
    board_lcd_set_display_data_address(IpaGuiBuffer);   // Set what will be transmitted to LCD
    assignButtonPressFunction(&ESC_BUTTON, &guiEscButtonClicked);
    assignButtonPressFunction(&DN_BUTTON, &guiDnButtonClicked);
    assignButtonPressFunction(&UP_BUTTON, &guiUpButtonClicked);
    assignButtonPressFunction(&OK_BUTTON, &guiOkButtonClicked);
    enterLnVoltageMenu();
    //enterTestMenu();
    //enterModbusSettingsMenu();

}

void enterTestMenu(void)
{
    setRenderFunction(&renderTestMenu);

    setEscButtonClickFunction(&blankFunction);
    setDnButtonClickFunction(&blankFunction);
    setUpButtonClickFunction(&blankFunction);
    setOkButtonClickFunction(&blankFunction);   
}
void renderTestMenu(void)
{
    // renderTextOnCenter("TEST MENU", 1);
    renderTextVertical("ABCDEFGHIJ", 0, 0);
    renderTextVertical("ABCDEFGHIJ", 1, 0);
    renderTextVertical("ABCDEFGHIJ", 2, 0);
    renderTextVertical("ABCDEFGHIJ", 3, 0);
    renderTextVertical("ABCDEFGHIJ", 4, 0);
    renderTextVertical("ABCDEFGHIJ", 5, 0);
    renderTextVertical("ABCDEFGHIJ", 6, 0);
    renderTextVertical("ABCDEFGHIJ", 7, 0);
    renderTextVertical("ABCDEFGHIJ", 8, 0);
    renderTextVertical("ABCDEFGHIJ", 9, 0);
    renderTextVertical("ABCDEFGHIJ", 10, 0);
    renderTextVertical("ABCDEFGHIJ", 11, 0);
    renderTextVertical("ABCDEFGHIJ", 12, 0);
    renderTextVertical("ABCDEFGHIJ", 13, 0);
    renderTextVertical("ABCDEFGHIJ", 14, 0);
    renderTextVertical("ABCDEFGHIJ", 15, 0);
    rotateVerticalBuffer(guiRotation);
}

// Phase Voltage Menu Window
guiObject_t lnVoltageL1Object=
{
    .objectLabel = "L1 :",
    .objectSourceValuePointer = (void *)&volatageRmsL1,
    .objectValueType = FLOAT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t lnVoltageL2Object=
{
    
    .objectLabel = "L2 :",
    .objectSourceValuePointer = (void *)&volatageRmsL2,
    .objectValueType = FLOAT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction

};

guiObject_t lnVoltageL3Object=
{
    .objectLabel = "L3 :",
    .objectSourceValuePointer = (void *)&volatageRmsL3,
    .objectValueType = FLOAT,
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

void enterLnVoltageMenu(void)
{
    if (guiRotation != ROTATION_NONE)    setRenderFunction(&renderLnVoltageMenuVertical);
    else setRenderFunction(&renderLnVoltageMenu);

    setEscButtonClickFunction(&blankFunction);
    setDnButtonClickFunction(&blankFunction);
    setUpButtonClickFunction(&blankFunction);
    setOkButtonClickFunction(&enterSettingsMenu);

    reloadObjectDisplayedValue(&lnVoltageL1Object);
    reloadObjectDisplayedValue(&lnVoltageL2Object);
    reloadObjectDisplayedValue(&lnVoltageL3Object);

}

void renderLnVoltageMenu(void)
{
    renderTextOnCenter("U L-N [V]", 0);

    renderObjectLabel(&lnVoltageL1Object, 2, 30);
    renderObjectValue(&lnVoltageL1Object, 2, 60);

    renderObjectLabel(&lnVoltageL2Object, 4, 30);
    renderObjectValue(&lnVoltageL2Object, 4, 60);
    
    renderObjectLabel(&lnVoltageL3Object, 6, 30);
    renderObjectValue(&lnVoltageL3Object, 6, 60);
    
}

void renderLnVoltageMenuVertical(void)
{
    renderTextOnCenterVertical("U L-N [V]", 0);

    renderObjectLabelVertical(&lnVoltageL1Object, 2, 0);
    renderObjectValueVertical(&lnVoltageL1Object, 4, 15);

    renderObjectLabelVertical(&lnVoltageL2Object, 6, 0);
    renderObjectValueVertical(&lnVoltageL2Object, 8, 15);
    
    renderObjectLabelVertical(&lnVoltageL3Object, 10, 0);
    renderObjectValueVertical(&lnVoltageL3Object, 12, 15);
    rotateVerticalBuffer(guiRotation);
}

// Settings Navigation Menu Window

guiObject_t modbusSettingsObject =
{
    .objectLabel = "MB RTU",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &enterModbusSettingsMenu
};
guiObject_t displaySettingsObject =
{
    .objectLabel = "DISP",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &enterDisplaySettingsMenu
};
guiObject_t InputOutputSettingsObject =
{
    .objectLabel = "IN / OUT",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};
guiObject_t calibrationSettingsObject =
{
    .objectLabel = "CALIB.",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction
};

guiObject_t deviceInfoSettingsObject =
{
    .objectLabel = "INFO",
    .valueDnFunction = &blankFunction,
    .valueUpFunction = &blankFunction,
    .valueSaveFunction = &blankFunction,
    .objectEnterFunction = &blankFunction    
};

guiObject_t *settingMenuObjects[5]=
{

    &modbusSettingsObject,
    &displaySettingsObject,
    &InputOutputSettingsObject,
    &calibrationSettingsObject,
    &deviceInfoSettingsObject
};

uint8_t settingsNavigationIndex = 0;
void settingsNavigationMarkerUp(void)
{
    if (settingsNavigationIndex > 0) settingsNavigationIndex--;
}

void settingsNavigationMarkerDown(void)
{
    if (settingsNavigationIndex < navigationIndexTop) settingsNavigationIndex++;
}

void enterSettingsMenu(void)
{
    if (guiRotation == ROTATION_NONE) setRenderFunction(&renderSettingsMenu);
    else setRenderFunction(&renderSettingsMenuVertical);

    setEscButtonClickFunction(&enterLnVoltageMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);
    settingsNavigationIndex = 0;
    navigationIndexTop = 4;
    guiCurrentObjectsList = settingMenuObjects;
}

void renderSettingsMenu(void)
{
    
    renderTextOnCenter("SETTINGS", 0);

    renderObjectLabel(settingMenuObjects[0], 2, 10);
    renderObjectLabel(settingMenuObjects[1], 3, 10);
    renderObjectLabel(settingMenuObjects[2], 4, 10);
    renderObjectLabel(settingMenuObjects[3], 5, 10);
    renderObjectLabel(settingMenuObjects[4], 6, 10);

    uint8_t markerPage      = settingMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = settingMenuObjects[settingsNavigationIndex]->objectColumnPosition - 10;
    renderSelectionMarker(markerPage, markerColumn);

}

void renderSettingsMenuVertical(void)
{
    
    renderTextOnCenterVertical("SETTINGS", 0);

    renderObjectLabelVertical(settingMenuObjects[0], 2, 10);
    renderObjectLabelVertical(settingMenuObjects[1], 3, 10);
    renderObjectLabelVertical(settingMenuObjects[2], 4, 10);
    renderObjectLabelVertical(settingMenuObjects[3], 5, 10);
    renderObjectLabelVertical(settingMenuObjects[4], 6, 10);

    uint8_t markerPage      = settingMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = settingMenuObjects[settingsNavigationIndex]->objectColumnPosition - 10;
    renderSelectionMarkerVertical(markerPage, markerColumn);
    rotateVerticalBuffer(guiRotation);

}


// Modbus Settings Menu
void reloadModbusSettings(void)
{
    modbusIdObject.objectDisplayedIntValue      = (int32_t)modbusId;
    rs485BaudrateObject.objectDisplayedIntValue = (int32_t)rs485Baudrate;
    rs485ParityObject.objectDisplayedIntValue   = (int32_t)rs485Parity;
    rs485StopBitsObject.objectDisplayedIntValue = (int32_t)rs485StopBits;    
}

void cancelModbusObjectEdit(void)
{
    settingsObjectEdit = false;
    setEscButtonClickFunction(&enterSettingsMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);

    reloadModbusSettings();
}

void editModbusObject(void)
{
    settingsObjectEdit = true;

    setEscButtonClickFunction(&cancelModbusObjectEdit);
    setDnButtonClickFunction(&selectedObjectValueDown);
    setUpButtonClickFunction(&selectedObjectValueUp);
    setOkButtonClickFunction(&selectedObjectValueSave);
}

extern void MB_init(void);
extern void saveModbusSettings(void);
void mbIdIncrement(void)
{
    if (modbusIdObject.objectDisplayedIntValue < 100) modbusIdObject.objectDisplayedIntValue ++;
}
void mbIdDecrement(void)
{
    if (modbusIdObject.objectDisplayedIntValue > 1) modbusIdObject.objectDisplayedIntValue --;
}
void mbIdSave(void)
{
    modbusId = (uint8_t)modbusIdObject.objectDisplayedIntValue;
    MB_init();
    saveModbusSettings();
    cancelModbusObjectEdit();
}

void mbBaudrateIncrement(void)
{
    if (rs485BaudrateObject.objectDisplayedIntValue < 5) rs485BaudrateObject.objectDisplayedIntValue ++; 
}
void mbBaudrateDecrement(void)
{
    if (rs485BaudrateObject.objectDisplayedIntValue > 0) rs485BaudrateObject.objectDisplayedIntValue --; 
}
void mbBaudratesave(void)
{
    rs485Baudrate = (rs485Baudrate_t)rs485BaudrateObject.objectDisplayedIntValue;
    MB_init();
    saveModbusSettings();
    cancelModbusObjectEdit();
}

void rs485ParityIncrement(void)
{
    if (rs485ParityObject.objectDisplayedIntValue < 2) rs485ParityObject.objectDisplayedIntValue ++;
}
void rs485ParityDecrement(void)
{
    if (rs485ParityObject.objectDisplayedIntValue > 0) rs485ParityObject.objectDisplayedIntValue --;
}
void rs485Paritysave(void)
{
    rs485Parity = (rs485Parity_t)rs485ParityObject.objectDisplayedIntValue;
    MB_init();
    saveModbusSettings();
    cancelModbusObjectEdit();
}

void rs485StopBitsIncrement(void)
{
    if (rs485StopBitsObject.objectDisplayedIntValue < 1) rs485StopBitsObject.objectDisplayedIntValue ++;
}
void rs485StopBitsDecrement(void)
{
    if (rs485StopBitsObject.objectDisplayedIntValue  > 0) rs485StopBitsObject.objectDisplayedIntValue --;
}
void rs485StopBitssave(void)
{
    rs485StopBits = (rs485StopBits_t)rs485StopBitsObject.objectDisplayedIntValue;
    MB_init();
    saveModbusSettings();
    cancelModbusObjectEdit();
}

guiObject_t modbusIdObject =
{
    .objectLabel = "ID :",
    .objectDisplayedIntValue = 1,
    .objectValueType = INT,
    .valueDnFunction = &mbIdDecrement,
    .valueUpFunction = &mbIdIncrement,
    .valueSaveFunction = &mbIdSave,
    .objectEnterFunction = &editModbusObject
};

guiObject_t rs485BaudrateObject =
{
    .objectLabel = "BD :",
    .objectDisplayedIntValue = 0,
    .objectValueType = INT,
    .valueDnFunction = &mbBaudrateDecrement,
    .valueUpFunction = &mbBaudrateIncrement,
    .valueSaveFunction = &mbBaudratesave,
    .objectEnterFunction = &editModbusObject
};

guiObject_t rs485ParityObject =
{
    .objectLabel = "PR :",
    .objectDisplayedIntValue = 0,
    .objectValueType = INT,
    .valueDnFunction = &rs485ParityDecrement,
    .valueUpFunction = &rs485ParityIncrement,
    .valueSaveFunction = &rs485Paritysave,
    .objectEnterFunction = &editModbusObject
};

guiObject_t rs485StopBitsObject =
{
    .objectLabel = "SB :",
    .objectDisplayedIntValue = 1,
    .objectValueType = INT,
    .valueDnFunction = &rs485StopBitsDecrement,
    .valueUpFunction = &rs485StopBitsIncrement,
    .valueSaveFunction = &rs485StopBitssave,
    .objectEnterFunction = &editModbusObject
};

guiObject_t *modbusMenuObjects[4]=
{
    &modbusIdObject,
    &rs485BaudrateObject,
    &rs485ParityObject,
    &rs485StopBitsObject    
};


void enterModbusSettingsMenu(void)
{
    if (guiRotation == ROTATION_NONE) setRenderFunction(&renderModbusSettingsMenu);
    else setRenderFunction(&renderModbusSettingsMenuVertical);

    setEscButtonClickFunction(&enterSettingsMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);
    settingsNavigationIndex = 0;
    settingsObjectEdit = false;
    navigationIndexTop = 3;

    reloadModbusSettings();

    guiCurrentObjectsList = modbusMenuObjects;
}


void renderModbusSettingsMenu(void)
{
    renderTextOnCenter("MB RTU", 0);

    renderObjectLabel(&modbusIdObject, 2, 10);
    renderObjectValue(&modbusIdObject, 2, 80);

    renderObjectLabel(&rs485BaudrateObject, 3, 10);
    // renderObjectValue(modbusMenuObjects[1], 3, 80);
    switch (rs485BaudrateObject.objectDisplayedIntValue)
    {
        case baudrate_4800:
            renderInt(4800, 3, 80);
            break;
        
        case baudrate_9600:
            renderInt(9600, 3, 80);
            break;
        
        case baudrate_19200:
            renderInt(19200, 3, 80);
            break;
        
        case baudrate_38400:
            renderInt(38400, 3, 80);
            break;
        
        case baudrate_57600:
            renderInt(57600, 3, 80);
            break;
        
        case baudrate_115200:
            renderInt(115200, 3, 80);
            break;

        default:
            break;

    }

    renderObjectLabel(&rs485ParityObject, 4, 10);
    switch(rs485ParityObject.objectDisplayedIntValue)
    {
        case parity_none:
            renderText("---", 4, 80);
            break;
        case parity_even:
            renderText("par", 4, 80);
            break;
        case parity_odd:
            renderText("n-par", 4, 80);
            break;
        default:
            break;
    }

    renderObjectLabel(&rs485StopBitsObject, 5, 10);
    renderInt(rs485StopBitsObject.objectDisplayedIntValue+1, 5, 80);

    uint8_t markerPage      = modbusMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = modbusMenuObjects[settingsNavigationIndex]->objectColumnPosition;
    renderSelectionMarker(markerPage, markerColumn + (settingsObjectEdit ? 60 : -10));

}

void renderModbusSettingsMenuVertical(void)
{
    renderTextOnCenterVertical("MB RTU", 0);

    renderObjectLabelVertical(&modbusIdObject, 2, 10);
    renderObjectValueVertical(&modbusIdObject, 3, 20);

    renderObjectLabelVertical(&rs485BaudrateObject, 5, 10);
    // renderObjectValue(modbusMenuObjects[1], 3, 80);
    switch (rs485BaudrateObject.objectDisplayedIntValue)
    {
        case baudrate_4800:
            renderIntVertical(4800, 6, 20);
            break;
        
        case baudrate_9600:
            renderIntVertical(9600, 6, 20);
            break;
        
        case baudrate_19200:
            renderIntVertical(19200, 6, 20);
            break;
        
        case baudrate_38400:
            renderIntVertical(38400, 6, 20);
            break;
        
        case baudrate_57600:
            renderIntVertical(57600, 6, 20);
            break;
        
        case baudrate_115200:
            renderIntVertical(115200, 6, 20);
            break;

        default:
            break;

    }

    renderObjectLabelVertical(&rs485ParityObject, 8, 10);
    switch(rs485ParityObject.objectDisplayedIntValue)
    {
        case parity_none:
            renderTextVertical("---", 9, 20);
            break;
        case parity_even:
            renderTextVertical("par", 9, 20);
            break;
        case parity_odd:
            renderTextVertical("n-par", 9, 20);
            break;
        default:
            break;
    }

    renderObjectLabelVertical(&rs485StopBitsObject, 11, 10);
    renderIntVertical(rs485StopBitsObject.objectDisplayedIntValue+1, 12, 20);

    uint8_t markerPage      = modbusMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = modbusMenuObjects[settingsNavigationIndex]->objectColumnPosition;
    renderSelectionMarkerVertical(markerPage + (settingsObjectEdit ? 1 : 0), markerColumn + (settingsObjectEdit ? 0 : -10));

    rotateVerticalBuffer(guiRotation);

}


// ##########   LCD     ##########

void reloadLCDSettings(void)
{
    backlightOnTimeObject.objectDisplayedIntValue = (int)boardLcdBacklightStandardOnTime;
    displayRotationObject.objectDisplayedIntValue = (int)guiRotation;
}

void cancelDisplayObjectEdit(void)
{
    settingsObjectEdit = false;
    setEscButtonClickFunction(&enterSettingsMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);

    reloadLCDSettings();
}

void editDisplayObject(void)
{

    settingsObjectEdit = true;

    setEscButtonClickFunction(&cancelDisplayObjectEdit);
    setDnButtonClickFunction(&selectedObjectValueDown);
    setUpButtonClickFunction(&selectedObjectValueUp);
    setOkButtonClickFunction(&selectedObjectValueSave);   
}

void lcdBacklightOnTimeIncrease(void)
{
    int32_t onTime = backlightOnTimeObject.objectDisplayedIntValue;
    if (onTime < 300) onTime ++;
    backlightOnTimeObject.objectDisplayedIntValue = onTime;
}

void lcdBacklightOnTimeDecrease(void)
{
    int32_t onTime = backlightOnTimeObject.objectDisplayedIntValue;
    if (onTime > 3) onTime --;
    backlightOnTimeObject.objectDisplayedIntValue = onTime;
}

extern void saveLCDSettings(void);
void lcdBackLightTimeSave(void)
{
    boardLcdBacklightStandardOnTime = (uint32_t)backlightOnTimeObject.objectDisplayedIntValue;
    boardLCDBacklightTurnOnStandardPeriod();
    saveLCDSettings();
    cancelDisplayObjectEdit();
}

void displayRotationIncrese(void)
{
    int currentDisplayedRotation = displayRotationObject.objectDisplayedIntValue;
    if (currentDisplayedRotation < 2) currentDisplayedRotation ++;
    displayRotationObject.objectDisplayedIntValue = currentDisplayedRotation;
}

void displayRotationDecrese(void)
{
    int currentDisplayedRotation = displayRotationObject.objectDisplayedIntValue;
    if (currentDisplayedRotation > 0) currentDisplayedRotation --;
    displayRotationObject.objectDisplayedIntValue = currentDisplayedRotation;
}

void displayRotationSave(void)
{
    guiRotation = (VbuffRotation_t)displayRotationObject.objectDisplayedIntValue;
    if (guiRotation != ROTATION_NONE)    setRenderFunction(&renderDisplaySettingsMenuVertical);

    else setRenderFunction(&renderDisplaySettingsMenu);
    saveLCDSettings();

    // If the display is rotated left, UP & DN buttons should be replaced
    if (guiRotation == ROTATION_LEFT)
    {
        assignButtonPressFunction(&UP_BUTTON, &guiDnButtonClicked);
        assignButtonPressFunction(&DN_BUTTON, &guiUpButtonClicked);
    }
    else
    {
        assignButtonPressFunction(&UP_BUTTON, &guiUpButtonClicked);
        assignButtonPressFunction(&DN_BUTTON, &guiDnButtonClicked);       
    }
    cancelDisplayObjectEdit();
}

guiObject_t backlightOnTimeObject =
{

    .objectLabel = "On T.:",
    .objectSourceValuePointer = (void *)&boardLcdBacklightStandardOnTime,
    .objectValueType = INT,
    .valueDnFunction = &lcdBacklightOnTimeDecrease,
    .valueUpFunction = &lcdBacklightOnTimeIncrease,
    .valueSaveFunction = &lcdBackLightTimeSave,
    .objectEnterFunction = &editDisplayObject

};

guiObject_t displayRotationObject =
{
    .objectLabel = "ROT  :",
    .objectSourceValuePointer = (void *)&guiRotation,
    .objectValueType = INT,
    .valueDnFunction = &displayRotationDecrese,
    .valueUpFunction = &displayRotationIncrese,
    .valueSaveFunction = &displayRotationSave,
    .objectEnterFunction = &editDisplayObject
};

guiObject_t *displayMenuObjects[2] =
{
    &backlightOnTimeObject,
    &displayRotationObject
};

void enterDisplaySettingsMenu(void)
{
    if (guiRotation != ROTATION_NONE)    setRenderFunction(&renderDisplaySettingsMenuVertical);
    else setRenderFunction(&renderDisplaySettingsMenu);

    setEscButtonClickFunction(&enterSettingsMenu);
    setUpButtonClickFunction(&settingsNavigationMarkerUp);
    setDnButtonClickFunction(&settingsNavigationMarkerDown);
    setOkButtonClickFunction(&enterObject);
    settingsNavigationIndex = 0;
    settingsObjectEdit = false;
    navigationIndexTop = 1;
    reloadLCDSettings();
    guiCurrentObjectsList = displayMenuObjects;

}

void renderDisplaySettingsMenu(void)
{
    
    renderTextOnCenter("DISP", 0);

    renderObjectLabel(displayMenuObjects[0], 3, 10);
    renderObjectValue(displayMenuObjects[0], 3, 95);

    renderObjectLabel(displayMenuObjects[1], 4, 10);
    switch ((VbuffRotation_t)displayRotationObject.objectDisplayedIntValue)
    {
        case ROTATION_NONE:
            renderText("0", 4, 95);
            break;
        case ROTATION_RIGHT:
            renderText("+90", 4, 95);
            break;
        case ROTATION_LEFT:
            renderText("-90", 4, 95);
            break;
        default:
            break;
    }

    uint8_t markerPage      = displayMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = displayMenuObjects[settingsNavigationIndex]->objectColumnPosition;

    renderSelectionMarker(markerPage, markerColumn + (settingsObjectEdit ? 75 : -10));

}

void renderDisplaySettingsMenuVertical(void)
{
    
    renderTextOnCenterVertical("DISP", 0);

    renderObjectLabelVertical(displayMenuObjects[0], 3, 5);
    renderObjectValueVertical(displayMenuObjects[0], 3, 45);

    renderObjectLabelVertical(displayMenuObjects[1], 5, 5);
    switch ((VbuffRotation_t)displayRotationObject.objectDisplayedIntValue)
    {
        case ROTATION_NONE:
            renderTextVertical("0", 5, 45);
            break;
        case ROTATION_RIGHT:
            renderTextVertical("+90", 5, 45);
            break;
        case ROTATION_LEFT:
            renderTextVertical("-90", 5, 45);
            break;
        default:
            break;
    }

    uint8_t markerPage      = displayMenuObjects[settingsNavigationIndex]->objectPagePosition;
    uint8_t markerColumn    = displayMenuObjects[settingsNavigationIndex]->objectColumnPosition;

    renderSelectionMarkerVertical(markerPage, markerColumn + (settingsObjectEdit ? 35 : -5));
    rotateVerticalBuffer(guiRotation);
}


void setEscButtonClickFunction(void(*functionPointer)(void))
{
    EscButtonClickFunction = functionPointer;
}

void setDnButtonClickFunction(void(*functionPointer)(void))
{
    DnButtonClickFunction = functionPointer;
}

void setUpButtonClickFunction(void(*functionPointer)(void))
{
    UpButtonClickFunction = functionPointer;
}

void setOkButtonClickFunction(void(*functionPointer)(void))
{
    OkButtonClickFunction = functionPointer;
}

void guiEscButtonClicked(void)
{
    EscButtonClickFunction();
}

void guiDnButtonClicked(void)
{
    DnButtonClickFunction();
}

void guiUpButtonClicked(void)
{
    UpButtonClickFunction();
}

void guiOkButtonClicked(void)
{
    OkButtonClickFunction();
}

void selectedObjectValueUp(void)
{
    (*(guiCurrentObjectsList + settingsNavigationIndex))->valueUpFunction();
}

void selectedObjectValueDown(void)
{
    (*(guiCurrentObjectsList + settingsNavigationIndex))->valueDnFunction();
}

void selectedObjectValueSave(void)
{
    (*(guiCurrentObjectsList + settingsNavigationIndex))->valueSaveFunction();
}

void editSelectedObject(void)
{
    settingsObjectEdit = true;
    setEscButtonClickFunction(&enterSettingsMenu);
    setDnButtonClickFunction(&selectedObjectValueDown);
    setUpButtonClickFunction(&selectedObjectValueUp);
    setOkButtonClickFunction(&selectedObjectValueSave);
}

void enterObject(void)
{
    (*(guiCurrentObjectsList + settingsNavigationIndex))->objectEnterFunction();
}

