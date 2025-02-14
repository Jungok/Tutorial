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

  LED2 = 0,    /* PA5*/

 USER_LED1,   /* PA8*/



  LED_GREEN = LED2

} Led_TypeDef;



typedef enum

{

  GPIO_PIN_RESET = 0U,

  GPIO_PIN_SET

} GPIO_PinState;



#define PERIPH_BASE       ((unsigned int)0x40000000U) /*!< Peripheral base address in the alias region */

#define AHBPERIPH_BASE    (PERIPH_BASE + 0x00020000U)

#define RCC_BASE          (AHBPERIPH_BASE + 0x00001000U)

//#define RCC_GPIOA (*((volatile unsigned char*)0x4002102C))

//#define RCC_GPIOA     *((volatile unsigned int*)(RCC_BASE + 0x0000002CUL))

#define RCC_IOPENR      (*((volatile unsigned int*)(RCC_BASE + 0x0000002CUL)))

#define RCC_GPIOA    RCC_IOPENR 

#define RCC_GPIOA_POS        0 // RCC_IOPENR LSB

#define RCC_ON       ((unsigned int)0x00000001U)



#define GPIO_NUMBER           (16U)

#define GPIOA_BASE (unsigned int)0x50000000U

#define GPIOA         ((GPIO_TypeDef *) GPIOA_BASE)

#define GPIO_2BIT_POS_MASK    ((unsigned int)0x00000003U)

#define GPIO_1BIT_POS_MASK  ((unsigned int)0x00000001U)

 

#define GPIO_MODE_OUTPUT         ((unsigned int)0x00000001U)   /*!< GPIO OutPut Mode  */

#define GPIO_OUTPUT_TYPE_0        ((unsigned int)0x00000000U)   /* PP Mode */

#define GPIO_SPEED_FREQ_VERY_HIGH   ((unsigned int)0x00000003U)  /*!< range  10 MHz to 35 MHz, please refer to the product datasheet */

#define GPIO_NOPULL        (0x00000000U)   /*!< No Pull-up or Pull-down activation  */



//PA5

#define GPIO_LED2_PIN_POS    5

//PA8

#define GPIO_USER_LED1_PIN_POS    8



#define LEDn         2

GPIO_TypeDef* LED_PORT[LEDn] = {GPIOA, GPIOA};



void GPIO_OutInit(unsigned int rcc_pos, GPIO_TypeDef* pGPIO_Type, unsigned short PinNum);

void GPIO_WritePin(GPIO_TypeDef *pGPIO_Type, unsigned short GPIO_Pin, GPIO_PinState PinState);

void GPIO_Toggle(GPIO_TypeDef* pGPIO_Type, unsigned short GPIO_Pin);

void delay(void);



int main(void)

{

 // PA5

 unsigned int GPIO_Pin_5 = 0x01 << GPIO_LED2_PIN_POS;

 GPIO_OutInit(RCC_GPIOA_POS,  LED_PORT[LED2], GPIO_LED2_PIN_POS);

   

 // PA8

 unsigned int GPIO_Pin_8 = 0x01 << GPIO_USER_LED1_PIN_POS;

 GPIO_OutInit(RCC_GPIOA_POS,  LED_PORT[USER_LED1], GPIO_USER_LED1_PIN_POS);

   

 while(1)

 {

  //LED2 Toggle

   GPIO_Toggle(LED_PORT[LED2], GPIO_Pin_5);

  //USER_LED1 Toggle

   GPIO_Toggle(LED_PORT[USER_LED1], GPIO_Pin_8);

  delay();

 }

}



void GPIO_OutInit(unsigned int rcc_pos, GPIO_TypeDef* pGPIO_Type, unsigned short PinNum)

{

 unsigned int position = PinNum; //Pin Position: PA5

 unsigned int temp = 0x0U;

 unsigned int mode = 0x0U;

 unsigned int reg = 0x0U;

 unsigned int GPIO_Pin = 0x01 << PinNum;

 

 //RCC-GPIOA

 reg = RCC_IOPENR;

 temp = ~(GPIO_1BIT_POS_MASK << rcc_pos);

 reg &= temp;

 mode = RCC_ON << rcc_pos ; //RCC_ON - 1bit 

 RCC_IOPENR = reg | mode;

 

 //GPIOA-Reg

 // Mode

 //GPIO_PA5PIN_MODE = (unsigned int)0xEBFFF4FF;

 reg = pGPIO_Type->MODER;

 temp = ~(GPIO_2BIT_POS_MASK << (position*2U)); //0xFFFFF3FF

 reg &= temp;

 // GPO Mode: 01

 mode = GPIO_MODE_OUTPUT << (position*2U) ; // GPIO OutPut Mode - 2bit 0x00000400

 pGPIO_Type->MODER = reg | mode;



 //PA5 OTYPER

 //GPIO_PA5PIN_OTYPE = 0;

 reg = pGPIO_Type->OTYPER;

 temp = ~(GPIO_1BIT_POS_MASK << position); //~0x20 ==> 0xFFFFFFDF

 reg &= temp;

 mode = GPIO_OUTPUT_TYPE_0 << position ; //Output type - 1bit

 reg |= mode;

 pGPIO_Type->OTYPER = reg;



 //PA5 OSPEEDR

 //GPIO_PA5PIN_OSPEEDR = (unsigned int)0x0C000C00;

 reg = pGPIO_Type->OSPEEDR;

 temp = ~(GPIO_2BIT_POS_MASK << (position*2U)); //0xFFFFF3FF

 reg &= temp;

 mode = GPIO_SPEED_FREQ_VERY_HIGH << (position*2U) ; //Very High Mode - 2bit 0x00000C00

 reg |= mode;

 pGPIO_Type->OSPEEDR = reg;

 

 //OPUPDR

 //GPIO_PA5PIN_OPUPDR = (unsigned int)0x24000000;

 reg = pGPIO_Type->PUPDR;

 temp = ~(GPIO_2BIT_POS_MASK << (position*2U)); //0xFFFFF3FF

 reg &= temp;

 mode = GPIO_NOPULL << (position*2U) ; //No Pull Up - Pull Down - 2bit 0x24000000

 reg |= mode;

 pGPIO_Type->PUPDR = reg;

 

 GPIO_WritePin(pGPIO_Type, GPIO_Pin, GPIO_PIN_RESET);

}



void GPIO_WritePin(GPIO_TypeDef *pGPIO_Type, unsigned short GPIO_Pin, GPIO_PinState PinState)

{

  if (PinState != GPIO_PIN_RESET)

  {

    pGPIO_Type->BSRR = GPIO_Pin;

  }

  else

  {

    pGPIO_Type->BRR = GPIO_Pin ;

  }

}



void GPIO_Toggle(GPIO_TypeDef* pGPIO_Type, unsigned short GPIO_Pin)

{

 unsigned int odr;

  /* get current Output Data Register value */

  odr = pGPIO_Type->ODR;



  /* Set selected pins that were at low level, and reset ones that were high */

  pGPIO_Type->BSRR = ((odr & GPIO_Pin) << GPIO_NUMBER) | (~odr & GPIO_Pin);

}



void delay(void)

{

 volatile int delay_count = 0;

 //delay loop

 while(delay_count < 0x20000) //delay loop

 {

  delay_count++;

 }

}



