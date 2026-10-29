/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
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
static uint8_t Scan_MatrixKey(void);
static void Key_To_LED(uint8_t key_val);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void delay(uint32_t t)
{
	while(t--);
}

// 3行PE4~PE6，4列PE0~PE3，返回键值0~9
uint8_t KeyScan(void)
{
	uint8_t keyval = 0xFF;
	//行1 PE4输出低
	GPIOE->ODR |= (1<<4);
	GPIOE->ODR &= ~(1<<4);
	delay(5000);
	if( !(GPIOE->IDR & (1<<0)) ){ delay(5000); while(!(GPIOE->IDR & (1<<0))); keyval = 1; }
	if( !(GPIOE->IDR & (1<<1)) ){ delay(5000); while(!(GPIOE->IDR & (1<<1))); keyval = 2; }
	if( !(GPIOE->IDR & (1<<2)) ){ delay(5000); while(!(GPIOE->IDR & (1<<2))); keyval = 3; }
	if( !(GPIOE->IDR & (1<<3)) ){ delay(5000); while(!(GPIOE->IDR & (1<<3))); keyval = 10; }

	//行2 PE5输出低
	GPIOE->ODR |= (1<<5);
	GPIOE->ODR &= ~(1<<5);
	delay(5000);
	if( !(GPIOE->IDR & (1<<0)) ){ delay(5000); while(!(GPIOE->IDR & (1<<0))); keyval = 4; }
	if( !(GPIOE->IDR & (1<<1)) ){ delay(5000); while(!(GPIOE->IDR & (1<<1))); keyval = 5; }
	if( !(GPIOE->IDR & (1<<2)) ){ delay(5000); while(!(GPIOE->IDR & (1<<2))); keyval = 6; }
	if( !(GPIOE->IDR & (1<<3)) ){ delay(5000); while(!(GPIOE->IDR & (1<<3))); keyval = 11; }

	//行3 PE6输出低
	GPIOE->ODR |= (1<<6);
	GPIOE->ODR &= ~(1<<6);
	delay(5000);
	if( !(GPIOE->IDR & (1<<0)) ){ delay(5000); while(!(GPIOE->IDR & (1<<0))); keyval = 7; }
	if( !(GPIOE->IDR & (1<<1)) ){ delay(5000); while(!(GPIOE->IDR & (1<<1))); keyval = 8; }
	if( !(GPIOE->IDR & (1<<2)) ){ delay(5000); while(!(GPIOE->IDR & (1<<2))); keyval = 9; }
	if( !(GPIOE->IDR & (1<<3)) ){ delay(5000); while(!(GPIOE->IDR & (1<<3))); keyval = 0; }

	GPIOE->ODR |= ((1<<4)|(1<<5)|(1<<6)); //全部行拉高
	return keyval;
}
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t key;
  while (1)
  {
	  uint8_t key;
	      key = Scan_MatrixKey();
	      if (key != 0xFF)
	      {
	          Key_To_LED(key);
	          HAL_Delay(300);
	      }
	      else
	      {
	          /* 无按键，熄灭 / 默认 */
	          GPIOF->ODR |= 0x00FF;
	      }
	      HAL_Delay(10);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */


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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
static uint8_t Scan_MatrixKey(void)
{
    static const uint16_t col_pins[] =
    {
        GPIO_PIN_4,
        GPIO_PIN_5,
        GPIO_PIN_6
    };
    static const uint16_t row_pins[] =
    {
        GPIO_PIN_0,
        GPIO_PIN_1,
        GPIO_PIN_2,
        GPIO_PIN_3
    };
    /* 键映射，3×4 */
    static const uint8_t key_map[3][4] =
    {
        { 1,  2,  3,  4 },
        { 5,  6,  7,  8 },
        { 9, 10, 11, 12 }
    };

    for (uint8_t c = 0; c < 3; c++)
    {
        /* 全部列先拉高，再拉低当前这一列 */
        for (uint8_t i = 0; i < 3; i++)
        {
            HAL_GPIO_WritePin(GPIOE, col_pins[i], GPIO_PIN_SET);
        }
        HAL_GPIO_WritePin(GPIOE, col_pins[c], GPIO_PIN_RESET);

        HAL_Delay(2);
        for (uint8_t r = 0; r < 4; r++)
        {
            if (HAL_GPIO_ReadPin(GPIOE, row_pins[r]) == GPIO_PIN_RESET)
            {
                HAL_Delay(10);
                if (HAL_GPIO_ReadPin(GPIOE, row_pins[r]) == GPIO_PIN_RESET)
                {
                    /* 松手检测 */
                    while (HAL_GPIO_ReadPin(GPIOE, row_pins[r]) == GPIO_PIN_RESET);
                    return key_map[c][r];
                }
            }
        }
    }
    return 0xFF;
}

static void Key_To_LED(uint8_t key_val)
{
    /* BCD：只用低 4 bit，送到 PF0～PF3；高四位保持高 */
    uint16_t led_out;
    led_out = ((0x000F & key_val) ^ 0x000F);
    led_out |= 0x00F0;
    GPIOF->ODR = (GPIOF->ODR & 0xFF00) | (led_out & 0x00FF);
}
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
