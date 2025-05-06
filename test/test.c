#include <stdint.h>
#define __IO            volatile // 최적화 x

typedef struct{
    __IO uint32_t FSR;
    __IO uint32_t FWD;
} UART_TypeDef;

#define APB_BASEADDR    0x10000000
#define UART_BASEADDR   (APB_BASEADDR + 0x3000)
#define UART            ((UART_TypeDef *) UART_BASEADDR)

void UART_send(UART_TypeDef *UARTx, uint8_t data);

int temp = 1;

int main(){
    while(1){
        if (temp == 1){
            UART_send(UART, 1);
            temp = 0;
        }
    }
    return 0;
}

void UART_send(UART_TypeDef *UARTx, uint8_t data) {  
    if((UARTx ->FSR & (1 << 1))){
        UARTx->FWD = data;
    }
}
