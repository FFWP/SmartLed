<?php
namespace app\index\controller;
use Workerman\Worker;
use GatewayWorker\Gateway;
use Workerman\Autoloader;
class Espgateway{
    public function __construct(){
        // gateway 进程
        $gateway = new Gateway("tcp://0.0.0.0:7273");
     // $gateway = new Gateway("tcp://0.0.0.0:7272");
        // 设置名称，方便status时查看
        $gateway->name = 'ChatGateway';
        // 设置进程数，gateway进程数建议与cpu核数相同
        $gateway->count = 4;
        // 分布式部署时请设置成内网ip（非127.0.0.1）
        $gateway->lanIp = '127.0.0.1';
        // 内部通讯起始端口，假如$gateway->count=4，起始端口为4000
        // 则一般会使用4000 4001 4002 4003 4个端口作为内部通讯端口
        $gateway->startPort = 4002;
        // 心跳间隔
        $gateway->pingInterval = 55;
      $gateway->pingNotResponseLimit = 0;
        // 心跳数据
        $gateway->pingData = '{"type":"ping"}';
        // 服务注册地址
       // $gateway->registerAddress = '127.0.0.1:1238';
        
        /*
         // 当客户端连接上来时，设置连接的onWebSocketConnect，即在websocket握手时的回调
         $gateway->onConnect = function($connection)
         {
         $connection->onWebSocketConnect = function($connection , $http_header)
         {
         // 可以在这里判断连接来源是否合法，不合法就关掉连接
         // $_SERVER['HTTP_ORIGIN']标识来自哪个站点的页面发起的websocket链接
         if($_SERVER['HTTP_ORIGIN'] != 'http://chat.workerman.net')
         {
         $connection->close();
         }
         // onWebSocketConnect 里面$_GET $_SERVER是可用的
         // var_dump($_GET, $_SERVER);
         };
         };
         */
      
      // 新增55250端口，开启socket连接
      //  $gateway_text = new Gateway("tcp://0.0.0.0:7273");
        // 进程名称，主要是status时方便识别
       // $gateway_text->name = 'Esp8266';
        // 开启多少text协议的gateway进程
       // $gateway_text->count =4;
        // 本机ip（分布式部署时需要设置成内网ip）
       // $gateway_text->lanIp = '127.0.0.1';
        // gateway内部通讯起始端口，起始端口不要重复
        //$gateway_text->startPort = 5000;
        // 心跳间隔
       // $gateway_text->pingInterval = 8;
       // $gateway_text->pingNotResponseLimit = 3;
        //$gateway_text->pingData = '{"type":"ping"}';
        // 服务注册地址
       // $gateway->registerAddress = '127.0.0.1:1236';
      
      
      
      
      
        
        // 如果不是在根目录启动，则运行runAll方法
        if(!defined('GLOBAL_START'))
        {
            Worker::runAll();
        }
        
        
    }
}