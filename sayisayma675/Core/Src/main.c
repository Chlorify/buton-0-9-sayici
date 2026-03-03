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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t counter = 0; //counter variable

const uint8_t segments[10][7] = { //array for 7 segment display
        //a  b  c  d  e  f  g
        { 1, 1, 1, 1, 1, 1, 0 }, // 0
        { 0, 1, 1, 0, 0, 0, 0 }, // 1
        { 1, 1, 0, 1, 1, 0, 1 }, // 2
        { 1, 1, 1, 1, 0, 0, 1 }, // 3
        { 0, 1, 1, 0, 0, 1, 1 }, // 4
        { 1, 0, 1, 1, 0, 1, 1 }, // 5
        { 1, 0, 1, 1, 1, 1, 1 }, // 6
        { 1, 1, 1, 0, 0, 0, 0 }, // 7
        { 1, 1, 1, 1, 1, 1, 1 }, // 8
        { 1, 1, 1, 1, 0, 1, 1 }, // 9
};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Display_Digit(uint8_t digit)
{
    // Segment encoding: {a, b, c, d, e, f, g}
    // Common anode assumed (SET = segment ON)
    // Adjust GPIO_PIN_SET/RESET if common cathode

    HAL_GPIO_WritePin(GPIOB, led_a_Pin, segments[digit][0] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, led_b_Pin, segments[digit][1] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, led_c_Pin, segments[digit][2] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, led_d_Pin, segments[digit][3] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, led_e_Pin, segments[digit][4] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, led_f_Pin, segments[digit][5] ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, led_g_Pin, segments[digit][6] ? GPIO_PIN_SET : GPIO_PIN_RESET);
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
  /* USER CODE BEGIN 2 */
  HAL_Delay(500);
  HAL_GPIO_WritePin(trs_7seg_GPIO_Port, trs_7seg_Pin, GPIO_PIN_SET);
  Display_Digit(counter);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      //Count UP
      if (HAL_GPIO_ReadPin(buton2_GPIO_Port, buton2_Pin) == GPIO_PIN_RESET)
      {
          HAL_Delay(20); // debounce
          if (HAL_GPIO_ReadPin(buton2_GPIO_Port, buton2_Pin) == GPIO_PIN_RESET)
          {
              counter++;
              if (counter > 9) counter = 0;

              // Wait for button release
              while (HAL_GPIO_ReadPin(buton2_GPIO_Port, buton2_Pin) == GPIO_PIN_RESET); //enter a while loop for release(not suitable for industrial code)
              HAL_Delay(20);
              Display_Digit(counter);
          }
      }

      // Count DOWN
      if (HAL_GPIO_ReadPin(buton1_GPIO_Port, buton1_Pin) == GPIO_PIN_RESET)
      {
          HAL_Delay(20); // debounce
          if (HAL_GPIO_ReadPin(buton1_GPIO_Port, buton1_Pin) == GPIO_PIN_RESET)
          {
              if (counter == 0) counter = 9;
              else counter--;

              // Wait for button release
              while (HAL_GPIO_ReadPin(buton1_GPIO_Port, buton1_Pin) == GPIO_PIN_RESET); //enter a while loop for release(not suitable for industrial code)
              HAL_Delay(20);
              Display_Digit(counter);
          }
      }

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, led_c_Pin|led_dot_Pin|led_a_Pin|led_g_Pin
                          |led_f_Pin|led_b_Pin|led_d_Pin|led_e_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, trs_led_Pin|trs_7seg_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : buton1_Pin buton2_Pin */
  GPIO_InitStruct.Pin = buton1_Pin|buton2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : led_c_Pin led_dot_Pin led_a_Pin led_g_Pin
                           led_f_Pin led_b_Pin led_d_Pin led_e_Pin */
  GPIO_InitStruct.Pin = led_c_Pin|led_dot_Pin|led_a_Pin|led_g_Pin
                          |led_f_Pin|led_b_Pin|led_d_Pin|led_e_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : trs_led_Pin trs_7seg_Pin */
  GPIO_InitStruct.Pin = trs_led_Pin|trs_7seg_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
