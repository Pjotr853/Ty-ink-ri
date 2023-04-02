/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under SLA0094 license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        https://www.st.com/SLA0094
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <callbacks.h>
#include <stdbool.h>
#include "led12a1.h"
#include <bridge.h>
#include <callbacks.h>
#include <stdbool.h>
//#include "led1202.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//#define GUI_MODE           /*Define it for running it in GUI MODE*/
#define MAX_CH_CURRENT ((uint8_t)20)    /*In Mili Ampere*/
//#define ACTUAL_CH_CURRENT ((uint8_t)0)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t GuiMode = 0;
uint8_t DevAddrArray[10];
uint8_t NumOfDev = 0;
extern  uint8_t bridge_IN_buffer[];
extern  uint8_t bridge_OUT_buffer[];
static void MX_USART2_UART_Init(void);
extern volatile uint8_t command_triggered;
extern   LED1202_Object_TypeDef LED1202Obj;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
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
#ifndef GUI_MODE
  uint16_t  digCurrReg = 0;
  uint8_t dev = 0;
  uint8_t color = 0;
  uint8_t cnt;
#endif
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  __HAL_TIM_CLEAR_IT(&htim2,TIM_SR_UIF);
  __HAL_TIM_CLEAR_IT(&htim3,TIM_SR_UIF);
  
  /*Wait Time assuring a properly steady state condition */
  HAL_Delay(100);
  
  interface_mode = UART_MODE;	/*Set a known condition */
  
  /*Clear Buffer*/
  ClearBuffers();
  
  /*LED12A1 Initialization*/ 
  LED12A1_Init();
  

  /*Scan for Number of available LED1202 ICs*/ 
  NumOfDev =  LED12A1_ScanAvailDevice( &LED1202Obj ,DevAddrArray);
#ifdef GUI_MODE
  HAL_UART_Receive_IT(&huart2,&bridge_OUT_buffer[0],64); 
#else  
  /*Enable available LED1202 ICs*/ 
  for (uint8_t dev = 0;dev<NumOfDev;dev++)
  {
    LED12A1_DeviceEnable(&LED1202Obj , (TypedefEnumDevAddr)(LED_DEVICE1 + dev));
    LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
  }
#endif


  uint16_t ACTUAL_CH_CURRENT = 0;
  int8_t direction = 1; // 1 for increasing, -1 for decreasing
  TypeDefChannel colors[] = {LED_CHANNEL_0, LED_CHANNEL_1, LED_CHANNEL_2,LED_CHANNEL_3, LED_CHANNEL_4,LED_CHANNEL_5, LED_CHANNEL_6,LED_CHANNEL_7, LED_CHANNEL_8,LED_CHANNEL_9, LED_CHANNEL_10,LED_CHANNEL_11};

 uint16_t convertRGBto16Bit(uint8_t red, uint8_t green, uint8_t blue) {
        uint16_t red16 = (uint16_t)(red / 8) << 11;
        uint16_t green16 = (uint16_t)(green / 4) << 5;
        uint16_t blue16 = (uint16_t)(blue / 8);
        return red16 | green16 | blue16;
      }

 int i = 0;
 int k = 0;

