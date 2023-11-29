/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "croutine.h"
#include "test.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "test_queue.h"
#include "test_task.h"
#include "test_misc.h"
#include "test_benchmark.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/**
 * The scope defines what tasks are going to be compiled.
 * The currently defined scopes are:
 * - 0: no benchmarks
 * - 1: Microbenchmarks
 * - 2: DES performance benchmark
 * - 3: MatMult performance benchmark
 * - 4: Lift performance benchmark
*/
#ifndef SCOPE
  #define SCOPE 1
#endif
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
void vTaskCode1( void * pvParameters );
void vTaskCode2( void * pvParameters );
void vTaskDES( void * pvParameters );
void vTaskMM(void * pvParameters);
void vTaskLift(void * pvParameters);
void vTaskCRC(void * pvParameters);
void vTaskSHA(void * pvParameters);
void vTaskDone(void * pvParameters);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
__attribute__((annotate("include"))) int main(void)
{
  /* USER CODE BEGIN 1 */
  
  // Create tasks below
  //      | | |
  //      V V V
  int xCanStartScheduler = 0;
#if SCOPE == 1 // microbenchmarks
  BaseType_t xRet5 = xTaskCreate( vTaskTaskTest, "task", 700, NULL, tskIDLE_PRIORITY + 5, NULL );
  BaseType_t xRet6 = xTaskCreate( vTaskBufferTestReceive, "buf1", 500, NULL, tskIDLE_PRIORITY+7, NULL); 
  BaseType_t xRet7 = xTaskCreate( vTaskBufferTestSend, "buf2", 700, NULL, tskIDLE_PRIORITY+6, NULL);
  BaseType_t xRet8 = xTaskCreate( vTaskTimerTest, "timer", 700, NULL, tskIDLE_PRIORITY+8, NULL); 
  BaseType_t xRetQ = xTaskCreate( vTaskTestQueue, "queue", 500, NULL, tskIDLE_PRIORITY + 3, NULL);
  BaseType_t xRetD = xTaskCreate( vTaskDone, "done", 200, NULL, tskIDLE_PRIORITY+10, NULL);
  if (xRetQ & xRet5 & xRet6 & xRet7 & xRet8 & xRetD == pdPASS)

#elif SCOPE == 2 // DES benchmark
  BaseType_t xRet = xTaskCreate(vTaskDES, "benchmark", 3000, NULL, tskIDLE_PRIORITY + 8, NULL);
  if (xRet == pdPASS)

#elif SCOPE == 3 // MatMult benchmark
  BaseType_t xRet = xTaskCreate(vTaskMM, "benchmark", 3000, NULL, tskIDLE_PRIORITY + 8, NULL);
  if (xRet == pdPASS)
#elif SCOPE == 4 // Lift benchmark
  BaseType_t xRet = xTaskCreate(vTaskLift, "benchmark", 3000, NULL, tskIDLE_PRIORITY + 8, NULL);
  if (xRet == pdPASS)
#elif SCOPE == 5
  BaseType_t xRet = xTaskCreate(vTaskCRC, "benchmark", 3000, NULL, tskIDLE_PRIORITY + 8, NULL);
  if (xRet == pdPASS)
#elif SCOPE == 6
  BaseType_t xRet = xTaskCreate(vTaskSHA, "benchmark", 3000, NULL, tskIDLE_PRIORITY + 8, NULL);
  if (xRet == pdPASS)
#endif
    xCanStartScheduler = 1;
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
  MX_TIM6_Init();

  /* USER CODE BEGIN 2 */
  if (xCanStartScheduler)
    vTaskStartScheduler();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // we shouldn't reach this line :)
    for (int i=0; i<100000; i++);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
__attribute__((annotate("exclude"))) void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
__attribute__((annotate("exclude"))) static void MX_USART2_UART_Init(void)
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
__attribute__((annotate("exclude"))) static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
__attribute__((annotate("exclude"))) static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}


/* Task to be created. */
//__attribute__((annotate("task"))) 
__attribute__((annotate("task"))) void vTaskCode1( void * pvParameters )
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 5;
    BaseType_t xWasDelayed;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();
    for( ;; )
    {
      // Wait for the next cycle.
      xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );

      // Perform action here. xWasDelayed value can be used to determine
      // whether a deadline was missed if the code here took too long.
      if (xWasDelayed)
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    }
}

/* Task to be created. */
//__attribute__((annotate("task"))) 
__attribute__((annotate("task"))) void vTaskCode2( void * pvParameters )
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 5;
    BaseType_t xWasDelayed;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();
    for( ;; )
    {
      // Wait for the next cycle.
      xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
      
      // Perform action here. xWasDelayed value can be used to determine
      // whether a deadline was missed if the code here took too long.
      if (xWasDelayed)
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

    }
}

int benchmark_res(int res) {
  return res;
}

/* Task to be created. */
__attribute__((annotate("include"))) void vTaskDES( void * pvParameters )
{
    vTaskDelay(50);
    
    for( ;; )
    {
      int start = xTaskGetTickCount();
      for (int i=0; i<4000; i++) {
        des_init();
      }
      int end = xTaskGetTickCount();
      benchmark_res(end-start);
    }
}

/* Task to be created. */
__attribute__((annotate("include"))) void vTaskMM( void * pvParameters )
{
    vTaskDelay(100);
    
    for( ;; )
    {
      int start = xTaskGetTickCount();
      for (int i=0; i<1000; i++) {
        mm_init();
      }
      int end = xTaskGetTickCount();
      benchmark_res(end-start);
    }
}

void vTaskLift(void * pvParameters) {
  vTaskDelay(100);
    
    for( ;; )
    {
      int start = xTaskGetTickCount();
      for (int i=0; i<1000; i++) {
        liftTask_init();
      }
      int end = xTaskGetTickCount();
      benchmark_res(end-start);
    }
}

void vTaskCRC(void * pvParameters) {
  vTaskDelay(100);
    
  for( ;; )
  {
    int start = xTaskGetTickCount();
    for (int i=0; i<1000; i++) {
      do_crc();
    }
    int end = xTaskGetTickCount();
    benchmark_res(end-start);
  }
}

void vTaskSHA(void * pvParameters) {
  vTaskDelay(100);
    
  for( ;; )
  {
    int start = xTaskGetTickCount();
    for (int i=0; i<250; i++) {
      do_sha();
    }
    int end = xTaskGetTickCount();
    benchmark_res(end-start);
  }
}

// periodically call done() to assure gdb that everything is going fine
void vTaskDone(void * pvParameters) {
  for ( ;; ) {
    vTaskDelay(200);
    done();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
__attribute__((annotate("exclude"))) void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

__attribute__((annotate("exclude"))) void DataCorruption_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

__attribute__((annotate("exclude"))) void SigMismatch_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


void assertionFailed() {
  for (;;);
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
