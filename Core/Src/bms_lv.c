#include "bms_lv.h"


// TEMP CODE
HAL_StatusTypeDef type = HAL_ERROR;

uint32_t counter = 0;

//ADC-DMA variables--
uint32_t curr_sens = 0; //32 bit because the DMA reading is set for WORD

//--CAN variables--
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
//CAN_FilterTypeDef  	  sFilterConfig;
uint16_t              Frame[4] = {0};  //TxData and Frame are two possible CAN frames, Frame is an array of four values in 16 bits.
uint8_t               TxData[8] = {0}; //TxData is an array of 8 values in 8 bits. In both cases the whole structure is of 64bits, that is the maximum size of the data frame in CAN

uint8_t               RxData[8] = {0};

//--UART variables--
uint8_t  uart_tx_cmd[10];
uint8_t  uart_rx_cmd[40];

uint16_t cell_voltages[8];
uint16_t cell_temperature[8];

uint8_t  cell_rx_uv[4];
uint8_t  cell_rx_ov[4];

//uint32_t overvoltage[8] = 0;
//uint32_t undertemperature[8] = 0;

unsigned short crc16_table[256] = {
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};


void SetupBmsLV(void) {
	
	//Start timer in IT mode for counter
	HAL_TIM_Base_Start_IT(&htim3);
	
	//INITIALIZATION CAN COMMUNICATION WITH VEHICLE
	if (HAL_CAN_Start(&hcan) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_CAN_INIT);
	}
	
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_CAN_RX);
	}
	
	
	//INITIALIZATION UART COMMUNICATION WITH SLAVE-------------------------------------------------------
	/*
		in questa funzione sono contenuti i messaggi di inizializzazione dello slave e la procedura di inizializzazione.
		è necessaria una procedura di accensione che prevede dei tempi di attesa dopo l'accensione e il wakeup
		il pin di wakeup va rimesso a 0 secondo istruzioni del datasheet
		è necessaria la configurazione del communication register prima di qualsiasi altra cosa, per abilitare la comunicazione uart
		è necessario configurare la direzione dei gpio (led e ntc), i led in pull down e gli ntc in pull up

		per generare messaggi bisogna attenersi alle istruzioni del datasheet
		per il calcolo dei 2 crc bytes si può usare il seguente sito: 
					http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
					è necessario impostare crc-16, crc16_arc 
					i due byte così ottenuti vanno scambiati in questo modo:
					es - msg [0xf1, 0x78, 0x3f] 
					valore crc - e333
					messaggio finale - [0xf1, 0x78, 0x3f, 0x33, 0xe3]
			
	*/
	
	HAL_GPIO_WritePin(gpio_BLUE_GPIO_Port, gpio_BLUE_Pin, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(RELAY_CMD_GPIO_Port, RELAY_CMD_Pin, GPIO_PIN_SET);
	
	//Powerdown slaves - reset operation(optional)
	if(Uart_Send_CMD(100, 3, 0xf1, 0x0c, 0x40) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	//after powering on wait at least 60ms
	HAL_Delay(100);
	//wakeup pin to high
	HAL_GPIO_WritePin(Wakeup_GPIO_Port, Wakeup_Pin, GPIO_PIN_RESET); //wakeup pin in inverting configuration so it is set as low state(RESET) 
	                                                                 //but the slave reads it as high state
	//wait at least 50ms after setting the wakeup pin to high
	HAL_Delay(100);
	//keep WAKEUP pin at low
	HAL_GPIO_WritePin(Wakeup_GPIO_Port, Wakeup_Pin, GPIO_PIN_SET); //wakeup pin in inverting configuration so it is set as high state(SET) 
	                                                               //but the slave reads it as low state
	HAL_Delay(200);
	
	//configure initial sample delay - we set it at 0
	if(Uart_Send_CMD(20, 3, 0xf1, 0x3d, 0x00) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	HAL_Delay(100);
	
	//configure cell voltage sample period (60 us) and internal temperature sampling period (100 us)
	if(Uart_Send_CMD(20, 3, 0xf1, 0x3e, 0xbc) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	HAL_Delay(100);
	
	//clear all fault summary flags
	if(Uart_Send_CMD(20, 4, 0xf2, 0x52, 0xff, 0xc0) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	HAL_Delay(100);
	
	//clear faults	
	if(Uart_Send_CMD(20, 3, 0xf1, 0x51, 0x38) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	HAL_Delay(100);
	
	//Set communication Baud Rate - 250kBaude
	/*if(Uart_Send_CMD(20, 4, 0xf2, 0x10, 0x10, 0x80) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART_BAUD_RATE);
	}
	
	HAL_Delay(100);
	*/
	//Set Active channels
	if(Uart_Send_CMD(20, 4, 0x91, 0x00, 0x0d, 0x07) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART_ACTIVE_CH);
	}
	
	HAL_Delay(100);
	
	//Set channel & aux
	if(Uart_Send_CMD(20, 7, 0x94, 0x00, 0x03, 0x00, 0xff, 0xff, 0x00) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	HAL_Delay(100);	
	
	//Set Cell Undervoltage Threshold - 2.7 V
	if(Uart_Send_CMD(20, 5, 0x92, 0x00, 0x8e, 0x8A, 0x3c) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART_CELL_UV_TH);
	}
	
	HAL_Delay(100);

	//Set Cell Overvoltage Threshold - 4.3 V
	if(Uart_Send_CMD(20, 5, 0x92, 0x00, 0x90, 0xdc, 0x28) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART_CELL_OV_TH);
	}
	
	HAL_Delay(100);
	
	for(int i=0; i<8; i++) {
		//Set AUXs Overvoltage Threshold - 4.28 V (60 °C)
		if(Uart_Send_CMD(100, 5, 0x92, 0x00, 0x94 + i*4, 0xdb, 0x1c) != HAL_OK) {
			Notify_Error(&TxMailbox, ERROR_UART_AUX_OV_TH);
		}
	}

	HAL_Delay(100);
	
	//Turning-off GPIO pull-downs 
	if(Uart_Send_CMD(20, 4, 0x91, 0x00, 0x7b, 0x00) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	HAL_Delay(100);
	
	//Turning-off GPIO pull-ups 
	if(Uart_Send_CMD(20, 4, 0x91, 0x00, 0x7a, 0x00) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	HAL_Delay(100);
	
	//Setting GPIO direction 
	if(Uart_Send_CMD(20, 4, 0x91, 0x00, 0x78, 0x3f) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	HAL_Delay(100);
	
		//if you enable output pull-ups you cannot enable pull-downs, and viceversa!!
	
//	//Setting GPIO out pull up
//	if(Uart_Send_CMD(0x91, 0x00, 0x7a, 0x3f, 0x4f, 0x8c, 0x00, 0x00, 0x00, 0x00, 6, 20) != HAL_OK)
//	{
//		Notify_Error(&TxMailbox, "");
//	}

	
	//Setting GPIO out pull down
	if(Uart_Send_CMD(20, 4, 0x91, 0x00, 0x7b, 0x3f) != HAL_OK) {
		Notify_Error(&TxMailbox, ERROR_UART);
	}
	
	HAL_Delay(100);
	//HAL_GPIO_WritePin(gpio_BLUE_GPIO_Port, gpio_BLUE_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(RELAY_CMD_GPIO_Port, RELAY_CMD_Pin, GPIO_PIN_SET);
}


void CoreBmsLV(uint32_t delay_100us) {
	
	/* LED LEGEND
		RGB RED: 
		- blink: no slave connection
	  - steady: fatal error
		RGB GREEN: core running
		RGB BLUE: start UART communication
		
		ORANGE: CAN communication
	*/
		
	static uint32_t delay_100us_last_can = 0;

	if(delay_fun(&delay_100us_last_can,delay_100us)) {	
	
		//Every cycle we read from DMA the value of the current sensor
		HAL_ADC_Start_DMA(&hadc2, &curr_sens, 1);
		
		HAL_GPIO_TogglePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin);
		
		// Turn on slave LEDs
		if(Uart_Send_CMD(10, 4, 0x91, 0x00, 0x79, 0xFF) != HAL_OK) {
			Notify_Error(&TxMailbox, ERROR_UART);
		}
		//Getting Voltages and Temperatures from slave
		GetVoltagesandTemperature(0);
		
		/*if (undertemperature[0] > 5 || overvoltage [0] > 20){
				HAL_GPIO_WritePin(RELAY_CMD_GPIO_Port, RELAY_CMD_Pin, GPIO_PIN_RESET);
		}
		else HAL_GPIO_WritePin(RELAY_CMD_GPIO_Port, RELAY_CMD_Pin, GPIO_PIN_SET);
		if(Uart_Send_CMD(10, 4, 0x91, 0x00, 0x79, 0x00) != HAL_OK) {
			Notify_Error(&TxMailbox, ERROR_UART);
		}*/
		
		HAL_GPIO_TogglePin(ORANGE_LED_GPIO_Port, ORANGE_LED_Pin);
		CAN_Tx(40);
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	//Maximum Threshold current 40A
		//minimum Threshold current -20A - while charging
		//the correlation between LEM output(V_5) and the input current to be measured(I_in) => V_5 = 2,5 + 0,025*I_in
		//V_3v3 = V_5 * 82/(82+42.2)
		//V_adc = V_3v3 * (2^12-1)/3.3;
		//I_in_MAX = 40A => V_adc_MAX =0xB33;
		//I_in_MAX = -20A => V_adc_min =0x666;
		//
		//For the DBC, the OFFSET is 2048(base 10) and the FACTOR is 0,0488
		//Example => V_adc_min = 0x666(base 16) = 1638(base 10) => I = (V_adc_min - OFFSET)*FACTOR = -20A c.v.d
		if ((curr_sens > 0x0b33) || (curr_sens < 0x0666)){
			//Error_Handler();
		}
} 


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	
	if((*htim).Instance == TIM3) {
		counter++;
	}
	
}



/*Return the value of the counter that is incremented every 100us*/
uint32_t ReturnTime_100us(void) {
	
	uint32_t time = counter;
	return time;

}

/*Return true if the delay is completed, false otherwise*/
int delay_fun (uint32_t *delay_100us_last, uint32_t delay_100us) {

	uint32_t current_time = ReturnTime_100us();

	if(current_time > *delay_100us_last && (current_time - *delay_100us_last) >= delay_100us)  {
		*delay_100us_last = current_time;
		return 1;
		
	} else if(current_time < *delay_100us_last && (0xFFFFFFFF - current_time - *delay_100us_last) >= delay_100us) {
		*delay_100us_last = current_time;
		return 1;
	}
	/*In case of timer overflow, the delay is computed correctly*/

	return 0;

}

/*Send message to CAN BUS*/

//APB1 at 30MHz, Prescaler = 6 
//Peripheral Clock = 30MHz/6 = 5 MHz
//Time quanta = 3(Bit segment 1) + 1(Bit segment 2) + 1(Synchronization jump) = 5
//  |
//  |
//  V
//CAN Bit-Rate = Peripheral Clock / Time quanta = 1 Mbit/s;
//
//This values can be avaluated at the link 'http://www.bittiming.can-wiki.info/'.
//Just set the Can Rate, the Clock Rate and click on Request table, you will be given the prescaler, the time quanta
//and the bits for each segment (1 and 2). You can find the sychronization jump by doing the math
//(usually it is 1)	

void CAN_Msg_Send(CAN_HandleTypeDef *hcan, CAN_TxHeaderTypeDef *pHeader, uint8_t aData[], uint32_t *pTxMailbox, uint32_t TimeOut)  {
	
	static uint32_t can_counter_100us = 0;
	can_counter_100us = ReturnTime_100us(); 

	while(HAL_CAN_GetTxMailboxesFreeLevel(hcan) < 1) {
	
		if(delay_fun(&can_counter_100us,TimeOut)) {
			HAL_CAN_ResetError(hcan);
			HAL_CAN_AbortTxRequest(hcan, *pTxMailbox);
			//Error_Handler();
		}
		
	}

	if (HAL_CAN_AddTxMessage(hcan, pHeader, aData, pTxMailbox) != HAL_OK) {    
		HAL_CAN_ResetError(hcan);
		HAL_CAN_AbortTxRequest(hcan, *pTxMailbox);
		//Error_Handler();
    }
	
}


/* Callback triggered with incoming notification CAN_IT_RX_FIFO0_MSG_PENDING */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {

	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
		/* Transmission request Error */
		HAL_CAN_ResetError(hcan);
	}	
}


/* */
void CAN_Tx(uint32_t delay_100us) {		
	
		// BMS_LV_CELL_1 => CELL[0], CELL[1], CELL[2], CELL[3]
		TxHeader.StdId = BMS_LV_CELL_1;
		TxHeader.ExtId = BMS_LV_CELL_1;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.DLC = 8;
		TxHeader.TransmitGlobalTime = DISABLE;
		
		memcpy(Frame, cell_voltages, 4 * sizeof(uint16_t));
		
	  for(int i=0; i<4; i++){
			Frame[i] = cell_voltages[i];	
		}
		
		CAN_Msg_Send(&hcan, &TxHeader, (uint8_t*)Frame, &TxMailbox, 40); //Sending via CAN first 4 cell's voltages (cell 1 to 4)=> 2bytes * cell = 8 byte = frame size
		
		TxHeader.StdId = BMS_LV_CELL_2;
		TxHeader.ExtId = BMS_LV_CELL_2;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.DLC = 8;
		TxHeader.TransmitGlobalTime = DISABLE;
			
		memcpy(Frame, cell_voltages+4, 4 * sizeof(uint16_t));

		for(int i=0; i<4; i++){
			Frame[i] = cell_voltages[i+4];
		}
			
		CAN_Msg_Send(&hcan, &TxHeader, (uint8_t*)Frame, &TxMailbox, 40); //Sending via CAN second 4 cell's voltages (cell 5 to 8) => 2bytes * cell = 8 byte = frame size
		
		TxHeader.StdId = BMS_LV_TEMP_1;
		TxHeader.ExtId = BMS_LV_TEMP_1;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.DLC = 8;
		TxHeader.TransmitGlobalTime = DISABLE;
	
		memcpy(Frame, cell_temperature, 4 * sizeof(uint16_t));
		
		Frame[0] = cell_temperature[0];
		Frame[1] = cell_temperature[3];
		Frame[2] = cell_temperature[4];
		Frame[3] = cell_temperature[5];
		
		CAN_Msg_Send(&hcan, &TxHeader, (uint8_t*)Frame, &TxMailbox, 40); //Sending via CAN second 4 cell's temperatures (AUX 1 to 4) => 2bytes * cell = 8 byte = frame size
		
		TxHeader.StdId = BMS_LV_TEMP_2;
		TxHeader.ExtId = BMS_LV_TEMP_2;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.DLC = 4;
		TxHeader.TransmitGlobalTime = DISABLE;
		
		Frame[0] = cell_temperature[6];
		Frame[1] = cell_temperature[7];
		
		memcpy(Frame, cell_temperature+4, 4 * sizeof(uint16_t));
	
		for(int i=0; i<4; i++){			
			Frame[i] = cell_temperature[i+4];
		}
		
		CAN_Msg_Send(&hcan, &TxHeader, (uint8_t*)Frame, &TxMailbox, 40); //Sending via CAN second 4 cell's temperatures (AUX 5 to 8) => 2bytes * cell = 8 byte = frame size		
	
		// Send current sens
		TxHeader.StdId = CURRENT_SENSOR;
		TxHeader.ExtId = CURRENT_SENSOR;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.DLC = 2;
		TxHeader.TransmitGlobalTime = DISABLE;
		
		// curr_sens 32 bit
		
		TxData[0] = curr_sens & 0x000000FF;
		TxData[1] = (curr_sens & 0x0000FF00) >> 8;
		
		CAN_Msg_Send(&hcan, &TxHeader, TxData, &TxMailbox, 40);
	
}

 
void LedBlinking(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t delay_100us) {
	
	static uint32_t delay_100us_last = 0;

	if(delay_fun(&delay_100us_last,delay_100us)) {
		HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
	}
	
}

 
HAL_StatusTypeDef Uart_Send_CMD(int timeout, int size, ...) {
	
	if( size+2 > 10) {
		return HAL_ERROR;
	}
	
	va_list valist;
	va_start(valist, size);
	
	uint8_t cmd[size+2];

	uint16_t crc;
	uint16_t crc_result = 0;
	
	for (int i=0; i<size; i++) {
		cmd[i] = va_arg(valist, int);
	}

	crc = crc16(crc_result, cmd, size);
	
	cmd[size+1] = crc >> 8;                     //I due byte ottenuti vanno scambiati in questo modo:
	cmd[size] = crc & 0x00FF;               //es - msg [0xf1, 0x78, 0x3f]
	                                          //valore crc - e333
	                                          //messaggio finale (cmd) - [0xf1, 0x78, 0x3f, 0x33, 0xe3]
	
	type = HAL_UART_Transmit(&huart2, (uint8_t*) cmd, size+2, timeout);

	return type;
}

void GetVoltagesandTemperature(uint32_t delay_100us) {
		
		//requesting the slave to sample and return the values - 0x81: single write with response request
		if(Uart_Send_CMD(20, 4, 0x81, 0x00, 0x02, 0x00) != HAL_OK) {
			Notify_Error(&TxMailbox, ERROR_UART);
		}
		
		
		// The first byte of uart_rx_cmd is equal to
		// [(num. of chan + num. of aux)*2]-1.
		// After the first byte the messagge will contain
		// 2 bytes of value from each channel and then 
		// 2 bytes of value from each AUX. Both channels 
		// and AUX start from the one with higher 
		// cardinality
		
		if(HAL_UART_Receive(&huart2, uart_rx_cmd, 35,  100) != HAL_OK) {
			Notify_Error(&TxMailbox, ERROR_UART_RX);
		} 
		
		//cell voltages and cell temperatures received are sorted from the message received in UART from the slave
		uint8_t b1_voltage, b2_voltage, b1_temp, b2_temp;
		
		/* 
		* The first byte of uart_rx_cmd is equal to
		* [(num. of chan + num. of aux)*2]-1.
		* After the first byte the messagge will contain
		* 2 bytes of value from each channel and then 
		* 2 bytes of value from each AUX. Both channels 
		* and AUX start from the one with higher 
		* cardinality
		*/
		
		for(int i=0; i<8; i++) { 
			b1_voltage = uart_rx_cmd[8*2-1 - 2*i];
			b2_voltage = uart_rx_cmd[8*2 - 2*i];
			 
			b1_temp = uart_rx_cmd[8*4-1 - 2*i];
			b2_temp = uart_rx_cmd[8*4 - 2*i];
			
			//voltage acquisition stops at cell[6], because cell[7] must be 0, since it is the GND
			if(i<7) {  
				cell_voltages[i] = (b1_voltage << 8) | b2_voltage; //the two bytes here are concateneted in 16 bits
				
				//if the voltage sampled is lower then 2.7V (0xdc28) and higher then 4.3V (0x8a3c) the BMS must open the shutdown	0x9935
				/*if (cell_voltages[i] >= 0xCC00){  
					overvoltage[i]++;
				}
				else {
					overvoltage[i] = 0;
				}*/
				
			}
			 
		cell_temperature[i] = (b1_temp << 8) | b2_temp;	//the two bytes here are concateneted in 16 bits
		cell_temperature[i] = 100 * ( 1.00 / ( (1.00/T0) + ( (1.00/BETA) * log( ((float)cell_temperature[i])/( 65535-(float)cell_temperature[i]-1 )) ) ) ); //converting AUX value from VOLT to KELVIN
			/*if (cell_temperature[i] < 100){
				undertemperature[i]++;
			}
			else undertemperature[i] = 0;*/
  		//cell_temperature[i] = 100 * ( 1.00 / ( 1.00/T0 + ( 1.00/BETA )*log( 1.00/( 65535.00/(double)cell_temperature[i] - 1.00 ) ) ) );
		}	
}

// Check for fault in register. Bitwise AND with 0xFF mask in order to see if any bit is 1. If 0 => NO FAULT
int check_fault_ov_uv(uint8_t fault_reg) {

	return fault_reg & 0xff;
	
}

void Ping_Mng(uint32_t delay_100us){
	
	static uint32_t delay_100us_last_can = 0;

	if(delay_fun(&delay_100us_last_can,delay_100us)) {
		TxHeader.StdId = PING_ID_Tx;
		TxHeader.ExtId = PING_ID_Tx;
		TxHeader.RTR = CAN_RTR_DATA;
		TxHeader.IDE = CAN_ID_STD;
		TxHeader.DLC = 1;
		TxHeader.TransmitGlobalTime = DISABLE;
		
		TxData[0] = 0xFF;
		
		CAN_Msg_Send(&hcan, &TxHeader, TxData, &TxMailbox, 40);
	}
}

void Open_Relay(uint8_t msg) {
	
	//CAN_TxHeaderTypeDef TxHeader;
	
	TxHeader.StdId = OPEN_RELAY;
	TxHeader.ExtId = OPEN_RELAY;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 1;
	TxHeader.TransmitGlobalTime = DISABLE;

	CAN_Msg_Send(&hcan, &TxHeader, &msg, &TxMailbox, 40); 
	
	// Open shutdown circuit
	//Error_Handler();
}


/* Compute the CRC16-ARC */
unsigned short crc16(unsigned short crc, unsigned char const *buffer, unsigned int len) {
	while (len--)
		crc = (unsigned short)((crc >> 8) ^ (crc16_table[(crc ^ (*buffer++)) & 0xff]));
	return crc;
}

