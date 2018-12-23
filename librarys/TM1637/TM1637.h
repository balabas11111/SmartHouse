//  Author:Frankie.Chu
//  Date:9 April,2012
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  Modified record:
//
/*******************************************************************************/

#ifndef TM1637_h
#define TM1637_h
#include <inttypes.h>
#include <Arduino.h>
//************definitions for TM1637*********************
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44

#define STARTADDR  0xc0 
/**** definitions for the clock point of the digit tube *******/
#define POINT_ON   1
#define POINT_OFF  0
/**************definitions for brightness***********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7

#define SYMBOL_A 10
#define SYMBOL_b 11
#define SYMBOL_C 12
#define SYMBOL_d 13
#define SYMBOL_E 14
#define SYMBOL_F 15
#define SYMBOL_G -70
#define SYMBOL_h 20
#define SYMBOL_H -50
#define SYMBOL_L -12
#define SYMBOL_n 21
#define SYMBOL_o 0
#define SYMBOL_P 46
#define SYMBOL_r 19
#define SYMBOL_S 5
#define SYMBOL_t 22

#define SYMBOL_u -120
#define SYMBOL_y -54

#define SYMBOL_SPACE  17
#define SYMBOL_MINUS  16
#define SYMBOL_DEGREE 18

class TM1637
{
  public:
    uint8_t Cmd_SetData;
    uint8_t Cmd_SetAddr;
    uint8_t Cmd_DispCtrl;
    boolean _PointFlag;     //_PointFlag=1:the clock point on
    TM1637(uint8_t, uint8_t);
    void init(void);        //To clear the display
    void writeByte(int8_t wr_data);//write 8bit data to tm1637
    void start(void);//send start bits
    void stop(void); //send stop bits
    void display(int8_t DispData[]);
    void display(uint8_t BitAddr,int8_t DispData);
    void clearDisplay(void);
    void set(uint8_t = BRIGHT_TYPICAL,uint8_t = 0x40,uint8_t = 0xc0);//To take effect the next time it displays.
    void point(boolean PointFlag);//whether to light the clock point ":".To take effect the next time it displays.
    void coding(int8_t DispData[]); 
    int8_t coding(int8_t DispData); 
  private:
    uint8_t Clkpin;
    uint8_t Datapin;
};

/*#define SYMBOL_EMPTY 0x7f

#define SYMBOL_A 10
#define SYMBOL_b 11
#define SYMBOL_C 12
#define SYMBOL_d 13
#define SYMBOL_E 14
#define SYMBOL_F 15
//17
#define SYMBOL_G -70
#define SYMBOL_h 47
#define SYMBOL_H -50
#define SYMBOL_L -12
#define SYMBOL_n -116

#define SYMBOL_P 46
#define SYMBOL_y -54

#define SYMBOL_DEGREE 60


#define SYMBOL_MINUS 99*/
#endif
