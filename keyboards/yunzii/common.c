#include "hal.h"
#include "quantum.h"
#include "common.h"

uint32_t exti_flag;
extern uint8_t isr_Trig;
extern uint16_t test_isr_count;

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#undef STM32_DISABLE_EXTI0_HANDLER
#undef STM32_DISABLE_EXTI1_HANDLER
#undef STM32_DISABLE_EXTI2_HANDLER
#undef STM32_DISABLE_EXTI3_HANDLER
#undef STM32_DISABLE_EXTI4_HANDLER
#undef STM32_DISABLE_EXTI5_9_HANDLER
#undef STM32_DISABLE_EXTI10_15_HANDLER

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#define exti_serve_irq(pr, channel) {                                       \
                                                                            \
  if ((pr) & (1U << (channel))) {                                           \
    _pal_isr_code(channel);                                                 \
  }                                                                         \
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (HAL_USE_PAL && (PAL_USE_WAIT || PAL_USE_CALLBACKS)) || defined(__DOXYGEN__)
#if !defined(STM32_DISABLE_EXTI0_HANDLER)
/**
 * @brief   EXTI[0] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector58) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 0);
  EXTI->PR = pr;

#ifdef isr_soruce0
   isr_Trig = 1;

#endif

  exti_serve_irq(pr, 0);

  OSAL_IRQ_EPILOGUE();
}
#endif // exti 0

#if !defined(STM32_DISABLE_EXTI1_HANDLER)
/**
 * @brief   EXTI[1] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector5C) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 1);
  EXTI->PR = pr;

#ifdef isr_soruce1
   isr_Trig = 1;
#endif
  exti_serve_irq(pr, 1);

  OSAL_IRQ_EPILOGUE();
}
#endif // exti 1

#if !defined(STM32_DISABLE_EXTI2_HANDLER)
/**
 * @brief   EXTI[2] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector60) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 2);
  EXTI->PR = pr;
#ifdef isr_soruce2
   isr_Trig = 1;
#endif
  exti_serve_irq(pr, 2);

  OSAL_IRQ_EPILOGUE();
}
#endif // exti 2

#if !defined(STM32_DISABLE_EXTI3_HANDLER)
/**
 * @brief   EXTI[3] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector64) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();
  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 3);
  EXTI->PR = pr;

#ifdef isr_soruce3
   isr_Trig = 1;
#endif
  exti_serve_irq(pr, 3);

  OSAL_IRQ_EPILOGUE();
}
#endif // exti 3

#if !defined(STM32_DISABLE_EXTI4_HANDLER)
/**
 * @brief   EXTI[4] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector68) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & (1U << 4);
  EXTI->PR = pr;
#ifdef isr_soruce4
   isr_Trig = 1;
#endif

  exti_serve_irq(pr, 4);

  OSAL_IRQ_EPILOGUE();
}
#endif // exti 4

#if !defined(STM32_DISABLE_EXTI5_9_HANDLER)
/**
 * @brief   EXTI[5]...EXTI[9] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(Vector9C) {
  uint32_t pr;
  OSAL_IRQ_PROLOGUE();
  pr = EXTI->PR;
  pr &= EXTI->IMR & ((1U << 5) | (1U << 6) | (1U << 7) | (1U << 8) |
                     (1U << 9));
  EXTI->PR = pr;

#ifdef isr_soruce5
  if(pr & (1 << 5))
    isr_Trig = 1;
#endif
#ifdef isr_soruce6
  if(pr & (1 << 6))
    isr_Trig = 1;
#endif
#ifdef isr_soruce7
  if(pr & (1 << 7))
    isr_Trig = 1;
#endif
#ifdef isr_soruce8
  if(pr & (1 << 8))
    isr_Trig = 1;
#endif
#ifdef isr_soruce9
  if(pr & (1 << 9))
    isr_Trig = 1;
#endif


#ifdef mk637_special_isr
  if(pr & 0x07)
  isr_specal_Trig = 1;
#endif

  exti_serve_irq(pr, 5);
  exti_serve_irq(pr, 6);
  exti_serve_irq(pr, 7);
  exti_serve_irq(pr, 8);
  exti_serve_irq(pr, 9);

  OSAL_IRQ_EPILOGUE();
}
#endif // exti 5-9

#if !defined(STM32_DISABLE_EXTI10_15_HANDLER)
/**
 * @brief   EXTI[10]...EXTI[15] interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(VectorE0) {
  uint32_t pr;

  OSAL_IRQ_PROLOGUE();

  pr = EXTI->PR;
  pr &= EXTI->IMR & ((1U << 10) | (1U << 11) | (1U << 12) | (1U << 13) |
                     (1U << 14) | (1U << 15));
  EXTI->PR = pr;


#ifdef isr_soruce10
  if(pr & (1 << 10))
    isr_Trig = 1;
#endif
#ifdef isr_soruce11
  if(pr & (1 << 11))
    isr_Trig = 1;
#endif
#ifdef isr_soruce12
  if(pr & (1 << 12))
    isr_Trig = 1;
#endif
#ifdef isr_soruce13
  if(pr & (1 << 13))
    isr_Trig = 1;
#endif
#ifdef isr_soruce14
  if(pr & (1 << 14))
    isr_Trig = 1;
#endif
#ifdef isr_soruce15
  if(pr & (1 << 15))
    isr_Trig = 1;
#endif


#ifdef mk637_special_isr
  if(pr & (1 << 14))
  isr_specal_Trig = 2;
#endif


#ifdef mk637_special_isr
  if(pr & (1 << 15))
  isr_specal_Trig = 3;
#endif


  exti_serve_irq(pr, 10);
  exti_serve_irq(pr, 11);
  exti_serve_irq(pr, 12);
  exti_serve_irq(pr, 13);
  exti_serve_irq(pr, 14);
  exti_serve_irq(pr, 15);

  OSAL_IRQ_EPILOGUE();
}

OSAL_IRQ_HANDLER(VectorE4) {

  OSAL_IRQ_PROLOGUE();


  if( RTC->CRL & RTC_CRL_ALRF )
  {

    RTC->CRL &= ~RTC_CRL_ALRF;
    EXTI->PR = 1<< 17;
    RTC->CRL |= RTC_CRL_CNF;
    while(!(RTC->CRL & RTC_CRL_RTOFF));


    RCC->APB1ENR |= RCC_APB1ENR_BKPEN;
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_DBP;

    RTC->CRL |= RTC_CRL_CNF;       //Configuration Flag
    RTC->CNTL = 0x00;   //时间时15s
    RTC->CRL &= ~RTC_CRL_CNF;
    while(!(RTC->CRL & RTC_CRL_RTOFF));  //RTC operation OFF


    RTC->CRL &= ~RTC_CRL_CNF;
  }
  exti_flag = 1;
  OSAL_IRQ_EPILOGUE();
}
#endif // exti 10-15

#endif /* HAL_USE_PAL */
