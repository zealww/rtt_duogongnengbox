/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-06-28     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#define fengming_PIN 33
#define USART_REC_LEN 200
extern rt_uint16_t USART_RX_STA;
extern rt_uint8_t USART_RX_BUF[USART_REC_LEN];
extern rt_device_t serial;
/*
 *
 * 用main这个线程去管理各个线程的标志位，注意main主线程占有最高优先级，也即用来管理各个线程的状态变量
 * 保证通过实时修改线程的优先级来修改线程的执行状态。
 */
rt_bool_t zuo_flag=0,you_flag=0,yang_flag=0,di_flag=0;
rt_bool_t light_flag = 1;       //光感学习灯启动功能√
rt_bool_t chaoshengbo_flag = 0; //超声波距离启动功能×
rt_bool_t chuankouping_flag = 0;//串口屏指令功能×
rt_bool_t duojifenjian_flag = 0;//搭配openmv的串口使用×
rt_bool_t my8266_flag_finish = 0;      //搭配8266串口使用 串口6√ 每个线程前都要有，为了先配网在执行其他线程
rt_bool_t fan_speed_flag =0;        //状态可切换风扇 ×
rt_bool_t fan_mode_shuimian = 0;
rt_bool_t fan_mode_ziran = 0;
rt_bool_t fansuo = 1;
rt_bool_t fan_tiaosu=0;
rt_bool_t fan_shuimian=0;
rt_bool_t fan_ziran=0;
rt_bool_t sec_dingshi=0;
rt_bool_t min_dingshi=0;
rt_bool_t sec_suo;
rt_bool_t min_suo;
int dingshi_time_min = 10;//0-100整数，单位：分钟
int dingshi_time_sec = 1000;//0-60整数，单位：秒
rt_bool_t dingshi_flag = 0;     //学习定时功能√
int dingshi_mode =0;
int daojishi = 100000;//定时的时间
rt_bool_t waiting =0;
rt_bool_t beep_flag = 0;
int index1 = -1;
rt_bool_t miaosuo=0,fensuo=0;
/*
 * 外设使用情况：
 * 1.串口3个 ：串口屏，openmv，8266
 * 2.pwm：舵机1个用于分拣，风扇（支持1.正常2.睡眠3.停止）
 * 3.Timer：超声波测距
 * 4.ADC：光敏电阻
 */

/*
 * 最后：
 * 1.
 */
int main(void)
{
    rt_pin_mode(fengming_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(fengming_PIN, PIN_HIGH);
//    char astr10[] = "AT+MQTTPUB=0,\"/k0lr3oY16Qc/esp8266/user/update\",\"2\",1,0\r\n";

    while(1)
    {
        if(my8266_flag_finish)
        {
//        rt_device_write(serial, 0, astr10, (sizeof(astr10) - 1));
//        rt_thread_mdelay(3000);
        //*************************************
        //*********下面是用来根据接收到8266的信息来决定标志位的改变
       //*************************************
        if(USART_RX_STA&0x8000)
                {
                    if(rt_strstr((const char*)USART_RX_BUF,"di"))//云台左转
                    {
                        zuo_flag =1;
                    }
                    if(rt_strstr((const char*)USART_RX_BUF,"yang"))//云台右转
                    {
                         you_flag =1;
                    }
                    if(rt_strstr((const char*)USART_RX_BUF,"you"))//云台抬头
                    {
                         yang_flag =1;
                    }
                    if(rt_strstr((const char*)USART_RX_BUF,"zuo"))//云台低头
                    {
                         di_flag =1;
                    }
                    rt_memset(USART_RX_BUF,0,200);
                    USART_RX_STA=0;
                }
        //*************************************
        //*************************************
        //*************************************



        }
        rt_thread_mdelay(10);
    }


    return RT_EOK;
}
