/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "calibrate_task.h"
#include "chassis_task.h"
#include "detect_task.h"
#include "naive_gimbal_task.h"
#include "INS_task.h"
#include "led_flow_task.h"
#include "oled_task.h"
#include "referee_usart_task.h"
#include "usb_task.h"
#include "voltage_task.h"
#include "servo_task.h"
#include "robot_total_mode.h"
#include "naive_shoot_task.h"
#include "communication_task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

//osThreadId calibrate_tast_handle;
//osThreadId chassisTaskHandle;
osThreadId detect_handle;
osThreadId gimbalTaskHandle;
osThreadId imuTaskHandle;
osThreadId led_RGB_flow_handle;
//osThreadId oled_handle;
//osThreadId referee_usart_task_handle;
//osThreadId usb_task_handle;
//osThreadId battery_voltage_handle;
osThreadId servo_task_handle;
osThreadId totalModeTask_handle;
osThreadId shootTaskTask_handle; 
osThreadId shootTaskTrgMntr_handle;
osThreadId communicationTask_handle;
osThreadId rcDetectTask_handle;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
//osThreadId testHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void start_INS_task(void const * argument);
/* USER CODE END FunctionPrototypes */

void test_task(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];
  
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )  
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}                   
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of test */
//  osThreadDef(test, test_task, osPriorityNormal, 0, 128);
//  testHandle = osThreadCreate(osThread(test), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    // osThreadDef(cali, calibrate_task, osPriorityNormal, 0, 512);
    // calibrate_tast_handle = osThreadCreate(osThread(cali), NULL);

//    osThreadDef(ChassisTask, chassis_task, osPriorityAboveNormal, 0, 512);
//    chassisTaskHandle = osThreadCreate(osThread(ChassisTask), NULL);

//---------------------------

    osThreadDef(DETECT, detect_task, osPriorityNormal, 0, 512);
    detect_handle = osThreadCreate(osThread(DETECT), NULL);
		
		osThreadDef(COMMUNICATION, communication_task, osPriorityRealtime, 0, 512);
    communicationTask_handle = osThreadCreate(osThread(COMMUNICATION), NULL);

    osThreadDef(gimbalTask, gimbal_task, osPriorityHigh, 0, 1024);
    gimbalTaskHandle = osThreadCreate(osThread(gimbalTask), NULL);

    osThreadDef(imuTask, start_INS_task, osPriorityRealtime, 0, 1024); // start_INS_task进程在下面
    imuTaskHandle = osThreadCreate(osThread(imuTask), NULL);

    osThreadDef(led, led_RGB_flow_task, osPriorityNormal, 0, 256); // 文件名叫led_flow_task
    led_RGB_flow_handle = osThreadCreate(osThread(led), NULL);

//------------------

//    osThreadDef(OLED, oled_task, osPriorityLow, 0, 512);
//    oled_handle = osThreadCreate(osThread(OLED), NULL);


//    osThreadDef(REFEREE, referee_usart_task, osPriorityNormal, 0, 128);
//    referee_usart_task_handle = osThreadCreate(osThread(REFEREE), NULL);


//    osThreadDef(USBTask, usb_task, osPriorityNormal, 0, 256);
//    usb_task_handle = osThreadCreate(osThread(USBTask), NULL);

//    osThreadDef(BATTERY_VOLTAGE, battery_voltage_task, osPriorityNormal, 0, 128);
//    battery_voltage_handle = osThreadCreate(osThread(BATTERY_VOLTAGE), NULL);

//----------------

    osThreadDef(SERVO, servo_task, osPriorityNormal, 0, 128);
    servo_task_handle = osThreadCreate(osThread(SERVO), NULL);

    osThreadDef(totalModeTask, robot_total_mode_task, osPriorityHigh, 0, 256);
    totalModeTask_handle = osThreadCreate(osThread(totalModeTask), NULL);

    osThreadDef(shootTask, shoot_task, osPriorityHigh, 0, 512);
    shootTaskTask_handle = osThreadCreate(osThread(shootTask), NULL); // 文件名叫naive_shoot_task

    osThreadDef(shootTrgMonitor, shootTaskTrggMonitor, osPriorityRealtime, 0, 128); // 文件名叫naive_shoot_task
    shootTaskTrgMntr_handle = osThreadCreate(osThread(shootTrgMonitor), NULL);

//------------------

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_test_task */
/**
  * @brief  Function implementing the test thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_test_task */
__weak void test_task(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN test_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END test_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void start_INS_task(void const * argurment)
{
	INS_Init();
	while(1){
		INS_Task();
		osDelay(1);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
