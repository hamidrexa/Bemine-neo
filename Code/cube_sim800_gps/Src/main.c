/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "SEGGER_RTT.h" 
#include <stdio.h>
#include <string.h>
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
 HAL_StatusTypeDef uart_stat;	char print[30];
char header[93]= "POST /input2.php HTTP/1.1\r\nHost: www.fdli.ir\r\nContent-Type:application/json;\r\nContent-Length:";
char* data="data";
int tim1=2000;
void SystemClock_Config(void);
void init_gsm(void);
int find(char* str,char*sub);
void send(char*pm);
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
  	
	char rec[100];
	while(!((GSM_STATUS_GPIO_Port->IDR)&GSM_STATUS_Pin)){
			HAL_UART_Transmit(&huart1, (uint8_t*)"try to turn on..\r\n", COUNTOF("try to turn on..\r\n"), 5000);
			
			GSM_POWER_KEY_GPIO_Port->ODR&=(~GSM_POWER_KEY_Pin);		
			HAL_Delay(1000);
			
		}
	GSM_POWER_KEY_GPIO_Port->ODR|=GSM_POWER_KEY_Pin;
//	HAL_UART_Transmit(&huart1, (uint8_t*)"ATZ0\r\n", COUNTOF("ATZ0\r\n"), 5000);
//	HAL_Delay(10000);
	HAL_UART_Transmit(&huart1, (uint8_t*)"ATE0\r\n", COUNTOF("ATE0\r\n"), 5000);
	HAL_Delay(1000);
  while (1)
  {
//		if((GPIOA->IDR)&GPIO_PIN_10){
//		GPIOA->ODR |=((GPIOA->IDR)&GPIO_PIN_10)>>8;
//		//for(int i=0;i<100;i++)rec[i]=0;
//		
//		//init_gsm();
//		}
//		else{
//			GPIOA->ODR &=((((GPIOA->IDR)&GPIO_PIN_10)>>8)+0xfb);
//		}
		GPIOA->ODR &=0xffb;
		
  }//endwhile
  
}//endmain



void init_gsm(void){
	char socket[45]="AT+CIPSTART=\"TCP\",\"www.fdli.ir\",\"80\"\r\n";
	char rec[100];
	char rec2[100];
	uint8_t z=0x1a; 
	for(int i=0;i<100;i++)rec[i]=0;
	
	for(int i=0;i<100;i++)rec2[i]=0;
	HAL_Delay(tim1*2);
//	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+COPS=4,2,\"43235\"\r\n", COUNTOF("AT+COPS=4,2,\"43235\"\r\n"), 5000);
// 
//	//send("AT+COPS=4,2,\"43235\"\r\n");
//	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CPIN?\r\n", COUNTOF("AT+CPIN?\r\n"), 5000);
//	HAL_Delay(tim1);
//	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CREG?\r\n",COUNTOF("AT+CREG?\r\n"), 5000);
//	HAL_Delay(tim1);
//	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CGACT?\r\n", COUNTOF("AT+CGACT?\r\n"), 5000);
//	HAL_Delay(tim1);
//	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CMEE=1\r\n",COUNTOF("AT+CMEE=1\r\n"), 1);
//	HAL_Delay(tim1);
//	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CSTT=\"bluevia.movistar.es\"\r\n", COUNTOF("AT+CSTT=\"bluevia.movistar.es\"\r\n"), 5000);
//	HAL_Delay(tim1);
//	HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CIICR\r\n", COUNTOF("AT+CIICR\r\n"), 5000);
	HAL_Delay(tim1);
	HAL_UART_Transmit(&huart1, (uint8_t*)socket, COUNTOF(socket), 3);
	HAL_Delay(tim1);
	HAL_UART_Transmit(&huart1, (uint8_t*)socket, COUNTOF(socket), 3);

	__HAL_UART_CLEAR_IT(&huart1,UART_CLEAR_NEF|UART_CLEAR_OREF);
	uart_stat=HAL_UART_Receive_IT(&huart1,(uint8_t*)rec,30);
		SEGGER_RTT_WriteString(0,"\nr1:");
		SEGGER_RTT_WriteString(0,rec);
		SEGGER_RTT_WriteString(0,"\n");
		
		while(find(rec,"ALREADY")){
			//connection stablished
				HAL_UART_Transmit(&huart1, (uint8_t*)"AT+CIPSEND\n", COUNTOF("AT+CIPSEND\n"), 5000);
				HAL_UART_Receive_IT(&huart1,(uint8_t*)rec,10);
				if(find(rec,">")!= NULL){
				HAL_UART_Transmit(&huart1, (uint8_t*) header , strlen(header), 5000);
				sprintf(print,"%d\r\n\r\n",strlen(data));
				HAL_UART_Transmit(&huart1, (uint8_t*) print , strlen(print), 5000);
				HAL_UART_Transmit(&huart1, (uint8_t*) data , strlen(data), 5000); 
				HAL_UART_Transmit(&huart1, &z,2, 5000);
				HAL_Delay(tim1);

				}
				

		}
		//else 	HAL_Delay(tim1*30);
		
		
}
void send(char*pm){
	char  rec[100];
	HAL_Delay(tim1);
	for(int i=0;i<100;i++)rec[i]=0;
		HAL_UART_Transmit(&huart1, (uint8_t*)pm, COUNTOF(pm), 5000);
		__HAL_UART_CLEAR_IT(&huart1,UART_CLEAR_NEF|UART_CLEAR_OREF);
		uart_stat=HAL_UART_Receive_IT(&huart1,(uint8_t*)rec,30);
		SEGGER_RTT_WriteString(0,"\nreceive:");
		SEGGER_RTT_WriteString(0,rec);
}
int find(char* str,char*sub){
	

  char *p1, *p2, *p3;
  int i=0,j=0,flag=0;

  p1 = str;
  p2 = sub;

  for(i = 0; i<strlen(str); i++)
  {
    if(*p1 == *p2)
      {
          p3 = p1;
          for(j = 0;j<strlen(sub);j++)
          {
            if(*p3 == *p2)
            {
              p3++;p2++;
            } 
            else
              break;
          }
          p2 = sub;
          if(j == strlen(sub))
          {
             flag = 1;     
						sprintf(print,"\nSubstring found at index : %d\n",i);
						SEGGER_RTT_WriteString(0,print);
          }
      }
    p1++; 
  }
  if(flag==0)
  {
       SEGGER_RTT_WriteString(0,"Substring NOT found");
		return 0;
  }
	return 1;
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
