/*
 *
 * 学习定时线程，只要当接收标志，才能执行。
 * 主要进行学习时间倒计时，时间到后，就会驱动蜂鸣器鸣叫，提示可以停止学习了
 * 此时可以通过按下串口屏发送复位闹钟操作。
 */
#include <rtthread.h>
#include <rtdevice.h>
#define fengming_PIN 33

extern rt_bool_t dingshi_flag,beep_flag;
extern int daojishi ;
extern struct rt_semaphore rx_sem2;
extern int dingshi_time_min ;//0-100整数，单位：分钟
extern int dingshi_time_sec ;//0-60整数，单位：秒
extern int dingshi_mode ;//1为分钟，2为秒钟
extern rt_bool_t waiting ;
extern rt_bool_t miaosuo,fensuo;
extern rt_bool_t my8266_flag_finish;
extern rt_bool_t sec_suo;
extern rt_bool_t min_suo;

static void dingshi_thread_entry(void *parameter)
{
    rt_pin_mode(fengming_PIN, PIN_MODE_OUTPUT);

    while(1)
    {
      if(my8266_flag_finish)
      {
       if(dingshi_flag)
       {
           switch(dingshi_mode)
           {
               case 1://分钟
               {
                   if(!fensuo)
                   {
//                     daojishi = dingshi_time_min*60*1000;
//                   for(int i =0;i<daojishi;i++)
//                           {
//                               rt_thread_mdelay(1);//1ms
//                           }
                           daojishi = dingshi_time_min*60*1000;
                           rt_thread_mdelay(daojishi);
                           beep_flag = 1;//定时结束，蜂鸣器置位，需要手动清零标志位,这个标志位的权限和超声波权限一样大用于决定其他外设是否使用
                           fensuo =1;
                   }break;
               }
               case 2://秒钟
               {
                   if(!miaosuo);
                   {daojishi =dingshi_time_sec*1000;
                   for(int i =0;i<daojishi;i++)
                           {
                                 rt_thread_mdelay(1);//1ms
                           }
                            beep_flag = 1;//定时结束，蜂鸣器置位，需要手动清零标志位,这个标志位的权限和超声波权限一样大用于决定其他外设是否使用
                            miaosuo =1;
                   }break;
               }

               default: break;

           }
           while(beep_flag)
           {
               rt_kprintf("time over!\n");
               //开启蜂鸣器 PC1引脚
               rt_pin_write(fengming_PIN, PIN_LOW);
               if(!beep_flag)
               {
                rt_kprintf("111!\n");
                rt_pin_write(fengming_PIN, PIN_HIGH);
                sec_suo=0;
                min_suo=0;
                //关闭蜂鸣器
               }
           }

           dingshi_flag = 0;//定完一次时，必须要先关闭蜂鸣器，才能再进行下一次定时
                            //否则会一直响。同时离开前清除标志位，进入信号量阻塞，等待下一次计时。
           waiting =0;
       }
        while(!waiting);//等待新的定时信息
        //确定定时模式：1.分钟 2.秒钟
        dingshi_flag = 1;

    }
      rt_thread_mdelay(1);
    }
}




static int dingshi_init(void)
{
    rt_thread_t dingshi_thread;

    dingshi_thread = rt_thread_create("dingshi_thread",
                                      dingshi_thread_entry,
                                      "dingshi_thread",
                                      1024,
                                      25,
                                      20);
    if (dingshi_thread != RT_NULL)
    {
        rt_thread_startup(dingshi_thread);
    }

    return RT_EOK;
}
INIT_APP_EXPORT(dingshi_init);

