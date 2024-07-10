#include <rtthread.h>
#include <rtdevice.h>
#include <rtdbg.h>
#include <math.h>

#define PWM_DEV_NAME        "pwm12"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     2      /* PWM通道 */

struct rt_device_pwm *pwm_dev; /* PWM设 备 句 柄 */
extern struct rt_device_pwm *pwm_dev_C;
extern rt_bool_t my8266_flag_finish ;
extern rt_bool_t zuo_flag,you_flag,yang_flag,di_flag;
int count1 =0;
int count2 =0;



static void pwm_duoji_entry(void *parameter)
{
    rt_uint32_t period, pulse1,pulse2;

    period = 20000000; /* 周 期 为20ms， 单 位 为 纳 秒ns,频率50HZ */
//    period = 1000000;
    //pulse = (period*SERVO_INIT)/10000; /* PWM脉 冲 宽 度 值， 单 位 为 纳秒ns */

   /*                                                       pules =1*500000 -90°
                                                            pulse =2*500000 -45°
                                                            pulse =3.3*500000   0°
                                                            pulse =3.7*500000   30°
                                                            pulse =4*500000  45°
                                                            pulse =5*500000  90°*/


    /* 查 找 设 备 */
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }
    rt_kprintf("pwm sample run ! find %s device!\n", PWM_DEV_NAME);
    /* 设 置PWM周 期 和 脉 冲 宽 度 默 认 值 */
   // rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period, pulse);
    /* 使 能 设 备 */
    pulse1 = 3*500000;
    pulse2 = 3*500000;
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
    rt_pwm_enable(pwm_dev_C, 1);
   // rt_pin_mode(key_PIN_NUM, PIN_MODE_INPUT_PULLUP);
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period,pulse1);
    rt_pwm_set(pwm_dev_C, 1, period,pulse2);
    while(1)
    {
        if(my8266_flag_finish)
        {

            if(zuo_flag)
            {

//                for(int i =0;i<10;i++)
//                {
//                    rt_thread_mdelay(100);
//                    if(pulse1<5*500000)
//                    {pulse1 += 2*50000;}
//                    else
//                    {
//                        pulse1 = pulse1;
//                    }
//                    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period,pulse1);
//                }
                pulse1 = 3.5*500000;
                rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period,pulse1);
                zuo_flag = 0;
            }
            if(you_flag)
            {
//                for(int i =0;i<10;i++)
//                {
//                    rt_thread_mdelay(100);
//                    if(pulse1>1*500000)
//                    {pulse1 -= 2*50000;}
//                    else
//                    {
//                        pulse1 = pulse1;
//                    }
//                    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period,pulse1);
//                }
                pulse1 = 1*500000;
                rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, period,pulse1);
                you_flag = 0;
             }
            if(yang_flag)
            {
//                for(int i =0;i<10;i++)
//                {
//                    rt_thread_mdelay(100);
//                    if(pulse2>1*500000)
//                    {pulse2 -= 2*50000;}
//                    else
//                    {
//                        pulse2 = pulse2;
//                    }
//                    rt_pwm_set(pwm_dev_C, 1, period,pulse2);
//                }
                 pulse2 = 1.5*500000;
                 rt_pwm_set(pwm_dev_C, 1, period,pulse2);
                 yang_flag = 0;
            }
            if(di_flag)
            {
//                  for(int i =0;i<10;i++)
//                  {
//                      rt_thread_mdelay(100);
//                      if(pulse2<5*500000)
//                      {pulse2 += 2*50000;}
//                      else
//                      {
//                          pulse2 = pulse2;
//                      }
//                      rt_pwm_set(pwm_dev_C, 1, period,pulse2);
//                   }
                  pulse2 = 5*500000;
                  rt_pwm_set(pwm_dev_C, 1, period,pulse2);
                  di_flag = 0;
              }


        }
        rt_thread_mdelay(10);
    }
}
static int pwm_duoji_init(void)
{
    static rt_thread_t tid_pwmduoji = RT_NULL;//定义了一个线程控制块。通过这个线程控制块

    rt_err_t ret = RT_EOK;

/* 创建 serial 线程 */
    tid_pwmduoji = rt_thread_create("t_pwmduoji",pwm_duoji_entry,RT_NULL, 512, 25, 10);
   /* 创建成功则启动线程 */
   if (tid_pwmduoji != RT_NULL)
   {
       rt_thread_startup(tid_pwmduoji);
   }
   else
   {
       ret = RT_ERROR;
   }

   return ret;

}
INIT_APP_EXPORT(pwm_duoji_init);//自动初始化，可以自动调用线程的初始化函数
