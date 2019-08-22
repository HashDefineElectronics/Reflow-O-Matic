///////////////////////////////////////////////////////////////////////////////
/// \file Debug_MainMenu.h
/// \author Ronald Sousa
/// \website www.HashDefineElectronics.com
/// \company Hash Define Electronics Ltd
///
/// \brief  handles the debug main screen used by our serial interface
///////////////////////////////////////////////////////////////////////////////
#include "OvenControl.h"
#include "./Terminal/Terminal.h"
#include "common.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief set the process update rate to 100ms
///////////////////////////////////////////////////////////////////////////////
static TickType UpdateMessages = { 0 , 500} ;

///////////////////////////////////////////////////////////////////////////////
/// \brief Draw main menu
///////////////////////////////////////////////////////////////////////////////
static void DrawMainMenu(void) {
    Terminal_WriteString( (uint8_t*)"\n\r"
                        "S1. Oven Stop\n\r"
                        "S2. Start\n\r"
    					"S3. Set Target Tempeature UXXX= tempeature\n\r"
    					"S4. Set Motor State U0 = off U1=on\n\r"
    					"S5. Set LED State U0=off U1=on else toggle\n\r"
                        "S6. Toggle status");
}

/**
 * used to keep track if we should send screen updates
 */
static uint_fast8_t EnableUpdate = FALSE;

///////////////////////////////////////////////////////////////////////////////
/// \brief This is the menu options
///////////////////////////////////////////////////////////////////////////////
typedef enum
{
    SDCommand_Stop = 1,
    SDCommand_Start,
	SDCommand_SetTargettemperature,
	SDCommand_SetMotor,
    SDCommand_SetLed,
    SDCommand_EnableUpdates,
} SDebug_CommandCode;

/**
 * Process the screen command
 * 
 * @param SystemData system parameter data structure
 * 
 * @return true on success else false
 */
static struct SDMenuStruture * MainMenu(TerminalType *SystemData) {

    switch(SystemData->Parameter[0].Data.ui32_t[0]) {
        case SDCommand_Stop:
            OvenControl_Stop();
            break;

        case SDCommand_Start:
                OverControl_Start();
            break;

        case SDCommand_SetTargettemperature:

            if( SystemData->ParameterLength > 1) {
                if(SystemData->Parameter[1].Data.ui32_t[0]) {
                	Led_On();
                } else {
                	Led_Off();
                }
                OvenControl_SetTemperature(SystemData->Parameter[1].Data.ui32_t[0]);
            }

            break;

        case SDCommand_SetMotor:

            if( SystemData->ParameterLength > 1) {
                if(SystemData->Parameter[1].Data.ui32_t[0] > 100) {
                	SystemData->Parameter[1].Data.f32_t[0] = 1.0;
                } else {
                	SystemData->Parameter[1].Data.f32_t[0] = ((float)SystemData->Parameter[1].Data.ui32_t[0] / 100.0);
                }


                OverControl_SetFanPower(SystemData->Parameter[1].Data.f32_t[0]);
            } else {
                OverControl_SetFanPower(0);
            }
            break;

        case SDCommand_SetLed:
            if( SystemData->ParameterLength > 1) {
                if(SystemData->Parameter[1].Data.ui8_t[0]) {
                    Led_On();  
                } else {
                    Led_Off();
                }
            } else {
                Led_Toggle();
            }
            break;
        case SDCommand_EnableUpdates:
            // toggle update state
            EnableUpdate = ~EnableUpdate;
            break;
        default:
            break;
    }

    return 0;
}

/**
 * this main loop process
 * 
 * @param character pointer to the current serail byte data
 */
void Process (uint8_t *character) {

	if(!character) {

		OvenControl_Process(character);

        if(EnableUpdate) {
            // check if we need to update the screen
            if (Tick_DelayMs_NonBlocking(FALSE, &UpdateMessages)) {
                Tick_DelayMs_NonBlocking(TRUE, &UpdateMessages); // reset the tick counter

                OvenControl_PrintStatus();
            }
        }

	}
}

/**
 * This is the defualt menu screen
 */
struct SDMenuStruture  DefaultMenu = {
    (uint8_t *)"Main Menu",
    DrawMainMenu,
	Process,
    MainMenu
};
