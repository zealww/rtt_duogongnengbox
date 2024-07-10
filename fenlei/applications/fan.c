/*
 * 此处是风扇线程，这个线程用来管理风扇的状态。
 *电机pwm波形正常，只是需要接一个电机驱动芯片比如TB6612
 *风扇用的是pwm9，ch1
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <rtdbg.h>



#define PWM_DEV_NAME_C        "pwm1"  /* PWM设备名称 */
#define PWM_DEV_NAME_B        "pwm9"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL_C1     1      /* PWM通道 */
#define PWM_DEV_CHANNEL_C2     2     /* PWM通道 */
#define PWM_DEV_CHANNEL_B1     1      /* PWM通道 */
#define PWM_DEV_CHANNEL_B2     2      /* PWM通道 */

struct rt_device_pwm *pwm_dev_C; /* PWM设 备 句 柄 */
struct rt_device_pwm *pwm_dev_B; /* PWM设 备 句 柄 */
extern rt_bool_t fan_speed_flag;
extern rt_bool_t fan_mode_shuimian;
extern rt_bool_t fan_mode_ziran;
extern int fan_speed;
extern rt_bool_t fansuo;
extern rt_bool_t my8266_flag_finish;



static void pwm_dianji_entry(void *parameter)
{
    rt_uint32_t period1;
    rt_uint32_t pulse1;
    period1 = 1000000; /* 周 期 为1ms， 单 位 为 纳 秒ns,频率1Khz */

     /* PWM脉 冲 宽 度 值， 单 位 为 纳秒ns */

    /*                                 */

    /* 查 找 设 备 */
    pwm_dev_C = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME_C);

    pwm_dev_B = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME_B);



    /* 设 置PWM周 期 和 脉 冲 宽 度 默 认 值 */
//    rt_pwm_set(pwm_dev_C, PWM_DEV_CHANNEL_C1, period, 0);
//    rt_pwm_set(pwm_dev_C, PWM_DEV_CHANNEL_C2, period, 0);
      rt_pwm_set(pwm_dev_B, PWM_DEV_CHANNEL_B1, period1, 0);
//    rt_pwm_set(pwm_dev_B, PWM_DEV_CHANNEL_B2, period, 0);
    /* 使 能 设 备 */
//    rt_pwm_enable(pwm_dev_C, PWM_DEV_CHANNEL_C1);
//    rt_pwm_enable(pwm_dev_C, PWM_DEV_CHANNEL_C2);
    rt_pwm_enable(pwm_dev_B, PWM_DEV_CHANNEL_B1);
//    rt_pwm_enable(pwm_dev_B, PWM_DEV_CHANNEL_B2);
   // rt_pin_mode(key_PIN_NUM, PIN_MODE_INPUT_PULLUP);

    while(1)
    {
        if(my8266_flag_finish)
        {
        if((!fan_mode_shuimian) && (!fan_mode_ziran))
        {
            if(fansuo)
            {
                pulse1 =0;
                rt_pwm_set(pwm_dev_B, PWM_DEV_CHANNEL_B1, period1, pulse1);
            }
            else
            {
            pulse1 =10000*fan_speed;
           rt_pwm_set(pwm_dev_B, PWM_DEV_CHANNEL_B1, period1, pulse1);
            }
        }
        if(fan_mode_ziran)//自然风
        {
            pulse1 =400000;
            rt_pwm_set(pwm_dev_B, PWM_DEV_CHANNEL_B1, period1, pulse1);
            rt_thread_mdelay(500);
            pulse1 =800000;
            rt_pwm_set(pwm_dev_B, PWM_DEV_CHANNEL_B1, period1, pulse1);
            rt_thread_mdelay(500);
        }
        if(fan_mode_shuimian)//睡眠风
        {

            for(int i =0;i<10;i++)
            {
                rt_thread_mdelay(100);
                pulse1 =i*100000;
                rt_pwm_set(pwm_dev_B, PWM_DEV_CHANNEL_B1, period1, pulse1);
            }
            rt_thread_mdelay(3000);

            for(int i =100;i>=0;i--)
                        {
                            rt_thread_mdelay(100);
                            pulse1 =i*10000;
                            rt_pwm_set(pwm_dev_B, PWM_DEV_CHANNEL_B1, period1, pulse1);
                            if(!fan_mode_shuimian) break;
                        }
//            rt_thread_mdelay(3000);
        }

        }
        rt_thread_mdelay(1);//让出cpu使用权的操作，当入口函数内部形式为while（1）时，必须使用否则，会卡死在这个线程，导致整个系统瘫痪！
    }
}

static int pwm_dianji_init(void)

{
    static rt_thread_t tid_dianji = RT_NULL;//定义了一个线程控制块。通过这个线程控制块

    rt_err_t ret = RT_EOK;

/* 创建 serial 线程 */
    tid_dianji = rt_thread_create("t_dianji",pwm_dianji_entry,RT_NULL, 512, 25, 10);
   /* 创建成功则启动线程 */
   if (tid_dianji != RT_NULL)
   {
       rt_thread_startup(tid_dianji);
   }
   else
   {
       ret = RT_ERROR;
   }

   return ret;

}
INIT_APP_EXPORT(pwm_dianji_init);//自动初始化，可以自动调用线程的初始化函数
