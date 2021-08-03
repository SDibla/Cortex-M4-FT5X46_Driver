/****************************************************************************************
  I2C Capacitive Touch Driver: Implementation

  File Name:
    drv_captouch_i2c.c

  Summary:
    Implementation of MCU specific API function for the I2C Capacitive Touch controller.

  Description:
    API function implementation for the I2C Capacitive Touch controller like the FT5xxx.
 ***************************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files

#include "drv_captouch_i2c.h"
#include "fsl_gpio.h"
#include "fsl_i2c.h"


// *****************************************************************************
// *****************************************************************************
// Section: Variables

//! I2C Receive buffer
uint8_t i2cRx[I2C_BUFFER_LENGTH];

//! I2C Transmit buffer
uint8_t i2cTx[I2C_BUFFER_LENGTH];

//! ISR
i2c_master_handle_t g_m_handle;
volatile bool g_MasterCompletionFlag = false;


// *****************************************************************************
// *****************************************************************************
// Section: Init Function

void i2c_master_callback(I2C_Type *base, i2c_master_handle_t *handle, status_t status, void *userData)
{
    /* Signal transfer success when received success status. */
    if (status == kStatus_Success)
    {
        g_MasterCompletionFlag = true;
    }
}

void DRV_CAPTOUCH_I2C_Init(void)
{
    i2c_master_config_t masterConfig;

    CLOCK_SetRootMux(kCLOCK_RootI2c3, kCLOCK_I2cRootmuxSysPll1Div5); /* Set I2C source to SysPLL1 Div5 160MHZ */
    CLOCK_SetRootDivider(kCLOCK_RootI2c3, 1U, 10U);                  /* Set root clock to 160MHZ / 10 = 16MHZ */

    I2C_MasterGetDefaultConfig(&masterConfig);
    
    /*
     * masterConfig->baudRate_Bps = I2C_BAUDRATE;
     * masterConfig->enableHighDrive = false;
     * masterConfig->enableStopHold = false;
     * masterConfig->glitchFilterWidth = 0U;
     * masterConfig->enableMaster = true;
     */
    masterConfig.baudRate_Bps = I2C_BAUDRATE;

    I2C_MasterInit(I2C_BASEADDR, &masterConfig, I2C_CLK_FREQ);

    memset(&g_m_handle, 0, sizeof(g_m_handle));
    memset(&i2cRx, 0, sizeof(i2cRx));

    I2C_MasterTransferCreateHandle(I2C_BASEADDR, &g_m_handle, i2c_master_callback, NULL);
}


// *****************************************************************************
// *****************************************************************************
// Section: I2C Access Functions

int8_t DRV_CAPTOUCH_I2C_ReadByte(uint8_t reg, uint8_t *rxd)
{
    i2c_master_transfer_t masterXfer;
    int8_t error = 0;

    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = &reg;
    masterXfer.dataSize       = BYTE;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    error = I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;

    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = i2cRx;
    masterXfer.dataSize       = BYTE;

    error = I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;

    // Update data
    *rxd = i2cRx[0];

    return error;
}

int8_t DRV_CAPTOUCH_I2C_ReadHalfWord(uint8_t start_reg, uint16_t *rxd)
{
    i2c_master_transfer_t masterXfer;
    int8_t error = 0;

    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = &start_reg;
    masterXfer.dataSize       = BYTE;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    error = (int8_t)I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;

    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = i2cRx;
    masterXfer.dataSize       = HALFWORD;

    error = (int8_t)I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;

    // Update data
    *rxd = (i2cRx[0] << 8) + i2cRx[1];

    return error;
}

int8_t DRV_CAPTOUCH_I2C_ReadWord(uint8_t start_reg, uint32_t *rxd)
{
    i2c_master_transfer_t masterXfer;
    int8_t error = 0;

    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = &start_reg;
    masterXfer.dataSize       = BYTE;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    error = (int8_t)I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;

    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = i2cRx;
    masterXfer.dataSize       = WORD;

    error = (int8_t)I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;

    // Update data
    *rxd = (i2cRx[0] << 24) + (i2cRx[1] << 16) + (i2cRx[2] << 8) + i2cRx[3];

    return error;
}

int8_t DRV_CAPTOUCH_I2C_ReadArray(uint8_t start_reg, uint8_t *rxd, uint8_t len)
{
    i2c_master_transfer_t masterXfer;
    int8_t error = 0;

    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = &start_reg;
    masterXfer.dataSize       = BYTE;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    error = (int8_t)I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;

    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Read;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = i2cRx;
    masterXfer.dataSize       = len;

    error = (int8_t)I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }


    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;
    
    // Update data
    for (uint8_t i = 0; i < len; i++) {
        rxd[i] = i2cRx[i];
    }

    return error;
}

