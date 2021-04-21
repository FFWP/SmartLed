<?php
namespace app\chat\controller;
/**
 * 用于检测业务代码死循环或者长时间阻塞等问题
 * 如果发现业务卡死，可以将下面declare打开（去掉//注释），并执行php start.php reload
 * 然后观察一段时间workerman.log看是否有process_timeout异常
 */
//declare(ticks=1);
/**
 * 聊天主逻辑
 * 主要是处理 onMessage onClose 
 */
use \GatewayWorker\Lib\Gateway;
use think\Db;
use think\Model;

class Events
{
    /**
     * 当客户端连接时触发
     * 如果业务不需此回调可以删除onConnect
     * 
     * @param int $client_id 连接id
     */
	
    public static function onConnect($client_id)
    {
        // 向当前client_id发送数据 
        Gateway::sendToClient($client_id,"Hello $client_id\r\n");
        // 向所有人发送
        Gateway::sendToAll("$client_id login\r\n");
    }
    
   /**
    * 当客户端发来消息时触发
    * @param int $client_id 连接id
    * @param mixed $message 具体消息
    */
   public static function onMessage($client_id, $message)
   {
     $ceshi='TEP';
     $getdata;
     $shou;
     $wei;
     $time1;
     $time2;
      // 向所有人发送 
       //Gateway::sendToAll("$client_id said $message\r\n");
     Gateway::sendToAll("$message\r\n");
        // $temp = new Stm32Temp;//数据库表
     // $temp->mingzi= '温度';
     // $temp->temp= '123';
     // $temp->color= '#2fc25b';
    //$temp->save();
    	//$result = Db::table('stm32_temp')->insert(['temp'=>'26.3','shijian'=>'23:12']);
    $getdata= explode("_",$message);
     $shou=$getdata[0];
       $wei=$getdata[1];
     if(strcmp($shou,$ceshi)==0){
     	date_default_timezone_set("Asia/Shanghai");
       $time2= date("Y-m-d H:i:s"); 
       $time1= date("H:i"); 
         Db::table('stm32_temp')->insert(['mingzi'=>'温度','color'=>'#2fc25b','temp'=>$wei,'shijian'=>$time1,'create_time'=>$time2]);
     
     }
    
     
     
   }
 
   
   /**
    * 当用户断开连接时触发
    * @param int $client_id 连接id
    */
   public static function onClose($client_id)
   {
       // 向所有人发送 
       GateWay::sendToAll("$client_id logout\r\n");
   }
}