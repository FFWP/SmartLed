#define AT_MODE_CONFIG 0 //0,采用ES/RST硬件控制进入AT模式;
#define ESP_RX_MAX_LEN   600
#define MODE_TCP  0
#define MODE_UDP  1
#define AP      0
#define STA     1
#define STA_AP  2


struct ESP8266_Fram
{
    char data_buff[ESP_RX_MAX_LEN];
    unsigned int len;
    char rx_flag;
};


//void USART1_Config(void)
//{
//GPIO_InitTypeDef GPIO_InitStructure;
//USART_InitTypeDef USART_InitStructure;

///* config USART1 clock */
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

///* USART1 GPIO config */
///* Configure USART1 Tx (PA.09) as alternate function push-pull */
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//GPIO_Init(GPIOA, &GPIO_InitStructure);    

///* Configure USART1 Rx (PA.10) as input floating */
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//GPIO_Init(GPIOA, &GPIO_InitStructure);

///* USART1 mode config */
//USART_InitStructure.USART_BaudRate = 115200;
//USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//USART_InitStructure.USART_StopBits = USART_StopBits_1;
//USART_InitStructure.USART_Parity = USART_Parity_No ;
//USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//USART_Init(USART1, &USART_InitStructure);
///* ê1?ü′??ú2?óê??D?? */
//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
///* ê1?ü′??ú2×ü?????D?D?? */
//USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);


//USART_Cmd(USART1, ENABLE);
//        MY_NVIC_Init(2,3,USART1_IRQn,2);
//        
//}


/*******************************************************************************
 * @description    ESP8266初始化函数
 * @in             无     
 * @out            无     
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
void esp8266_init(void) 
{
   
    USART1_Config();          //串口初始化
    
    esp8266_send_cmd("ATE0","OK",0,500);  //关闭回显
    esp8266_Rst();
}




//串口中断

//void USART1_IRQHandler(void)
//{   
//    char ch;
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//    {
//        ch  = USART_ReceiveData( USART1 );
//        
//        if( record.len< ( ESP_RX_MAX_LEN - 1 ) )                       //?¤á?1??×??úD′?áê?·?
//        {
//           record.data_buff[record.len++]=ch;


//        }
//    }
//    if( USART_GetITStatus( USART1, USART_IT_IDLE ) == SET )                                         //êy?Y???óê?íê±?
//    {
//       //strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
//       ch = USART_ReceiveData( USART1 );                                                              //óéèí?tDòáD??3y?D??±ê????(?è?áUSART_SR￡?è?oó?áUSART_DR)
//     }


//}
















/*
char * ESP8266_ReceiveString ( FunctionalState enumEnUnvarnishTx )
{
char * pRecStr = 0;

strEsp8266_Fram_Record .InfBit .FramLength = 0;
strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
while ( ! strEsp8266_Fram_Record .InfBit .FramFinishFlag );
strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';

if ( enumEnUnvarnishTx )
{
if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, ">" ) )
pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;


}

else 
{
if ( strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "+IPD" ) )
pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;


}


return pRecStr;

}
*/




