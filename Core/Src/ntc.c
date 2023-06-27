#include "ntc.h"
#include "L9963E_utils.h"

#include <memory.h>

#define NTC_MEASURE_INTERVAL 50

#define NTC_EXP_SMOOTH_ALPHA 0.1f

// constant values by the curve fit of the ntc

#define    ADCTEMP_EXT_CONST_a	121.77018530814999
#define    ADCTEMP_EXT_CONST_b	-92.86284471272114
#define    ADCTEMP_EXT_CONST_c	39.33876760742037
#define    ADCTEMP_EXT_CONST_d	-8.798397291741638
#define    ADCTEMP_EXT_CONST_e	0.7020187110716422

#define    ADCTEMP_INT_CONST_a	121.7701871084035
#define    ADCTEMP_INT_CONST_b	-140.70128755197035
#define    ADCTEMP_INT_CONST_c	90.30939373795056
#define    ADCTEMP_INT_CONST_d	-30.60355162614069
#define    ADCTEMP_INT_CONST_e	3.699755615443203

uint16_t ntc_int_data[NTC_INT_ADC_N];
uint16_t ntc_ext_data[NTC_EXT_ADC_N];

uint16_t ntc_dma_data[NTC_INT_ADC_N];

void ntc_init(void) {
    memset(ntc_dma_data, 0, sizeof(ntc_dma_data));
    memset(ntc_int_data, 0, sizeof(ntc_int_data));
    memset(ntc_ext_data, 0, sizeof(ntc_ext_data));

    HAL_ADC_Start_DMA(&hadc2, (uint32_t*)ntc_dma_data, NTC_INT_ADC_N);
}

//every NTC_MEASURE_INTERVAL voltage measures, start gpio conversion too
uint8_t ntc_is_measure_ext_time(void) {
    static uint8_t counter = 0;

    counter = (counter+1) % NTC_MEASURE_INTERVAL;

    return counter == 0;
}

void ntc_set_ext_data(uint16_t const *data, uint8_t len, uint8_t offset) {
    if(len+offset > NTC_EXT_ADC_N)
        return;

    for(uint8_t i=0; i<len; ++i) {
        ntc_ext_data[i+offset] = NTC_EXP_SMOOTH_ALPHA*data[i+offset] + (1-NTC_EXP_SMOOTH_ALPHA)*ntc_ext_data[i+offset];
    }
}

void ntc_adc_callback(ADC_HandleTypeDef *hadc) {
    static uint8_t is_first = 1;
    if(is_first) {
        is_first = 0;
        memcpy(ntc_int_data, ntc_dma_data, sizeof(ntc_int_data));
    }
    for(uint8_t i=0; i<NTC_INT_ADC_N; ++i) {
        ntc_int_data[i] = NTC_EXP_SMOOTH_ALPHA*ntc_dma_data[i] + (1-NTC_EXP_SMOOTH_ALPHA)*ntc_int_data[i];
    }
}

float ntc_get_ext_resistance(uint8_t index) {
    if(index >= NTC_EXT_ADC_N)
        return 0;

    return (float)ntc_ext_data[index]/(56180-ntc_ext_data[index])*10e3;
}

float ntc_get_int_resistance(uint8_t index) {
    if(index >= NTC_INT_ADC_N)
        return 0;

    return (float)ntc_int_data[index]/(4095-ntc_int_data[index])*10e3;
}

float ntc_get_ext_temp(uint8_t index) {
    if(index >= NTC_EXT_ADC_N)
        return 0;

    float val = ntc_ext_data[index] * 0.000089;
    float val2 = val*val;
    float val3 = val2*val;
    float val4 = val2*val2;

    return ADCTEMP_EXT_CONST_a + ADCTEMP_EXT_CONST_b * val + ADCTEMP_EXT_CONST_c * val2 + ADCTEMP_EXT_CONST_d * val3 + ADCTEMP_EXT_CONST_e * val4;
}

float ntc_get_int_temp(uint8_t index) {
    if(index >= NTC_INT_ADC_N)
        return 0;

    float val = ntc_int_data[index] * 3.3 / 4095;
    float val2 = val*val;
    float val3 = val2*val;
    float val4 = val2*val2;

    return ADCTEMP_INT_CONST_a + ADCTEMP_INT_CONST_b * val + ADCTEMP_INT_CONST_c * val2 + ADCTEMP_INT_CONST_d * val3 + ADCTEMP_INT_CONST_e * val4;
}