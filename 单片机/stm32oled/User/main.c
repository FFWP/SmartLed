
#include "stm32f10x.h" //STM32头文件
#include <string.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled0561.h"
#include "led.h"
#include "DS18B20.h"
#include "stdio.h"
int tutu=0,c1,c2,c3,c4;
u8 bian;//是否联网
u8 bian2;//是否tcp连接
char *saywhat;
//*********************************  ESP8266   ************************************************************/
//定义一个结构体，用于串口中断数据的传输
typedef struct {

    char USART_BUFF[300];
    int USART_Length;
    int flag;

}Usart_Struct;

Usart_Struct struct_usart2;

//串口中断函数
void USART2_IRQHandler(void)
{
    uint8_t ch;
	#ifdef SYSTEM_SUPPORT_OS        
    OSIntEnter();    
	#endif
    if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        ch = USART_ReceiveData(USART2);
        struct_usart2.USART_BUFF[struct_usart2.USART_Length++] = ch;
        struct_usart2.flag = 1;
    }
    if( USART_GetITStatus( USART2, USART_IT_IDLE ) == SET )
    {
        USART_ClearITPendingBit(USART2,USART_IT_IDLE);
        struct_usart2.flag = 1;
        ch = USART_ReceiveData(USART2);
    }

#ifdef SYSTEM_SUPPORT_OS     
    OSIntExit();                                             
#endif
}
void leddq(){
	OLED_DISPLAY_8x16_BUFFER(0,"   DQ"); //显示字符串
	OLED_DISPLAY_16x16(0,3*16,0);//汉字显示	 
	OLED_DISPLAY_16x16(0,4*16,1);
	OLED_DISPLAY_16x16(0,5*16,2);
}
/*******************************************************************************  
* 函 数 名         : uart2_send_char  
* 函数功能         : 串口2发送一字节        
* 输    入         : 无  
* 输    出         : 无  
*******************************************************************************/    
void uart2_send_char(u8 temp)      
{        
    USART_SendData(USART2,(u8)temp);        
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);         
}    

/*******************************************************************************  
* 函 数 名         : uart2_send_buff  
* 函数功能         : 串口2发送一字符串       
* 输    入         : 无  
* 输    出         : 无  
*******************************************************************************/    
void uart2_send_buff(u8 buf[],u32 len)     
{    
    u32 i;    
    for(i=0;i<len;i++)    
    uart2_send_char(buf[i]);
    // 字符串末尾发送回车换行
    uart2_send_char('\r');
    uart2_send_char('\n');

}
/*******************************************************************************  
* 函 数 名         : get_uart2  
* 函数功能         : 串口2接受一字符串       
* 输    入         : 无  
* 输    出         : 无  
*******************************************************************************/    
void get_uart2()     
{    
   
	//查询方式接收
//		if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET){  //查询串口待处理标志位
//			a =USART_ReceiveData(USART2);//读取接收到的数据
//			printf("返回%c",a); //把收到的数据发送回电脑		  
//		}

}
/*******************************************************************************  
* 函 数 名         : get_uart2  
* 函数功能         : 字符串比较      
* 输    入         : 无  
* 输    出         : 无  
*******************************************************************************/ 
//char strCmp(char *s1,char *s2){
//    char i;//字符串很长的话改成int变量 
//    for(i=0;s1[i]!=0||s2[i]!=0;i++){
//        if(s1[i]<s2[i])return -1;
//        else if(s1[i]>s2[i])return 1;
//    }
//    return 1;    
//}
/**
 * 
 * [ESP8266_SendCmd 发送命令到ESP8266]
 * @param  reply [期望模块回显的内容]
 * @param  wait  [等待的时间(ms)]
 * @return       [期望内容等于实际回显内容返回1，否则0]
 * 
 */
