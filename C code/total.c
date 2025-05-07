#include <stdint.h>
#include <stdio.h>

#define __IO            volatile

typedef struct {
    __IO uint32_t MODER;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
} GPIO_TypeDef;

typedef struct {
    __IO uint32_t FCR;
    __IO uint32_t FDR;
    __IO uint32_t FPR;
} FND_TypeDef;

typedef struct {
    __IO uint32_t UCR;
    __IO uint32_t UDR;
} ULTRA_TypeDef;

typedef struct {
    __IO uint32_t BDR;
} BLINK_TypeDef;

typedef struct {
    __IO uint32_t DDR;
    __IO uint32_t DMR;
} DHT_TypeDef;

typedef struct {
    __IO uint32_t TCNR;
    __IO uint32_t TCR;
    __IO uint32_t PSC;
    __IO uint32_t ARR;
} TIMER_TypeDef;

typedef struct{
    __IO uint32_t FSR;
    __IO uint32_t FWD;
    __IO uint32_t FRD;
} UART_TypeDef;

typedef struct {
    __IO uint32_t TILT_REG;
} TILT_TypeDef;

#define APB_BASEADDR        0x10000000
#define TIMER_BASEADDR      (APB_BASEADDR + 0x1000)
#define GPIOA_BASEADDR      (APB_BASEADDR + 0x1400)
#define GPIOB_BASEADDR      (APB_BASEADDR + 0x1800)
#define GPIOC_BASEADDR      (APB_BASEADDR + 0x1C00)
#define FND_BASEADDR        (APB_BASEADDR + 0x2000)

#define ULTRA_BASEADDR      (APB_BASEADDR + 0x2400)
#define DHT_BASEADDR        (APB_BASEADDR + 0x2800)
#define BLINK_BASEADDR      (APB_BASEADDR + 0x2C00)

#define TIMER2_BASEADDR     (APB_BASEADDR + 0x3000)
#define UART_BASEADDR       (APB_BASEADDR + 0x3400)
#define TILT_BASEADDR       (APB_BASEADDR + 0x4400)
#define NOTHING_BASEADDR    (APB_BASEADDR + 0x3C00)
#define BUZZER_BASEADDR     (APB_BASEADDR + 0x4000)

#define TIMER            ((TIMER_TypeDef *) TIMER_BASEADDR)
#define GPIOA            ((GPIO_TypeDef *) GPIOA_BASEADDR)
#define GPIOB           ((GPIO_TypeDef *) GPIOB_BASEADDR)
#define GPIOC           ((GPIO_TypeDef *) GPIOC_BASEADDR)
#define FND             ((FND_TypeDef *) FND_BASEADDR)

#define ULTRA           ((ULTRA_TypeDef *) ULTRA_BASEADDR)
#define DHT             ((DHT_TypeDef *) DHT_BASEADDR)
#define BLINK           ((BLINK_TypeDef *) BLINK_BASEADDR)

#define TIMER2            ((TIMER_TypeDef *) TIMER2_BASEADDR)
#define UART       ( (UART_TypeDef *) UART_BASEADDR)
#define TILT       ( (TILT_TypeDef *) TILT_BASEADDR)
#define NOTHING    ( (NOTHING_TypeDef *) NOTHING_BASEADDR)
#define BUZZER     ( (BLINK_TypeDef *) BUZZER_BASEADDR)

#define POWER_ON    1
#define POWER_OFF   0
#define TEMPERATURE   0
#define HUMIDITY   1

#define LEFT  16000
#define RIGHT  16001
#define HAZARD  16002

#define DEFAULT_STATE           0
#define HAZARD_BLINK_STATE      1
#define RIGHT_BLINK_STATE       2
#define LEFT_BLINK_STATE         3


#define F_CPU 100000000


void delay(int n);
void DOT3_Timer(uint32_t *DOT3, uint32_t *btn_flag2);
void LED_init(GPIO_TypeDef *GPIOx);
void LED_write(GPIO_TypeDef *GPIOx, uint32_t data);

void Switch_init(GPIO_TypeDef *GPIOx);
uint32_t Switch_read(GPIO_TypeDef *GPIOx);

void FND_init(FND_TypeDef *fnd, uint32_t power);
void FND_writeData(FND_TypeDef *fnd, uint32_t data);
void FND_writeDot(FND_TypeDef *fnd, uint32_t dot);

void Ultra_init(ULTRA_TypeDef *ultra, uint32_t power);
uint32_t Ultra_read(ULTRA_TypeDef *ultra);

void DHT_init(DHT_TypeDef *dht, uint32_t moder);

uint32_t DHT_read(DHT_TypeDef *dht);

void BLINK_init(BLINK_TypeDef *blink, uint32_t duty_rate);

