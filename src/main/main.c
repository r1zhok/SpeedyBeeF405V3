/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "scheduler/tasks.h"

#include "config/config.h"

#include "fc/rc_modes.h"
#include "fc/rc_controls.h"

#include "flight/imu.h"
#include "flight/position.h"

#include "sensors/sensors.h"
#include "sensors/barometer.h"
#include "sensors/compass.h"
#include "sensors/gyro_init.h"
#include "sensors/adcinternal.h"
#include "sensors/battery.h"

#include "drivers/gps/gps.h"

#include "rx/rx.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void hwInit(void);
void fcInit(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  cycleCounterInit();

  /* USER CODE BEGIN SysInit */
  hwInit();
  fcInit();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
//  MX_GPIO_Init();
//  MX_DMA_Init();
//  MX_USB_DEVICE_Init();
//  MX_TIM4_Init();
//  MX_I2C2_Init();
//  MX_SPI1_Init();
//  MX_SPI2_Init();
//  MX_USART2_UART_Init();
//  MX_USART6_UART_Init();
//  MX_USART3_UART_Init();
//  MX_UART4_Init();
//  MX_UART5_Init();
//  MX_USART1_UART_Init();
//  MX_FATFS_Init();
//  MX_ADC1_Init();
//  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  scheduler();
	//SerialCom();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void hwInit(void)
{
  #ifdef _USE_HW_RTC
    rtcInit();
  #endif
  gpioInit();
  flashInit();
  ledInit();
  MX_DMA_Init();
  usbInit();
  uartInit();
  cliInit();
  i2cInit();
  spiInit();
  adcInit();

  if (sdInit() == true)
  {
    fatfsInit();
  }

	ledOn(ST1);
	for (int i = 0; i < 10; i++)
	{
		ledToggle(ST1);
		#if defined(USE_BEEPER)
			HAL_Delay(25);
			BEEP_ON;
			HAL_Delay(25);
			BEEP_OFF;
		#else
			HAL_Delay(50);
		#endif
	}
	ledOff(ST1);
}

void fcInit(void)
{
	tasksInitData();
	cliOpen(_DEF_USB, 57600);

	readEEPROM();

	Sensor_Init();
	Baro_Init();
	compassInit();
	adcInternalInit();
	gyroSetTargetLooptime(1);
	gyroInitFilters();
	imuInit();
	positionConfig_Init();
	rcControlsConfig_Init();
	armingConfig_Init();
	flight3DConfig_Init();
	rxInit();
	batteryInit(); // always needs doing, regardless of features.
	gpsInit();
	tasksInit();
	MSP_SET_MODE_RANGE(0, 0, 0, 1700, 2100);
	MSP_SET_MODE_RANGE(1, 1, 0, 1700, 2100);
	MSP_SET_MODE_RANGE(2, 6, 1, 1700, 2100);
	MSP_SET_MODE_RANGE(3, 27, 4, 1700, 2100);
	MSP_SET_MODE_RANGE(4, 7, 5, 1700, 2100);
	MSP_SET_MODE_RANGE(5, 26, 0, 1700, 2100);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
