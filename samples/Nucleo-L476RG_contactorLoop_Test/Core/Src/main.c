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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// Debug (1) or Active (0) Mode
#define DEBUG					1

// Define Contactor GPIO and GPIO PINs
#define POS_LSD_OUTPUT_GPIO		GPIOB
#define POS_LSD_OUTPUT_PIN		GPIO_PIN_0
#define POS_LSD_INPUT_GPIO		GPIOC
#define POS_LSD_INPUT_PIN		GPIO_PIN_1

#define POS_HSD_OUTPUT_GPIO		GPIOC
#define POS_HSD_OUTPUT_PIN		GPIO_PIN_3
#define POS_HSD_INPUT_GPIO		GPIOC
#define POS_HSD_INPUT_PIN		GPIO_PIN_2

#define POS_CON_INPUT_GPIO		GPIOC
#define POS_CON_INPUT_PIN		GPIO_PIN_0

#define ANALOG_INPUT_GPIO		GPIOA
#define ANALOG_INPUT_PIN		GPIO_PIN_0

#define NEG_LSD_OUTPUT_GPIO		GPIOC
#define NEG_LSD_OUTPUT_PIN		GPIO_PIN_12
#define NEG_LSD_INPUT_GPIO		GPIOC
#define NEG_LSD_INPUT_PIN		GPIO_PIN_10

#define NEG_HSD_OUTPUT_GPIO		GPIOB
#define NEG_HSD_OUTPUT_PIN		GPIO_PIN_8
#define NEG_HSD_INPUT_GPIO		GPIOB
#define NEG_HSD_INPUT_PIN		GPIO_PIN_9

#define NEG_CON_INPUT_GPIO		GPIOA
#define NEG_CON_INPUT_PIN		GPIO_PIN_15

#define PRE_LSD_OUTPUT_GPIO		GPIOC
#define PRE_LSD_OUTPUT_PIN		GPIO_PIN_11
#define PRE_LSD_INPUT_GPIO		GPIOD
#define PRE_LSD_INPUT_PIN		GPIO_PIN_2

#define PRE_HSD_OUTPUT_GPIO		GPIOB
#define PRE_HSD_OUTPUT_PIN		GPIO_PIN_5
#define PRE_HSD_INPUT_GPIO		GPIOB
#define PRE_HSD_INPUT_PIN		GPIO_PIN_4

#define PRE_CON_INPUT_GPIO		GPIOC
#define PRE_CON_INPUT_PIN		GPIO_PIN_9

// Define Constants (Fault Conditions, Limits & Other Conditions)
#define SERIES_MOD				1	  // # of Modules in Series
#define SERIES_CELL				12    // # of Cells in Series
#define MAX_VOLT				4.20  // Maximum Voltage from a Cell
#define MIN_VOLT				2.85  // Minimum Voltage from a Cell
#define MIN_TEMP 				0     // Minimum (surface) Temperature of a cell
#define MAX_TEMP 				60    // Maximum (surface) Temperature of a cell
#define TSREF_ADC				5	  // Reference voltage for thermistor
#define TS_R1					10000 // Reference series resistance for themistor

const int8_t PRE_CHG_PACK_VOLT = 3; //SERIES_MOD * SERIES_CELL * MAX_VOLT;
const float known_resistance = 220.0; //10000.0;  // series resistance in Ohms for thermistor

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
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

CAN_HandleTypeDef hcan1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CAN1_Init(void);
static void MX_ADC1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

uint8_t TxData[8];
uint8_t RxData[8];

uint32_t TxMailbox;

uint16_t cell_analog_values[3];
char msg[30];  // message box for analog values

float pre_charge_voltage; 	 // Measurement for startupSequence
float cell_voltage; 	  	 // Measurement for checkVIT
float cell_current; 	  	 // Calculated current from checkVIT
float thermistor_resistance; // Calculated unknown resistance from checkVIT
float cell_vref; 			 // measurement voltage for vref

