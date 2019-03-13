
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

#include "SEGGER_RTT.h" 
#include <stdio.h>
#include <string.h>
#include <math.h>
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

//#include "sim800.h"
#include "gps.h"
#include "gps_buff.h"
#include "string.h"
/* USER CODE END Includes */
int getHexVal(char charachter)
{
  if(charachter >= '0' && charachter<= '9')
    return charachter - '0';
  else if(charachter >= 'a' && charachter<= 'f')
    return charachter - 'a' + 10;
  else if(charachter >= 'A' && charachter<= 'F')
    return charachter - 'A' + 10;
  else
    return -1;//error
}


char header[98]= "POST /get.php HTTP/1.1\r\nHost: hw.smartbemine.com\r\nContent-Type:application/json;\r\nContent-Length:";
char data[100]="location";
char socket[70]="AT+CIPSTART=\"TCP\",\"hw.smartbemine.com\",\"80\"\r\n";
uint8_t z=0x1a;
int tim1=1000;
char print[100];
char rec[200]; 
double  Langitude;
double  Latiude;
gps_t hgps;
//bool	SendRaw(uint8_t *data,uint16_t len);
//bool	SendString(char *data);
void SystemClock_Config(void);
void my_location(gps_t * hgps);
void Gps_Init(void);
void init_gsm(void);
int find(char* str,char*sub);
void send(char*pm);
int respons(char* res,char* pm);
void send_socket(char* data);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//uint8_t RBuf[64];
//uint8_t TBuf[64];
uint8_t Buf_pointer = 0;
uint8_t recieve_char = 0, serial_print = 0;
void checkpower(void){
	while(!((GSM_STATUS_GPIO_Port->IDR)&GSM_STATUS_Pin)){
		GSM_POWER_KEY_GPIO_Port->ODR|=GSM_POWER_KEY_Pin;
		HAL_Delay(500);
			SEGGER_RTT_WriteString(0,".");
			GSM_POWER_KEY_GPIO_Port->ODR&=(~GSM_POWER_KEY_Pin);		
			HAL_Delay(1000);
		}
	GSM_POWER_KEY_GPIO_Port->ODR|=GSM_POWER_KEY_Pin;
	//z	respons("Ready","");
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
	gps_init(&hgps); 
  /* USER CODE BEGIN 2 */
	//Gsm_Init();
//CHANGE_UART_PINS(0);
		
/* GPS handle  */

/**
 * \brief           Dummy data from GPS receiver
 */
//const char
//gps_rx_data[] = ""
//"$GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F\r\n"
//"$GPRMB,A,,,,,,,,,,,,V*71\r\n"
//"$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75\r\n"
//"$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D\r\n"
//"$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71\r\n"
//"$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77\r\n"
//"$PGRME,22.0,M,52.9,M,51.0,M*14\r\n"
//"$GPGLL,3907.360,N,12102.481,W,183730,A*33\r\n"
//"$PGRMZ,2062,f,3*2D\r\n"
//"$PGRMM,WGS84*06\r\n"
//"$GPBOD,,T,,M,,*47\r\n"
//"$GPRTE,1,1,c,0*07\r\n"
//"$GPRMC,183731,A,3907.482,N,12102.436,W,000.0,360.0,080301,015.5,E*67\r\n"
//"$GPRMB,A,,,,,,,,,,,,V*71\r\n";

int i=0; 
	SEGGER_RTT_WriteString(0, "\nstart\n");
	for(int i=0;i<100;i++)rec[i]=0;
	HAL_Delay(4000);
	checkpower();
//	HAL_UART_Transmit(&huart1,(uint8_t*)"AT&F0\r\n", COUNTOF("AT&F0\r\n"),100);
	init_gsm();
	double x=0;
	int deg=0;
	  while (1)
  {
		 my_location( &hgps);
		Langitude =(double)hgps.longitude;
		Latiude=(double)hgps.latitude;
		deg=(int) Langitude/100;
		Langitude = deg +(fmod(Langitude,100.0000000)/60);
		deg=(int) Latiude/100;
		Latiude = deg +(fmod(Latiude,100.0000000)/60);
	//	sprintf(data,"W:%f\nE:%f", Langitude,Latiude);
		if(Latiude>0){
		sprintf(data,"\nlang:%f---lat:%f\nin this time:%d:%d:%d\n", Langitude,Latiude,hgps.hours,hgps.minutes,hgps.seconds);
		}
		else{
			sprintf(data,"\ngps no signal \nin this time:%d:%d:%d\n",hgps.hours,hgps.minutes,hgps.seconds);
		}
		SEGGER_RTT_WriteString(0, data);
		send_socket(data);
		 
  }
  /* USER CODE END 3 */
}


