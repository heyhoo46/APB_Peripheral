#include <stdint.h>

#define __IO volatile

#define APB_BASEADDR     0x10000000        // APB base address (남겨둠)

/* ==========================
   BUZZER 레지스터 구조체
   ========================== */
typedef struct {
    __IO uint32_t EN;       // 0x00: Enable (0=OFF, 1=ON)
    __IO uint32_t DUTY;     // 0x04: PWM Duty Cycle
    __IO uint32_t PERIOD;   // 0x08: PWM Period
} BUZZER_TypeDef;

/* ==========================
   BUZZER 베이스 주소 정의
   ========================== */
#define BUZZER_BASEADDR  (APB_BASEADDR + 0x6000)
#define BUZZER           ((BUZZER_TypeDef *) BUZZER_BASEADDR)

/* ==========================
   함수 프로토타입
   ========================== */
void delay(int n);  // 디바운싱 등에서 사용 (남겨둠)

void BUZZER_init(void);
void BUZZER_on(void);
void BUZZER_off(void);
void BUZZER_setPWM(uint32_t duty, uint32_t period);
void BUZZER_setFrequency(uint32_t freq);
void BUZZER_beep(uint32_t duration_ms);



// 🔻 main 함수
int main(void) {
    Buzzer_Init(300, 1000);
    Buzzer_Enable();

    for (volatile int i = 0; i < 1000000; i++);

    Buzzer_Disable();

    Buzzer_Set(250, 500);
    Buzzer_Enable();

    for (volatile int i = 0; i < 1000000; i++);

    Buzzer_Disable();

    while (1);
    return 0;
}

/* ==========================
   BUZZER 함수 구현
   ========================== */

// 초기화
void BUZZER_init(void)
{
    BUZZER->EN     = 0;
    BUZZER->PERIOD = 1000;
    BUZZER->DUTY   = 500;
}

// ON
void BUZZER_on(void)
{
    BUZZER->EN = 1;
}

// OFF
void BUZZER_off(void)
{
    BUZZER->EN = 0;
}

// PWM 설정
void BUZZER_setPWM(uint32_t duty, uint32_t period)
{
    if (duty > period) duty = period;
    if (period == 0) period = 1;

    BUZZER->PERIOD = period;
    BUZZER->DUTY   = duty;
}

// 주파수 기반 PWM 설정
void BUZZER_setFrequency(uint32_t freq)
{
    uint32_t clk_divider = 50000;  // SystemVerilog 분주기와 일치
    uint32_t clk_freq = 100000000 / clk_divider;  // 2kHz
    uint32_t period = clk_freq / freq;
    uint32_t duty   = period / 2;

    BUZZER_setPWM(duty, period);
}

// 일정 시간 울리기
void BUZZER_beep(uint32_t duration_ms)
{
    BUZZER_on();
    delay(duration_ms);
    BUZZER_off();
}