int startupSequence(void){

	//
	// Negative Contactor Enable
	//

	// Send high Signal to NEG_LSD_OUTPUT
	HAL_GPIO_WritePin(NEG_LSD_OUTPUT_GPIO, NEG_LSD_OUTPUT_PIN, 1);

	// Read NEG_LSD_INPUT
		// If high, proceed, else EPO/return -1
		if (!HAL_GPIO_ReadPin(NEG_LSD_INPUT_GPIO, NEG_LSD_INPUT_PIN)){ return -1; }


	// Send high Signal to NEG_HSD_OUTPUT
	HAL_GPIO_WritePin(NEG_HSD_OUTPUT_GPIO, NEG_HSD_OUTPUT_PIN, 1);

	// Read NEG_HSD_INPUT
		// If high, proceed, else EPO/return -1
		if (!HAL_GPIO_ReadPin(NEG_LSD_INPUT_GPIO, NEG_LSD_INPUT_PIN)){ return -1; }

	// Read NEG_CON_OUTPUT
		// If connected (low for nand, high for and, between HSD and LSD), proceed, else EPO/return -1
		if (HAL_GPIO_ReadPin(NEG_CON_INPUT_GPIO, NEG_CON_INPUT_PIN)){ return -1; }

	//
	// Pre Charge Contactor Enable
	//

	// Send high Signal to PRE_LSD_OUTPUT
	HAL_GPIO_WritePin(PRE_LSD_OUTPUT_GPIO, PRE_LSD_OUTPUT_PIN, 1);

	// Read PRE_LSD_INPUT
		// If high, proceed, else EPO/return -1
		if (!HAL_GPIO_ReadPin(PRE_LSD_INPUT_GPIO, PRE_LSD_INPUT_PIN)){ return -1; }

	// Send high Signal to PRE_HSD_OUTPUT
	HAL_GPIO_WritePin(PRE_HSD_OUTPUT_GPIO, PRE_HSD_OUTPUT_PIN, 1);

	// Read PRE_HSD_INPUT
		// If high, proceed, else EPO/return -1
		if (!HAL_GPIO_ReadPin(PRE_HSD_INPUT_GPIO, PRE_HSD_INPUT_PIN)){ return -1; }

	// Read PRE_CON_OUTPUT
		// If connected (low for nand, high for and, between HSD and LSD), proceed, else EPO/return 0
		if (HAL_GPIO_ReadPin(PRE_CON_INPUT_GPIO, PRE_CON_INPUT_PIN)){ return -1; }

	// Read voltage at ANALOG_INPUT
		// If 90% of pack voltage after time t proceed, else EPO/return -1
		while (pre_charge_voltage < PRE_CHG_PACK_VOLT) {
		    HAL_ADC_Start_DMA(&hadc1,(uint32_t *) cell_analog_values, 3); // Start Analog to Digital conversion
		    HAL_ADC_PollForConversion(&hadc1, 1000);					 // Poll Value Read
		    pre_charge_voltage = (float)cell_analog_values[0]/1000;   // Store Value in (mV) form (V)

		    // Convert the integer part to a hexadecimal string
		    uint16_t intPart = (uint16_t)pre_charge_voltage;
		    char IntPartStr[3];
		    sprintf(IntPartStr, "%02X", intPart);

		    // Convert the fractional part to a hexadecimal string
		    uint16_t fracPart = (uint16_t)((pre_charge_voltage - intPart) * 100);
		    char FracPartStr[3];
		    sprintf(FracPartStr, "%02X", fracPart);

		    // Write to UART Over Temp Fault in the desired format
		    char ResultStr[8];
		    sprintf(ResultStr, "%s.%s\r\n", IntPartStr, FracPartStr);
		    HAL_UART_Transmit(&huart2, (uint8_t*)ResultStr, sizeof(ResultStr), 100);


		    // Delay for debug
		    HAL_Delay(1000);
		}

	//
	// Positive Contactor Enable
	//

	// Send high Signal to POS_LSD_OUTPUT
	HAL_GPIO_WritePin(POS_LSD_OUTPUT_GPIO, POS_LSD_OUTPUT_PIN, 1);

	// Read POS_LSD_INPUT
		// If high, proceed, else EPO/return -1
		if (!HAL_GPIO_ReadPin(POS_LSD_INPUT_GPIO, POS_LSD_INPUT_PIN)){ return -1; }

	// Send high Signal to POS_HSD_OUTPUT
	HAL_GPIO_WritePin(POS_HSD_OUTPUT_GPIO, POS_HSD_OUTPUT_PIN, 1);

	// Read POS_HSD_INPUT
		// If high, proceed, else EPO/return -1
		if (!HAL_GPIO_ReadPin(POS_HSD_INPUT_GPIO, POS_HSD_INPUT_PIN)){ return -1; }

	// Read POS_CON_OUTPUT
		// If connected (low for nand, high for and, between HSD and LSD), proceed, else EPO/return 0
		if (HAL_GPIO_ReadPin(POS_CON_INPUT_GPIO, POS_CON_INPUT_PIN)){ return -1; }

	//
	// Pre Charge Contactor Disable
	//

	// Send low Signal to PRE_HSD_OUTPUT
	HAL_GPIO_WritePin(PRE_HSD_OUTPUT_GPIO, PRE_HSD_OUTPUT_PIN, 0);

	// Read PRE_HSD_INPUT
		// If low, proceed, else EPO/return -1
		if (HAL_GPIO_ReadPin(PRE_HSD_INPUT_GPIO, PRE_HSD_INPUT_PIN)){ return -1; }

	// Send low Signal to PRE_LSD_OUTPUT
	HAL_GPIO_WritePin(PRE_LSD_OUTPUT_GPIO, PRE_LSD_OUTPUT_PIN, 0);

	// Read PRE_LSD_INPUT
		// If low, proceed, else EPO/return -1
		if (HAL_GPIO_ReadPin(PRE_LSD_INPUT_GPIO, PRE_LSD_INPUT_PIN)){ return -1; }

	// Read PRE_CON_OUTPUT
		// If connected (low for nand, high for and, between HSD and LSD), proceed, else EPO/return 0
		if (!HAL_GPIO_ReadPin(PRE_CON_INPUT_GPIO, PRE_CON_INPUT_PIN)){ return -1; }

	// Start-up Sequence Concluded Successfully
	return 0;
}

