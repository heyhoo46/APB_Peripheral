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

#define LED_COUNT        6  // LED 개수

void delay(int n);
void GPIO_init(void);
void LEDs_on(void);
void LEDs_off(void);

int main() {
    // GPIO 초기화
    GPIO_init();
    
    // 초기 상태에서 모든 LED 끄기
    LEDs_off();
    
    // 시작 표시: 모든 LED 3번 깜빡임
    for(int i = 0; i < 3; i++) {
        LEDs_on();
        delay(300);
        LEDs_off();
        delay(300);
    }
    
    // 메인 루프
    while(1) {
        volatile uint32_t tilt_state = TILT->TILT_REG & 0x01;
        
        if(tilt_state) {
            // 센서가 기울어진 상태: 모든 LED 켜기
            LEDs_on();
        } else {
            // 센서가 기울어지지 않은 상태: 모든 LED 끄기
            LEDs_off();
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
    // 모든 핀 초기화 후 LED 핀만 출력으로 설정
    GPIOA->MODER = 0;  // 모든 핀을 입력 모드로 초기화
    
    // LED 핀만 출력 모드로 설정
    for(int i = 0; i < LED_COUNT; i++) {
        GPIOA->MODER |= (1 << (i * 2));  // 각 핀마다 01로 설정
    }
    
    // 초기 상태에서 모든 LED 끄기
    GPIOA->ODR = 0;
}

void LEDs_on(void) {
    uint32_t odr_value = GPIOA->ODR;
    // 하위 6비트만 수정 (LED_COUNT = 6)
    odr_value = (odr_value & ~0x3F) | 0x3E;  // 0x3F = 0b111111, 0x3E = 0b111110
    GPIOA->ODR = odr_value;
}

void LEDs_off(void) {
    // 모든 LED 끄기 (하위 8비트를 0으로 설정)
    uint32_t odr_value = GPIOA->ODR;
    odr_value &= ~0xFF;  // 하위 8비트를 0으로 설정
    GPIOA->ODR = odr_value;
}
