///////////////////////////////////////////////////////////////////////////////
/// \file Terminal.h
/// \author Ronald Sousa
/// \website www.HashDefineElectronics.com
/// \company Hash Define Electronics Ltd
///
/// \brief  handles the terminal serial interface
///////////////////////////////////////////////////////////////////////////////
#ifndef __TERMINAL_H__
#define __TERMINAL_H__

	#include "../common.h"
    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Define the maximun number of charater is string parameter can support
    ///////////////////////////////////////////////////////////////////////////////
    #define Terminal_STRING_LENGTH 30

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Define home many parameter we are willing to support per data
    /// transmission
    ///////////////////////////////////////////////////////////////////////////////
    #define Terminal_MAX_PARAMETER_SUPPORT 5

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Define the supported data type
    ///////////////////////////////////////////////////////////////////////////////
    typedef enum{
        SDEnum_U = 'U',         ///< unsinged integer 32 bit
        SDEnum_I = 'I',         ///< singed integer 32 bit
        SDEnum_F = 'F',         ///< float 32 bit
        SDEnum_T = 'T',         ///< string
        SDEnum_L = 'L',         ///< boolean false
        SDEnum_H = 'H',         ///< boolean true

        // Lower case support
        SDEnum_u = 'u',         ///< unsinged integer 32 bit
        SDEnum_i = 'i',         ///< singed integer 32 bit
        SDEnum_f = 'f',         ///< float 32 bit
        SDEnum_t = 't',         ///< string
        SDEnum_l = 'l',         ///< boolean false
        SDEnum_h = 'h',         ///< boolean true

        SDEnum_E = 'E',         ///< Error code. unsinged integer
        SDEnum_S = 'S',         ///< Select option. unsinged integer 32 bit
        SDEnum_s = 's',         ///< Select option. unsinged integer 32 bit
    }TerminalDataTypeEnum;

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Define the supported data type
    ///////////////////////////////////////////////////////////////////////////////
    typedef enum{
        SDErrorEnum_E0 = 0,         ///< Incorrect command parameter
        SDErrorEnum_E1,             ///< Buffer overflow
        SDErrorEnum_E2,             ///< No parameter detected
        SDErrorEnum_E3,             ///< too many parameters
        SDErrorEnum_E4,             ///< Invalid parameter
        SDErrorEnum_E5,             ///< parameter data is too long. Its should be less than (Terminal_STRING_LENGTH - 2). see Terminal_STRING_LENGTH
    }TerminalErrorEnum;

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Define the paramter data holder
    ///////////////////////////////////////////////////////////////////////////////
    typedef union {
        struct{
            uint8_t Type;                           ///< see TerminalDataTypeEnum
            DataConverter Data;                     ///< The convert signed, unsinged and float
        };
        uint8_t String[Terminal_STRING_LENGTH];       ///< stores the text representation of the data received
    }TerminalParameterStruct;

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Define the parameter data structure
    ///////////////////////////////////////////////////////////////////////////////
    typedef struct{
        uint32_t ParameterLength;                                           ///< number of parameter detected
        TerminalParameterStruct Parameter[Terminal_MAX_PARAMETER_SUPPORT];      ///< holds all the detected parameters
    }TerminalType;

    ////////////////////////////////////////////////////////////////////////////////
    ///	\brief	Define the menu debug screen used by the serial interface
    ////////////////////////////////////////////////////////////////////////////////
    typedef struct SDMenuStruture{
        uint8_t *MenuTitle;                                                  ///< define the menu title
        void (*DrawOptions)(void);                                           ///< define the draw menu options
        void (*Process )(uint8_t *character);                                ///< define the process screen. this screen is passed every charater we've detect on the screen
        struct SDMenuStruture * (* MenuPointer) (TerminalType *systemData);    ///< define the current menu pointer that will handle the input data
    }SDMenuStruture;

    void Terminal_Init(SerialInterface *uartInterface);
    void Terminal_WriteString(uint8_t *source);
    uint_fast8_t Terminal_Monitor(void);

    ///////////////////////////////////////////////////////////////////////////////
    /// \brief Macro for clearing the terminal screen
    ///////////////////////////////////////////////////////////////////////////////
    #define CLEAR_TERMINAL_SCREEN() Terminal_WriteByte(0x0C)

#endif