void	ESP8266_SendCmd(char* cmd, char* reply, int wait)
{
    struct_usart2.USART_Length = 0;
   printf("[ESP8266_SendCmd] %s\r\n", cmd);

    uart2_send_buff((u8*)cmd, strlen(cmd));

    delay_ms(wait);

    if (strcmp(reply, "") == 0)
    {
       // return 0;
    }

    if (struct_usart2.USART_Length != 0)
    {
        struct_usart2.USART_BUFF[struct_usart2.USART_Length] = '\0';

        if (strstr((char*)struct_usart2.USART_BUFF, reply))
        {
            //printf("\r\n%s+++YES\r\n", struct_usart2.USART_BUFF);
					
			

            //return 1;
        }
        else if (strstr((char*)struct_usart2.USART_BUFF, "busy"))
        {
            //printf("busy...\r\n");

            delay_ms(3000);

            //return 0;
        }
        else
        {  
           // printf("\r\n%s+++NO\r\n", struct_usart2.USART_BUFF);

            //return 0;
        }  
    }  
}
/**
 * [ESP8266_Init 模块初始化]
 * @return [成功为1，否则为0]
 */
 void ESP8266_Init()  
{
    //int ret = 0;

    ESP8266_SendCmd("AT+CWMODE=1","OK", 1000);
	ESP8266_SendCmd("AT+CIPCLOSE","OK", 1000);
    
		//delay_ms(10000);
		 //ESP8266_SendCmd("AT+CWQAP","", 1000);//断开wifi
		delay_ms(2000);
//    if (!ret)
//    {
//        printf("Cannot initialize ESP module");
//        return 0;
//    }
//    return ret;
}
//是否联网
 void ESP8266_Connectif()  
{
		char *str="\r\nERROR";
		char *tmp;
		int p;

    ESP8266_SendCmd("AT+PING=\"www.qq.com\"","ERROR", 8000);
		tmp = strchr(struct_usart2.USART_BUFF,'\n')+1;
		printf("%s",tmp);
		p=memcmp(tmp,str,strlen(str));	
					if (p== 0)
					{
						OLED_DISPLAY_CLEAR();
						leddq();
						OLED_DISPLAY_8x16_BUFFER(6,"Wifi Not Connect");
						delay_ms(500);
						printf("Wifi not connect");
						bian=0;//全局变量 没有联网
					}else
					{	
						OLED_DISPLAY_CLEAR();
						leddq();
						OLED_DISPLAY_8x16_BUFFER(6,"Wifi Connect");
						delay_ms(500);
						printf("Wifi connect");
						bian=1;
						
					}
				
 
}
//智能配网
void ESP8266_Connect()  
{
	
		ESP8266_SendCmd("AT+CWSMARTSTART=1","OK", 1000);
		printf("zhi neng pei wang");
		printf("----------- %s",struct_usart2.USART_BUFF);
	
		OLED_DISPLAY_CLEAR();
		leddq();
		OLED_DISPLAY_8x16_BUFFER(6,"Smart Wifi"); //显示配网
	
}

//tcp连接
void ESP8266_Tcp()  
{
		char *str="CONN";
		char *tmp;
		int p;
		ESP8266_SendCmd("AT+CIPSTART=\"TCP\",\"149.129.103.5\",7273","OK", 1000);
		tmp = struct_usart2.USART_BUFF;
		tmp = strchr(tmp,'\n')+1;
	  printf("%s",tmp);
		p=strncmp(tmp,str,3);
	  printf("mm %d",p);
	//memset(struct_usart2.USART_BUFF, p, sizeof(struct_usart2.USART_BUFF)); 
	//memset(struct_usart2.USART_BUFF, 0, sizeof(struct_usart2.USART_BUFF)); 
	  struct_usart2.USART_BUFF[struct_usart2.USART_Length] = '\0';
    struct_usart2.USART_Length=0;
		if (p== 0)
					{
						printf("TCP connect");
						OLED_DISPLAY_CLEAR();
						leddq();
						OLED_DISPLAY_8x16_BUFFER(6,"TCP Connect ");
						delay_ms(500);	
						bian2=0;
			
					}else
					{	
						OLED_DISPLAY_CLEAR();
						leddq();
						OLED_DISPLAY_CLEAR();
					  OLED_DISPLAY_8x16_BUFFER(6,"TCP Not Connect");
						printf("TCP not connect");
						bian2=1;//全局变量 没有联网
						
					}
}
//温度显示发送
void wendu(){
		char *kong=" Temp: ";//占位5
		float temp;
		char ledtemp[10];
		temp=DS18B20_Get_Temp();
		sprintf(ledtemp, "%s%.1f", kong, temp);	

		OLED_DISPLAY_8x16_BUFFER(6,ledtemp);
		OLED_DISPLAY_16x16(6,6*15,3);//显示℃
		printf("\r\n temperature %.1f\r\n",temp);	
		ESP8266_SendCmd("AT+CIPSEND=8","OK", 1000);
		USART2_printf("TEP_%.1f",temp);	
			//sprintf(ledtemp,"%.1f",temp);	
			

}
//引脚电位获取并发送
void dianwei(){
	char data[22];
	char *a="LED1_";
	char *b="%LED2_";
	char *c="!TEP_";
	char *led1;
	char *led1_1="LED1:ON ";
	char *led1_0="LED1:OFF";
	char *led2;
	char *led2_1="LED2:ON ";
	char *led2_0="LED2:OFF";
	float temp;

	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0)
	{
			led1=led1_0;
	}
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==1)
	{
		led1=led1_1;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)
	{
			led2=led2_0;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1)
	{
		led2=led2_1;
	}		
	

	OLED_DISPLAY_8x16_BUFFER(2,led1); //显示字符串
	OLED_DISPLAY_8x16_BUFFER(4,led2); //显示字符串
	temp=DS18B20_Get_Temp();
	sprintf(data, "%s%d%s%d%s%.1f",a,GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0),b,GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1),c,temp);	
	//GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	//GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
	ESP8266_SendCmd("AT+CIPSEND=22","OK", 1000);
	USART2_printf("%s",data);	
	printf("%s\r\n",data);
	
}
//2串口接受的数据
void gatedata(){
	char *ce="LED1_1%";
	char *cc="LED1_0%";
	char *led11="LED1_1";
	char *led10="LED1_0";
	char *led21="LED2_1";
	char *led20="LED2_0";
  int a,b,c,d,e,aa,bb,ff,dd;
		
		saywhat = strchr(struct_usart2.USART_BUFF,':')+1;
	
		struct_usart2.USART_BUFF[struct_usart2.USART_Length] = '\0';
		struct_usart2.USART_Length=0;
	
		aa=strncmp(saywhat,led11,6);
		bb=strncmp(saywhat,led10,6);
		ff=strncmp(saywhat,led21,6);
		dd=strncmp(saywhat,led20,6);
	
		b=strncmp(saywhat,ce,7);
		c=strncmp(saywhat,cc,7);
	  c1=aa;
	  c2=bb;
	  c3=ff;
	  c4=dd;
		a=aa&&bb&&ff&&dd;

		d=a&&b;
		e=b&&c;
	  tutu=d+e;
	
		
}

