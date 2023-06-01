/******************************************************************************
 * @file     mpu_lib.c
 * @brief    Implementation file of custom functions library
 * @version  V1.0
 * @date     17 June 2022
 * @author	 De Vivo Domenico, Libretti Vittorio, Ricciardi Armando (group 8)
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "mpu_lib.h"

/* Static variables ----------------------------------------------------------*/
static uint8_t switchConf= 0;			//variabile statica di gestione bottone
										//(no proprietà reentrant del codice)

/* Functions definition ------------------------------------------------------*/


// Definizione funzione inizializzazione e configurazione periferiche board

/* Questa funzione serve per inizializzare e configurare il clock e abilitare 
 * diverse periferiche della board: GPIOD per i LED (precisamente sui pin 12,
 * 13,14,15); GPIOA per il pulsante User Button (precisamente sul pin 0). 
 * Inoltre viene abilitata e configurata l'interruzione relativa alla pressione 
 * del bottone. Tale abilitazione viene fatta con l'ausilio della libreria HAL 
 * di ST.
 */
 
void GPIO_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOA clock */
	__HAL_RCC_GPIOA_CLK_ENABLE(); //User_b
	__HAL_RCC_GPIOD_CLK_ENABLE(); //Leds

	/* Configure PA0 pin as input floating */
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*Configure GPIO pins : PD12 PD13 PD14 PD15 */
	GPIO_InitStructure.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Enable and set EXTI Line0 Interrupt to the lowest priority */
	HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}


// Definizione funzione per controllare la presenza della MPU

/* Questa funzione serve a verificare la presenza fisica della MPU all'interno 
 * del sistema. Infatti la MPU è opzionale. Nel registro TYPE c'è bun bit che 
 * specifica questa informazione.
 */

uint32_t MPU_is_present(void) {
	
	if (MPU->TYPE==0)
		return 0;
	else
		return 1;
}


// Definizione funzione di configurazione della MPU

/* Questa funzione permette di configurare la MPU seguendo i passi canonici di
 * corretta configurazione della stessa. In particolare, la configurazione viene
 * fatta secondo gli scopi di questo esempio, ovvero: le regioni da 1 a 7 sono
 * disabilitate; la regione 0 è abilitata e configurata come segue: l'area di 
 * memoria da gestire è quella relativa agli indirizzi di GPIOD, di dimensione
 * 1KB, il tipo di memoria è Normal Memory con TEX=0, S=0, C=0, B=1 e con XN=0;
 * infine i permessi di accesso sono passati come parametro.
 */

void MPU_Configuration(uint32_t permessi_accesso) {

		//1. Disabilitazione MPU
		ARM_MPU_Disable();
		
		//2. Configurazione regione 0 con pieno accesso o in sola lettura
		ARM_MPU_SetRegionEx(0UL, 0x40020C00UL, ARM_MPU_RASR(0UL, permessi_accesso, 0UL, 0UL, 0UL, 1UL, 0x00UL, ARM_MPU_REGION_SIZE_1KB));
		
		if ( permessi_accesso == ARM_MPU_AP_FULL )
			printf("MPU configurata: accesso in lettura/scrittura...\n");
		else
			printf("MPU configurata: accesso in sola lettura...\n");

		//3. Disabilitazione regioni non utilizzate
		for (int i=1; i<8; i++) {
			ARM_MPU_ClrRegion (i);
		}

		//4. Abilitazione MPU (con abilitazione della regione di background)
		ARM_MPU_Enable (MPU_CTRL_PRIVDEFENA_Msk);
		printf("MPU abilitata...\n");
}


// Definizione funzione di callback pressione User Button

/* Questa funzione di callback viene invocata dal gestore dell'eccezione della 
 * pressione dell'User Button. All'interno di tale funzione di callback bisogna
 * specificare la logica da eseguire con la pressione del bottone. In tal caso,
 * alla pressione del bottone viene riconfigurata la MPU dinamicamente 
 * modificando i permessi della regione 0 in permessi di sola lettura oppure di
 * lettura/scrittura.
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	
	printf("Bottone premuto: \n riconfigurazione MPU...\n");
	
    if (switchConf==0){
		
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_Pin)==1 && switchConf==0);//debouncing
		
		//Configurazione MPU in sola lettura
		MPU_Configuration(ARM_MPU_AP_RO);
	
		switchConf=1;
		
	}else if (switchConf==1){
		
		while(HAL_GPIO_ReadPin(GPIOA,GPIO_Pin)==1 && switchConf==1);//debouncing
		
		//Configurazione MPU in lettura/scrittura
		MPU_Configuration(ARM_MPU_AP_FULL);
	
		switchConf=0;
	}
	
	//Azzeramento di (eventuale) interruzione pending su GPIO_EXTI
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);

}


// Definizione funzione di ritardo

/* Questa funzione introduce un ritardo nell'esecuzione tramite un semplice 
 * ciclo for iterato s volte. Il valore di s passato nel main sarà la costante 
 * LEDDELAY, definita nell'header file mpu_lib.h.
 */

void delay(volatile unsigned long s){
    for(; s>0; s--);
}


