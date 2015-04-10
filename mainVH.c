#include <__cross_studio_io.h>
#include  <msp430x16x.h>
#include <ecran.h>




void
main(void)
{
   int i;
   int j;
   char b[10] = {0xFF,0xCD,0x00,0x40,0x00,0x40,0x00,0x14,0x00,0x10} ;
   char Clear[2] = {0xFF,0xD7};
   char ScreenModeLR[4] = {0xFF,0x68,0x00,0x01};
   


//////////////////////////////////
//  INITIALISATION DES PORTS   //
/////////////////////////////////

  P1DIR = 0xFF;                             // All P1.x outputs
  P1OUT = 0;                                // All P1.x reset

  P2DIR = 0x00;                             // All P2.x inputs

  P3SEL = 0xF0;                             // P3.4,5,6,7 = USART0 TXD/RXD
  P3DIR = 0x50;                             // P3.4,6 output direction
  P3DIR &= ~(0xA0);                         // P3.5,7 input direction

  P4DIR = 0x07;                             // P4.1,2,3 outputs (enable-gps and cmd-switch)
  P4OUT = 0x02;                             //ecran
    //reset de l'ecran
    P4OUT = 0x00;                                
    for (i = 25000; i>0; i--);              // Delay
    P4OUT = 0x02;                                


////////////////////
//  CLINIOTAGE    //
////////////////////

for(j=0;j<2;j++)
  {
    int i;
    P1OUT |= 0x1e;                          // Set P1.0 LED on
    for (i = 25000; i>0; i--);              // Delay
    P1OUT &= ~0x1f;                         // Clear P1.0 LED off
    for (i = 25000; i>0; i--);              // Delay
    for (i = 25000; i>0; i--);              // Delay
    P1OUT |= 0x1e;                          // Set P1.0 LED on
    for (i = 25000; i>0; i--);              // Delay
    P1OUT &= ~0x1f;                         // Clear P1.0 LED off
    for (i = 25000; i>0; i--);              // Delay
    for (i = 25000; i>0; i--);              // Delay
    P1OUT |= 0x1e;                          // Set P1.0 LED on
    for (i = 25000; i>0; i--);              // Delay
    P1OUT &= ~0x1f;                         // Clear P1.0 LED off
    for (i = 25000; i>0; i--);              // Delay
    for (i = 25000; i>0; i--);              // Delay
    P1OUT |= 0x01;                          // Set P1.0 LED on
    for (i = 25000; i>0; i--);              // Delay
    P1OUT &= ~0x1f;                         // Clear P1.0 LED off
    for (i = 25000; i>0; i--);              // Delay
    for (i = 25000; i>0; i--);              // Delay
  }
  P1OUT = 0;                                // All P1.x reset
  P4OUT = 0x06;                             // cmd_switch sur le usb et reset-ecran 

//  WDTCTL = WDTPW + WDTHOLD;               // Stop WDT



/////////////////////////////////
//  INITIALISATION DES UARTS   //
/////////////////////////////////
	

//UART0
  UCTL0 |= SWRST;                       // Initialize USART state machine
  BCSCTL1 &= ~XT2OFF;                   // XT2 activé
  BCSCTL2 |= SELS;                      // SMCLOCL = xt2
  UTCTL0 = SSEL1;                       // UCLK = SMCLK
  UCTL0 |= CHAR;                        // 8-bit character
  UBR00 = 0x41;                         // Octet0
  UBR10 = 0x03;                         // Octet1
  UMCTL0 = 0x92;                        // modulation
  ME1 |= UTXE0 | URXE0;                 // Enable USART0 TXD/RXD
  UCTL0 &= ~SWRST;                      // Initialize USART state machine
//UART1
  UCTL1 |= SWRST;                       // Initialize USART state machine
  BCSCTL1 &= ~XT2OFF;                   // XT2 activé
  BCSCTL2 |= SELS;                      // SMCLOCL = xt2
  UTCTL1 = SSEL1;                       // UCLK = SMCLK
  UCTL1 |= CHAR;                        // 8-bit character
  UBR01 = 0x41;                         // Octet0
  UBR11 = 0x03;                         // Octet1
  UMCTL1 = 0x92;                        // modulation
  ME2 |= UTXE1 | URXE1;                 // Enable USART0 TXD/RXD
  UCTL1 &= ~SWRST;                      // Initialize USART state machine


////////////////////////////////////
//  ACTIVATION DES INTERRUPTION   //
////////////////////////////////////

  _EINT();                              //enable interupts

  IFG1 &= ~URXIFG0;                     // flag reset
  IE1 |= URXIE0;                        // Enable USART0 RX interrupt
  IFG1 &= ~UTXIFG0;                     // flag reset
  IE1 |= UTXIE0;                        // Enable USART0 TX interrupt

  IFG2 &= ~URXIFG1;                     // flag reset
  IE2 |= URXIE1;                        // Enable USART0 RX interrupt
  IFG2 &= ~UTXIFG1;                     // flag reset
  IE2 |= UTXIE1;                        // Enable USART0 TX interrupt






//////////////////
//  MAINLOOP    //
//////////////////
  
  envoicommande(b,10);
//  envoicommande(Clear,2);
//  envoicommande(ScreenModeLR,4);
//  
//  printe("C'est le test$",1,1);
  while(1)
  {
    if(P2IN == 0x1E) { P1OUT |=0x01;}else{ P1OUT = 0;}        //PUSH
    if(P2IN == 0x1D) { P1OUT |=0x04;}else{ P1OUT = 0;}        //TOP
    if(P2IN == 0x0F) { P1OUT |=0x08;}else{ P1OUT = 0;}        //RIGHT
    if(P2IN == 0x1B) { P1OUT |=0x02;}else{ P1OUT = 0;}        //BOTTOM
    if(P2IN == 0x17) { P1OUT |=0x10;}else{ P1OUT = 0;}        //LEFT
    
  }
}

/////////////////////
//  INTERUPTIONS   //
/////////////////////

#pragma vector=USART0RX_VECTOR
__interrupt void usart0_rx (void)
{
   TXBUF1 = RXBUF0;                           // RXBUF0 to TXBUF1
}

 #pragma vector=USART0TX_VECTOR
__interrupt void usart0_tx (void)
{
  next=1;
 } 

#pragma vector=USART1RX_VECTOR
__interrupt void usart1_rx (void)
{
  ACK=1;
  TXBUF0=RXBUF1;
 }

#pragma vector=USART1TX_VECTOR
__interrupt void usart1_tx (void)
{
  next=1;
 }


