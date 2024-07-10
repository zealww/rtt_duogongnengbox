#include <rtthread.h>
#include <rtdevice.h>
#define SAMPLE_UART_NAME       "uart6"      /* 串口设备名称 */
#define USART_REC_LEN 200
/* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
rt_device_t serial;

extern rt_bool_t my8266_flag_finish ;
rt_uint16_t USART_RX_STA=0;
rt_uint8_t USART_RX_BUF[USART_REC_LEN];
/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{


    rt_uint8_t ch;
    char str1[] = "AT+RST\r\n";
    char str2[] = "AT+RESTORE\r\n";
    char str3[] = "AT+CWMODE=1\r\n";
    char str4[] = "AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n";
    char str5[] = "AT+CWJAP=\"oppo\",\"n8pdvv2e\"\r\n";
    char str6[] = "AT+MQTTUSERCFG=0,1,\"NULL\",\"esp8266&k0lr3oY16Qc\",\"3d6c171591e30d02c2630fa74208b27afc6cab4aa15d38b375ebb2acbc967d49\",0,0,\"\"\r\n";
    char str7[] = "AT+MQTTCLIENTID=0,\"k0lr3oY16Qc.esp8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1716629463953|\"\r\n";
    char str8[] = "AT+MQTTCONN=0,\"iot-06z00hna6bo4f2k.mqtt.iothub.aliyuncs.com\",1883,1\r\n";
    char str9[] = "AT+MQTTSUB=0,\"/k0lr3oY16Qc/esp8266/user/get\",1\r\n";
    char str10[] = "AT+MQTTPUB=0,\"/k0lr3oY16Qc/esp8266/user/update\",\"1\",1,0\r\n";
      //***下面是APP666
//        char str1[] = "AT+RST\r\n";
//        char str2[] = "AT+RESTORE\r\n";
//        char str3[] = "AT+CWMODE=1\r\n";
//        char str4[] = "AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"\r\n";
//        char str5[] = "AT+CWJAP=\"oppo\",\"n8pdvv2e\"\r\n";
//        char str6[] = "AT+MQTTUSERCFG=0,1,\"NULL\",\"APP666&k0lr3oY16Qc\",\"312d3166fa0745dc7c7c722e753259de782ace7a8b04a1332ca0189b18e57e30\",0,0,\"\"\r\n";
//        char str7[] = "AT+MQTTCLIENTID=0,\"k0lr3oY16Qc.APP666|securemode=2\\,signmethod=hmacsha256\\,timestamp=1720444232274|\"\r\n";
//        char str8[] = "AT+MQTTCONN=0,\"iot-06z00hna6bo4f2k.mqtt.iothub.aliyuncs.com\",1883,1\r\n";
//        char str9[] = "AT+MQTTSUB=0,\"/k0lr3oY16Qc/APP666/user/get\",1\r\n";
//        char str10[] = "AT+MQTTPUB=0,\"/k0lr3oY16Qc/APP666/user/update\",\"1\",1,0\r\n";
    rt_device_write(serial, 0, str1, (sizeof(str1) - 1));
    rt_thread_mdelay(5000);
    rt_device_write(serial, 0, str2, (sizeof(str2) - 1));
    rt_thread_mdelay(5000);
    rt_device_write(serial, 0, str3, (sizeof(str3) - 1));
    rt_thread_mdelay(5000);
    rt_device_write(serial, 0, str4, (sizeof(str4) - 1));
    rt_thread_mdelay(5000);
    rt_device_write(serial, 0, str5, (sizeof(str5) - 1));
    rt_thread_mdelay(5000);
    rt_device_write(serial, 0, str6, (sizeof(str6) - 1));
    rt_thread_mdelay(5000);
    rt_device_write(serial, 0, str7, (sizeof(str7) - 1));
    rt_thread_mdelay(5000);
    rt_device_write(serial, 0, str8, (sizeof(str8) - 1));
    rt_thread_mdelay(5000);
    rt_device_write(serial, 0, str9, (sizeof(str9) - 1));
    rt_thread_mdelay(5000);
    rt_device_write(serial, 0, str10, (sizeof(str10) - 1));
    my8266_flag_finish = 1;
    while (1)
    {

        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }

//        data[count] = ch;
//        if (ch == '+' || count > 0)
//                    {count++;}
//                else
//                    {count = 0;}
//        if (count == 53)
//                {
//                    count = 0;
//                    if (data[52] == 0x0a)
//                    {
//                        if(data[49] == 1)
//                        {
//                            kai = 1;
//                        }
//                      //rt_device_write(serial, 0, &data[49], 1);
//                        if(data[49] == 0)
//                                                {
//                                                    kai = 0;
//                                                }
//
//                    }
//                }
        if((USART_RX_STA&0X8000) ==0)
        {
            if(USART_RX_STA&0x4000)
            {
                if(ch !=0x0a)USART_RX_STA=0;
                else USART_RX_STA|=0x8000;
            }
            else
            {
                if(ch==0x0d)USART_RX_STA|=0x4000;
                else
                    {
                        USART_RX_BUF[USART_RX_STA&0x3FFF]=ch;
                        USART_RX_STA++;
                        if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;
                    }
                }
            }






        rt_thread_mdelay(10);
    }
}

static int uart_sample(void)
{
    rt_err_t ret = RT_EOK;

    /* 查找串口设备 */
    serial = rt_device_find(SAMPLE_UART_NAME);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", SAMPLE_UART_NAME);
        return RT_ERROR;
    }

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);
    /* 以读写及中断接收方式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial6", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}

INIT_APP_EXPORT(uart_sample);//自动初始化，可以自动调用线程的初始化函数

