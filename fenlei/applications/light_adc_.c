#include <rtthread.h>
#include <rtdevice.h>

#define ADC_DEV_NAME        "adc1"      /* ADC 设备名称 */
#define ADC_DEV_CHANNEL     0           /* ADC 通道 */

rt_uint32_t value, vol;

static void adc_vol_thread_entry(void *parameter)
{
    rt_adc_device_t adc_dev;
//    rt_uint32_t value, vol;
    rt_err_t ret = RT_EOK;

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        return RT_ERROR;
    }

    /* 使能设备 */
    ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);
    while(1)
    {
    /* 读取采样值 */
    value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
//    rt_kprintf("the value is :%d \n", value);

    /* 转换为对应电压值,3.3V对应12位最大值4096,数据精度乘以100保留2位小数 */
    vol = value * 330 / 4096;
//    rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);
    rt_thread_mdelay(10);
    }
    /* 关闭通道 */
    ret = rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);


}

static int adc_init(void)
{
    rt_thread_t adc_thread;

    adc_thread = rt_thread_create("adc_thread",
                                      adc_vol_thread_entry,
                                      "adc_thread",
                                      1024,
                                      25,
                                      20);
    if (adc_thread != RT_NULL)
    {
        rt_thread_startup(adc_thread);
    }

    return RT_EOK;
}

INIT_APP_EXPORT(adc_init);//自动初始化，可以自动调用线程的初始化函数