#define MAX_BRIGHTNESS 255 // Define the maximum brightness level
#define BRIGHTNESS_STEP 5 // Define the step size for each brightness level change
#define DELAY_MS 20 // Define the delay between each brightness step (in milliseconds)


 void setBrightness(uint8_t brightness)
 {
     // Iterate through all devices and channels

     for (uint8_t dev = 0; dev < NumOfDev; dev++)
     {
         for (uint8_t channel = 0; channel <= 11; channel += 3)
         {
             // Calculate the analog dimming value based on the brightness level
             uint8_t analogValue = (brightness * MAX_CH_CURRENT) / MAX_BRIGHTNESS;

             // Enable the channel and set the analog dimming value
             LED12A1_ChannelEnable(&LED1202Obj, (TypeDefChannel)(colors[0] << channel), (TypedefEnumDevAddr)(LED_DEVICE1 + dev));
             LED12A1_AnalogDimming(&LED1202Obj, analogValue, channel, (TypedefEnumDevAddr)(LED_DEVICE1 + dev));
         }
     }
 }


 void colorWaterfall() {
	 // cervena 0, zelena 1, modra 2

	 for (dev = 0;dev<NumOfDev;dev++)
	         {
	           LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
	         }
	         HAL_Delay(10);




	     for (int i = 0; i<3;i++){

	    	 for (dev = 0;dev<NumOfDev;dev++)
	    	 	         {
	    	 	           for (uint8_t channel = 0;channel<=11;channel+=3)
	    	 	           {
	    	 	             LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i]<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));

	    	 	             LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel, (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	    	 	             //HAL_Delay(200);

	    	 	           }


	    	 	         }

	    	 	         HAL_Delay(500);
	    	 	         LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE_ALL));




	         for (dev = 0;dev<NumOfDev;dev++)
	         {
	           for (uint8_t channel = 0;channel<=11;channel+=3)
	           {
	             LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i]),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	             //HAL_Delay(200);
	             LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i+3]),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	             //HAL_Delay(200);
	             LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i+6]),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	             //HAL_Delay(200);
	             LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i+9]),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	             HAL_Delay(20);
	             LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel, (TypedefEnumDevAddr)(LED_DEVICE1+dev));

	           }
	         }
	         for (dev = 0;dev<NumOfDev;dev++)
	                 {
	                   for (uint8_t channel = 0;channel<=11;channel+=3)
	                   {
	                     LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(colors[i]),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	                     //HAL_Delay(200);
	                     LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(colors[i+3]),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	                     //HAL_Delay(200);
	                     LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(colors[i+6]),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	                     //HAL_Delay(200);
	                     LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(colors[i+9]),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	                     HAL_Delay(20);
	                     LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel, (TypedefEnumDevAddr)(LED_DEVICE1+dev));

	                   }
	                 }
	         HAL_Delay(50);
	         }



 }

 void allWhite(){
	 for (dev = 0;dev<NumOfDev;dev++)
	         {
	           LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
	         }
	         HAL_Delay(300);
	         for (dev = 0;dev<NumOfDev;dev++)
	         {
	           for (uint8_t channel = 0;channel<=11;channel++)
	           {
	             LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	             LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel, (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	           }
	         }
	         HAL_Delay(300);

 }

 void scrollThemColors() {

	 for (dev = 0;dev<NumOfDev;dev++)
	         {
	           LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
	         }
	         HAL_Delay(10);


	        for (int i = 0; i < 3; i++) {

	         for (dev = 0;dev<NumOfDev;dev++)
	         {
	           for (uint8_t channel = 0;channel<=11;channel+=3)
	           {
	             LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i]<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));

	             LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel, (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	             //HAL_Delay(200);

	           }


	         }

	         HAL_Delay(500);
	         LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE_ALL));

	        }

 }

 void policeLights(){

	 for (dev = 0;dev<NumOfDev;dev++)
	         {
	           LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
	         }
	         HAL_Delay(10);


	         for (int i = 0; i < 3; i+=2) {
	        	 for (k = 0; k<5;k++) {
	                 for (dev = 0;dev<NumOfDev;dev++)
	                 {
	                   for (uint8_t channel = 0;channel<=11;channel+=3)
	                   {
	                     LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i]<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));

	                     LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel, (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	                   }

	                 }
	                 HAL_Delay(60);
	                     LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE_ALL));
	                 }

	                 HAL_Delay(500);
	                 LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE_ALL));

	                }


 }




 void oneByOne(){
	 for (dev = 0;dev<NumOfDev;dev++)
	         {
	           LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
	         }

	     	for (uint8_t channel = 0; channel <= 11; channel++)

	     	       {
	     	         for (dev = 0; dev < NumOfDev; dev++)
	     	         {
	     	        	 digCurrReg =  4095;
	     	        	         LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	     	        	         LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	     	        	         LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	     	        	         HAL_Delay(100);
	     	        	         LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));

	     	         }
	     	       }
 }

 void topDownSwing(){
	 for (int i=0; i<3; i++){
	      for (dev = 0;dev<NumOfDev;dev++)
	      {
	        for (uint8_t channel = 0;channel<=11;channel+=3)
	        {
	          digCurrReg =  4095;
	          LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i]<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	          LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	          LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	        }
	        HAL_Delay(100);
	        LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_ALL),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	      }
	      for (dev = NumOfDev-1;dev>0;dev-- )
	                     {
	                       for (uint8_t channel = 0;channel<=11;channel+=3)
	                       {
	                         digCurrReg =  4095;
	                         LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i]<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	                         LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	                         LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	                       }
	                       HAL_Delay(100);
	                       LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_ALL),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
	                     }

	 }
 }

 void demoPresentation () {



	 colorWaterfall();

	 allWhite();

	 scrollThemColors();

	 oneByOne();

	 topDownSwing();

	 for (int i=0;i<3;i++){
	 	    	 policeLights();
	 	     }


 }

 	 uint8_t brightness = 0;
	 bool increasing = true;

     bool call911now = false;
     bool start_dimming = false;
     bool demoMode = false;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#ifdef GUI_MODE
    WaitForCommand();
