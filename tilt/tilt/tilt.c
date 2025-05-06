#include <stdint.h>

#define __IO volatile 

typedef struct {
    __IO uint32_t TILT_REG;
} TILT_TypeDef;

typedef struct {
    __IO uint32_t MODER;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
} GPIO_TypeDef;

#define APB_BASEADDR     0x10000000
#define TILT_BASEADDR    (APB_BASEADDR + 0x3000)  // 0x10003000
#define GPIOA_BASEADDR   (APB_BASEADDR + 0x2000)  // 0x10002000

#define TILT             ((TILT_TypeDef *) TILT_BASEADDR)
#define GPIOA            ((GPIO_TypeDef *) GPIOA_BASEADDR)

#define LED_PIN          0  // LED 핀

void delay(int n);
void GPIO_init(void);

int main() {
    // GPIO 초기화
    GPIO_init();
    
    // 시작 표시: LED 3번 깜빡임
    for(int i = 0; i < 3; i++) {
        GPIOA->ODR |= (1 << LED_PIN);   // LED 켜기
        delay(300);
        GPIOA->ODR &= ~(1 << LED_PIN);  // LED 끄기
        delay(300);
    }
    
    // 메인 루프: 틸트 센서 상태에 따라 LED 제어
    while(1) {
        volatile uint32_t tilt_state = TILT->TILT_REG & 0x01;
        
        if(tilt_state) {
            // 센서가 기울어진 상태: LED 켜기
            GPIOA->ODR |= (1 << LED_PIN);
        } else {
            // 센서가 기울어지지 않은 상태: LED 끄기
            GPIOA->ODR &= ~(1 << LED_PIN);
        }
        
        // 짧은 지연
        delay(10);
    }
    
    return 0;
}

void delay(int n) {
    volatile uint32_t temp = 0;
    for (int i = 0; i < n; i++) {
        temp = 0;
        for(int j = 0; j < 500; j++) {
            temp++;
        }
    }
}

void GPIO_init(void) {
    // LED 핀을 출력 모드로 설정
    GPIOA->MODER = 0;
    GPIOA->MODER |= (1 << LED_PIN);
    
    // 초기 상태에서 LED 끄기
    GPIOA->ODR &= ~(1 << LED_PIN);
}