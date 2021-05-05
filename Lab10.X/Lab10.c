/*
 * Archivo:   Lab10.c
 * Dispositivo: PIC16F887
 * Autor: Margareth Vela 
 * 
 * Programa: Módulo UART
 * Hardware:Leds en PORTA y PORTB y Terminal Virtual en PORTC
 * 
 * Creado: Mayo 03, 2021
 * Última modificación: Mayo 05, 2021
 */


//------------------------------------------------------------------------------
//                          Importación de librerías
//------------------------------------------------------------------------------    
#include <xc.h>
#include <string.h>

//------------------------------------------------------------------------------
//                          Directivas del compilador
//------------------------------------------------------------------------------
#define _XTAL_FREQ 1000000 //Para delay

//------------------------------------------------------------------------------
//                          Palabras de configuración
//------------------------------------------------------------------------------    
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT//Oscillator Selection bits(INTOSCIO 
                              //oscillator: I/O function on RA6/OSC2/CLKOUT pin, 
                              //I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled and 
                          //can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR  
                                //pin function is digital input, MCLR internally 
                                //tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code 
                                //protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code 
                                //protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
                                //Internal/External Switchover mode is disabled
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
                                //(Fail-Safe Clock Monitor is disabled)
#pragma config LVP = ON         //Low Voltage Programming Enable bit(RB3/PGM pin 
                                //has PGM function, low voltage programming 
                                //enabled)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out 
                                //Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 
                                //(Write protection off)

//------------------------------------------------------------------------------
//                          Prototipos
//------------------------------------------------------------------------------
void setup(void);  //Configuración
void String_Completo(char *var); //Subrutina para mostrar texto en la terminal
//------------------------------------------------------------------------------
//                          Variables
//------------------------------------------------------------------------------
int flag = 1;
unsigned char option=0;

//------------------------------------------------------------------------------
//                          Loop principal
//------------------------------------------------------------------------------
void main(void) {
    
    setup(); //Configuración
    
    while(1)//loop forever
    {
        __delay_ms(100);
        
        if (PIR1bits.TXIF){ 
            if(flag){ //Muestra el menú desplegable
                String_Completo("Que accion desea ejecutar?");
                String_Completo("(1)Desplegar cadena de caracteres");
                String_Completo("(2)Cambiar PORTA");
                String_Completo("(3)Cambiar PORTB");
                flag = 0;
            }
            if(option==49){//Si el usuario presiona '1' en ascii (49 en decimal)
                TXREG = 13; // Agrega una nueva línea
                String_Completo("Cadena de prueba"); //Despliega el texto
                flag = 1;  //Vuelve a mostrar el menú desplegable
                option = 0; //Se limpia la bandera de option
                TXREG = 13; // Agrega una nueva línea
            }
            if(option==50){
                TXREG = 13; // Agrega una nueva línea
                //Pregunta que caracter mostrar en las leds
                String_Completo("Que caracter desea desplegar en PORTA?");
                
                flag = 1; //Se enciende la bandera para mostrar el menú
                option = 0; //Se limpia la bandera de option
                
                while(!option){} //Se mantiene en el while hasta que el usuario
                 //presione algún caracter para desplegar en las leds de PORTA
                
                PORTA = option; //Se despliega el caracter en PORTA
                option = 0; //Se limpia la bandera de option
                TXREG = 13; // Agrega una nueva línea
            }
            
            if (option==51){
                TXREG = 13; // Agrega una nueva línea
                //Pregunta que caracter mostrar en las leds
                String_Completo("Que caracter desea desplegar en PORTB?");
                
                flag = 1; //Se enciende la bandera para mostrar el menú
                option = 0; //Se limpia la bandera de option
                
                while(!option){} //Se mantiene en el while hasta que el usuario
                 //presione algún caracter para desplegar en las leds de PORTB
                
                PORTB = option; //Se despliega el caracter en PORTB
                option = 0; //Se limpia la bandera de option
                TXREG = 13; // Agrega una nueva línea
            } 
        }
    }

}

void __interrupt() isr(void){
    
    if (PIR1bits.RCIF){
        option = RCREG;
}
}

//------------------------------------------------------------------------------
//                          Configuración
//------------------------------------------------------------------------------
void setup(){
    
    //Configuracion reloj
    OSCCONbits.IRCF2 = 1; //Frecuencia a 1MHz
    OSCCONbits.IRCF1 = 0;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1;
    
    //Configurar entradas y salidas
    ANSELH = 0x00;//Pines digitales
    ANSEL = 0x00; 
    
    TRISA = 0x00; //Salidas de las leds
    TRISB = 0x00;
    
    PORTA = 0x00; //Se limpian los puertos
    PORTB = 0x00;

    //Configurar la interrupcion
    INTCONbits.GIE = 1;  //Enable interrupciones globales
    INTCONbits.PEIE = 1; //Enable interrupciones periféricas
    PIE1bits.RCIE = 1; //Enable interrupción EUSART Receive
    PIR1bits.RCIF = 0; //Se limpia la bandera de interrupción EUSART Receive
             
    
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0; //EUSART en modo asíncrono
    TXSTAbits.BRGH = 1; //Velocidad alta 
    
    BAUDCTLbits.BRG16 = 1; //Se utilizan 16 bit Baud Rate Generator
    
    SPBRG = 25; 
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1; //Enable Puerto Serial 
    RCSTAbits.RX9 = 0; //Recibe 8 bits 
    RCSTAbits.CREN = 1; //Enable recepción de datos 
    
    TXSTAbits.TXEN = 1; //Enable transmisión de datos
 } 

void String_Completo(char *var){
    int i;   
    for (i = 0; i < strlen(var); i++) {
         __delay_ms(20); 
        TXREG = var[i]; //Se transmite caracter por caracter en la terminal
    }
    TXREG = 13; //Agrega una nueva línea
} 