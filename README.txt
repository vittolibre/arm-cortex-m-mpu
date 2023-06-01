0.
COLLEGARE LA SCHEDA AL PC



- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 



1.
APRIRE UNA SHELL MSYS2 PER IL SERVER DEBUG (OpenOCD):


cd /C/Users/ricci/Desktop/LaboratorioSE/librerie_usate/xpack-openocd-0.11.0-1-win32-x64/xpack-openocd-0.11.0-1/bin


./openocd -f ../scripts/board/stm32f4discovery.cfg



- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 



2.
APRIRE UNA SECONDA SHELL MSYS2:


cd /C/Users/ricci/Desktop/LaboratorioSE/Progetti_Esame/P1/codice/MPU_2


Comandi make per compilare, avviare il debug e ripulire il workspace:


make, make all


make debug


make clean


Nota: per usare il comando di debug su MSYS2, può essere necessario installare il comando della shell "nc" tramite il seguente comando: pacman -S gnu-netcat



- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 



3.
ESEMPI DI COMANDI DI DEBUG


per eseguire il debug come nell'approfondimento:

1) Settare un breakpoint hardware all'inizio dell'handler MemManage_handler:

b MemManage_Handler

2) Avviare l'esecuzione del programma:

c

3) Attendere che il LED inizi a lampeggiare, dopodiché premere il bottone:

*pressione bottone USER*

4) A questo punto: la MPU viene riconfigurata, quindi alla prossima scrittura sul LED, la MPU lancia un'eccezione e l'handler MemManage_handler viene invocato. In questo momento il programma si blocca per la presenza del breakpoint.
In questo momento si può indagare la memoria per approfondire la causa dell'eccezione tramite i seguenti comandi:


p/x *(unsigned int*) 0xE000ED34     MemManage Fault Address Register-> contiene l'indirizzo della locazione che ha generato il MemManage Fault è un registro ARM

p/x *(unsigned int*) 0xE000ED28     Configurable Fault Status Register -> indica la causa del MemManage Fault ->0x82-> 0101 0010-> solo il bit[1]=1 è importante. Indica che la causa è che il processore sta cercando di memorizzare in un'area senza i permessi di scrittura.

                                                                                       blu,  red,  orange, green
p/x *(unsigned long*) 0x40020C10    legge il registro GPIODxIDR (gli ultimi 4 bit sono PD15, PD14, PD13  , PD12)