void Timer_stop(TIMER_TypeDef *timerx);
void Timer_start(TIMER_TypeDef *timerx);
void Timer_clear(TIMER_TypeDef *timerx);
void Timer_write_psc(TIMER_TypeDef *timerx, uint32_t psc);
void Timer_write_arr(TIMER_TypeDef *timerx, uint32_t arr);
uint32_t Timer_read(TIMER_TypeDef *timerx);

void UART_send(UART_TypeDef *UARTx, uint8_t data);

void value_divide(uint32_t value, uint8_t *acii);
void UART_Send_Temp(UART_TypeDef *uart, uint32_t temp_integral_10, uint32_t temp_integral_1, uint32_t temp_decimal_10, uint32_t temp_decimal_1);
void UART_Send_Humi(UART_TypeDef *uart, uint32_t humi_integral_10, uint32_t humi_integral_1, uint32_t humi_decimal_10, uint32_t humi_decimal_1);
void UART_Send_distance(UART_TypeDef *uart, uint32_t dist_1000, uint32_t dist_100, uint32_t dist_10, uint32_t dist_1);
void UART_Send_tilt_msg(UART_TypeDef *uart);

int get_thousands_place(uint32_t *value);
int get_hundreds_place(uint32_t *value);
int get_tens_place(uint32_t *value);
int get_ones_place(uint32_t *value);

int tilt_read(TILT_TypeDef *tilt);


#define led_default 0b11

