#include "rtc-board.h"
#include "lpm_bsp.h"
#include "usart.h"
#include "tim.h"
#include "i2c.h"

/* Constants required to manipulate the core.  Registers first... */
#define portNVIC_SYSTICK_CTRL_REG			( * ( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG			( * ( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG	( * ( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_SYSPRI2_REG				( * ( ( volatile uint32_t * ) 0xe000ed20 ) )

/* ...then bits in the registers. */
#define portNVIC_SYSTICK_INT_BIT			( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT			( 1UL << 0UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT		( 1UL << 16UL )
#define portNVIC_PENDSVCLEAR_BIT 			( 1UL << 27UL )
#define portNVIC_PEND_SYSTICK_CLEAR_BIT		( 1UL << 25UL )

extern void SystemClock_Config(void);

static bool board_lpm_allowed(void)
{
    return true;
}

static void board_periph_deinit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin   = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = NFC_DECT_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(NFC_DECT_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = BUTTON_Pin | GPIO_PIN_1 | WL_DECT_Pin | USB_DECT_Pin | GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = MOTION_INT1_Pin | MOTION_INT2_Pin | MCS_DECT_Pin | GPIO_PIN_5;
    GPIO_InitStruct.Mode  = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(NFC_PWR_EN_GPIO_Port, NFC_PWR_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(SENSOR_PWR_EN_GPIO_Port, SENSOR_PWR_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTION_PWR_EN_GPIO_Port, MOTION_PWR_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(NB_RESET_GPIO_Port, NB_RESET_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(NB_PSM_WAKE_GPIO_Port, NB_PSM_WAKE_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(NB_PWR_EN_GPIO_Port, NB_PWR_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(VBAT_PWR_EN_GPIO_Port, VBAT_PWR_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(WL_PWR_EN_GPIO_Port, WL_PWR_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MCS_PWR_EN_GPIO_Port, MCS_PWR_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MCS_SOURCE_GPIO_Port, MCS_SOURCE_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(NB_PWR_KEY_GPIO_Port, NB_PWR_KEY_Pin, GPIO_PIN_RESET);

    HAL_UART_DeInit(&huart1);
    HAL_I2C_MspDeInit(&hi2c1);
    HAL_I2C_MspDeInit(&hi2c2);
    HAL_TIM_Base_DeInit(&htim6);
}

static void board_periph_reinit(void)
{
    HAL_I2C_MspInit(&hi2c1);
    HAL_I2C_MspInit(&hi2c2);

    // HAL_UART_Init(&huart1);
    // MX_TIM6_Init();
    // HAL_TIM_Base_Start_IT(&htim6);
}

//用于做休眠前的准备工作,关闭不需要的外设和IO
static void pre_sleep_processing(TickType_t xExpectedIdleTime)
{
    printf("enter lpm, expected:%ld\r\n", xExpectedIdleTime);
    board_periph_deinit();
}

//用于休眠唤醒后恢复现场,IO口及外设恢复
static void post_sleep_processing(TickType_t* exceptIdleTick)
{
    //STOP模式 从休眠中醒来，时钟会变为MSI
    if( __HAL_RCC_GET_SYSCLK_SOURCE() == RCC_CFGR_SWS_MSI) {
        SystemClock_Config();
        board_periph_reinit();
        *exceptIdleTick = RtcTick2Ms(RtcGetTimerElapsedTime());
    } else {
        //非从休眠中醒来，时间不加
        *exceptIdleTick = 0;
    }
    printf("leave lpm, elapsed :%ld\r\n", *exceptIdleTick);
}

//用于检查是否能进入休眠状态,如果不能休眠，修改exceptIdleTick的值为0
void lpm_allow_check(TickType_t* exceptIdleTick)
{
    if(*exceptIdleTick < 20) {
        *exceptIdleTick = 0;
    } else if(board_lpm_allowed() == false) {
        *exceptIdleTick = 0;
    }
}

//弱函数重写, 用于stop2模式下的休眠。函数名字必须为vPortSuppressTicksAndSleep
void vPortSuppressTicksAndSleep( TickType_t xExpectedIdleTime )
{
    if(xExpectedIdleTime > 86400000) {
        xExpectedIdleTime = 86400000;
    }
    portNVIC_SYSTICK_CTRL_REG &= ~portNVIC_SYSTICK_ENABLE_BIT;

    do {
        //设置RTC
        RtcSetTimerContext( );
        RtcStartAlarm(RtcMs2Tick(xExpectedIdleTime));

        //关闭不需要的外设和IO
        pre_sleep_processing( xExpectedIdleTime );
        __disable_irq();
        HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
        post_sleep_processing( &xExpectedIdleTime );
    } while(0);

    portNVIC_SYSTICK_CTRL_REG |= portNVIC_SYSTICK_ENABLE_BIT;
    vTaskStepTick( xExpectedIdleTime );
    __enable_irq();
}
