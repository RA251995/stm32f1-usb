#include <stdint.h>
#include "system_stm32f1xx.h"
#include "stm32f1xx.h"
#include "Helpers/logger.h"

LOG_LEVEL_ENUM systemLogLevel = LOG_LEVEL_DEBUG;
uint32_t SystemCoreClock = 72000000; /* 72 MHz */

/*
 * PLL Source = HSE
 * PLLMul = X 9
 * USB Prescaler = /1.5
 * System Clock = PLLCLK
 * SYSCLK = 72 MHz
 * AHB Prescaler = /1
 * HCLK = 72 MHz
 * APB1 Prescaler = /2
 * PCLK1 = 36 MHz
 * APB2 Prescaler = /1
 * PCLK2 = 72 MHz
 */
static void configureClock() {
	/* Set flash latency */
	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY_Msk,
			_VAL2FLD(FLASH_ACR_LATENCY, 2UL));

	/* Enable HSE */
	SET_BIT(RCC->CR, RCC_CR_HSEON);
	while (!READ_BIT(RCC->CR, RCC_CR_HSERDY))
		; /* Wait till HSE clock is stable */

	/* Configure PLL */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLMULL_Msk,
			_VAL2FLD(RCC_CFGR_PLLMULL, 7UL)); /* Configure PLL multiplier */
	SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC); /* Set HSE clock as PLL input */

	/* Enable PLL module */
	SET_BIT(RCC->CR, RCC_CR_PLLON);
	while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY))
		; /* Wait till PLL clock is stable */

	/* Switch SYSCLK to PLL clock */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW_Msk, _VAL2FLD(RCC_CFGR_SW, 2UL));
	while (_FLD2VAL(RCC_CFGR_SWS, READ_REG(RCC->CFGR)) != 2U)
		; /* Wait till switch is complete */

	/* Change APB2 prescaler */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1_Msk, _VAL2FLD(RCC_CFGR_PPRE1, 4UL));

	/* Disable HSI */
	CLEAR_BIT(RCC->CR, RCC_CR_HSION);
}

static void confiureMCO()
{
	/* Configure MCO source */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO_Msk, RCC_CFGR_MCOSEL_PLL_DIV2);

	/* Enable GPIOA */
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);

	/* Configure PA8 */
	MODIFY_REG(GPIOA->CRH, GPIO_CRH_MODE8_Msk, _VAL2FLD(GPIO_CRH_MODE8, 3U)); /* Configure as output mode, 50 MHz speed */
	MODIFY_REG(GPIOA->CRH, GPIO_CRH_CNF8_Msk, _VAL2FLD(GPIO_CRH_CNF8, 2U)); /* Configure as alternate function push-pull */
}

void SystemInit(void) {
	confiureMCO();
	configureClock();
}
