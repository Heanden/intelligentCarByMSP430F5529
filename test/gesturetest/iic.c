#include "iic.h"
#include "Delay.h"

extern unsigned char RXByteCtr;
extern unsigned char TXByteCtr;
extern unsigned char TxData[10];
extern unsigned char RxBuf[10];

void iic_init(uint8_t SLAVE_ADDRESS)
{
    P4SEL |= 0x06;
    UCB1CTL1 |= UCSWRST;                   // Enable SW reset
    UCB1CTL0 |= UCMST + UCMODE_3 + UCSYNC; // I2C Master, synchronous mode
    UCB1CTL1 |= UCSSEL_2 + UCSWRST;        // Use SMCLK
    UCB1BR0 = 240;                         // fSCL = SMCLK/240 = 100kHz
    UCB1BR1 = 0;
    UCB1I2CSA = SLAVE_ADDRESS; // Slave Address
    UCB1CTL1 &= ~UCSWRST;      // Clear SW reset, resume operation
}
void iic_WriteMode(void)
{
    UCB1CTL1 |= UCTR;
    USCI_B_I2C_clearInterrupt(USCI_B0_BASE, USCI_B_I2C_TRANSMIT_INTERRUPT);
    UCB1IE &= ~UCRXIE; // Disable RX interrupt
    UCB1IE |= UCTXIE;  // Enable TX interrupt
}
void iic_ReadMode(void)
{
    UCB1CTL1 &= ~UCTR;
    USCI_B_I2C_clearInterrupt(USCI_B0_BASE, USCI_B_I2C_RECEIVE_INTERRUPT);
    UCB1IE &= ~UCTXIE; // Disable TX interrupt
    UCB1IE |= UCRXIE;  // Enable RX interrupt
}
void iic_writebyte(uint8_t reg_address, uint8_t write_number, uint8_t *data_to_write)
{
    uint8_t i = 0;
    while (UCB1STAT & UCBBUSY)
        ;
    iic_WriteMode();
    TxData[write_number] = reg_address;
    for (i = write_number; i > 0; i--)
    {
        TxData[i - 1] = *data_to_write++;
    }
    TXByteCtr = write_number + 1; // Load TX byte counter
    UCB1CTL1 |= UCTXSTT;          // I2C TX, start condition
    while (UCB1CTL1 & UCTXSTP)
        ;
}
void iic_readbyte(uint8_t reg_address, uint8_t read_number)
{
    while (UCB1STAT & UCBBUSY)
        ;
    iic_WriteMode();
    TxData[0] = reg_address;
    TXByteCtr = 1;       // Load TX byte counter
    UCB1CTL1 |= UCTXSTT; // I2C TX, start condition
    while (UCB1CTL1 & UCTXSTP)
        ;
    Delay_us(200);
    iic_ReadMode();
    RXByteCtr = read_number;
    UCB1CTL1 |= UCTXSTT;
    while (USCI_B_I2C_getInterruptStatus(USCI_B0_BASE, USCI_B_I2C_RECEIVE_INTERRUPT))
        ;
    while (UCB1CTL1 & UCTXSTP)
        ;
}