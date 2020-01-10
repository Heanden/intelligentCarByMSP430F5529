/************************************************************************
 * @file	main.c
 * @author	Mr.Blue
 * @version V1.0
 * @date 	2015-6-16
 * @brief   This file provides functions to manage the functionalities
 *    		of the IIC
 * @verbatim
************************************************************************/
/******************************INCLUDE**********************************/
#include "MSP430F5529_conf.h"
#include "Delay.h"
#include "IIC_device.h"
/******************************DEFINE***********************************/
#define UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ   24000
//Target frequency for MCLK in kHz
#define UCS_MCLK_FLLREF_RATIO   6
//MCLK/FLLRef Ratio
#define UCS_XT1_TIMEOUT 50000
//Desired Timeout for XT1 initialization
#define UCS_XT2_TIMEOUT 50000
//Desired Timeout for XT2 initialization

#define IIC_SLAVE_ADDRESS 0X50

void UCS_init();
void GPIO_init();
/*****************************VARIABLE**********************************/
u16 status;
//Variable to store status of Oscillator fault flags

unsigned char TXByteCtr=0,RXByteCtr=0;
unsigned char TxData[10]={0};
unsigned char RxBuf[10]={0};

uint8_t data_address=2;
uint8_t data_to_write[10]={'A','B','C','D','E','F','G','H','I','J'};
uint8_t data_to_read[10]={0};
/*******************************MAIN************************************/
int main(void)
{
    u8 j=0;
    WDT_A_hold(WDT_A_BASE);
    UCS_init();
    GPIO_init();
    iic_init(IIC_SLAVE_ADDRESS);
    iic_writebyte(data_address,4,data_to_write);
    Delay_ms(2);
    iic_readbyte(data_address,4);
    while(1)
    {
        for(j=0;j<=9;j++)
        {
            data_to_read[j]=RxBuf[9-j];
        }
    	GPIO_toggleOutputOnPin(GPIO_PORT_P1,GPIO_PIN0);
    	Delay_ms(500);
    }
}

void UCS_init()
{
	//Set VCore = 3 for 25MHz clock
    PMM_setVCore(PMM_CORE_LEVEL_3 );
    UCS_setExternalClockSource(32768,4000000);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,GPIO_PIN4);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5,GPIO_PIN5);
    UCS_turnOnLFXT1WithTimeout(UCS_XT1_DRIVE_0,UCS_XCAP_3,UCS_XT1_TIMEOUT);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5,GPIO_PIN2);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P5,GPIO_PIN3);
    UCS_turnOnXT2WithTimeout(UCS_XT2_DRIVE_4MHZ_8MHZ,UCS_XT2_TIMEOUT);

    //Select XT1 as ACLK source
    UCS_initClockSignal(UCS_ACLK,UCS_XT1CLK_SELECT,UCS_CLOCK_DIVIDER_1);
    //Select XT2 as FLL reference
	UCS_initClockSignal(UCS_FLLREF,UCS_XT2CLK_SELECT,UCS_CLOCK_DIVIDER_1);
	//Set Ratio and Desired MCLK Frequency  and initialize DCO
	UCS_initFLLSettle(UCS_MCLK_DESIRED_FREQUENCY_IN_KHZ,UCS_MCLK_FLLREF_RATIO);
	//Select DCO as SMCLK source
	UCS_initClockSignal(UCS_SMCLK,UCS_DCOCLK_SELECT,UCS_CLOCK_DIVIDER_1);

    // Enable global oscillator fault flag
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);

    // Enable global interrupt
    __bis_SR_register(GIE);
}

void GPIO_init()
{
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN7);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=UNMI_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(UNMI_VECTOR)))
#endif
void NMI_ISR(void)
{
    do
    {
        // If it still can't clear the oscillator fault flags after the timeout,
        // trap and wait here.
        status = UCS_clearAllOscFlagsWithTimeout(1000);
    }
    while(status != 0);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCI_B0_ISR(void)
{
    {
      switch(__even_in_range(UCB0IV,12))
      {
      case  0: break;                           // Vector  0: No interrupts
      case  2: break;                           // Vector  2: ALIFG
      case  4: break;                           // Vector  4: NACKIFG
      case  6: break;                           // Vector  6: STTIFG
      case  8: break;                           // Vector  8: STPIFG
      case 10:                                  // Vector 10: RXIFG
          if (RXByteCtr)
          {
              RXByteCtr--;
              RxBuf[RXByteCtr] = UCB0RXBUF;
              if(RXByteCtr==0)
              {
                  UCB0CTL1 |= UCTXSTP+UCTXNACK;    //必须在接受最后一个数据之前发送停止和NACK
              }
              UCB0CTL1 &=~UCTXNACK;
          }
          else
          {
              USCI_B_I2C_clearInterrupt(USCI_B0_BASE,USCI_B_I2C_RECEIVE_INTERRUPT);
          }
        break;
      case 12:
          if (TXByteCtr)                          // Check TX byte counter
          {
            TXByteCtr--;
            UCB0TXBUF = TxData[TXByteCtr];                // Load TX buffer
          }
          else
          {
            UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
            UCB0IFG &= ~UCTXIFG;                  // Clear USCI_B0 TX int flag
          }
          break;                                // Vector 12: TXIFG
      default: break;
      }
    }
}

