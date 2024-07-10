#include <rtthread.h>
#include <rtdevice.h>


extern rt_uint32_t value, vol;
extern rt_bool_t light_flag;
extern rt_bool_t my8266_flag_finish;
#define taideng_PIN 32


static void Light_thread_entry(void *parameter)
{

    rt_pin_mode(taideng_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(taideng_PIN, PIN_LOW);
 if(light_flag)
 {
    while(1)
    {
        if(my8266_flag_finish)
        {
        if(vol>50)
        {
            rt_kprintf("the value is :%d \n", 1);
            rt_pin_write(taideng_PIN, PIN_HIGH);
        }
        if(vol<50)
        {
            rt_kprintf("the value is :%d \n", 0);
            rt_pin_write(taideng_PIN, PIN_LOW);
        }


    }
        rt_thread_mdelay(10);
    }
 }

}


static int  Light_init(void)
{
    rt_thread_t Light_thread;

    Light_thread = rt_thread_create("Light_thread",
                                      Light_thread_entry,
                                      "Light_thread",
                                      1024,
                                      25,
                                      20);
    if (Light_thread != RT_NULL)
    {
        rt_thread_startup(Light_thread);
    }

    return RT_EOK;
}
INIT_APP_EXPORT(Light_init);