int main() {
    LED_init(GPIOA);
    Switch_init(GPIOB);
    Switch_init(GPIOC);

    FND_init(FND, POWER_ON);
    FND_writeDot(FND, 0);

    Timer_write_psc(TIMER2, (10000000 - 1));
    Timer_write_arr(TIMER2, 5 - 1);
    Timer_start(TIMER2);

    uint32_t DOT3 = (1 << 2);
    uint32_t temp = 0;
    uint32_t temperature = 0;
    uint32_t humidity = 0;

    uint32_t distance = 0;
    uint32_t blinker_state = 0;
    uint32_t btn_flag3 = 0;
    uint32_t btn_detect = 0;
    uint32_t blink_flag = 0;
    uint32_t fnd_blink = 0;
    uint32_t ggambbak = 0;
    uint32_t fnd_shape = 0;
    uint32_t led_data = 0b11;


    while (1) {
        DOT3_Timer(&DOT3, &btn_flag3);

        uint32_t sw = Switch_read(GPIOB);

        switch (sw) {
            case 0x00:
                if (temp == 10000) temp = -1;
                temp++;
                FND_writeData(FND, temp);
                delay(100);
                break;

            case (1 << 7):
                if (temp == 0) temp = 10000;
                temp--;
                FND_writeData(FND, temp);
                delay(100);
                break;

            case (1 << 6):
                delay(500);
                Ultra_init(ULTRA, POWER_ON);
                delay(10);
                distance = Ultra_read(ULTRA);
                Ultra_init(ULTRA, POWER_OFF);
                FND_writeData(FND, distance);
                BLINK_init(BLINK, distance);
                BLINK_init(BUZZER, distance);
                delay(100);
                UART_Send_distance(UART, get_thousands_place(&distance), get_hundreds_place(&distance), get_tens_place(&distance), get_ones_place(&distance));
                break;

            case (1 << 5):
                DHT_init(DHT, TEMPERATURE);
                delay(1000);
                temperature = DHT_read(DHT);
                FND_writeData(FND, temperature);
                delay(100);
                UART_Send_Temp(UART, get_thousands_place(&temperature), get_hundreds_place(&temperature), get_tens_place(&temperature), get_ones_place(&temperature));
                break;

            case (1 << 4):
                DHT_init(DHT, HUMIDITY);
                delay(1000);
                humidity = DHT_read(DHT);
                FND_writeData(FND, humidity);
                delay(100);
                UART_Send_Humi(UART, get_thousands_place(&humidity), get_hundreds_place(&humidity), get_tens_place(&humidity), get_ones_place(&humidity));
                break;

            case (1 << 3):
                LED_write(GPIOA, led_default);
                FND_init(FND,POWER_OFF);
                while(Switch_read(GPIOB) == (1<<3))
                {
                    uint8_t sw_btn = Switch_read(GPIOC);

                    switch(blinker_state){
                        case HAZARD_BLINK_STATE:
                            ggambbak = 0b11;
                            fnd_shape = HAZARD;
                            if(Timer_read(TIMER2) == 0 && blink_flag == 0)
                            {
                                blink_flag = 1;
                                led_data ^= ggambbak;
                                fnd_blink = (led_data & ggambbak) == 0 ? POWER_OFF : POWER_ON;
                            }
                            else if(Timer_read(TIMER2) != 0) blink_flag = 0;

                            delay(10);

                            BLINK_init(BUZZER, 49);
                            LED_write(GPIOA, led_data);
                            FND_init(FND,fnd_blink);
                            FND_writeData(FND, fnd_shape);
                            break;
                        case LEFT_BLINK_STATE:
                            ggambbak = 0b01;
                            fnd_shape = RIGHT;
                            if(Timer_read(TIMER2) == 0 && blink_flag == 0)
                            {
                                blink_flag = 1;
                                led_data ^= ggambbak;
                                fnd_blink = (led_data & ggambbak) == 0 ? POWER_OFF : POWER_ON;
                            }
                            else if(Timer_read(TIMER2) != 0) blink_flag = 0;

                            delay(10);

                            BLINK_init(BUZZER, 49);
                            LED_write(GPIOA, led_data);
                            FND_init(FND,fnd_blink);
                            FND_writeData(FND, fnd_shape);
                            break;
                        case RIGHT_BLINK_STATE:
                            ggambbak = 0b10;
                            fnd_shape = LEFT;
                            if(Timer_read(TIMER2) == 0 && blink_flag == 0)
                            {
                                blink_flag = 1;
                                led_data ^= ggambbak;
                                fnd_blink = (led_data & ggambbak) == 0 ? POWER_OFF : POWER_ON;
                            }
                            else if(Timer_read(TIMER2) != 0) blink_flag = 0;

                            delay(10);

                            BLINK_init(BUZZER, 49);
                            LED_write(GPIOA, led_data);
                            FND_init(FND,fnd_blink);
                            FND_writeData(FND, fnd_shape);
                            break;
                        case DEFAULT_STATE: break;
                    }

                    switch(sw_btn)
                    {
                        case (1<<0):
                            blinker_state = LEFT_BLINK_STATE;
                            break;

                        case (1<<1):
                            blinker_state = RIGHT_BLINK_STATE;
                            break;

                        case (1<<4):
                            delay(10);
                            if((Switch_read(GPIOC) == (1<<4)) && (btn_detect == 0))
                            {
                                btn_detect = 1;
                                blinker_state ^= HAZARD_BLINK_STATE;
                            }
                            break;

                        default:
                            btn_detect = 0;
                            if(!(blinker_state == 1))
                            {
                                blinker_state = DEFAULT_STATE;
                                led_data = 0b11;
                                LED_write(GPIOA, led_default);
                                FND_init(FND,POWER_OFF);
                                BLINK_init(BUZZER, 1);
                            }
                    }

                }

                LED_write(GPIOA, 0);
                FND_init(FND, POWER_ON);
                break;
            
            case (1<<2):
                FND_init(FND, POWER_OFF);
                while(Switch_read(GPIOB) == (1<<2))
                {
                    delay(10);
                    if(tilt_read(TILT))
                    {
                        LED_write(GPIOA,(1<<7));
                        UART_send(UART, 'w');
                        UART_send(UART, 'a');
                        UART_send(UART, 'r');
                        UART_send(UART, 'n');
                        UART_send(UART, 'i');
                        UART_send(UART, 'n');
                        UART_send(UART, 'g');
                        UART_send(UART, '!');
                        UART_send(UART, '!');
                        UART_send(UART, 0x0A);
                        //UART_Send_tilt_msg(UART);
                        delay(500);
                    }
                    else LED_write(GPIOA, 0);
                }
                FND_init(FND, POWER_ON);
                break;


            default:
                FND_writeData(FND, 7777);
                break;
        }
    }
    return 0;
}

void delay(int n){
    uint32_t temp = 0;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < 1000; j++){
            temp++;
        }
    }
}

void LED_init(GPIO_TypeDef *GPIOx){
    GPIOx->MODER = 0xff;
}

void LED_write(GPIO_TypeDef *GPIOx, uint32_t data){
    GPIOx->ODR = data;
}

void Switch_init(GPIO_TypeDef *GPIOx){
    GPIOx->MODER = 0x00;
}

uint32_t Switch_read(GPIO_TypeDef *GPIOx){
    return GPIOx->IDR;
}

void FND_init(FND_TypeDef *fnd, uint32_t power){
    fnd->FCR = power;
}

void FND_writeData(FND_TypeDef *fnd, uint32_t data){
    fnd->FDR = data;
}

void FND_writeDot(FND_TypeDef *fnd, uint32_t dot){
    fnd->FPR = dot;
}

void Ultra_init(ULTRA_TypeDef *ultra, uint32_t power){
    ultra->UCR = power;
}

uint32_t Ultra_read(ULTRA_TypeDef *ultra){
    return ultra->UDR;
}

void DHT_init(DHT_TypeDef *dht, uint32_t moder){
    dht->DMR = moder;
}

uint32_t DHT_read(DHT_TypeDef *dht){
    return dht->DDR;
}