#else

    /*** toto tu strieda RGB farby po jednotlivych led po riadkoch alebo po stlpcoch podla toho ktory for ide prvy ***/


    	/*** Turn ALL leds ON ***/


/*** po riadkoch to ide hore dole AJ TO MNENI FARBYYYYY ***/

//colorWaterfall();

    /*** Whole GRID meni farby | channel numbers | Red = 0, Green = 1, Blue = 2 ***/

/*
for (dev = 0;dev<NumOfDev;dev++)
        {
          LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
        }
        HAL_Delay(10);


       for (int i = 0; i < 3; i++) {

        for (dev = 0;dev<NumOfDev;dev++)
        {
          for (uint8_t channel = 0;channel<=11;channel+=3)
          {
            LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(colors[i]<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));

            LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel, (TypedefEnumDevAddr)(LED_DEVICE1+dev));
            //HAL_Delay(200);

          }


        }

        HAL_Delay(500);
        LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE_ALL));

       }
}
*/




  /*** chaos ***/



 if (call911now) {

	  policeLights();
	  HAL_Delay(150);
  }

 //allWhite();

 if (demoMode) {
	 demoPresentation();
 }


 setBrightness(brightness);
 	               HAL_Delay(DELAY_MS);

 	               if (increasing)
 	                       {
 	                           brightness += BRIGHTNESS_STEP;
 	                           if (brightness >= MAX_BRIGHTNESS)
 	                           {
 	                               increasing = false;
 	                           }
 	                       }
 	                       else
 	                       {
 	                           brightness -= BRIGHTNESS_STEP;
 	                           if (brightness == 0)
 	                           {
 	                               increasing = true;
 	                           }
 	                      }

 /*** top - down ***/





    /*** Turn ON LED one by one ****/
/*
    for (dev = 0;dev<NumOfDev;dev++)
    {
      LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
    }

    for (dev = 0;dev<NumOfDev;dev++)
    {
      for (uint8_t channel = 0;channel<=11;channel++)
      {
        digCurrReg =  4095;
        LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
        LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
        LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
        HAL_Delay(500);
        LED12A1_ChannelDisable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
      }

      HAL_Delay(500);
    }

    for(cnt = 0;cnt<4;cnt++)
    {
*/
    /*** Turn ON/OFF All LED at once in Toggling Mode ****/
/*
    for (dev = 0;dev<NumOfDev;dev++)
    {
      LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
    }
    HAL_Delay(300);
    for (dev = 0;dev<NumOfDev;dev++)
    {
      for (uint8_t channel = 0;channel<=11;channel++)
      {
        LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
        LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
      }
    }
    HAL_Delay(300);
    }
*/
    /*** Start LED from minimum intensity to maximum intensity one by one ****/
    /*
    for (dev = 0;dev<NumOfDev;dev++)
    {
      LED12A1_ChannelDisable( &LED1202Obj , LED_CHANNEL_ALL , (TypedefEnumDevAddr)( LED_DEVICE1 + dev));
    }
    
    for (dev = 0;dev<NumOfDev;dev++)
    {
      for (uint8_t channel = 0;channel<=11;channel++)
      {
        LED12A1_ChannelEnable( &LED1202Obj , (TypeDefChannel)(LED_CHANNEL_0<<channel),  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
        LED12A1_AnalogDimming( &LED1202Obj , MAX_CH_CURRENT, channel,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
        for(uint8_t cnt = 0;cnt<10;cnt++)
        {
          digCurrReg =  400*cnt;
          LED12A1_DigitalDimming( &LED1202Obj ,&digCurrReg,channel,0,  (TypedefEnumDevAddr)(LED_DEVICE1+dev));
          HAL_Delay(100);
        }
      }

      HAL_Delay(300);
    }
  */
#endif
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
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
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
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0010061A;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 9000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 9000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 400;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
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
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

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
