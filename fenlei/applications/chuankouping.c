#include <rtthread.h>
#include <rtdevice.h>
#define SAMPLE_UART_NAME       "uart4"      /* 串口设备名称 */

/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem1;
/* 用于定时的信号量 */
//**风扇变量定义
struct rt_semaphore rx_sem2;
static rt_device_t serial;
extern rt_bool_t fan_speed_flag;
extern rt_bool_t fan_mode_shuimian;
extern rt_bool_t fan_mode_ziran;
extern rt_bool_t fansuo;
extern rt_bool_t fan_tiaosu;
extern rt_bool_t fan_shuimian;
extern rt_bool_t fan_ziran;
int fan_speed = 0;
//*****定时变量定义
extern rt_bool_t dingshi_flag;
extern int dingshi_time_min ;//0-100整数，单位：分钟
extern int dingshi_time_sec ;//0-60整数，单位：秒
extern int dingshi_mode ;
extern rt_bool_t waiting ;
extern int index1;
extern rt_bool_t beep_flag;
extern rt_bool_t miaosuo,fensuo;
extern rt_bool_t sec_dingshi;
extern rt_bool_t min_dingshi;
extern rt_bool_t sec_suo;
extern rt_bool_t min_suo;
//********8266
extern rt_bool_t my8266_flag_finish;

rt_bool_t state = 1;

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem1);

    return RT_EOK;
}



