void delay(unsigned int delay_cnt);

int main(void)
{
	unsigned int reg;
	unsigned int temp;
	unsigned int mode;
	unsigned int position = 5;
	
	volatile int count = 0x0;
	
	//RCC-GPIOA enable
	
	RCC_GPIOA = RCC_GPIOA | (unsigned int)0x01;
	
	// GPIO init value set 0xEBFF FCFF -> EBFF3FF
	
// Mode ??? 0xEBFFFCFF
// *((unsigned int*)0x50000000U) = 0xEBFFF4FF;
	
	
	reg = GPIO_PA5PIN_MODE; 
	temp = ~(0x03 << (position*2U)); 
	reg &= temp; 
	mode = 0x01 << (position*2U) ; // GPOMode=01 ? 0x00000400 
	reg |= mode; 
	GPIO_PA5PIN_MODE = reg;
	
	
	//OTYPER - 
	
	reg = GPIO_PA5PIN_OTYPE; 
	temp = ~(0x01 << (position)); 
	reg &= temp; 
	mode = 0x00 << (position) ; // GPOMode=01 ? 0x00000400 
	reg |= mode; 
	GPIO_PA5PIN_OTYPE = reg;

	//OSPEEDR
	
	reg = GPIO_PA5PIN_SPEED; 
	temp = ~(0x01 << (position)); 
	reg &= temp; 
	mode = 0x01 << (position) ; // GPOMode=01 ? 0x00000400 
	reg |= mode; 
	GPIO_PA5PIN_SPEED = reg;
	
	//OPUPDR
	GPIO_PA5PIN_OPUPDR = (unsigned int)0x24000000;
	//ODR r/w
	
	
	while(1){		
		delay(100000);		
		GPIO_PA5PIN_ODR ^= (unsigned char)0x20;
	}
}

void delay(unsigned int delay_cnt)
{
	volatile int count =0;
	while(count< delay_cnt){
		count++;
	}
}