void BLINK_init(BLINK_TypeDef *blink, uint32_t duty_rate){
    blink->BDR = duty_rate;
}


void Timer_stop(TIMER_TypeDef *timerx){
    timerx->TCR = 0;
}
void Timer_start(TIMER_TypeDef *timerx){
    timerx->TCR = (1<<0);
}
void Timer_clear(TIMER_TypeDef *timerx){
    timerx->TCR = (1<<1);
}
void Timer_write_psc(TIMER_TypeDef *timerx, uint32_t psc){
    timerx->PSC = psc;
}
void Timer_write_arr(TIMER_TypeDef *timerx, uint32_t arr){
    timerx->ARR = arr;
}
uint32_t Timer_read(TIMER_TypeDef *timerx){
    return timerx->TCNR;
}

void DOT3_Timer(uint32_t *DOT3, uint32_t *btn_flag2){
    if(Timer_read(TIMER2) == 0 && *btn_flag2 == 0){
        *DOT3 ^= (1<<2);
        *btn_flag2 = 1;
    }
    else if(Timer_read(TIMER2)!=0) *btn_flag2 = 0;

    FND_writeDot(FND, *DOT3);
}

void UART_send(UART_TypeDef *UARTx, uint8_t data)
{  
    while ((UARTx->FSR & (1<<1)));// UART TX READY 
    UARTx->FWD = data;
}

void value_divide(uint32_t value, uint8_t *ascii)
{
    ascii[3] = 0;
    while (value >= 1000) {
        value -= 1000;
        ascii[3]++;
    }

    ascii[2] = 0;
    while (value >= 100) {
        value -= 100;
        ascii[2]++;
    }

    ascii[1] = 0;
    while (value >= 10) {
        value -= 10;
        ascii[1]++;
    }

    ascii[0] = value;
}


void UART_Send_Temp(UART_TypeDef *uart, uint32_t temp_integral_10, uint32_t temp_integral_1, uint32_t temp_decimal_10, uint32_t temp_decimal_1)
{
    uint32_t text_string[] = {
        'T',         // T
        'e',         // e
        'm',         // m
        'p',         // p
        ':',         // :
        '0' + temp_integral_10,   // tens
        '0' + temp_integral_1,    // ones
        '.',
        '0' + temp_decimal_10,    // tens
        '0' + temp_decimal_1,     // ones
        0x0A,         // \n (newline)
        0x00          // NULL (string terminator)
    };
    
    for (int i = 0; text_string[i] != 0x00; i++) {
        UART_send(uart, text_string[i]);
    }

}

void UART_Send_Humi(UART_TypeDef *uart, uint32_t humi_integral_10, uint32_t humi_integral_1, uint32_t humi_decimal_10, uint32_t humi_decimal_1)
{
    uint32_t text_string[] = {
        'H',         // H
        'u',         // u
        'm',         // m
        'i',         // i
        'd',         // d
        'i',         // i
        't',         // t
        'y',         // y
        ':',         // :
        '0' + humi_integral_10,   // tens
        '0' + humi_integral_1,    // ones
        '0' + humi_decimal_10,    // tens
        '0' + humi_decimal_1,     // ones
        '%',
        0x0A,         // \n (newline)
        0x00          // NULL (string terminator)
    };
    
    for (int i = 0; text_string[i] != 0x00; i++) {
        UART_send(uart, text_string[i]);
    }
    
}

void UART_Send_distance(UART_TypeDef *uart, uint32_t dist_1000, uint32_t dist_100, uint32_t dist_10, uint32_t dist_1)
{
    uint32_t text_string[] = {
        'd', // d
        'i', // i
        's', // s
        't', // t
        ':', // :                    
        '0' + dist_1000,  // 1000
        '0' + dist_100,   // 100
        '.',
        '0' + dist_10,    // 10
        '0' + dist_1,     // 1
        0x0A, // \n (newline)
        0x00  // NULL (string terminator)
    };
    
    for (int i = 0; text_string[i] != 0x00; i++) {
        UART_send(uart, text_string[i]);
    }
}


int get_thousands_place(uint32_t *value)
{
    int k = 0;
    while (*value >= 1000) {
        *value -= 1000;
        k++;
    }
    return k;
}

int get_hundreds_place(uint32_t *value)
{
    int k = 0;
    while (*value >= 100) {
        *value -= 100;
        k++;
    }
    return k;
}

int get_tens_place(uint32_t *value)
{
    int k = 0;
    while (*value >= 10) {
        *value -= 10;
        k++;
    }
    return k;
}

int get_ones_place(uint32_t *value)
{
    int k = 0;
    return k = *value;
}

int tilt_read(TILT_TypeDef *tilt)
{
    return (tilt->TILT_REG & 0x01);
}