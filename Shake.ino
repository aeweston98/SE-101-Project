#include <Wire.h>

void WireRequestArray(int address, uint8_t* buffer, uint8_t amount);
void WireWriteRegister(int address, uint8_t reg, uint8_t value);
void WireWriteByte(int address, uint8_t value);

static float const    SensorMaximumReading= 512.0;
static float const    SensorMaximumAccel  = 9.81 * 4.0;
static uint8_t const  SensorAccelerometer = 0x1D;

int direction = 2;

void tiltInit()
{
    WireWriteRegister(SensorAccelerometer, 0x31, 1);
    WireWriteRegister(SensorAccelerometer, 0x2D, 1 << 3);
}

/* No input
 * Gets the acceleration values in the x and y directions
 * Since our game is only 2 dimensional (you can't move in the z direction we can ignore that value)
 * Gets the accelerometer values, and takes the largest magnitude out of x and y and returns a character
 * which represents the dominant acceleration direction
 * There is a threshold, so if the player is holding the board relatively flat, the ball will move
 * The delay at the end of the function controls how fast the ball can move
 */
 
char tiltTick()
{
    size_t const DataLength = 4;
    uint32_t data[DataLength] = { 0 };
  
    WireWriteByte(SensorAccelerometer, 0x32);
    WireRequestArray(SensorAccelerometer, data, DataLength);

    uint16_t xi = (data[1] << 8) | data[0];
    uint16_t yi = (data[3] << 8) | data[2];
    float x = *(int16_t*)(&xi) / SensorMaximumReading * SensorMaximumAccel;
    float y = *(int16_t*)(&yi) / SensorMaximumReading * SensorMaximumAccel;

    char accl_direction;
    float x_absolute = fabs(x);
    float y_absolute = fabs(y);

    if(x_absolute > 3.0 && y_absolute > 3.0)
    {
       if(direction == 0)
       {
           if(x<0)
           {
               accl_direction = 'R';
           }
           else if(x>0)
           {
               accl_direction = 'L';
           }
       }

       if(direction == 1)
       {
           if(y<0)
           {
               accl_direction = 'D';
           }
           else if(y>0)
           {
               accl_direction = 'U'; 
           }           
       }
    }
  
    else if(x_absolute > 3.0 || y_absolute > 3.0)
    {
        if(x_absolute > y_absolute && x < 0)
        {
             accl_direction = 'R';
             direction = 0;
        }
        else if(x_absolute > y_absolute && x > 0)
        {
            accl_direction = 'L';
            direction = 0;
        }
        else if(x_absolute < y_absolute && y > 0)
        {
            accl_direction = 'U';
            direction = 1;
        }
        else
        {
            accl_direction = 'D';
            direction = 1;
        }
    }
    else
    {
        accl_direction = 'N'; 
    }
    delay(20);

    return accl_direction;
}