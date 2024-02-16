/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include <stdbool.h>
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// Define the Cell
typedef struct {
	// ID
    uint8_t module; // Module Number
    uint8_t cell;   // Cell Number

    // State Estimations
    uint32_t SOC; // State of Charge
    uint32_t SOH; // State of Health
    uint32_t SOP; // State of Power

    // Physics
    uint32_t V; // Voltage
    uint32_t I; // Current
    uint32_t T; // Temperature

    // Faults
    bool power_rail;
    bool comm;
    bool over_voltage;
    bool under_voltage;
    bool over_temp;
    bool under_temp;
    bool over_current;
    bool under_current;
} Cell;

typedef enum {
    POWER_RAIL_FAULT = 1 << 7,
    COMM_FAULT = 1 << 6,
    OVER_VOLTAGE_FAULT = 1 << 5,
    UNDER_VOLTAGE_FAULT = 1 << 4,
    OVER_TEMP_FAULT = 1 << 3,
    UNDER_TEMP_FAULT = 1 << 2,
    OVER_CURRENT_FAULT = 1 << 1,
    UNDER_CURRENT_FAULT = 1
} CellFault;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

uint16_t getCellFaults(Cell* cell){
    uint16_t result = 0;

    // Get the lower 4 bits of module and cell
    result |= (cell->module & 0x0F) << 12;
    result |= (cell->cell & 0x0F) << 8;

    // Concatenate all the booleans
    result |= (cell->power_rail & 0x01) << 7;
    result |= (cell->comm & 0x01) << 6;
    result |= (cell->over_voltage & 0x01) << 5;
    result |= (cell->under_voltage & 0x01) << 4;
    result |= (cell->over_temp & 0x01) << 3;
    result |= (cell->under_temp & 0x01) << 2;
    result |= (cell->over_current & 0x01) << 1;
    result |= (cell->under_current & 0x01);

    return result;
}

void setCellFaults(Cell* cell, CellFault value) {
    // Extract the boolean values
    if (value & POWER_RAIL_FAULT) cell->power_rail = true;
    if (value & COMM_FAULT) cell->comm = true;
    if (value & OVER_VOLTAGE_FAULT) cell->over_voltage = true;
    if (value & UNDER_VOLTAGE_FAULT) cell->under_voltage = true;
    if (value & OVER_TEMP_FAULT) cell->over_temp = true;
    if (value & UNDER_TEMP_FAULT) cell->under_temp = true;
    if (value & OVER_CURRENT_FAULT) cell->over_current = true;
    if (value & UNDER_CURRENT_FAULT) cell->under_current = true;
}

void clearCellFault(Cell* cell, CellFault fault) {
    // Clear the corresponding fault
    switch (fault) {
        case POWER_RAIL_FAULT:
            cell->power_rail = false;
            break;
        case COMM_FAULT:
            cell->comm = false;
            break;
        case OVER_VOLTAGE_FAULT:
            cell->over_voltage = false;
            break;
        case UNDER_VOLTAGE_FAULT:
            cell->under_voltage = false;
            break;
        case OVER_TEMP_FAULT:
            cell->over_temp = false;
            break;
        case UNDER_TEMP_FAULT:
            cell->under_temp = false;
            break;
        case OVER_CURRENT_FAULT:
            cell->over_current = false;
            break;
        case UNDER_CURRENT_FAULT:
            cell->under_current = false;
            break;
    }
}

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
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

  // Fault
  uint16_t Fault;

  // Define the Cell
  Cell cell0 = {
	.module = 1, // Module Number
	.cell = 2,   // Cell Number
	.SOC = 0, // State of Charge
	.SOH = 0, // State of Health
	.SOP = 0, // State of Power
	.V = 0, // Voltage
	.I = 0, // Current
	.T = 0, // Temperature
	.power_rail = false,
	.comm = false,
	.over_voltage = false,
	.under_voltage = false,
	.over_temp = false,
	.under_temp = false,
	.over_current = false,
	.under_current = false
  };

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) && !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8)){
		  // Turn ON LED
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
		  HAL_Delay(100);

		  // Set the Cell Over Temp Fault
		  setCellFaults(&cell0, OVER_TEMP_FAULT);

		  // Get the Cell Over Temp Fault
		  Fault = getCellFaults(&cell0);

		  // Convert the result to a hexadecimal string
		  char FaultStr[5]; // Buffer to hold the result string
		  sprintf(FaultStr, "%04X", Fault);

		  // Write to UART Over Temp Fault
		  HAL_UART_Transmit(&huart2, (uint8_t*)FaultStr, sizeof(FaultStr), 10);

		  // Print to UART for Debug
		  uint8_t Test[] = " OVER_TEMP_FAULT\r\n"; //Data to send
		  HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);// Sending in normal mode
		  HAL_Delay(1000);

		  // Clear the Cell Over Temp Fault
		  clearCellFault(&cell0, OVER_TEMP_FAULT);

		  // Turn OFF LED
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
	  }

	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) && !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)){
		  // Turn ON LED
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
		  HAL_Delay(100);

		  // Set the Cell Over Current Fault
		  setCellFaults(&cell0, OVER_CURRENT_FAULT);

		  // Get the Cell Over Current Fault
		  Fault = getCellFaults(&cell0);

		  // Convert the result to a hexadecimal string
		  char FaultStr[5]; // Buffer to hold the result string
		  sprintf(FaultStr, "%04X", Fault);

		  // Write to UART Over Temp Fault
		  HAL_UART_Transmit(&huart2, (uint8_t*)FaultStr, sizeof(FaultStr), 10);

		  // Print to UART for Debug
		  uint8_t Test[] = " OVER_CURRENT_FAULT\r\n"; //Data to send
		  HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);// Sending in normal mode
		  HAL_Delay(1000);

		  // Clear the Cell Over Current Fault
		  clearCellFault(&cell0, OVER_CURRENT_FAULT);

		  // Turn OFF LED
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
	  }

	  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8) && HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)){
		  // Turn ON LEDs
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
		  HAL_Delay(100);

		  // Set the Cell Over Voltage Fault
		  setCellFaults(&cell0, OVER_CURRENT_FAULT);
		  setCellFaults(&cell0, UNDER_VOLTAGE_FAULT);

		  // Get the Cell Over Voltage Fault
		  Fault = getCellFaults(&cell0);

		  // Convert the result to a hexadecimal string
		  char FaultStr[5]; // Buffer to hold the result string
		  sprintf(FaultStr, "%04X", Fault);

		  // Write to UART Over Temp Fault
		  HAL_UART_Transmit(&huart2, (uint8_t*)FaultStr, sizeof(FaultStr), 10);

		  // Print to UART for Debug
		  uint8_t Test[] = " UNDER_VOLTAGE_FAULT and OVER_CURRENT_FAULT\r\n"; //Data to send
		  HAL_UART_Transmit(&huart2,Test,sizeof(Test),10);// Sending in normal mode
		  HAL_Delay(1000);

		  // Clear the Cell Over Voltage Fault
		  clearCellFault(&cell0, UNDER_VOLTAGE_FAULT);
		  clearCellFault(&cell0, OVER_CURRENT_FAULT);

		  // Turn OFF LEDs
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
	  }

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA6 PA7 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
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