void checkVIT(uint8_t mod_number, uint8_t cell_number){

	// append id, mod and cell number to sensor reading
    HAL_ADC_Start_DMA(&hadc1,(uint32_t *) cell_analog_values, 3); // Start Analog to Digital conversion
	HAL_ADC_PollForConversion(&hadc1, 1000);					 // Poll Value Read
	cell_voltage = (float)cell_analog_values[1]/1000;   // Store Value in (mV) form (V)
	cell_vref = (float)cell_analog_values[2]/1000;

	// calculate I = cell_voltage / known R
	cell_current = cell_voltage / known_resistance;

	// calculate thermistor R = (Vref - V) /I
	thermistor_resistance = (cell_vref - cell_voltage) / cell_current;

	// Convert module number to hex
	uint16_t mod_num = (uint16_t)mod_number;
	char mod_numStr[2];
	sprintf(mod_numStr, "%1X", mod_num);

	// Convert cell number to hex
	uint16_t cell_num = (uint16_t)cell_number;
	char cell_numStr[2];
	sprintf(cell_numStr, "%1X", cell_num);

	// Convert the integer part to a hexadecimal string
	uint16_t intPart = (uint16_t)cell_voltage;
	char IntPartStr[3];
	sprintf(IntPartStr, "%02X", intPart);

	// Convert the fractional part to a hexadecimal string
	uint16_t fracPart = (uint16_t)((cell_voltage - intPart) * 100);
	char FracPartStr[3];
	sprintf(FracPartStr, "%02X", fracPart);

	// Write to UART
	char ResultStr[11];
	sprintf(ResultStr, "1%s%s%s.%s\r\n", mod_numStr, cell_numStr, IntPartStr, FracPartStr);
	HAL_UART_Transmit(&huart2, (uint8_t*)ResultStr, sizeof(ResultStr), 100);

	// Convert the integer part to a hexadecimal string
	intPart = (uint16_t)cell_current;
	sprintf(IntPartStr, "%02X", intPart);

	// Convert the fractional part to a hexadecimal string
	fracPart = (uint16_t)((cell_current - intPart) * 100);
	sprintf(FracPartStr, "%02X", fracPart);

	// Write to UART
	sprintf(ResultStr, "2%s%s%s.%s\r\n", mod_numStr, cell_numStr, IntPartStr, FracPartStr);
	HAL_UART_Transmit(&huart2, (uint8_t*)ResultStr, sizeof(ResultStr), 100);

	// TODO: determine temp based off linear eq of degrees vs ohms
	// Convert the integer part to a hexadecimal string
	intPart = (uint16_t)thermistor_resistance;
	sprintf(IntPartStr, "%02X", intPart);

	// Convert the fractional part to a hexadecimal string
	fracPart = (uint16_t)((thermistor_resistance - intPart) * 100);
	sprintf(FracPartStr, "%02X", fracPart);

	// Write to UART
	sprintf(ResultStr, "3%s%s%s.%s\r\n", mod_numStr, cell_numStr, IntPartStr, FracPartStr);
	HAL_UART_Transmit(&huart2, (uint8_t*)ResultStr, sizeof(ResultStr), 100);

	// Delay for debug
	HAL_Delay(1000);

	// TODO: send V and I to the model to get SOH, SOC and SOP

	return 0;
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
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_CAN1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  HAL_CAN_Start(&hcan1);

  // Activate the notification
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

  TxHeader.DLC = 2; // data length
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.StdId = 0x446; // ID

//  TxData[0] = 200; // ms Delay
//  TxData[1] = 20;  // loop rep

  // Initializing
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);

  int success;
  success = startupSequence();

  if (success == -1) { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1); }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  // TODO: loop through every cell in the pack for every module
	  checkVIT(cell0.module, cell0.cell);

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
		  HAL_UART_Transmit(&huart2, (uint8_t*)FaultStr, sizeof(FaultStr), 100);

		  // Print to UART for Debug
		  uint8_t Test[] = " OVER_TEMP_FAULT\r\n"; //Data to send
		  HAL_UART_Transmit(&huart2,Test,sizeof(Test),100);// Sending in normal mode
		  HAL_Delay(100);

		  // Write to CAN Over Temp Fault
		  HAL_CAN_AddTxMessage(&hcan1, &TxHeader, FaultStr, &TxMailbox);

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

		  // Write to UART Over Current Fault
		  HAL_UART_Transmit(&huart2, (uint8_t*)FaultStr, sizeof(FaultStr), 100);

		  // Print to UART for Debug
		  uint8_t Test[] = " OVER_CURRENT_FAULT\r\n"; //Data to send
		  HAL_UART_Transmit(&huart2,Test,sizeof(Test),100);// Sending in normal mode
		  HAL_Delay(100);

		  // Write to CAN Over Current Fault
		  HAL_CAN_AddTxMessage(&hcan1, &TxHeader, FaultStr, &TxMailbox);

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

		  // Write to UART Over Voltage Fault
		  HAL_UART_Transmit(&huart2, (uint8_t*)FaultStr, sizeof(FaultStr), 100);

		  // Print to UART for Debug
		  uint8_t Test[] = " UNDER_VOLTAGE_FAULT and OVER_CURRENT_FAULT\r\n"; //Data to send
		  HAL_UART_Transmit(&huart2,Test,sizeof(Test),100);// Sending in normal mode
		  HAL_Delay(100);

		  // Write to CAN Over Voltage Fault
		  HAL_CAN_AddTxMessage(&hcan1, &TxHeader, FaultStr, &TxMailbox);

		  // Clear the Cell Over Voltage Fault
		  clearCellFault(&cell0, UNDER_VOLTAGE_FAULT);
		  clearCellFault(&cell0, OVER_CURRENT_FAULT);

		  // Turn OFF LEDs
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
	  }

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
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 3;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = ADC_REGULAR_RANK_3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 32;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_2TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  CAN_FilterTypeDef canfilterconfig;

  canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
  canfilterconfig.FilterBank = 18;  // which filter bank to use from the assigned ones
  canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  canfilterconfig.FilterIdHigh = 0x103<<5; // ID of external ECU
  canfilterconfig.FilterIdLow = 0;
  canfilterconfig.FilterMaskIdHigh = 0x103<<5;
  canfilterconfig.FilterMaskIdLow = 0x0000;
  canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
  canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
  canfilterconfig.SlaveStartFilterBank = 20;  // how many filters to assign to the CAN1 (master can)

  HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);

  /* USER CODE END CAN1_Init 2 */

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
  huart2.Init.BaudRate = 9600;
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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 PC9
                           PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_9
                          |GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC3 PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA6 PA7 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB5 PB8 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA9 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB4 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
