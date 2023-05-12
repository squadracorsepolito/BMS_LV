#include "ntc.h"
#include "L9963E_utils.h"

#include <memory.h>

#define NTC_MEASURE_INTERVAL 5

#define NTC_EXP_SMOOTH_ALPHA 0.1f

#define NTC_EXT_ADC_N 7
#define NTC_INT_ADC_N 5
#define NTC_ADC_N (NTC_INT_ADC_N + NTC_EXT_ADC_N)

uint16_t ntc_data[NTC_ADC_N];
uint16_t ntc_dma_data[NTC_INT_ADC_N];

void ntc_init(void) {
    memset(ntc_dma_data, 0, sizeof(ntc_dma_data));
    memset(ntc_data, 0, sizeof(ntc_data));

    HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ntc_dma_data, NTC_INT_ADC_N);
}

//every NTC_MEASURE_INTERVAL voltage measures, start gpio conversion too
uint8_t ntc_is_measure_ext_time(void) {
    static uint8_t counter = 0;

    counter = (counter+1) % NTC_MEASURE_INTERVAL;

    return counter == 0;
}

void ntc_set_data(uint16_t const *data, uint8_t len, uint8_t offset) {
    if(len+offset > NTC_ADC_N)
        return;

    for(uint8_t i=0; i<len; ++i) {
        ntc_data[i+offset] = (10*NTC_EXP_SMOOTH_ALPHA*data[i+offset] + 10*(1-NTC_EXP_SMOOTH_ALPHA)*ntc_data[i+offset])/10;
    }
}

void ntc_adc_callback(ADC_HandleTypeDef *hadc) {
    for(uint8_t i=0; i<NTC_INT_ADC_N; ++i) {
        ntc_data[i+NTC_EXT_ADC_N] = (10*NTC_EXP_SMOOTH_ALPHA*ntc_dma_data[i] + 10*(1-NTC_EXP_SMOOTH_ALPHA)*ntc_data[i+NTC_EXT_ADC_N])/10;
    }
}