void ledoffon(){//控制led

				printf("---------------");
				delay_ms(200);
//				struct_usart2.USART_BUFF[struct_usart2.USART_Length] = '\0';
//				struct_usart2.USART_Length=0;
				if (c1==0)
				{
					printf("111");
					GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1));
				}
					
				if (c2==0)
				{
					printf("1222");
				GPIO_WriteBit(LEDPORT,LED1,(BitAction)(0));

				}
				if (c3==0)
				{
					printf("3333");
				GPIO_WriteBit(LEDPORT,LED2,(BitAction)(1));

				}
				if (c4==0)
				{
					printf("6666");
				GPIO_WriteBit(LEDPORT,LED2,(BitAction)(0));

				}

}

//透传
//void ESP8266_touchaun()  
//{
//		char *str="OK";
//		char *tmp;
//		int p;
//		ESP8266_SendCmd("AT+CIPMODE=1","OK", 1000);
//		ESP8266_SendCmd("AT+CIPSEND","OK", 1000);
//		tmp = struct_usart2.USART_BUFF;
//		tmp = strchr(tmp,'\n')+1;
//		p=strncmp(tmp,str,2);
//	//memset(struct_usart2.USART_BUFF, p, sizeof(struct_usart2.USART_BUFF)); 
//	//memset(struct_usart2.USART_BUFF, 0, sizeof(struct_usart2.USART_BUFF)); 
//	struct_usart2.USART_BUFF[struct_usart2.USART_Length] = '\0';
//       struct_usart2.USART_Length=0;
//	
//		if (p== 0)
//					{
//						printf("tou chuan");
//						
//			
//					}else
//					{	
//						
//						printf("tou chuan lost");
//						
//					}
//}
/**
 * [ESP8266_JoinAP 连接WiFi]
 * @param  ssid [WiFi名]
 * @param  psd  [WiFi密码]
 * @return      [成功为1，否则为0]
 */
//int ESP8266_JoinAP(char* ssid, char* psd)
//{
//    int ret = 0;
//    char ssid_psd[120] = {0};
//    sprintf(ssid_psd,"AT+CWJAP_CUR=\"%s\",\"%s\"",ssid, psd);

//    ret = ESP8266_SendCmd(ssid_psd,"WIFI CONNECTED", 5000);

//    return ret;
//}

//*********************************************************************************************/


int main (void){

	int biana=0;
	//delay_ms(100); //上电时等待其他器件就绪
	RCC_Configuration(); //系统时钟初始化 
	LED_Init();
	USART1_Init(115200); //串口初始化（参数是波特率）
	USART2_Init(115200);
	I2C_Configuration();//I2C初始化
	 //温度初始化
	GPIO_ResetBits(GPIOB , GPIO_Pin_9);
	delay_ms(500);
	GPIO_SetBits(GPIOB , GPIO_Pin_9); 	
	
	OLED0561_Init(); //OLED初始化
	OLED_DISPLAY_LIT(100);//亮度设置
	//OLED_DISPLAY_PIC1();//显示全屏图片

	//OLED_DISPLAY_CLEAR();
  leddq();
	OLED_DISPLAY_8x16_BUFFER(3,"   Welcome"); //显示字符串
	OLED_DISPLAY_8x16_BUFFER(6," Chu Shi Hua..."); //显示初始化如果返回值 = 0，则表示 str1 等于 str2。

	
//==============网络初始化========================================
	ESP8266_Init();
	ESP8266_Connectif();
	while(bian==0){
		ESP8266_Connect();
		delay_ms(40000);//智能配网等待50s
		ESP8266_Connectif();
	}
		printf("tvtv%d", bian);
		if(bian==1)//网络是连接的
		ESP8266_Tcp();
		OLED_DISPLAY_CLEAR();
		 leddq();
		 dianwei();
	
	while(bian2==0){
			gatedata();
		
			if(tutu==1){
			ledoffon();
			
			}
			biana=biana+1;
				while(biana==4){
				 dianwei();//获取电平温度 led1_1%led2_1!tep_30

					biana=0;
			
					}
			wendu();//获取温度发送

	}
	

}