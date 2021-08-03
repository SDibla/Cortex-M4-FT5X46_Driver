/****************************************************************************************
  I2C Capacitive Touch Driver: API Functions Header File

  File Name:
    drv_captouch_i2c.h

  Summary:
    This header file provides the API function prototypes for the I2C Capacitive Touch 
    controller.

  Description:
    API function prototypes for the I2C Capacitive Touch controller like the FT5xxx.
 ***************************************************************************************/


#ifndef DRV_CAPTOUCH_I2C_LIBRARY_H
#define DRV_CAPTOUCH_I2C_LIBRARY_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "drv_captouch_i2c_defines.h"


// *****************************************************************************
// *****************************************************************************
// Section: Defines

#define TWOTOUCH

#define I2C_BASEADDR                I2C3
#define INT_GPIO                    GPIO5
#define INT_PIN                     4
#define I2C_CLK_FREQ                (CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(kCLOCK_RootI2c3)) / (CLOCK_GetRootPostDivider(kCLOCK_RootI2c3)) / 5) /* SYSTEM PLL1 DIV5 */

#define I2C_SLAVE_ADDR              0x38U
#define I2C_BAUDRATE                100000U // 100kHz
#define I2C_BAUDRATE_MAX            400000U // 400kHz
#define I2C_BUFFER_LENGTH           30

#define MAX_X_PIXEL                 800
#define MAX_Y_PIXEL                 480
#define WIDTH                       154     // 153.6 mm
#define HEIGHT                      86      // 86.64 mm
#define ORIENTATION                 0       // 0� also supported 90�, 180�, 270�

// *****************************************************************************
// *****************************************************************************
// Section: Init Function

void DRV_CAPTOUCH_I2C_Init(void);


// *****************************************************************************
// *****************************************************************************
// Section: I2C Access Functions

int8_t DRV_CAPTOUCH_I2C_ReadByte(uint8_t reg, uint8_t *rxd);
int8_t DRV_CAPTOUCH_I2C_ReadHalfWord(uint8_t start_reg, uint16_t *rxd);
int8_t DRV_CAPTOUCH_I2C_ReadWord(uint8_t start_reg, uint32_t *rxd);
int8_t DRV_CAPTOUCH_I2C_ReadArray(uint8_t start_reg, uint8_t *rxd, uint8_t len);
inline int8_t DRV_CAPTOUCH_I2C_ReadRawData(uint8_t *rxd, uint8_t len){
    return DRV_CAPTOUCH_I2C_ReadArray(0x00, rxd, len);
}
int8_t DRV_CAPTOUCH_I2C_WriteByte(uint8_t reg, uint8_t data);
int8_t DRV_CAPTOUCH_I2C_WriteHalfWord(uint8_t start_reg, uint16_t data);
int8_t DRV_CAPTOUCH_I2C_WriteWord(uint8_t start_reg, uint32_t data);
int8_t DRV_CAPTOUCH_I2C_WriteArray(uint8_t start_reg, uint8_t *txd, uint8_t len);


// *****************************************************************************
// *****************************************************************************
// Section: Get Functions

int8_t DRV_CAPTOUCH_I2C_GetSinglePixelPoint(POINT_OBJ* point);
int8_t DRV_CAPTOUCH_I2C_GetMultiPixelPoint(POINT_OBJ* point, uint8_t n);
int8_t DRV_CAPTOUCH_I2C_GetTouch(bool *touch);
int8_t DRV_CAPTOUCH_I2C_GetNumberOfTouch(uint8_t *n);
int8_t DRV_CAPTOUCH_I2C_GetDeviceMode(uint8_t *rxd);
int8_t DRV_CAPTOUCH_I2C_GetGestureID(uint8_t *rxd);
int8_t DRV_CAPTOUCH_I2C_GetState(uint8_t *rxd);
int8_t DRV_CAPTOUCH_I2C_GetThresholdObject(THRESHOLD_OBJ *th);


// *****************************************************************************
// *****************************************************************************
// Section: Set Functions

int8_t DRV_CAPTOUCH_I2C_SetDeviceMode(DEVICE_MODE mode);
int8_t DRV_CAPTOUCH_I2C_SetThresholdObject(THRESHOLD_OBJ *th);
int8_t DRV_CAPTOUCH_I2C_SetThresholdDefault(void);

#endif //DRV_CAPTOUCH_I2C_LIBRARY_H
