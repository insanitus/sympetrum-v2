/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
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
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"


/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
 * @brief  This function handles TIM16 interrupt request.
 * @param  None
 * @retval None
 */
void TIM16_IRQHandler(void)
{
   if(RFDemoStatus == RC5DEMO)
   {
      RC5_Encode_SignalGenerate(RC5_FrameManchestarFormat);
   }

   /* Clear TIM16 update interrupt */
   TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
}
/**
 * @brief  This function handles TIM14 overflow and update interrupt request.
 * @param  None
 * @retval None
 */
void TIM2_IRQHandler(void)
{
   /* Clear the TIM2 Update pending bit */
   TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

   if(RFDemoStatus == RC5DEMO)
   {
      /* - Timer Falling Edge Event:
       *     The Timer interrupt is used to measure the period between two 
       *     successive falling edges (The whole pulse duration).
       *
       * - Timer Rising Edge Event:  
       *     It is also used to measure the duration between falling and rising 
       *     edges (The low pulse duration).
       *     The two durations are useful to determine the bit value. Each bit is 
       *     determined according to the last bit. 
       *
       *  Update event:InfraRed decoders time out event.
       *  ---------------------------------------------
       *     It resets the InfraRed decoders packet.
       *     - The Timer Overflow is set to 3.6 ms .*/
      /* IC1 Interrupt*/
      if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC2) != RESET))
      {
         TIM_ClearFlag(IR_TIM, TIM_FLAG_CC2);
         /* Get the Input Capture value */
         ICValue2 = TIM_GetCapture2(IR_TIM);
         /* RC5 */
         RC5_DataSampling( ICValue2 - ICValue1 , 0);

      }  /* IC2 Interrupt */   
      else  if((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_CC1) != RESET))
      {
         TIM_ClearFlag(IR_TIM, TIM_FLAG_CC1);
         /* Get the Input Capture value */
         ICValue1 = TIM_GetCapture1(IR_TIM);
         RC5_DataSampling(ICValue1 , 1);
      } 
      /* Checks whether the IR_TIM flag is set or not.*/
      else if ((TIM_GetFlagStatus(IR_TIM, TIM_FLAG_Update) != RESET))
      { 
         /* Clears the IR_TIM's pending flags*/
         TIM_ClearFlag(IR_TIM, TIM_FLAG_Update);

         RC5_ResetPacket(); 
      }
   }
   else
   {}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/