int8_t DRV_CAPTOUCH_I2C_WriteByte(uint8_t reg, uint8_t data)
{
    i2c_master_transfer_t masterXfer;
    int8_t error = 0;
    
    i2cTx[0] = reg;
    i2cTx[1] = data;
    
    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = i2cTx;
    masterXfer.dataSize       = HALFWORD;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    error = I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;
    
    return error;
}

int8_t DRV_CAPTOUCH_I2C_WriteHalfWord(uint8_t start_reg, uint16_t data)
{
    i2c_master_transfer_t masterXfer;
    int8_t error = 0;
    
    i2cTx[0] = start_reg;
    i2cTx[1] = (data & 0xFF00) >> 8;
    i2cTx[2] = data & 0x00FF;
    
    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = i2cTx;
    masterXfer.dataSize       = HALFWORD + 1;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    error = I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;
    
    return error;
}

int8_t DRV_CAPTOUCH_I2C_WriteWord(uint8_t start_reg, uint32_t data)
{
    i2c_master_transfer_t masterXfer;
    int8_t error = 0;
   
    i2cTx[0] = start_reg;
    i2cTx[1] = (data & 0xFF000000) >> 24;
    i2cTx[2] = (data & 0x00FF0000) >> 16;
    i2cTx[3] = (data & 0x0000FF00) >> 8;
    i2cTx[4] = data & 0x000000FF;
    
    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = i2cTx;
    masterXfer.dataSize       = WORD + 1;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    error = I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;
    
    return error;
}

int8_t DRV_CAPTOUCH_I2C_WriteArray(uint8_t start_reg, uint8_t *data, uint8_t len)
{
    i2c_master_transfer_t masterXfer;
    int8_t error = 0;

    if(len > I2C_BUFFER_LENGTH);
      len = I2C_BUFFER_LENGTH;
    
    i2cTx[0] = start_reg;
    for(uint8_t i = 1; i < len; i++)
      i2cTx[i] = data[i-1];
    
    
    masterXfer.slaveAddress   = I2C_SLAVE_ADDR;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = (uint32_t)NULL;
    masterXfer.subaddressSize = 0;
    masterXfer.data           = i2cTx;
    masterXfer.dataSize       = len + 1;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    error = I2C_MasterTransferNonBlocking(I2C_BASEADDR, &g_m_handle, &masterXfer);
    if(error){
        return error;
    }

    /*  Wait for transfer completed. */
    while (!g_MasterCompletionFlag){}
    g_MasterCompletionFlag = false;
    
    return error;
}


// *****************************************************************************
// *****************************************************************************
// Section: Get Functions

int8_t DRV_CAPTOUCH_I2C_GetSinglePixelPoint(POINT_OBJ* point)
{
    uint32_t data;
    int8_t error = 0;
    uint16_t x, y;

    error = DRV_CAPTOUCH_I2C_ReadWord(OP_REG_TOUCHX1H, &data);
    if(error){
        return error;
    }

    point->event_flag = (data & 0xC0000000) >> 30;
    point->id = (data & 0x0000F000) >> 12;
    x = (data & 0x0FFF0000) >> 16;
    y = data & 0x00000FFF;
    
    switch(ORIENTATION){
      case 90:
        point->x = MAX_Y_PIXEL - y;
        point->y = MAX_X_PIXEL - x;
        break;
      case 180:
        point->x = x;
        point->y = MAX_Y_PIXEL - y;
        break;
      case 270:
        point->x = y;
        point->y = x;
        break;
      default: // 0�
        point->x = MAX_X_PIXEL - x;
        point->y = y;
        break;
    }
    

    return error;
}

int8_t DRV_CAPTOUCH_I2C_GetMultiPixelPoint(POINT_OBJ* point, uint8_t n)
{
    if(n > MAX_TOUCHES)
        n = MAX_TOUCHES;
    
    uint8_t array[MAX_TOUCHES*6];
    int8_t error = 0;
    uint16_t x, y;

    error = DRV_CAPTOUCH_I2C_ReadArray(OP_REG_TOUCHX1H, array, n*6);
    if(error){
        return error;
    }

    for(uint8_t i = 0; i < n; i++){
        point[i].event_flag = (array[i*6] & 0xC0) >> 6;
        point[i].id = (array[i*6+2] & 0xF0) >> 4;
        x = ((array[i*6+0] & 0x0F) << 8) + array[i*6+1];
        y = ((array[i*6+2] & 0x0F) << 8) + array[i*6+3];
        
        switch(ORIENTATION){
          case 90:
            point[i].x = MAX_Y_PIXEL - y;
            point[i].y = MAX_X_PIXEL - x;
            break;
          case 180:
            point[i].x = x;
            point[i].y = MAX_Y_PIXEL - y;
            break;
          case 270:
            point[i].x = y;
            point[i].y = x;
            break;
          default: // 0�
            point[i].x = MAX_X_PIXEL - x;
            point[i].y = y;
            break;
        }    
    }

    return error;
}

