/* 
 * MIT License
 *
 * Copyright (c) 2022 CoretronicMEMS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/


#include "AcousticNode.h"
#include "global.h"
#include "mbed.h"
#include "drivers/BufferedSerial.h"
BufferedSerial device(MIKOR_TX, MIKOR_RX, 9600);
BufferedSerial device_1(CONSOLE_TX, CONSOLE_RX, 115200);

namespace CMC {

AcousticNode::AcousticNode(PinName TX, PinName RX, int odr):
        m_ODR(odr)
{
    
}

AcousticNode::~AcousticNode()
{

}

int32_t AcousticNode::Initialize()
{
    SetODR(m_ODR);
    SetGain(1);//**********************
    DBG_MSG("%s initialized\n", Name());
    return 0;
}

int32_t AcousticNode::Uninitialize()
{
    return 0;
}

int32_t AcousticNode::Write(const void *data, size_t num)
{
    return 0;
}

int32_t AcousticNode::Read(void *data, size_t num)
{
    //uint16_t buffer;
    //////char str[6];
    //////char buffer[6];
    if(device.readable())
    {   
        return ReadData((float *)data, num);
        /*
        device.read(&buffer, sizeof(buffer));
        if(buffer[sizeof(buffer)-1] == '\n')
        {  
            //buffer[sizeof(buffer)-1] = '\n\r';
            //device_1.write(&buffer, sizeof(buffer));
            //printf("%d", *buffer); 


            // buffer[sizeof(buffer)-1] = '0';
            // device_1.write(&buffer, sizeof(buffer));

            
            char str[6];
            buffer[sizeof(buffer)-1] = '0';
            strcpy(str, buffer);
            device_1.write(str, sizeof(str));
            memcpy(data, str, sizeof(str));
        }
        */
        ////// device.read(&buffer, sizeof(buffer));
        ////// if(buffer[sizeof(buffer)-1] == '\n')
        ////// {
        //////     buffer[sizeof(buffer)-1] = '0';
        //////     strcpy(str, buffer);
            

            //////int value = atoi(str);

            //char test = value+'0';
            ////device_1.write(&str  , sizeof(str));

            //////value -= 0x7FFF;
            //////value = value*100;
            //////((int16_t*)data)[0] = value;

            // float f = atof(str);
            // char temp[1];
            // sprintf(temp, "%f", f);
            ////memcpy(data, value, sizeof(value));
            //((float *)data)[0] =  f*100; 
        //////}
    }
    return 0;
    /* 
    int32_t len = 6;
    return  len;
    */


    // uint16_t buf;
    // ReadData(&buf, 2);

    // int temp = buf;
    // temp -= 0x7FFF;
    // ((int16_t*)data)[0] = temp;

    // return 2;
}

int32_t AcousticNode::Control(uint32_t control, uint32_t arg)
{
    if(control == SENSOR_CTRL_START)
    {
        m_timer.attach(callback(this, &AcousticNode::TimerCallback), std::chrono::microseconds((int)(1000000/m_ODR)));
        m_isOn = true;
    }
    else if(control == SENSOR_CTRL_STOP)
    {
        m_timer.detach();
        m_isOn = false;
    }
    else if(control == SENSOR_CTRL_SET_ODR)
    {
        if(m_isOn)
            m_timer.detach();
        int32_t odr = SetODR(arg);
        if(m_isOn)
            m_timer.attach(callback(this, &AcousticNode::TimerCallback), std::chrono::microseconds((int)(1000000/m_ODR)));
        return odr;
    }
    else if(control == SENSOR_CTRL_SELFTEST)
    {
        return SelftTest();
    }
    else if(control == SENSOR_CTRL_GET_ODR)
    {
        *((uint32_t*)arg) = m_ODR;
    }
    else if(control == SENSOR_CTRL_SET_GAIN)
    {
        //return 0;
        return SetGain(arg);
    }

    return 0;
}

int32_t AcousticNode::ReadData(float *data, uint32_t num)
{  
    char str[6];
    char buffer[6];
    device.read(&buffer, sizeof(buffer));
    if(buffer[sizeof(buffer)-1] == '\n')
    {
        buffer[sizeof(buffer)-1] = '0';
        strcpy(str, buffer);
        data[0] = atof(str);
    }
    // if(num < 2)
    //     return 0;
    // data[0] = AUDIO_DATA.read_u16();
    return 1;
}

int32_t AcousticNode::SetODR(uint32_t arg)
{
    m_ODR = arg;
    return m_ODR; 
}

int32_t AcousticNode::SetGain(uint32_t arg)
{
    m_gain = arg;
    return m_gain; 
}

int32_t AcousticNode::SelftTest()
{
    // uint16_t adc = AUDIO_DATA.read_u16();

    // if(adc > 0x7F00 && adc < 0x8100)
    //     printf("SelftTest OK, %u\n", adc);
    // else
    //     printf("SelftTest NG, %u\n", adc);

    // return 1; 


    return 0;
}

void AcousticNode::TimerCallback()
{
    SetDataReady();
}

}; //namespace CMC