#include <rtthread.h>
#include <rtdevice.h>


extern rt_bool_t my8266_flag_finish;
#define key_PIN 45
rt_bool_t key=0;
extern rt_device_t serial;
static void qingqiu_thread_entry(void *parameter)
{

    rt_pin_mode(key_PIN, PIN_MODE_INPUT_PULLDOWN);
    char str13[] = "AT+MQTTPUB=0,\"/k0lr3oY16Qc/esp8266/user/update\",\"2\",1,0\r\n";
    while(1)
    {
        if(my8266_flag_finish)
        {
            if(rt_pin_read(key_PIN) == PIN_HIGH)
                            {
                                rt_thread_mdelay(10);
                                while(rt_pin_read(key_PIN) == PIN_HIGH){}
                                rt_thread_mdelay(10);
                                key = 1;
                            }
            if(1)
            {
                rt_device_write(serial, 0, str13, (sizeof(str13) - 1));
                rt_thread_mdelay(3000);
            }


    }
        rt_thread_mdelay(10);
    }


}


static int  qingqiu_init(void)
{
    rt_thread_t qingqiu_thread;

    qingqiu_thread = rt_thread_create("qingqiu_thread",
                                      qingqiu_thread_entry,
                                      "qingqiu_thread",
                                      1024,
                                      25,
                                      20);
    if (qingqiu_thread != RT_NULL)
    {
        rt_thread_startup(qingqiu_thread);
    }

    return RT_EOK;
}
INIT_APP_EXPORT(qingqiu_init);
