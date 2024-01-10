#include "stm32l4r9_module_clk_config.h"
#include "stm32l4r9xx.h"

void clock_configure(void) {
#ifdef CLK_MSI_120MHZ
  //   if current clock is not PLL
  if ((RCC->CFGR & RCC_CFGR_SWS_Msk) != 0b11 << RCC_CFGR_SWS_Pos) {
    // TODO: make more robust, pll active does not
    // mean it is configured

    RCC->CR &= ~(RCC_CR_PLLON_Msk);
    while ((RCC->CR & RCC_CR_PLLRDY_Msk)) {
      asm("nop");
    };

    FLASH->ACR |= (5 << FLASH_ACR_LATENCY_Pos);
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN_Msk | // clear coefficient fields
                      RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLR_Msk);

    RCC->PLLCFGR |=
        ((40 << RCC_PLLCFGR_PLLN_Pos) | // 4Mhz input multiplied by 60 = 240Mhz
         (0x00 << RCC_PLLCFGR_PLLM_Pos) | // divived by 1
         (0x00 << RCC_PLLCFGR_PLLR_Pos)); // divided by 2

    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC_Msk);
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_MSI; // select msi as pll clock source

    RCC->CR |= (RCC_CR_PLLON); // easy way yo set the register
    while (!(RCC->CR & RCC_CR_PLLRDY)) {
      asm("nop");
    };

    // activate prescaler
    RCC->CFGR |= RCC_CFGR_HPRE_DIV16;

    // RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_PLL; // select PLL as clock source

    // wait for clock to stabilize
    u32 count = 100000UL; // 1.5us
    while (count--)
      asm("nop");

    // wait for PLL to be used as clock source
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL) {
      asm("nop");
    };

    // deactivate prescaler
    RCC->CFGR &= ~(RCC_CFGR_HPRE_DIV16);

    // remember that you need to change the power status to be able to clock AHB
    // for f> 80Mhz
  }
#endif

#ifdef CLK_HSE_120MHZ
  // if current clock is not PLL
  if ((RCC->CFGR & RCC_CFGR_SWS_Msk) != 0b11 << RCC_CFGR_SWS_Pos) {
    // TODO: make more robust, pll active does not
    // mean it is configured

    RCC->CR &= ~(RCC_CR_PLLON_Msk);
    while ((RCC->CR & RCC_CR_PLLRDY_Msk)) {
      asm("nop");
    };

    FLASH->ACR |= (5 << FLASH_ACR_LATENCY_Pos);
    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLN_Msk | // clear coefficient fields
                      RCC_PLLCFGR_PLLM_Msk | RCC_PLLCFGR_PLLR_Msk);

    RCC->PLLCFGR |=
        ((30 << RCC_PLLCFGR_PLLN_Pos) | // 8Mhz input multiplied by 20 = 240Mhz
         (0x06 << RCC_PLLCFGR_PLLM_Pos) | // divived by 4
         (0x00 << RCC_PLLCFGR_PLLR_Pos)); // divided by 2

    RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLSRC_Msk);
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE; // select msi as pll clock source

    RCC->CR |= (RCC_CR_PLLON); // easy way yo set the register
    while (!(RCC->CR & RCC_CR_PLLRDY)) {
      asm("nop");
    };

    RCC->CFGR &= ~(RCC_CFGR_SW);
    RCC->CFGR |= RCC_CFGR_SW_PLL; // select PLL as clock source

    // wait for PLL to be used as clock source
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL) {
      asm("nop");
    };
  }
#endif
}