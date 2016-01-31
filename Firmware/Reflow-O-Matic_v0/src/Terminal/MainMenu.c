///////////////////////////////////////////////////////////////////////////////
/// \file Debug_MainMenu.h
/// \author Ronald Sousa
/// \website www.HashDefineElectronics.com
/// \company Hash Define Electronics Ltd
///
/// \brief  handles the debug main screen used by our serial interface
///////////////////////////////////////////////////////////////////////////////
#include "./Terminal/Terminal.h"
#include "common.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief Draw main menu
///////////////////////////////////////////////////////////////////////////////
static void DrawMainMenu(void)
{
    Terminal_WriteString( (uint8_t*)"\n\r"
                        "S1. Set LED State\n\r");
}
///////////////////////////////////////////////////////////////////////////////
/// \brief This is the menu options
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    SDCommand_LEDControl = 1,
} SDebug_CommandCode;

///////////////////////////////////////////////////////////////////////////////
/// \brief Process the screen command
///
/// \param SystemData system parameter data structure
/// \return true on success else false
///////////////////////////////////////////////////////////////////////////////
static struct SDMenuStruture * MainMenu(TerminalType *SystemData)
{
    switch(SystemData->Parameter[0].Data.ui32_t[0])
    {
        case SDCommand_LEDControl:

            if( SystemData->ParameterLength > 1)
            {
                if(SystemData->Parameter[1].Data.ui32_t[0])
                {
                	Led_On();
                }
                else
                {
                	Led_Off();
                }
            }
            else
            {
            	Led_Toggle();
            }

        break;
        default:
            break;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief This is the defualt menu screen
///////////////////////////////////////////////////////////////////////////////
struct SDMenuStruture  DefaultMenu = {
    (uint8_t *)"Main Menu",
    DrawMainMenu,
    NULL,
    MainMenu
};
