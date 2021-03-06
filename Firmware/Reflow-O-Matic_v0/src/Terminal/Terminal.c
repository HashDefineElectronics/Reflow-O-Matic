///////////////////////////////////////////////////////////////////////////////
/// \file SerialInterface.h
/// \author Ronald Sousa
/// \website www.HashDefineElectronics.com
/// \company Hash Define Electronics Ltd
///
/// \brief  this is the serial interface used to debug and test the CPU1
///////////////////////////////////////////////////////////////////////////////
#include <Terminal/MainMenu.h>
#include <Terminal/Terminal.h>
#include "common.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief Holds our serial instance
///////////////////////////////////////////////////////////////////////////////
static SerialInterface *COMPort;

///////////////////////////////////////////////////////////////////////////////
/// \brief Maximum serial buffer
///////////////////////////////////////////////////////////////////////////////
#define READ_BUFFER_LENGTH 30

///////////////////////////////////////////////////////////////////////////////
/// \brief set the process update rate to 100ms
///////////////////////////////////////////////////////////////////////////////
static TickType ProcessUpdateRate = { 0 , 10} ;

///////////////////////////////////////////////////////////////////////////////
/// \brief Define the read buffer which all data is store while we wait
/// for the end of line transmission
///////////////////////////////////////////////////////////////////////////////
uint8_t ReadBuffer[READ_BUFFER_LENGTH];

///////////////////////////////////////////////////////////////////////////////
/// \brief ReadBuffer pointer
///////////////////////////////////////////////////////////////////////////////
uint8_t * ReadBufferPointer = ReadBuffer;

///////////////////////////////////////////////////////////////////////////////
/// \brief how many bytes we have in the read buffer
///////////////////////////////////////////////////////////////////////////////
uint32_t ReadBufferLength = 0;

///////////////////////////////////////////////////////////////////////////////
/// \brief this is the number of TRANS_END_BYTE byte in a row we need before we can trigger
/// a system reset.
/// \sa ResetCounter TRANS_END_BYTE
///////////////////////////////////////////////////////////////////////////////
#define RESET_COUNTER_TRIGGER 3

///////////////////////////////////////////////////////////////////////////////
/// \brief Define the number of reset command we received
/// \sa RESET_COUNTER_TRIGGER
///////////////////////////////////////////////////////////////////////////////
static uint32_t ResetCounter = 0;

///////////////////////////////////////////////////////////////////////////////
/// \brief Define the end of transmission byte
///////////////////////////////////////////////////////////////////////////////
#define TRANS_END_BYTE 0x0D

///////////////////////////////////////////////////////////////////////////////
/// \brief the escape character will reset the current prompt and redraw the menu
///////////////////////////////////////////////////////////////////////////////
#define DRAW_MENU_KEY 0x1B

///////////////////////////////////////////////////////////////////////////////
/// \brief Holds the current menu pointer
///////////////////////////////////////////////////////////////////////////////
SDMenuStruture  *ActiveMenuPointer = &DefaultMenu;

///////////////////////////////////////////////////////////////////////////////
/// \brief Holds the process data packet
///////////////////////////////////////////////////////////////////////////////
static TerminalType TerminalSystem;