//发送命令
/*******************************************************************************
 * @description    对ESP8266模块发送AT指令
 * @in             cmd，待发送指令
 *                 reply，reply1  ，期待的响应，为NULL时不需要响应   
 *                 waittime，等待响应时间


 * @out            无     
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
bool esp8266_send_cmd(char *cmd,char *reply,char *reply1,u32 waittime)
{
    char cstr[20];
    record.len=0;           //接收长度清零  
    IWDG_Feed();
    printf("%s\r\n",cmd);
    
    if(reply==NULL&&reply1==NULL)
      return true;
    delay_ms(waittime);
    record.data_buff[record.len]='\0';
    if(reply!=0&&reply1!=0)
        return ((bool)strstr(record.data_buff,reply)||(bool)strstr(record.data_buff,reply1));
    else if(reply!=0)
        return (bool)strstr(record.data_buff,reply);
    else
        return (bool)strstr(record.data_buff,reply1);
 }
//发送数据
/*******************************************************************************
 * @description    ESP8266模块发送字符串
 * @in             enuvarnishTx，是否使能透传模式
 *                 pStr，要发送的字符串     
 *                 len   ，字符串长度  
 *                 waittime，延时
 *                 ucid  ，那个ID发送的字符串   


 * @out            无     
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
bool esp8266_send_data(u8 enuvarnishTx,char *pStr,u32 len,u8 ucid)
{
    char cstr[20];
    
    bool bRet=false;
    if(enuvarnishTx)      //透传模式
    {
      printf("%s",pStr);
    }
    else
    {
       if(ucid<5)            //多连接模式
       {
          sprintf(cstr,"AT+CIPSEND=%d,%d",ucid,len);
       }
       else                 //单链接模式
       {
          sprintf(cstr,"AT+CIPSEND=%d",len);
       }
      // esp8266_send_cmd(cstr,">",0,500);
       if(esp8266_send_cmd(cstr,">",0,500))       //发送
       {
       
          //bRet=esp8266_send_cmd(pStr,"SEND OK",0,300);
         delay_ms(100);
         USART_Puts(USART1,pStr,len); 
          
       }
     }


    return bRet;


}




//
/*******************************************************************************
 * @description    ESP8266模块进入透传模式
 * @in             无     
 * @out            无     
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
bool esp8266_UnvarnishSend(void)
{
    bool return_flag;
    return_flag=esp8266_send_cmd("AT+CIPMODE=1","OK",0,300)&&esp8266_send_cmd("AP+CIPSEND","\r\n",0,300);


}








/*******************************************************************************
 * @description    ESP8266模块开启或关闭多连接
 * @in             mode，开启/关闭
 *                 pPortNum，服务器端口号字符串
 *                 pTimeover，超时时间字符串
 * @out            1，操作成功
 *                 0，操作失败    
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
bool esp8266_Enable_MultipleId ( char enumEnUnvarnishTx )
{
char cStr [20];

sprintf ( cStr, "AT+CIPMUX=%d", ( enumEnUnvarnishTx ? 1 : 0 ) );

return esp8266_send_cmd( cStr, "OK", 0, 500 );

}




/*******************************************************************************
 * @description    ESP8266模块开启或关闭服务器模式
 * @in             mode，开启/关闭
 *                 pPortNum，服务器端口号字符串
 *                 pTimeover，超时时间字符串
 * @out            1，操作成功
 *                 0，操作失败    
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
bool esp8266_set_Server(u8 mode,char *pPortNum,char *pTimeover)
{
   char cCmd1[120],cCmd2[120];
   if(mode)        //开启服务
   {
       sprintf(cCmd1,"AT+CIPSERVER=%d,%s",1,pPortNum);  //端口号字符串
       sprintf(cCmd2,"AT+CIPSTO=%s",pTimeover);         //超时时间
       return (esp8266_send_cmd(cCmd1,"OK",0,300)&&esp8266_send_cmd(cCmd1,"OK",0,300));
      
   }
   else           //关闭
   {
        sprintf(cCmd1,"AT+CIPSERVER=%d,%s",0,pPortNum);  //端口号字符串
        return esp8266_send_cmd(cCmd1,"OK",0,300);
   }


}




/*******************************************************************************
 * @description    ESP8266模块连接外部服务器
 * @in             tcp_ip_mode ，网络协议 
 *                 ip   ，服务器ip字符串  
 *                 comNum，服务器端口字符串
 *                 ucid    ，模块连接服务器的ID 
 * @out            1，连接成功     
 *                 0，连接失败
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
bool esp8266_link_Server(u8 tcp_ip_mode,char *ip,char *comNum,char *LocalcomNum,u8 ucid)
{
    char cStr[100],cCmd[120];
    
    switch(tcp_ip_mode)
    {
        case MODE_TCP:
           sprintf(cStr,"\"%s\",\"%s\",%s","TCP",ip,comNum);  //端口号字符串
          break;
        case MODE_UDP:
          sprintf(cStr,"\"%s\",\"%s\",%s,%s,0","UDP",ip,comNum,LocalcomNum);  
          break;
        default:
          break;
    }
     if(ucid<5)            //多连接模式
     {
        sprintf(cCmd,"AT+CIPSTART=%d,%s",ucid,cStr);
     }
     else                 //单链接模式
     {
        sprintf(cCmd,"AT+CIPSTART=%s",cStr);
     }
    
    return esp8266_send_cmd(cCmd,"OK",0,300);   
    








}


/*******************************************************************************
 * @description    ESP8266创建WIFI热点
 * @in             pSSID，WIFI名称字符串
 *                 pPassWord，WIFI密码字符串  
 *                 mode，加密方式代号字符串
 * @out            1，创建成功     
 *                 0，创建失败           
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
bool esp8266_Build_AP(char *pSSID,char *pPassWord,u8 *mode)
{


   char cCmd[120];
   sprintf(cCmd,"AT+CWSAP\"%s\",\"%s\",1,%s",pSSID,pPassWord,mode);
   return esp8266_send_cmd(cCmd,"OK",0,300);   


}
/*******************************************************************************
 * @description    ESP8266模块连接外部wifi
 * @in             pSSID，WIFI名称字符串
 *                 pPassWord，WIFI密码字符串   
 * @out             1，连接成功     
 *                 0，连接失败        
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
bool esp8266_Join_AP(char *pSSID,char *pPassWord)
{


   char cCmd[120];
   sprintf(cCmd,"AT+CWJAP\"%s\",\"%s\"",pSSID,pPassWord);
   return esp8266_send_cmd(cCmd,"OK",NULL,1000);   


}


/*******************************************************************************
 * @description    选择ESP8266模块的工作模式
 * @in             enumMode，工作模式     
 * @out            1，选择成功     
 *                 0，选择失败    
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
bool esp8266_Net_mode_Choose(u8 enumMode)
{
    
   switch(enumMode)
    {
        case STA:
           return esp8266_send_cmd("AT+CWMODE=1","OK","no change",1000);  
           break;
        case AP:
           return esp8266_send_cmd("AT+CWMODE=2","OK","no change",1000);  
           break;
        case STA_AP:
           return esp8266_send_cmd("AT+CWMODE=3","OK","no change",1000);    
           break;
        default:
          break;
    }
}
/*******************************************************************************
 * @description    重启ESP8266模块
 * @in             无     
 * @out            无     
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
void esp8266_Rst(void)
{


    esp8266_send_cmd("AT+RST","OK","ready",2500);    




}
/*******************************************************************************
 * @description    对ESP8266模块进行AT指令测试
 * @in             无     
 * @out            无     
 * @author   
 * @date           2017/11/9 
 * @修改记录：
*******************************************************************************/
void esp8266_AT_test(void)
{


    while(!esp8266_send_cmd("AT","OK",NULL,1000))
      esp8266_Rst();


}
/*******************************************************************************
 * @description    使能/禁用ESP8266
 * @in             enumChoose =ENABLE，使能模块
 *                 enumChoose =DISABLE，禁用模块     
 * @out            无     
 * @author   
 * @date            2017/11/9 
 * @修改记录：
*******************************************************************************/
void esp8266_EN_Choose(FunctionalState  enumChoose)
{
    if(enumChoose==ENABLE)
      esp8266_Rst();
}
