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
/// \brief set the process update rate to 100ms
///////////////////////////////////////////////////////////////////////////////
static TickType UpdateMessages = { 0 , 500} ;

static float MotorPWM = 0;
///////////////////////////////////////////////////////////////////////////////
/// \brief Draw main menu
///////////////////////////////////////////////////////////////////////////////
static void DrawMainMenu(void) {
    Terminal_WriteString( (uint8_t*)"\n\r"
                        "S1. Everything Off\n\r"
    					"S2. Set Target Tempeature UXXX= tempeature\n\r"
    					"S3. Set Motor State U0 = off U1=on\n\r"
    					"S4. Set LED State U0=off U1=on else toggle\n\r"
                        "S5. U0= Disable updates U1= Enabled updated");
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
    SDCommand_EverythingOff = 1,
	SDCommand_SetTargettemperature,
	SDCommand_SetMotor,
    SDCommand_SetLed,
    SDCommand_EnableUpdates,
} SDebug_CommandCode;

static uint32_t DisableOven = TRUE;

///////////////////////////////////////////////////////////////////////////////
/// \brief Process the screen command
///
/// \param SystemData system parameter data structure
/// \return true on success else false
///////////////////////////////////////////////////////////////////////////////
static struct SDMenuStruture * MainMenu(TerminalType *SystemData)
{

    switch(SystemData->Parameter[0].Data.ui32_t[0]) {
        case SDCommand_EverythingOff:
            OvenControl_Stop();
            MotorPWM = 0;
            break;

        case SDCommand_SetTargettemperature:

            if( SystemData->ParameterLength > 1) {
                if(SystemData->Parameter[1].Data.ui32_t[0]) {
                	Led_On();
                } else {
                	Led_Off();
                }
                OvenControl_SetTemperature(SystemData->Parameter[1].Data.ui32_t[0]);
                DisableOven = FALSE;
            }

            break;

        case SDCommand_SetMotor:

            if( SystemData->ParameterLength > 1) {
                if(SystemData->Parameter[1].Data.ui32_t[0] > 100) {
                	SystemData->Parameter[1].Data.f32_t[0] = 1.0;
                } else {
                	SystemData->Parameter[1].Data.f32_t[0] = ((float)SystemData->Parameter[1].Data.ui32_t[0] / 100.0);
                }

               	PWM_SetDuty(MoterPwmPin, SystemData->Parameter[1].Data.f32_t[0]);
               	MotorPWM = SystemData->Parameter[1].Data.f32_t[0];
               	DisableOven = FALSE;
            } else {
            	PWM_SetDuty(MoterPwmPin, 0);
            	MotorPWM = 0;
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
            if(SystemData->ParameterLength) {
                EnableUpdate = SystemData->Parameter[1].Data.ui8_t[0] ? TRUE : FALSE;
            }
            break;
        default:
            break;
    }

    return 0;
}

void Process (uint8_t *character) {
	float Temperature = 0;
	float TargetTemperature = 0;
	float HeaterPWM = 0;
	uint8_t MessageBuffer[100];

	if(!character) {
		OvenControl_Process(&Temperature, &TargetTemperature, &HeaterPWM);

        if(EnableUpdate) {
            // check if we need to update the screen
            if (Tick_DelayMs_NonBlocking(FALSE, &UpdateMessages)) {
                // reset the tick counter
                Tick_DelayMs_NonBlocking(TRUE, &UpdateMessages);
                snprintf((char *)&MessageBuffer[0], 100, "T%4.4f,A%4.4f,H%4.4f,M%4.4f\n\r",Temperature, TargetTemperature, HeaterPWM,MotorPWM);
                Terminal_WriteString(&MessageBuffer[0]);
            }
        }

	}
}

///////////////////////////////////////////////////////////////////////////////
/// \brief This is the defualt menu screen
///////////////////////////////////////////////////////////////////////////////
struct SDMenuStruture  DefaultMenu = {
    (uint8_t *)"Main Menu",
    DrawMainMenu,
	Process,
    MainMenu
};