void send_socket(char* data){
//	send("AT+CIICR\r\n");	
//	send("AT+CIFSR\r\n");
	
	CHANGE_UART_PINS(0);
	send(socket);
	if(respons("CONNECT",socket)){
		send("AT+CIPSEND\n");
		if(respons(">","AT+CIPSEND\n")){
					SEGGER_RTT_WriteString(0,"\nyes:");
					HAL_UART_Transmit(&huart1, (uint8_t*) header , strlen(header), 5000);
					sprintf(print,"%d\r\n\r\n",strlen(data));
					HAL_UART_Transmit(&huart1, (uint8_t*) print , strlen(print), 5000);
					HAL_UART_Transmit(&huart1, (uint8_t*) data , strlen(data), 5000); 
					HAL_UART_Transmit(&huart1, &z,2, 5000);
					HAL_Delay(tim1);
			}
		}
		else{
			send("AT&F0\r\n");
			send("AT+CIPCLOSE\r\n");
			send("AT+CGATT=0\r\n");
			HAL_Delay(tim1*5);
			send("AT+CGATT=1\r\n");
			init_gsm();
		}
	
}
void init_gsm(void){
	
	CHANGE_UART_PINS(0);
		checkpower();
	send("AT+COPS=4,2,\"43235\"\r\n");
	send("AT+CPIN?\r\n");
	send("AT+CSQ\r\n");
	send("ATE0\r\n");
{	send("AT+CREG?\r\n");
	send("AT+CGACT?\r\n");
	send("AT+CMEE=1\r\n");
	send("AT+CGATT=1\r\n");
	//send("AT+CSTT=\"bluevia.movistar.es\"\r\n", COUNTOF("AT+CSTT=\"bluevia.movistar.es\"\r\n"), 5000);
	send("AT+CSTT=\"mtnirancell\",\"\",\"\"\r\n");
	send("AT+CIICR\r\n");	
	send("AT+CIFSR\r\n");
}
int i=0;
		send(socket);
		if(respons("CONNECT",socket)){
			//
		}
		else{
			send("AT&F0\r\n");
			send("AT+CIPCLOSE\r\n");
			send("AT+CGATT=0\r\n");
			HAL_Delay(tim1*5);
			send("AT+CGATT=1\r\n");
		}
		i++;

		
}
void send(char*pm){
		HAL_Delay(tim1);
		for(int i=0;i<100;i++){rec[i]=0;}
		HAL_UART_Transmit(&huart1, (uint8_t*)pm, strlen(pm), 5000);
}
int find(char* str,char*sub){
  char *p1, *p2, *p3;
  int i=0,j=0,flag=0;

  p1 = str;
  p2 = sub;

  for(i = 0; i<100; i++)
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
							SEGGER_RTT_WriteString(0,"\nfound\n");
          }
      }
    p1++; 
  }
  if(flag==0)
  {
       SEGGER_RTT_WriteString(0," \nNOT found\n");
		return 0;
  }
	return 1;
}
void my_location(gps_t* hgps){
	double x;
	CHANGE_UART_PINS(1);
		HAL_UART_Receive_IT(&huart1,(uint8_t*)rec,200);
			HAL_Delay(1000);
			SEGGER_RTT_WriteString(0,"\nr2:");
			SEGGER_RTT_WriteString(0,rec);
			SEGGER_RTT_WriteString(0,"\n");     
			gps_process(hgps, rec, strlen(rec));
}
int respons(char* res,char* pm){
	int try=0;
	while(try<10){
	//		__HAL_UART_CLEAR_IT(&huart1,UART_CLEAR_NEF|UART_CLEAR_OREF);
			HAL_UART_Receive_IT(&huart1,(uint8_t*)rec,200);
			HAL_Delay(1000);
			SEGGER_RTT_WriteString(0,"\nr2:");
			SEGGER_RTT_WriteString(0,rec);
			SEGGER_RTT_WriteString(0,"\n");
			if(find(rec,res))return 1;
			else {
				checkpower();
				for(int i=0;i<100;i++){rec[i]=0;}
				HAL_UART_Transmit(&huart1, (uint8_t*)pm, strlen(pm), 5000);
				try++;
			}
			}
	return 0;
		
	
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
void HAL_UART_TxCpltCallback (UART_HandleTypeDef * huart)
{
//	TBuf[0] = 23;
}
//void Gps_Init(void){//for change buad rate
//	char buffer[100];
//	uint8_t bit=0;
//	
//	for(int i=0;i<100;i++)buffer[i]=0;
//	CHANGE_UART_PINS(1);
//	sprintf(buffer,"B5620600140001000000D0080000802500000700030000000000A2B5");
//	for(int i = 0; i <strlen(buffer) ; i +=2)
//							{
//								bit=getHexVal(buffer[i])*16 + getHexVal(buffer[i+1]);
//								HAL_UART_Transmit(&huart1,&bit,1,5000);//send vaue pm in protocol
//							}
//	for(int i=0;i<100;i++)buffer[i]=0;
//	sprintf(buffer,"B56206090D0000000000FF000000000000031DAB");
//	for(int i = 0; i <strlen(buffer) ; i +=2)
//							{
//								bit=getHexVal(buffer[i])*16 + getHexVal(buffer[i+1]);
//								HAL_UART_Transmit(&huart1,&bit,1,5000);//send vaue pm in protocol
//							}
//	
//							MX_USART1_UART_Init_9600();
//}
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart)
{	
	
//	HAL_UART_Receive_IT(&huart1, &recieve_char, 1);
//	Buf_pointer = (Buf_pointer+1)&0x3F;
//	RBuf[Buf_pointer] = recieve_char;
//	if(Buf_pointer==63) serial_print = 1;
}

//bool	SendRaw(uint8_t *data,uint16_t len)
//{
//	if(len <= _GSM_TX_SIZE)
//	{		
//		memcpy(TBuf,data,len);		
//		if(HAL_UART_Transmit(&_GSM_USART,data,len,5000) == HAL_OK)
//			return true;
//		else
//			return false;
//	}
//	else
//		return false;
//}

//bool	SendString(char *data)
//{
//	return SendRaw((uint8_t*)data,strlen(data));
//}

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
