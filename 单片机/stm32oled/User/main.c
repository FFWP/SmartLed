
#include "stm32f10x.h" //STM32ͷ�ļ�
#include <string.h>
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled0561.h"
#include "led.h"
#include "DS18B20.h"
#include "stdio.h"
int tutu=0,c1,c2,c3,c4;
u8 bian;//�Ƿ�����
u8 bian2;//�Ƿ�tcp����
char *saywhat;
//*********************************  ESP8266   ************************************************************/
//����һ���ṹ�壬���ڴ����ж����ݵĴ���
typedef struct {

    char USART_BUFF[300];
    int USART_Length;
    int flag;

}Usart_Struct;

Usart_Struct struct_usart2;

//�����жϺ���
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
	OLED_DISPLAY_8x16_BUFFER(0,"   DQ"); //��ʾ�ַ���
	OLED_DISPLAY_16x16(0,3*16,0);//������ʾ	 
	OLED_DISPLAY_16x16(0,4*16,1);
	OLED_DISPLAY_16x16(0,5*16,2);
}
/*******************************************************************************  
* �� �� ��         : uart2_send_char  
* ��������         : ����2����һ�ֽ�        
* ��    ��         : ��  
* ��    ��         : ��  
*******************************************************************************/    
void uart2_send_char(u8 temp)      
{        
    USART_SendData(USART2,(u8)temp);        
    while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);         
}    

/*******************************************************************************  
* �� �� ��         : uart2_send_buff  
* ��������         : ����2����һ�ַ���       
* ��    ��         : ��  
* ��    ��         : ��  
*******************************************************************************/    
void uart2_send_buff(u8 buf[],u32 len)     
{    
    u32 i;    
    for(i=0;i<len;i++)    
    uart2_send_char(buf[i]);
    // �ַ���ĩβ���ͻس�����
    uart2_send_char('\r');
    uart2_send_char('\n');

}
/*******************************************************************************  
* �� �� ��         : get_uart2  
* ��������         : ����2����һ�ַ���       
* ��    ��         : ��  
* ��    ��         : ��  
*******************************************************************************/    
void get_uart2()     
{    
   
	//��ѯ��ʽ����
//		if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET){  //��ѯ���ڴ������־λ
//			a =USART_ReceiveData(USART2);//��ȡ���յ�������
//			printf("����%c",a); //���յ������ݷ��ͻص���		  
//		}

}
/*******************************************************************************  
* �� �� ��         : get_uart2  
* ��������         : �ַ����Ƚ�      
* ��    ��         : ��  
* ��    ��         : ��  
*******************************************************************************/ 
//char strCmp(char *s1,char *s2){
//    char i;//�ַ����ܳ��Ļ��ĳ�int���� 
//    for(i=0;s1[i]!=0||s2[i]!=0;i++){
//        if(s1[i]<s2[i])return -1;
//        else if(s1[i]>s2[i])return 1;
//    }
//    return 1;    
//}
/**
 * 
 * [ESP8266_SendCmd �������ESP8266]
 * @param  reply [����ģ����Ե�����]
 * @param  wait  [�ȴ���ʱ��(ms)]
 * @return       [�������ݵ���ʵ�ʻ������ݷ���1������0]
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
 * [ESP8266_Init ģ���ʼ��]
 * @return [�ɹ�Ϊ1������Ϊ0]
 */
 void ESP8266_Init()  
{
    //int ret = 0;

    ESP8266_SendCmd("AT+CWMODE=1","OK", 1000);
	ESP8266_SendCmd("AT+CIPCLOSE","OK", 1000);
    
		//delay_ms(10000);
		 //ESP8266_SendCmd("AT+CWQAP","", 1000);//�Ͽ�wifi
		delay_ms(2000);
//    if (!ret)
//    {
//        printf("Cannot initialize ESP module");
//        return 0;
//    }
//    return ret;
}
//�Ƿ�����
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
						bian=0;//ȫ�ֱ��� û������
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
//��������
void ESP8266_Connect()  
{
	
		ESP8266_SendCmd("AT+CWSMARTSTART=1","OK", 1000);
		printf("zhi neng pei wang");
		printf("----------- %s",struct_usart2.USART_BUFF);
	
		OLED_DISPLAY_CLEAR();
		leddq();
		OLED_DISPLAY_8x16_BUFFER(6,"Smart Wifi"); //��ʾ����
	
}

//tcp����
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
						bian2=1;//ȫ�ֱ��� û������
						
					}
}
//�¶���ʾ����
void wendu(){
		char *kong=" Temp: ";//ռλ5
		float temp;
		char ledtemp[10];
		temp=DS18B20_Get_Temp();
		sprintf(ledtemp, "%s%.1f", kong, temp);	

		OLED_DISPLAY_8x16_BUFFER(6,ledtemp);
		OLED_DISPLAY_16x16(6,6*15,3);//��ʾ��
		printf("\r\n temperature %.1f\r\n",temp);	
		ESP8266_SendCmd("AT+CIPSEND=8","OK", 1000);
		USART2_printf("TEP_%.1f",temp);	
			//sprintf(ledtemp,"%.1f",temp);	
			

}
//���ŵ�λ��ȡ������
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
	

	OLED_DISPLAY_8x16_BUFFER(2,led1); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(4,led2); //��ʾ�ַ���
	temp=DS18B20_Get_Temp();
	sprintf(data, "%s%d%s%d%s%.1f",a,GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0),b,GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1),c,temp);	
	//GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
	//GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1);
	ESP8266_SendCmd("AT+CIPSEND=22","OK", 1000);
	USART2_printf("%s",data);	
	printf("%s\r\n",data);
	
}
//2���ڽ��ܵ�����
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

void ledoffon(){//����led

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

//͸��
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
 * [ESP8266_JoinAP ����WiFi]
 * @param  ssid [WiFi��]
 * @param  psd  [WiFi����]
 * @return      [�ɹ�Ϊ1������Ϊ0]
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
	//delay_ms(100); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	LED_Init();
	USART1_Init(115200); //���ڳ�ʼ���������ǲ����ʣ�
	USART2_Init(115200);
	I2C_Configuration();//I2C��ʼ��
	 //�¶ȳ�ʼ��
	GPIO_ResetBits(GPIOB , GPIO_Pin_9);
	delay_ms(500);
	GPIO_SetBits(GPIOB , GPIO_Pin_9); 	
	
	OLED0561_Init(); //OLED��ʼ��
	OLED_DISPLAY_LIT(100);//��������
	//OLED_DISPLAY_PIC1();//��ʾȫ��ͼƬ

	//OLED_DISPLAY_CLEAR();
  leddq();
	OLED_DISPLAY_8x16_BUFFER(3,"   Welcome"); //��ʾ�ַ���
	OLED_DISPLAY_8x16_BUFFER(6," Chu Shi Hua..."); //��ʾ��ʼ���������ֵ = 0�����ʾ str1 ���� str2��

	
//==============�����ʼ��========================================
	ESP8266_Init();
	ESP8266_Connectif();
	while(bian==0){
		ESP8266_Connect();
		delay_ms(40000);//���������ȴ�50s
		ESP8266_Connectif();
	}
		printf("tvtv%d", bian);
		if(bian==1)//���������ӵ�
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
				 dianwei();//��ȡ��ƽ�¶� led1_1%led2_1!tep_30

					biana=0;
			
					}
			wendu();//��ȡ�¶ȷ���

	}
	

}