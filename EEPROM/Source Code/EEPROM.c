#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/eeprom.h"
#include "driverlib/flash.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"

// Global variable for system clock frequency
uint32_t ui32SysClkFreq;

// Function to perform EEPROM operations
void EEPROM(void)
{
    // Define arrays for data read and write
    uint32_t data_read[5];
    uint32_t data_write[5];

    // Initialize data to be written
    data_write[0] = 0x01;
    data_write[1] = 0x02;

    // Set system clock frequency
    ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);

    // Enable GPIO port N and configure pins as outputs
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x00);
    SysCtlDelay(ui32SysClkFreq / 3);

    // Erase flash memory
    FlashErase(0x10000);

    // Program data into flash memory
    FlashProgram(data_write, 0x10000, sizeof(data_write));

    // Enable EEPROM module
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    EEPROMInit();

    // Perform mass erase of EEPROM
    EEPROMMassErase();

    // Read data from EEPROM
    EEPROMRead(data_read, 0x0, sizeof(data_read));

    // Program data into EEPROM
    EEPROMProgram(data_write, 0x0, sizeof(data_write));

    // Read data from EEPROM again
    EEPROMRead(data_read, 0x0, sizeof(data_read));

    // Set GPIO pins to indicate completion
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0x01);
}
