#include "stm32f10x.h" // Device header

void ADC1_Init(void);
uint16_t ADC1_Read(void);

volatile uint16_t adc_value = 0;
volatile float voltvalue = 0.0f; // To store the calculated voltage

int main(void)
{
    ADC1_Init(); // Initialize ADC1

    while (1)
    {
        adc_value = ADC1_Read(); // Read ADC value
        // Calculate the corresponding voltage
        voltvalue = (adc_value * 3.3f) / 4095.0f;

        // Add further processing or visualization logic here
    }
}

void ADC1_Init(void)
{
    // Enable clock for ADC1 and GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;

    // Configure PA0 as analog input (reset state)
    GPIOA->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    // Configure ADC1
    ADC1->SQR1 = 0;                      // Regular sequence length = 1
    ADC1->SQR3 = 0;                      // Set channel 0 (PA0) as first conversion
    ADC1->CR2 |= ADC_CR2_ADON;           // Enable ADC1
    for (volatile int i = 0; i < 10000; i++); // Delay for ADC stabilization

    ADC1->CR2 |= ADC_CR2_CAL;            // Start ADC calibration
    while (ADC1->CR2 & ADC_CR2_CAL);     // Wait for calibration to complete
}

uint16_t ADC1_Read(void)
{
    ADC1->CR2 |= ADC_CR2_ADON;           // Start ADC conversion
    while (!(ADC1->SR & ADC_SR_EOC));    // Wait for conversion to complete
    return ADC1->DR;                     // Return ADC conversion result
}
