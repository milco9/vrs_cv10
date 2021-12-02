/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void proccesDmaData(uint8_t* sign,uint16_t len);
void calculateMemory();
void sendData(uint8_t* data,uint16_t len);
void pwmToLed(uint8_t* sign,uint16_t len);
int checkMode(uint8_t* sign,uint16_t len);
char *convert (uint8_t *a);
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
  //uint8_t tx_data[] = "Data to send over UART DMA!\n\r";
 // uint8_t rx_data[10];
 // uint8_t count = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int mode =2; // 2 = Manual 1 = Automatically  0-none


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
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  USART2_RegisterCallback(proccesDmaData);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* USER CODE BEGIN 3 */
  while (1)
  {
    /* USER CODE END WHILE */

	//USART2_PutBuffer(tx_data, sizeof(tx_data));
	//LL_mDelay(1000);
	  calculateMemory();
	  LL_mDelay(1000);

  }

}


/* USER CODE END 3 */
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}

/* USER CODE BEGIN 4 */


void proccesDmaData(uint8_t* sign,uint16_t len)
{
	uint8_t *tx_data;
	const char automatic[6]="$auto$";
	const char manually[8]="$manual$";
	char *ret;
	int result=2;

     // ret=strstr(sign,automatic);
     // int len_data = asprintf(&tx_data, "%s\n\r",ret);
      //			sendData(tx_data,len_data);
      //			free(tx_data);
      //result=strcmp(ret,automatic);
      //if(result==0){
    //	  mode=1;
      //}

	mode=checkMode(sign,len);


	if (mode == 1){
		int len_data = asprintf(&tx_data, "Mode is set to:  automatic\n\r");
		sendData(tx_data,len_data);
		free(tx_data);
	}
	if (mode == 2){
		pwmToLed(sign,len);
		LL_mDelay(50);
		int len_data = asprintf(&tx_data, "Mode is set to:  Manually\n\r");
		sendData(tx_data,len_data);
		free(tx_data);
	}

	if (mode == 0){
		int len_data = asprintf(&tx_data, "Mode is set to:  none\n\r");
		sendData(tx_data,len_data);
		free(tx_data);
	}
}

void pwmToLed(uint8_t* sign,uint16_t len){
	uint8_t *tx_data;
	int count=0;
	int startBit=0;
	int number=0;
	char *ptr;
	number=atoi(str);
	char breakset[] = "0123456789";
	int len_data = asprintf(&tx_data, "The brightness is set to: : %d %\n\r",number);
									sendData(tx_data,len_data);
									free(tx_data);

	for(int j=0;j<len;j++){
		str[j] = *(sign+j);
		/*if (*(sign+j)=='P'&&startBit==0){
			startBit=1;
		}

		if(startBit==1 && (*(sign+j)=='W') && (count == 0)){
				count++;
		}
			if(startBit==1 && (*(sign+j)=='M') && (count == 1)){
				count++;
				number=atoi(sign);
				int len_data = asprintf(&tx_data, "The brightness is set to: : %d %\n\r",number);
						sendData(tx_data,len_data);
						free(tx_data);
			}*/
	}
	number = atoi(strpbrk(str, breakset));
	count=0;
}

int checkMode(uint8_t* sign,uint16_t len){
	int count=0;
	int startBit=0;
	for(int j=0;j<len;j++){
			if (*(sign+j)=='$'&&startBit==0){
				startBit=1;

			}

			if(startBit==1 && (*(sign+j)=='a') && (count == 0)){
					count++;

				}
				if(startBit==1 && (*(sign+j)=='u') && (count == 1)){
					count++;

				}
				if(startBit==1 && (*(sign+j)=='t') && (count == 2)){
					count++;

				}
				if(startBit==1 && (*(sign+j)=='o') && (count == 3)){
					count++;

				}
			    if ((*(sign+j)=='$')&&startBit==1 && (count == 4)){
				startBit=0;
				count=0;
				return 1;
			    }

			    if(startBit==1 && (*(sign+j)=='m') && count == 0){
			    	count++;
			    	}
			    if(startBit==1 && (*(sign+j)=='a') && count == 1){
			    	count++;
			    	}
			    if(startBit==1 && (*(sign+j)=='n') && count == 2){
			    	count++;
			    	}
			    if(startBit==1 && (*(sign+j)=='u') && count == 3){
			    	count++;
			    	}
			    if(startBit==1 && (*(sign+j)=='a') && count == 4){
			    	count++;
			    	}
			    if(startBit==1 && (*(sign+j)=='l') && count == 5){
			    	count++;
			    	}
			    if ((*(sign+j)=='$')&&startBit==1 && count == 6){
			    	startBit=0;
			    	count=0;
			    	return 2;
			    	}
   }
	return mode;

}

char *toString(uint8_t* sign,uint16_t len)
{
	char *str;
	for(int j=0;j<len;j++){
		*(str+j) = *(sign+j);
	}
	return *str;
}

void sendData(uint8_t* data,uint16_t len)
{
	USART2_PutBuffer(data, len);
}

void calculateMemory()
{
	uint16_t size;
	uint16_t occupied;
	uint8_t *tx_data;
	uint16_t len_data;
	int percent = 0;


	size = DMA_USART2_BUFFER_SIZE;
	occupied = DMA_USART2_BUFFER_SIZE - LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);
	percent = 100.0/size*occupied;
	//len_data = asprintf(&tx_data, "Buffer capacity: %3d bytes, occupied memory: %3d bytes, load [in ~%%]: %3d%%\n\r",size,occupied,percent);
	//sendData(tx_data,len_data);
	//free(tx_data);
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
