/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

	#include <stdio.h>
	#include <string.h>
	#include "lcd.h"

	#include "93c86-eeprom-sm.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

	typedef struct
	{
		UART_HandleTypeDef *uart;
	}	Debug_struct;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */



/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

	#define	DEBUG_STRING_SIZE		100
	char DebugString[DEBUG_STRING_SIZE];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

	Debug_struct DebugH;
	DebugH.uart = &huart2;
	sprintf(DebugString,"\r\nHello 93c86 2020.11.21v4 dec2 \r\n");
	HAL_UART_Transmit(DebugH.uart, (uint8_t *)DebugString, strlen(DebugString), 100);

	LCD_Init();
	LCD_SetRotation(1);
	LCD_SetCursor(0, 0);
	LCD_FillScreen(ILI92_WHITE);
	LCD_SetTextColor(ILI92_MAGENTA, ILI92_WHITE);
	LCD_Printf("%s",DebugString);

//	EVEN  () ;						HAL_Delay(1);
//	WRITE_ONE_CELL (11, 0x1111) ;	HAL_Delay(1);
//	EWDS  () ;						HAL_Delay(1);
	//HAL_Delay(1000);

	sprintf(DebugString,"\r\n0000) ") ;
	LCD_Printf("%s",DebugString);
	HAL_UART_Transmit(DebugH.uart, (uint8_t *)DebugString, strlen(DebugString), 100) ;

	  for (int cell_int = 0; cell_int < 1024; cell_int++) {
		uint16_t res_u16 = READ (cell_int) ;

		{	//	for read char serial number
		res_u16 = inverse_order_in_two_byte_V2 (res_u16);
		char first_ch  = (char) ((res_u16 & 0xFF00)>>8) ;
		char second_ch = (char)  (res_u16 & 0x00FF)     ;
		sprintf(DebugString,"%c %c ", first_ch, second_ch ) ;
		}

//		{	//	for read SOH and Hx
//		sprintf(DebugString,"%05d ", res_u16) ;
//		}

		LCD_Printf("%s",DebugString);
		HAL_UART_Transmit(DebugH.uart, (uint8_t *)DebugString, strlen(DebugString), 100) ;

		if (((cell_int+1)%16 == 0) && (cell_int != 0))  {
			sprintf(DebugString,"\r\n%04d) ", cell_int) ;
			LCD_Printf("%s",DebugString);
			HAL_UART_Transmit(DebugH.uart, (uint8_t *)DebugString, strlen(DebugString), 100) ;
		}

		//LCD_Printf("%s",DebugString);
		//HAL_UART_Transmit(DebugH.uart, (uint8_t *)DebugString, strlen(DebugString), 100) ;
		//HAL_GPIO_TogglePin(LD2_GPIO_Port,LD2_Pin);
		//HAL_Delay(1);
	  }

		sprintf(DebugString," The END.") ;
		LCD_Printf("%s",DebugString);
		HAL_UART_Transmit(DebugH.uart, (uint8_t *)DebugString, strlen(DebugString), 100) ;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
