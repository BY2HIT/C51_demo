#include <stc12c5a.h>
#include "LCD1602.h"

void main()
{
	init_LCD1602();
	WriteString("BY2HIT");
	while(1)
	{
		delay(2000);
		WriteCmd(0x1C);//屏幕右移
	}
}


