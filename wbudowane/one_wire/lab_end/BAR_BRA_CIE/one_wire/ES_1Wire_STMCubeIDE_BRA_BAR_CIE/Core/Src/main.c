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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define DQ			DS18B20_Pin					// Definicja pinu dla magistrali 1-Wire
												// Do wyboru:	iButton_Pin - DS1920
												// 				DS18B20_Pin - DS18B20
#define DQ_Port		DS18B20_GPIO_Port			// Definicja portu dla magistrali 1-Wire
												// Do wyboru:	iButton_GPIO_Port - DS1920
												// 				DS18B20_GPIO_Port - DS18B20
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char SendRSData[20];						// Bufor na dane wysylane przez UART2
char DQ_Value;								// Pomocnicza zmienna do przechowywania wartości z linii DQ
uint8_t Iter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void Write_DQ(uint8_t);		// Funkcja do zapisu lini danych DQ
char Read_DQ(void);			// Funkcja do odczytu lini danych DQ
void delay_us(uint16_t);   	// Funkcja do opóźnienia poniżej 1ms
void send_command(uint8_t, char); // Funkcja wysyłająca komendę
char initialize(void);
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
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim1);
  char serial_number[64];
  char val;
  char tablica_czarow[16];
  char fireball[72];
  int ser;
  float hot;
  char crc;
  char my_crc;
  char X;
  char TH;
  char TL;
  char RK;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

//	  send_command(0x33, 0);
//	  delay_us(100);
//
//	  for(int i = 0; i < 64; i++){
//		  serial_number[i] = 0;
//		  Write_DQ(0);
//		  delay_us(10);
//		  Write_DQ(1);
//		  delay_us(10);
//		  val = Read_DQ();
//		  serial_number[i] = val;
//		  delay_us(20);
////		  sprintf(SendRSData, "%d", val);
////		  HAL_UART_Transmit( &huart2, (uint8_t *) SendRSData, strlen(SendRSData),100 );
//	  }
//	  ser = 0;
//	  for(int i = 0; i < 64; i++){
//		  ser = ser + (serial_number[i] << (i));
//	  }
//	  for(int i = 15; i >= 0; i--){
//		  tablica_czarow[i] = 0;
//		  for(int j = 0; j < 4; j++){
//			  tablica_czarow[i] += (serial_number[4*i+j]) << j;
//		  }
//		  sprintf(SendRSData, "%x", tablica_czarow[i]);
//		  HAL_UART_Transmit( &huart2, (uint8_t *) SendRSData, strlen(SendRSData),100 );
//	  }
//	  sprintf(SendRSData, "\n\r");
//	  HAL_UART_Transmit( &huart2, (uint8_t *) SendRSData, strlen(SendRSData),100 );
//	  HAL_Delay(100);
	  //33h <- instukcja do adresu
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  send_command(0xCC, 0);

	  delay_us(100);

	  send_command(0x44, 1);

	  HAL_Delay(100);

	  send_command(0xCC, 0);

	  delay_us(50);

	  send_command(0xBE, 1);

	  delay_us(50);

	  for(int i = 0; i < 72; i++){
	  		  fireball[i] = 0;
	  		  Write_DQ(0);
	  		  delay_us(10);
	  		  Write_DQ(1);
	  		  delay_us(10);
	  		  val = Read_DQ();
	  		  fireball[i] = val;
	  		  delay_us(20);
	  //		  sprintf(SendRSData, "%d", val);
	  //		  HAL_UART_Transmit( &huart2, (uint8_t *) SendRSData, strlen(SendRSData),100 );
	  	  }
	  ser = 0;