static void chuankouping_thread_entry(void *parameter)
{

 static rt_uint8_t last_ch;
    rt_uint8_t ch;


    while (1)
    {
        if(my8266_flag_finish)
        {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem1, RT_WAITING_FOREVER);
        }



/****/ if(state)//用于页面内容和功能确定
        {


        if(ch==0xfe)//风扇调速模式，但在系统第一次启动时，需要先打开睡眠模式，解除风扇锁
        {
            fan_tiaosu = 1;
            state = 0;

        }
        if(ch==0xfd)//风扇睡眠模式  按下开启
        {

            fan_shuimian =1;
            state = 0;

        }
        if(ch==0xfc)//风扇自然模式  按下开启
        {

             fan_ziran =1;
             state = 0;

        }
        if(ch==0xfb)//按秒定时模式
        {
            sec_dingshi =1;
            state =0;

        }
        if(ch==0xfa)//按分定时模式
        {
            min_dingshi =1;
            state =0;

        }
        if(ch==0xee)//按关闭按键
        {
            beep_flag =0;


        }

        }

/****/ if(!state)//页面确定，进行具体页面的具体功能确定
        {
            if(fan_tiaosu)
            {
                if(ch<150)
                {
                    fan_speed = ch;
                }

                if(ch == 0xf9)//按下返回
                {
                    state = 1;
                    fan_tiaosu=0;
                }

            }
            if(fan_shuimian)
            {
                fan_mode_shuimian =1;
                if(ch == 0xf8)//按下关闭
                {
                fan_mode_shuimian =0;
                fansuo =0;
                }
                if(ch == 0xf7)//按下返回
                {
                    fan_mode_shuimian =0;
                    state =1;
                    fan_shuimian=0;
                }
            }
            if(fan_ziran)
            {
                fan_mode_ziran =1;
                if(ch == 0xf6)//按下关闭
                {
                fan_mode_ziran =0;
                }
                if(ch == 0xf5)//按下返回
                {
                    state =1;
                    fan_ziran=0;
                    fan_mode_ziran =0;
                }
            }
            if(sec_dingshi)
            {
                dingshi_mode=2;
                if((ch<0xcc) && !sec_suo)//按下置数
                {
                dingshi_time_sec =ch;
                sec_suo =1;
                }
                if(ch == 0xf4)//按下开始计时
                {
                      waiting = 1;
                }
                if(ch == 0xf3)//按下返回
                {
                    state =1;
                    sec_dingshi=0;

                }
            }

            if(min_dingshi)
            {
                dingshi_mode=1;
                if((ch<0xcc) && !min_suo)//按下置数
                {
                    dingshi_time_min =ch;
                    min_suo =1;
                }
                if(ch == 0xf2)//按下开始计时
                {
                      waiting = 1;
                }
                if(ch == 0xf1)//按下返回
                {
                    state =1;
                    min_dingshi=0;
                }
            }
        }

//        if(ch == 0xFF)
//        {
//            fan_speed_flag =1;
//            dingshi_mode =0;
//        }
//        if(ch == 0xFE)
//        {
//            fan_speed_flag =0;
//            dingshi_mode=0;
//        }
//        if(fan_speed_flag)
//        {
//             fan_speed = ch;
//             dingshi_mode=0;
//        }
//        if(ch ==0xFD)
//        {
//            fan_mode_shuimian =1;
//            dingshi_mode=0;
//        }
//        if(ch ==0xFC)
//        {
//            fansuo=0;
//            fan_mode_shuimian =0;
//            dingshi_mode=0;
//        }
//        if(ch ==0xFB)
//        {
//            fan_mode_ziran =1;
//            dingshi_mode=0;
//        }
//        if(ch ==0xFA)
//        {
//            fan_mode_ziran =0;
//            dingshi_mode=0;
//        }
//        if(ch ==0xF9)//确定定时模式为分钟
//        {
//             dingshi_mode =1;//1为分钟
//
//        }
//        if(ch ==0xF8)//确定定时模式为秒
//        {
//            dingshi_mode =2;//2为秒钟
//
//
//        }
//
//        if(ch ==0xF6)//确定开始定时
//        {
//          if(!miaosuo||!fensuo)  waiting = 1;
//
//
//        }
//
//        if(dingshi_mode ==1)//确定定时时间
//        {
//
//            if(ch!=last_ch) index1++;
//
//                       last_ch = ch;
//                       if(index1)
//                           {
//                           if((int)ch>60)
//                           {
//                               ch = 60;
//                               index1 = -1;
//                           }
//                           dingshi_time_min =ch;
//                           fensuo = 0;
//                           index1 =-1;
//                           }
//
//        }
//        if(dingshi_mode ==2)//确定定时时间
//        {
//            if(ch!=last_ch) index1++;
//
//            last_ch = ch;
//            if(index1)
//                {
//                if((int)ch>60)
//                 {
//                    ch = 60;
//                    index1 = -1;
//                 }
//                dingshi_time_sec =ch;
//                miaosuo = 0;
//                index1 =-1;
//                }
//
//
//        }
//
//        if(ch ==0xF5)//关闭定时
//                {
//            beep_flag=0;
//            dingshi_mode=0;
//                }
//
//        rt_device_write(serial, 0, &ch, 1);
//
//        }
//        rt_thread_mdelay(10);
    }
        rt_thread_mdelay(10);
 }
}
static int chuankouping_sample(void)
{
    rt_err_t ret = RT_EOK;

   // char str[] = "hello RT-Thread!\r\n";



    /* 查找串口设备 */
    serial = rt_device_find(SAMPLE_UART_NAME);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", SAMPLE_UART_NAME);
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&rx_sem2, "rx_sem2", 0, RT_IPC_FLAG_FIFO);//定时使用
    rt_sem_init(&rx_sem1, "rx_sem1", 0, RT_IPC_FLAG_FIFO);//串口接收
    /* 以读写及中断接收方式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);

    /* 发送字符串 */
  //  rt_device_write(serial, 0, str, (sizeof(str) - 1));

    /* 创建 serial 线程 */
    rt_thread_t thread1 = rt_thread_create("serial4", chuankouping_thread_entry, RT_NULL, 1024, 2, 10);
    /* 创建成功则启动线程 */
    if (thread1 != RT_NULL)
    {
        rt_thread_startup(thread1);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}

INIT_APP_EXPORT(chuankouping_sample);//自动初始化，可以自动调用线程的初始化函数

