/******************************************************************************
 * @file     main_MPU.c
 * @brief    Main function of the MPU use example
 * @version  V1.0
 * @date     17 June 2022
 * @author	 De Vivo Domenico, Libretti Vittorio, Ricciardi Armando (group 8)
 ******************************************************************************/

#include "mpu_lib.h"			//libreria custom con funzioni di utilità

// Funzione main
/* Il main comprende un esempio di configurazione ed utilizzo della MPU, 
 * mostrato tramite l'uso dei 4 LED e del pulsante User Button.  
 * Il main è suddiviso nelle seguenti parti:
 *  - inizializzazione programma;
 *  - operazioni preliminari alla configurazione della MPU;
 *  - configurazione MPU;
 *  - caso d'uso: scrittura multipla nella regione 0 della MPU.
 */

int main() 
{
	
	/*!   ***INIZIALIZZAZIONE***	*/
  
	//Funzione di abilitazione Monitor di debug (inizializzaz. handler monitor)
	initialise_monitor_handles();

	//Inizializzazione libreria HAL
	HAL_Init();

	//Funzione di abilitazione dell'User Button e dei LED
	GPIO_Init();

	/*!   ***OPERAZIONE PRELIMINARE ALLA CONFIGURAZIONE DELLA MPU***	*/

	//Controllo presenza fisica della MPU nel sistema
	if (MPU_is_present()){
		
		printf("MPU presente!\n");
		
	/*!   ***CONFIGURAZIONE MPU***	*/

		MPU_Configuration(ARM_MPU_AP_FULL);

	/*!   ***CASO D'USO: SCRITTURA MULTIPLA NELLA REGIONE 0 DELLA MPU***	*/

		while(1){
			
			delay(LEDDELAY);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			delay(LEDDELAY);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_14);
			delay(LEDDELAY);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			delay(LEDDELAY);
			HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			
			printf("LED lampeggianti...\n");
			
		}
	 
	}else {
		printf("MPU non presente...\n");
	}
}