//	  for(int i = 1; i < 10; i++){   // 11 bit
//	  		  ser = ser + (fireball[i] << (i-1));
//	  	  }
	  for(int i = 3; i < 10; i++){   //  9 bit
		  ser = ser + (fireball[i] << (i-3));
	  }
	  int sum = 0;
	  for(int i = 10; i < 16; i++){
	  		  sum = sum + fireball[i];
	  }
	  if(sum >= 3){
		  ser = ser*(-1);
	  }
	  hot = 0;
	  hot = (float)ser/2;   // 9 bit
	  sprintf(SendRSData, "%3.1f \n\r", hot);

//	  hot  = (float)ser/16; //11 bit
//	  sprintf(SendRSData, "%3.3f \n\r", hot);
	  HAL_UART_Transmit( &huart2, (uint8_t *) SendRSData, strlen(SendRSData),100 );

	  crc = 0;
	  for(int i = 64; i < 72; i++){
			  crc = crc + (fireball[i] << (i-64));
	  }

	  my_crc = 0;
	  X = 0b00000000;
	  for(int i = 0; i < 64;i++){
		  char xor = 0;
		  if(fireball[i] != X%2){
			  xor = 1;
		  }
		  X = X>>1;
		  X += xor<<7;
		  X = X^(xor*0b00001100);
	  }
	  my_crc = X;

	  sprintf(SendRSData, "%d |-| %d\n\r", my_crc, crc);
	  HAL_UART_Transmit( &huart2, (uint8_t *) SendRSData, strlen(SendRSData),100 );

	  for(int i = 16; i < 24; i++){
		  TH = TH + (fireball[i] << (i-16));
	  }
	  for(int i = 24; i < 32; i++){
		  TL = TL + (fireball[i] << (i-24));
	  }

	  RK = 0b00011111;

	  send_command(0xCC, 0);
	  delay_us(50);
	  send_command(0x4E, 1);
	  delay_us(50);
	  send_command(TH, 1);
	  send_command(TL, 1);
	  send_command(RK, 1);


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
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
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
  htim1.Init.Prescaler = 99;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DS18B20_Pin|iButton_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DS18B20_Pin */
  GPIO_InitStruct.Pin = DS18B20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : iButton_Pin LD2_Pin */
  GPIO_InitStruct.Pin = iButton_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
// Zapis linii danych magistrali 1-Wire
void Write_DQ(uint8_t DQValue)
{
	if (DQValue == 0) HAL_GPIO_WritePin(DQ_Port,DQ,GPIO_PIN_RESET);
	else HAL_GPIO_WritePin(DQ_Port,DQ,GPIO_PIN_SET);
}
// END - Zapis linii danych

// Odczyt linii danych magistrali 1-Wire
char Read_DQ(void)
{
	return HAL_GPIO_ReadPin(DQ_Port,DQ);
}
// END - Odczyt linii danych

// Init
char initialize(void)
{
	Write_DQ(0);
	delay_us(1000);
	Write_DQ(1);
	delay_us(90);
	DQ_Value = Read_DQ();
	delay_us(1000);
	sprintf(SendRSData, "PUE-2020: %d \n\r", DQ_Value);
	HAL_UART_Transmit( &huart2, (uint8_t *) SendRSData, strlen(SendRSData),100 );
	return 1 - DQ_Value;
}
// END - Init

//  Opóżnienie poniżej 1ms
void delay_us(uint16_t us)
{
	__HAL_TIM_SetCounter(&htim1,0);
	while(__HAL_TIM_GetCounter(&htim1) < us){};
}
// END - Opóźnienie

// Wysyłanie komendy
void send_command(uint8_t command, char second)
{
	if(second || initialize()){
		for(int i = 0; i < 8; i++){
				Write_DQ(0);
				delay_us(10);
				if((command >> i) % 2){
					Write_DQ(1);
					delay_us(60);
				}else{
					delay_us(60);
				}
				Write_DQ(1);
				delay_us(10);
			}
	}else{
		sprintf(SendRSData, "JEST_ZLE \n\r");
		HAL_UART_Transmit( &huart2, (uint8_t *) SendRSData, strlen(SendRSData),100 );
	}

}
// END - komenda
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

