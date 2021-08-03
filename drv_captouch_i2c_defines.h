
/****************************************************************************************
  I2C Capacitive Touch Driver: API Defines Header File

  File Name:
    drv_captouch_i2c_defines.h

  Summary:
    This header file contains device specific defines. 
    This also contains object declarations used in the API.

  Description:
    This file is used by the API.
 ***************************************************************************************/


#ifndef DRV_CAPTOUCH_DRV_CAPTOUCH_I2C_DEFINES_H
#define DRV_CAPTOUCH_DRV_CAPTOUCH_I2C_DEFINES_H


// *****************************************************************************
// *****************************************************************************
// Section: Defines

#define DRV_CAPTOUCH_I2C_ADD    0x38
#define BYTE                    0x01
#define HALFWORD                0x02
#define WORD                    0x04
#define MAX_TOUCHES             10

/* Register Addresses in Operating Mode */
#define OP_REG_DEVICEMODE       0x00
#define OP_REG_GESTID           0x01
#define OP_REG_TDSTATUS         0x02
#define OP_REG_TOUCHX1H         0x03 // | event flag (2bit) | none (2bit) | touchX1H |
#define OP_REG_TOUCHX1L         0x04
#define OP_REG_TOUCHY1H         0x05 // | Touch ID (4bit) | touchY1H |
#define OP_REG_TOUCHY1L         0x06
#define OP_REG_TOUCHX2H         0x09 // | event flag (2bit) | none (2bit) | touchX2H |
#define OP_REG_TOUCHX2L         0x0A
#define OP_REG_TOUCHY2H         0x0B // | Touch ID (4bit) | touchY2H |
#define OP_REG_TOUCHY2L         0x0C
#ifndef TWOTOUCH
#define OP_REG_TOUCHX3H         0x0F // | event flag (2bit) | none (2bit) | touchX3H |
#define OP_REG_TOUCHX3L         0x10
#define OP_REG_TOUCHY3H         0x11 // | Touch ID (4bit) | touchY3H |
#define OP_REG_TOUCHY3L         0x12
#define OP_REG_TOUCHX4H         0x15 // | event flag (2bit) | none (2bit) | touchX4H |
#define OP_REG_TOUCHX4L         0x16
#define OP_REG_TOUCHY4H         0x17 // | Touch ID (4bit) | touchY4H |
#define OP_REG_TOUCHY4L         0x18
#define OP_REG_TOUCHX5H         0x1B // | event flag (2bit) | none (2bit) | touchX5H |
#define OP_REG_TOUCHX5L         0x1C
#define OP_REG_TOUCHY5H         0x1D // | Touch ID (4bit) | touchY5H |
#define OP_REG_TOUCHY5L         0x1E
// ... till 10
#endif
#define OP_REG_THGROUP          0x80 // valid touching detect threshold
#define OP_REG_THPEAK           0x81 // valid touching peak detect threshold
#define OP_REG_THCAL            0x82 // the threshold when calculating the focus of touching
#define OP_REG_THWATER          0x83 // the threshold when there is surface water
#define OP_REG_THTEMP           0x84 // the threshold of temperature compensation
#define OP_REG_THTDIFF          0x85 // the threshold whether the coordinate is different from the original
#define OP_REG_CTRL             0x86
#define OP_REG_TIMMONITOR       0x87 // the timer of entering monitor status
#define OP_REG_PERIODACTIVE     0x88
#define OP_REG_PERIODMONITOR    0x89 // the timer of entering idle while in monitor status
#define OP_REG_AUTOCLBMONITOR   0xA0 // auto calibration mode
#define OP_REG_LIBVERSIONH      0xA1
#define OP_REG_LIBVERSIONL      0xA2
#define OP_REG_CIPHER           0xA3
#define OP_REG_MODE             0xA4 // interrupt status to host
#define OP_REG_PMODE            0xA5 // Power Consume Mode
#define OP_REG_FIRMID           0xA6
#define OP_REG_STATE            0xA7 // running state
#define OP_REG_ERR              0xA9 // error code
#define OP_REG_CLB              0xAA // Configure TP module during calibration in Test Mode

/* Register Addresses in Testing Mode */
#ifdef TEST_MODE_EN
#define TE_REG_DEVICEMODE       0x00
#define TE_REG_ROWADD           0x01
#define TE_REG_STARTSCAN        0x02
#define TE_REG_ROWNUM           0x03
#define TE_REG_COLNUM           0x04
#define TE_REG_DRIVERVOL        0x05
#define TE_REG_STARTRX          0x06
#define TE_REG_GAIN             0x07
#define TE_REG_ORIGINXH         0x08
#define TE_REG_ORIGINXL         0x09
#define TE_REG_ORIGINYH         0x0A
#define TE_REG_ORIGINYL         0x0B
#define TE_REG_RESWH            0x0C
#define TE_REG_RESWL            0x0D
#define TE_REG_RESHH            0x0E
#define TE_REG_RESHL            0x0F
#define TE_REG_RAWDATA0H        0x10
#define TE_REG_RAWDATA0L        0x11
// ... till 29
#define TE_REG_THPOINTNUM       0x4C
#define TE_REG_TXORDER0         0x50
// ... till 39
#define TE_REG_ROW0CAC          0x78
// ... till 39
#define TE_REG_COL0CAC          0xA0
// ... till 39
#define TE_REG_ROW01OFF         0xBF
// ... till 38_39
#define TE_REG_COL01OFF         0xD3
// ... till 28_29
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Types

/* Modality */
typedef enum {
    NORMAL_MODE         = 0X00,
    SYSTEM_MODE         = 0X01,
    TEST_MODE           = 0x04    
} DEVICE_MODE;   
   
/* Events */
typedef enum {
    EVENT_DOWN          = 0X00,
    EVENT_UP            = 0X01,
    EVENT_HOLD          = 0x02    
} EVENT_VALUE;

/* Gesture ID */
typedef enum {
    GESTURE_NO          = 0x00,
    GESTURE_SWIPE_UP    = 0x10,
    GESTURE_SWIPE_LEFT  = 0x14,
    GESTURE_SWIPE_DOWN  = 0x18,
    GESTURE_SWIPE_RIGHT = 0x1C,
    GESTURE_ZOOM_IN     = 0x48,
    GESTURE_ZOOM_OUT    = 0x49
} GESTURE_ID;

/* Running State */
typedef enum {
    CONFIGURE           = 0x00,
    WORK,
    CALIBRATION,
    FACTORY,
    AUTOCALIBRATION
} RUNNING_STATE;


// *****************************************************************************
// *****************************************************************************
// Section: Object definitions

/* Point Structure */
typedef struct
{
    uint8_t     event_flag;
    uint16_t    x;
    uint16_t    y;
    uint8_t     id;
} POINT_OBJ;

/* Threshold Structure */
typedef struct
{
    uint16_t    threshold;          // describes valid touching detect threshold
    uint8_t     peak;               // describes valid touching peak detect threshold
    uint8_t     focus;              // describes threshold when calculating the focus of touching
    uint8_t     water;              // describes threshold when there is surface water
    uint8_t     temperature;        // describes threshold of temperature compensation
    uint8_t     difference;         // describes threshold whether the coordinate is different from the original
} THRESHOLD_OBJ;

#endif //DRV_CAPTOUCH_DRV_CAPTOUCH_I2C_DEFINES_H
