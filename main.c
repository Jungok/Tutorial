//Structure & Array

typedef struct

{

  volatile unsigned int MODER;        /*!< GPIO port mode register,                     Address offset: 0x00 */

  volatile unsigned int OTYPER;       /*!< GPIO port output type register,              Address offset: 0x04 */

  volatile unsigned int OSPEEDR;      /*!< GPIO port output speed register,             Address offset: 0x08 */

  volatile unsigned int PUPDR;        /*!< GPIO port pull-up/pull-down register,        Address offset: 0x0C */

  volatile unsigned int IDR;          /*!< GPIO port input data register,               Address offset: 0x10 */

  volatile unsigned int ODR;          /*!< GPIO port output data register,              Address offset: 0x14 */

  volatile unsigned int BSRR;         /*!< GPIO port bit set/reset registerBSRR,        Address offset: 0x18 */

  volatile unsigned int LCKR;         /*!< GPIO port configuration lock register,       Address offset: 0x1C */

  volatile unsigned int AFR[2];       /*!< GPIO alternate function register,            Address offset: 0x20-0x24 */

  volatile unsigned int BRR;          /*!< GPIO bit reset register,                     Address offset: 0x28 */

}GPIO_TypeDef;



typedef enum 
{

  LED2 = 0,
  LED_GREEN = LED2
} Led_TypeDef;



//#define RCC_GPIOA (*((unsigned char*)0x4002102C))

#define PERIPH_BASE       (0x40000000UL) /*!< Peripheral base address in the alias region */

#define AHBPERIPH_BASE    (PERIPH_BASE + 0x00020000UL)

#define RCC_BASE          (AHBPERIPH_BASE + 0x00001000UL)

#define RCC_GPIOA     *((volatile unsigned int*)(RCC_BASE + 0x0000002CUL))

 

//#define GPIO_PA5PIN_BASE (unsigned int)0x50000000U

//#define GPIO_PA5PIN_MODE  (*((unsigned int*)GPIO_PA5PIN_BASE))

//#define GPIO_PA5PIN_OTYPE (*((unsigned int*)(GPIO_PA5PIN_BASE+0x00000004U)))

//#define GPIO_PA5PIN_OSPEEDR (*((unsigned int*)(GPIO_PA5PIN_BASE+0x00000008U)))

//#define GPIO_PA5PIN_OPUPDR (*((unsigned int*)(GPIO_PA5PIN_BASE+0x0000000CU)))

//#define GPIO_PA5PIN_IDR (*((unsigned int*)(GPIO_PA5PIN_BASE+0x00000010U)))

//#define GPIO_PA5PIN_ODR (*((unsigned int*)(GPIO_PA5PIN_BASE+0x00000014U)))



#define GPIOA_BASE      ((unsigned int)0x50000000UL)

#define GPIOA         ((GPIO_TypeDef *) GPIOA_BASE)



#define GPIO_PIN_5       ((unsigned short)0x0020U)  /* Pin 5 selected    */

#define GPIO_PIN_5_POS    5

#define GPIO_2BIT_POS_MASK    ((unsigned int)0x00000003U)

#define GPIO_1BIT_POS_MASK  ((unsigned int)0x00000001U)

 

#define GPIO_MODE_OUTPUT         ((unsigned int)0x00000001U)   /*!< GPIO OutPut Mode                 */

#define GPIO_OUTPUT_TYPE_0        ((unsigned int)0x00000000U)

#define GPIO_SPEED_FREQ_VERY_HIGH   ((unsigned int)0x00000003U)  /*!< range  10 MHz to 35 MHz, please refer to the product datasheet */



#define LEDn         1

#define LED2_GPIO_PORT    GPIOA



GPIO_TypeDef* LED_PORT[LEDn] = {LED2_GPIO_PORT};



void delay(unsigned int delay_cnt);



int main()

{

// unsigned int position = 5U; //Pin Position: PA5

 unsigned int temp = 0x0U;

 unsigned int mode = 0x0U;

 unsigned int reg = 0x0U;

 

 // RCC - GPIO A Port Pwr

 reg = RCC_GPIOA;

 RCC_GPIOA = (unsigned int)(reg | 0x01U);

 

 // GPIO-PA5

 //GPIOA-Reg 

 // Mode

 reg = LED_PORT[LED2]->MODER; //GPIO_PA5PIN_MODE

 temp = ~(GPIO_2BIT_POS_MASK << (GPIO_PIN_5_POS*2U)); //0xFFFFF3FF

 reg &= temp;

 mode = GPIO_MODE_OUTPUT << (GPIO_PIN_5_POS*2U) ; // GPO Mode - 2bit 0x00000400

 reg |= mode;

 LED_PORT[LED2]->MODER = reg;



 //OTYPER

 reg = LED_PORT[LED2]->OTYPER; //GPIO_PA5PIN_OTYPE

 temp = ~(GPIO_1BIT_POS_MASK << GPIO_PIN_5_POS); //~0x20 ==> 0xFFFFFFDF

 reg &= temp;

 mode = GPIO_OUTPUT_TYPE_0 << GPIO_PIN_5_POS ; //PP Output type - 1bit

 reg |= mode;

 LED_PORT[LED2]->OTYPER = reg;



 //OSPEEDR

 reg = LED_PORT[LED2]->OSPEEDR; //GPIO_PA5PIN_OSPEEDR

 temp = ~(GPIO_2BIT_POS_MASK << (GPIO_PIN_5_POS*2U)); //0xFFFFF3FF

 reg &= temp;

 mode = GPIO_SPEED_FREQ_VERY_HIGH << (GPIO_PIN_5_POS*2U) ; //Very High Mode - 2bit 0x00000C00

 reg |= mode;

 LED_PORT[LED2]->OSPEEDR = reg;



 //ODR

 reg = LED_PORT[LED2]->ODR; //GPIO_PA5PIN_ODR

 temp = ~(GPIO_1BIT_POS_MASK << GPIO_PIN_5_POS); //~0x20 ==> 0xFFFFFFDF

 reg &= temp;



 mode = 1 << GPIO_PIN_5_POS ; 

 LED_PORT[LED2]->ODR = reg | mode;



 

 while(1)

 {

  //ODR 

   //GPIO_PA5PIN_ODR ^= 0x20;

  mode = ~mode&(GPIO_1BIT_POS_MASK << GPIO_PIN_5_POS);

  LED_PORT[LED2]->ODR = reg | mode;

  delay(0x20000);



 }

}



void delay(unsigned int delay_cnt)

{

  volatile int counter = 0;



  while(counter < delay_cnt) //delay loop

  {

   counter++;

  }

}