///////////////////////////////////////////////////////////////////////////////
/// \brief Portal layer for serial send byte
///
/// \param source byte to write
///////////////////////////////////////////////////////////////////////////////
void Terminal_WriteByte(uint8_t source)
{
    if ( COMPort ) // make sure that we can actual send data
    {
        COMPort->SendByte(source);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Write serial string
///
/// \param source pointer to the string to write. must end with null
///////////////////////////////////////////////////////////////////////////////
void Terminal_WriteString(uint8_t *source)
{
    if ( COMPort ) // make sure that we can actual send data
    {
        COMPort->SendString(source);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Portal layer for reading byte
///
/// \param source byte to write
///////////////////////////////////////////////////////////////////////////////
static inline uint8_t Terminal_ReadByte(void)
{
    uint8_t Result = 0;
    if ( COMPort ) // make sure that we can actual send data
    {
        if ( COMPort->GetByte(&Result) )
        {
            return Result;
        }

        Result = 0; // error return 0
    }

    return Result;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Portal layer for checking is we have data available to read
///
/// \return true when there is no data to read
///////////////////////////////////////////////////////////////////////////////
static inline uint8_t Terminal_IsSerialBufferEmpty(void)
{
    if ( COMPort ) // make sure that we can actual send data
    {
        if ( COMPort->DoesReceiveBufferHaveData() )
        {
            return FALSE;
        }
    }

    // fail with a true.
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief This function handles the drawing of the command header
/// ie. the micro robot and hash define electronics detail
///////////////////////////////////////////////////////////////////////////////
static void Terminal_DrawHeader(void)
{
    Terminal_WriteString( (uint8_t *)"\t [@.@]\n\r"
                        "\t/|___|\\\n\r"
                        "\t d   b\n\r");
    Terminal_WriteString((uint8_t *)"Ronald Sousa\n\r");
    Terminal_WriteString((uint8_t *)"HashDefineElectronics.com\n\r\n\r");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Draws the system promp
///////////////////////////////////////////////////////////////////////////////
static void Terminal_DrawPrompt(void)
{
    Terminal_WriteString((uint8_t *)"\n\r[");
    if(ActiveMenuPointer && ActiveMenuPointer->MenuTitle)
    {
        Terminal_WriteString(ActiveMenuPointer->MenuTitle);
    }
    Terminal_WriteString((uint8_t *)"] > ");
}

///////////////////////////////////////////////////////////////////////////////
/// \brief This function is used to send error code back to the user
///
/// \param ack if true then success message is send else the error code
/// \param code ack = true then, the received command  [TerminalType.Command]
///     ack = false, then the error code.
///
/// \param code ack = true then, the number of parameter detected [TerminalType.ParameterLength]
///     ack = false, ignored
///
///////////////////////////////////////////////////////////////////////////////
void Terminal_SendAcknowledgement(uint_fast8_t ack, uint32_t code, uint32_t parameterLength)
{
    uint8_t Buffer[20];

    if( ack )
    {
        snprintf((char *)&Buffer[0], 20, "\n\r%c%u %c%u\n\r",SDEnum_S, (unsigned int)code, (unsigned int)SDEnum_U, (unsigned int)parameterLength);
    }
    else
    {
        snprintf((char *)&Buffer[0], 20, "\n\r%c%u\n\r",(unsigned int)SDEnum_E, (unsigned int)code);
    }

    Terminal_WriteString(&Buffer[0]);

    if( !ack )
    {
        Terminal_DrawPrompt();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Extract system parameter
///
/// \return 0 = success; 1 = source or SystemData pointer error; 2 = too many parameter
/// 3 = no parameter detected, 4 = invalid parameter type, 5= parameter data is too long
///////////////////////////////////////////////////////////////////////////////
uint_fast8_t Terminal_ExtractParameters(uint8_t *source, TerminalType *SystemData)
{
    uint32_t CharacterIndex = 0;

    uint8_t *StringPointerTemp; // used to hold the working parameter string reference for easier data manipulation
    uint32_t ParameterIndex;

    if(!source || !SystemData)
    {
        return 1; // one or all pointers are blank
    }
    else if ( !*source ) // in the event that we might have no data to process
    {
        Terminal_SendAcknowledgement(FALSE,SDErrorEnum_E2, 0);
        return 3; // no parameter detected
    }



    // reset parameter
    SystemData->ParameterLength = 1;
    StringPointerTemp = &SystemData->Parameter[0].String[0];
    *StringPointerTemp = 0;

    // Interact through all the data until we have found what we need
    for( ; *source ; source++ )
    {
        *StringPointerTemp = 0;

        if ( ' ' == *source )
        {
            if(SystemData->ParameterLength < Terminal_MAX_PARAMETER_SUPPORT)
            {
                // new parameter
                StringPointerTemp = &SystemData->Parameter[SystemData->ParameterLength].String[0];
                SystemData->ParameterLength++;
                CharacterIndex = 0;
            }
            else
            {
                // Too many parameters
                Terminal_SendAcknowledgement(FALSE,SDErrorEnum_E3, 0);
                return 2;
            }
        }
        else
        {
            if ( CharacterIndex < (Terminal_STRING_LENGTH -2) )
            {
                *StringPointerTemp = *source;
                StringPointerTemp++;
                *StringPointerTemp = 0;
                CharacterIndex++;
            }
            else
            {
                // paramter data is too large
                Terminal_SendAcknowledgement(FALSE,SDErrorEnum_E5, 0);
                return 5;
            }
        }
    }


    for(ParameterIndex = 0; ParameterIndex <  SystemData->ParameterLength; ParameterIndex++)
    {
        switch(SystemData->Parameter[ParameterIndex].Type)
        {
            /// unsigned integer 32 bit
            case  SDEnum_S:
            case SDEnum_s:

                if( ParameterIndex )
                {
                    Terminal_SendAcknowledgement(FALSE,SDErrorEnum_E4, 0);
                    return 4;
                }

                // fallthrough
            case  SDEnum_U:
            case SDEnum_u:
                SystemData->Parameter[ParameterIndex].Data.ui32_t[0] = atoll((char *)&SystemData->Parameter[ParameterIndex].String[1]);
                break;

            // singed integer 32 bit
            case  SDEnum_I:
            case SDEnum_i:
                SystemData->Parameter[ParameterIndex].Data.i32_t[0] = atoi((char *)&SystemData->Parameter[ParameterIndex].String[1]);
                break;

            // float 32 bit
            case  SDEnum_F:
            case SDEnum_f:
                SystemData->Parameter[ParameterIndex].Data.f32_t[0] = atof((char *)&SystemData->Parameter[ParameterIndex].String[1]);
                break;

            // boolean false
            case  SDEnum_L:
            case SDEnum_l:
                SystemData->Parameter[ParameterIndex].Data.ui32_t[0] = 0x00000000;
                break;

            // boolean true
            case  SDEnum_H:
            case SDEnum_h:
                SystemData->Parameter[ParameterIndex].Data.ui32_t[0] = 0xFFFFFFFF;
                break;
            case  SDEnum_T:
            case SDEnum_t:
                // Text doesn't need processing
                break;

            default: // error invalid parameter
                Terminal_SendAcknowledgement(FALSE,SDErrorEnum_E4, 0);
                return 4;
                break;
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief This function monitor the serial line and process the incoming data
///
/// return true = we have data; false = no data
///////////////////////////////////////////////////////////////////////////////
uint_fast8_t Terminal_Monitor(void)
{
    struct SDMenuStruture  *MenuTemp;

 //  while(!Terminal_IsSerialBufferEmpty() )
    {
        if ( !Terminal_IsSerialBufferEmpty() ) // do we have data
        {
            *ReadBufferPointer = Terminal_ReadByte();

            if ( TRANS_END_BYTE == *ReadBufferPointer) // we detected end of transmission.
            {
                *ReadBufferPointer = 0; // replace the termination character to a null for better string support

                // for a valid command, we need at lease 2 bytes
                if(ReadBufferLength > 1)
                {
                    ResetCounter = 0; // clear the reset counter

                    if (!Terminal_ExtractParameters(&ReadBuffer[0] , &TerminalSystem) )
                    {
                        // The first parameter should be of type U or u for unsigned integer
                        if ( SDEnum_S == TerminalSystem.Parameter[0].Type
                            || SDEnum_s == TerminalSystem.Parameter[0].Type )
                        {
                            Terminal_SendAcknowledgement(TRUE,TerminalSystem.Parameter[0].Data.ui32_t[0], TerminalSystem.ParameterLength);
                            if (ActiveMenuPointer)
                            {
                                MenuTemp = ActiveMenuPointer->MenuPointer(&TerminalSystem);

                                // have we changed screen?
                                if( MenuTemp )
                                {
                                    ActiveMenuPointer = MenuTemp;
                                    CLEAR_TERMINAL_SCREEN();
                                    Terminal_DrawHeader();
                                    ActiveMenuPointer->DrawOptions();
                                }

                            }
                            Terminal_DrawPrompt();
                        }
                        else
                        {
                            Terminal_SendAcknowledgement(FALSE,SDErrorEnum_E0, 0);
                        }
                    }
                }
                else
                {
                    if( ResetCounter < (RESET_COUNTER_TRIGGER -1 ) )
                    {
                        ResetCounter++;
                    }
                    else
                    {
                        Terminal_Init(NULL);
                        ResetCounter = 0;
                        ReadBufferLength = 0;
                        ReadBufferPointer = &ReadBuffer[0];
                        return TRUE;
                    }
                }

                // reset read buffer
                ReadBufferPointer = &ReadBuffer[0];
                ReadBufferLength = 0;
            }
            else if( DRAW_MENU_KEY == *ReadBufferPointer && ActiveMenuPointer)
            {
                CLEAR_TERMINAL_SCREEN();
                Terminal_DrawHeader();
                ActiveMenuPointer->DrawOptions();
                Terminal_DrawPrompt();

                ResetCounter = 0;
                ReadBufferLength = 0;
                ReadBufferPointer = &ReadBuffer[0];
                return TRUE;
            }
            else if( *ReadBufferPointer >= ' ' && *ReadBufferPointer <= '~')
            {
                Terminal_WriteByte(*ReadBufferPointer);

                if (ActiveMenuPointer && ActiveMenuPointer->Process)
                {
                    ResetCounter = 0; // clear the reset counter
                    // pass the screen data
                    ActiveMenuPointer->Process(ReadBufferPointer);
                }

                if( ReadBufferLength < (READ_BUFFER_LENGTH -1) )
                {
                    ReadBufferLength++;
                    ReadBufferPointer++;
                }
                else
                {
                    // reset read buffer
                    ReadBufferPointer = &ReadBuffer[0];
                    ReadBufferLength = 0;
                    ResetCounter = 0;
                    Terminal_SendAcknowledgement(FALSE, SDErrorEnum_E1, 0);
                }
            }
        }
        else
        {
            if (ActiveMenuPointer && ActiveMenuPointer->Process)
            {
                // check if we need to update the screen
                if ( Tick_DelayMs_NonBlocking(FALSE, &ProcessUpdateRate) )
                {
                    // reset the tick counter
                    Tick_DelayMs_NonBlocking(TRUE, &ProcessUpdateRate);
                    ActiveMenuPointer->Process(NULL);
                }
            }
            else
            {
                Tick_DelayMs_NonBlocking(TRUE, &ProcessUpdateRate);
            }
        }
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Initialize the serial interface
///
/// \param uartInterface pointer to the serial interface to use. if null is pass then
/// the serial port will not be open therefore it till be assume that
/// it already open.
///////////////////////////////////////////////////////////////////////////////
void Terminal_Init(SerialInterface *uartInterface)
{
    if(uartInterface) // if the pointer is not null
    {
        COMPort = uartInterface; // update our copy of the serial port

        COMPort->Open(115200);

    }

    // reset pointer and length
    ReadBufferPointer = &ReadBuffer[0];
    ReadBufferLength = 0;

    CLEAR_TERMINAL_SCREEN();
    Terminal_DrawHeader();
    // draw the system detail
    Terminal_WriteString( (uint8_t *)"COMPILED       "  COMPILED_DATA_TIME  "\n\r"
									"FIRMWARE       "  FIRMWARE_VERSION  "\n\r"
									"Press ESC to show current menu\n\r"
									"Press enter three times to show the program information\n\r");
    Terminal_DrawPrompt();

    Tick_DelayMs_NonBlocking(TRUE, &ProcessUpdateRate);
}