int8_t DRV_CAPTOUCH_I2C_GetTouch(bool *touch)
{
    uint8_t res;
    int8_t error = 0;

    error = DRV_CAPTOUCH_I2C_ReadByte(OP_REG_TDSTATUS, &res);
    if(error){
        return error;
    }
   
    if(res > 0)
        *touch = true;
    else
        *touch = false;

    return error;
}

int8_t DRV_CAPTOUCH_I2C_GetNumberOfTouch(uint8_t *n)
{
    uint8_t res;
    int8_t error = 0;

    error = DRV_CAPTOUCH_I2C_ReadByte(OP_REG_TDSTATUS, &res);
    if(error){
        return error;
    }

    *n = res;

    return error;
}

int8_t DRV_CAPTOUCH_I2C_GetDeviceMode(uint8_t *rxd)
{
    uint8_t res;
    int8_t error = 0;

    error = DRV_CAPTOUCH_I2C_ReadByte(OP_REG_DEVICEMODE, &res);
    if(error){
        return error;
    }

    *rxd = res & 0x3F;

    return error;
}

int8_t DRV_CAPTOUCH_I2C_GetGestureID(uint8_t *rxd)
{
    uint8_t res;
    int8_t error = 0;

    error = DRV_CAPTOUCH_I2C_ReadByte(OP_REG_GESTID, &res);
    if(error){
        return error;
    }

    *rxd = res;

    return error;
}

int8_t DRV_CAPTOUCH_I2C_GetState(uint8_t *rxd)
{
    uint8_t res;
    int8_t error = 0;

    error = DRV_CAPTOUCH_I2C_ReadByte(OP_REG_STATE, &res);
    if(error){
        return error;
    }
    

    *rxd = res;

    return error;
}

int8_t DRV_CAPTOUCH_I2C_GetThresholdObject(THRESHOLD_OBJ *th)
{
    uint8_t array[6];
    int8_t error = 0;

    error = DRV_CAPTOUCH_I2C_ReadArray(OP_REG_THGROUP, array, 6);
    if(error){
        return error;
    }
    
    th->threshold       = array[0]*4;
    th->peak            = array[1];
    th->focus           = array[2];
    th->water           = array[3];
    th->temperature     = array[4];
    th->difference      = array[5];

    return error;
}


// *****************************************************************************
// *****************************************************************************
// Section: Set Functions

int8_t DRV_CAPTOUCH_I2C_SetDeviceMode(DEVICE_MODE mode)
{       
    int8_t error = 0;
    uint8_t md = 0x00;
    
    error = DRV_CAPTOUCH_I2C_GetDeviceMode(&md);
    if(error){
        return error;
    }
    
    md &= 0xC0;
    md |= mode;
    
    error = (int8_t) DRV_CAPTOUCH_I2C_WriteByte(OP_REG_DEVICEMODE, mode);
    
    return error;
}

int8_t DRV_CAPTOUCH_I2C_SetThresholdObject(THRESHOLD_OBJ *th)
{
    uint8_t array[6];
    int8_t error = 0;

    array[0] = th->threshold / 4;
    array[1] = th->peak;
    array[2] = th->focus;
    array[3] = th->water;
    array[4] = th->temperature;
    array[5] = th->difference;
    
    error = DRV_CAPTOUCH_I2C_WriteArray(OP_REG_THGROUP, array, 6);

    return error;
}

int8_t DRV_CAPTOUCH_I2C_SetThresholdDefault(void)
{
    uint8_t array[6];
    int8_t error = 0;
    
#ifdef FROMDATASHEET              // Default parameters defined in the Datasheet
    array[0] = 280 / 4;
    array[1] = 60;
    array[2] = 16;
    array[3] = 60;
    array[4] = 10;
    array[5] = 20;
#elsif
    array[0] = 240 / 4;
    array[1] = 50;
    array[2] = 17;
    array[3] = 17;
    array[4] = 17;
    array[5] = 160;
#endif
    
    error = DRV_CAPTOUCH_I2C_WriteArray(OP_REG_THGROUP, array, 6);

    return error;
}






