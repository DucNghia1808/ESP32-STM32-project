/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "stdio.h"
#include "cJSON.h"
#include "DHT.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t ND = 8;
uint8_t DA = 8;
uint8_t TB1 = 0;
uint8_t TB2 = 0;
uint8_t TB3 = 0;
uint8_t TB4 = 0;
uint8_t TB5 = 0;

DHT_DataTypedef DHT11_Data;


uint8_t rx_data;

char rx_buffer[70];

uint8_t rx_data1;

char rx_buffer1[70];
unsigned int rx_index1 = 0;

unsigned int rx_index = 0;
cJSON *str_json;

void Read_dht11()
{
	DHT_GetData(&DHT11_Data);
	ND = DHT11_Data.Temperature;
	DA = DHT11_Data.Humidity;
}


void XulyJson(char *DataJson) // xu ly data nhan tu SPI
{
	str_json = cJSON_Parse(DataJson);
	if(!str_json)
	{

	}
	else
	{
		if(cJSON_GetObjectItem(str_json, "TB1"))
		{
			if(strstr( cJSON_GetObjectItem(str_json, "TB1")->valuestring,"0") != NULL )
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
				TB1 = 0;
			}
			else if(strstr( cJSON_GetObjectItem(str_json, "TB1")->valuestring,"1") != NULL )
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);
				TB1 = 1;
			}					
		}
		if(cJSON_GetObjectItem(str_json, "TB2")){
			if(strstr( cJSON_GetObjectItem(str_json, "TB2")->valuestring,"0") != NULL )
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
				TB2 = 0;
			}
			else if(strstr( cJSON_GetObjectItem(str_json, "TB2")->valuestring,"1") != NULL )
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
				TB2 = 1;
			}					
		}

	}
	cJSON_Delete(str_json); // x?a m?ng json
}
void XulyJson1(char *DataJson) // xu ly data nhan tu SPI
{
	str_json = cJSON_Parse(DataJson);
	if(!str_json)
	{

	}
	else
	{
		if(cJSON_GetObjectItem(str_json, "TB3")){
			if(strstr( cJSON_GetObjectItem(str_json, "TB3")->valuestring,"0") != NULL )
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				TB3 = 0;
			}
			else if(strstr( cJSON_GetObjectItem(str_json, "TB3")->valuestring,"1") != NULL )
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
				TB3 = 1;
			}					
		}
		if(cJSON_GetObjectItem(str_json, "TB4")){
			if(strstr( cJSON_GetObjectItem(str_json, "TB4")->valuestring,"0") != NULL )
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
				TB4 = 0;
			}
			else if(strstr( cJSON_GetObjectItem(str_json, "TB4")->valuestring,"1") != NULL )
			{
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
				TB4 = 1;
			}					
		}
	}
	cJSON_Delete(str_json); // x?a m?ng json
}
void Received(void)
{
	if (rx_data != '\n')
	{
			rx_buffer[rx_index++] = rx_data;
	}
	else 
	{
		XulyJson(rx_buffer);
		rx_index = 0;
		for (int i = 0; i < 70; i++)
		{
			rx_buffer[i] = 0;
		}
	}
}
void Received1(void)
{
	if (rx_data1 != '\n')
	{
			rx_buffer1[rx_index1++] = rx_data1;
	}
	else 
	{
		XulyJson1(rx_buffer1);
		rx_index1 = 0;
		for (int i = 0; i < 70; i++)
		{
			rx_buffer1[i] = 0;
		}
	}
}
void load_data_SPI1(uint8_t ND, uint8_t DA, uint8_t tb1, uint8_t tb2)
{
	char JSON[50] = "";
	char Str_ND[10] = ""; 
	char Str_DA[10] = "";
	char Str_TB1[10] = "";
	char Str_TB2[10] = "";

	sprintf(Str_ND, "%d", ND); 
	sprintf(Str_DA, "%d", DA);
	sprintf(Str_TB1, "%d", TB1);
	sprintf(Str_TB2, "%d", TB2);

	// {\"TB1\":"1"}
	strcat(JSON, "{\"ND\":\"");
	strcat(JSON, Str_ND); strcat(JSON, "\",");
	strcat(JSON, "\"DA\":\"");
	strcat(JSON, Str_DA); strcat(JSON, "\",");

	strcat(JSON, "\"TB1\":\"");
	strcat(JSON, Str_TB1); strcat(JSON, "\",");
	strcat(JSON, "\"TB2\":\"");
	strcat(JSON, Str_TB2); strcat(JSON, "\"}\n");
	
	for (int i = 0; i < sizeof JSON; i++){
		JSON[i] = HAL_SPI_Transmit(&hspi1, JSON, sizeof JSON, 100); //
	}

}
void load_data_SPI2(uint8_t tb3, uint8_t tb4)
{
	char JSON[50] = "";

	char Str_TB3[10] = "";
	char Str_TB4[10] = "";
	sprintf(Str_TB3, "%d", TB3);
	sprintf(Str_TB4, "%d", TB4);

	// {\"TB1\":"1"}
	
	strcat(JSON, "{\"TB3\":\"");
	strcat(JSON, Str_TB3); strcat(JSON, "\",");
	
	strcat(JSON, "\"TB4\":\"");
	strcat(JSON, Str_TB4); strcat(JSON, "\"}\n");
	for (int i = 0; i < sizeof JSON; i++){
		JSON[i] = HAL_SPI_Transmit(&hspi2, JSON, sizeof JSON, 100); //
	}
}


void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  UNUSED(hspi);
  if(hspi->Instance == SPI2)
	{
		Received();
		HAL_SPI_Receive_IT(&hspi2, &rx_data, 1); // cong SPI2, data, size
	}
	else if(hspi->Instance == SPI1)
	{
		Received1();
		HAL_SPI_Receive_IT(&hspi1, &rx_data1, 1); // cong SPI1, data, size
	}
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // call back interupt timer
{

	if(htim->Instance == htim1.Instance ){
		Read_dht11(); // timer 1 read DHT11.
		load_data_SPI1(ND, DA, TB1, TB2);
		load_data_SPI2(TB3, TB4);
	}
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

 HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	

	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);	
	
	HAL_SPI_Receive_IT(&hspi1, &rx_data1, 1); // cong SPI1, data, size
	HAL_SPI_Receive_IT(&hspi2, &rx_data, 1); // cong SPI, data, size1
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(500);
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_SLAVE;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 8000;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 499;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
