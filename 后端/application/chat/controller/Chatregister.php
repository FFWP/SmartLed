<?php
namespace app\chat\controller;
use Workerman\Worker;
use GatewayWorker\Register;
class Chatregister{
    public function __construct(){
        // register 服务必须是text协议
        $register = new Register('text://0.0.0.0:1236');
       //  $register = new Register('text://0.0.0.0:0628');
        // 如果不是在根目录启动，则运行runAll方法
        if(!defined('GLOBAL_START'))
        {
            Worker::runAll();
        }
    }
}