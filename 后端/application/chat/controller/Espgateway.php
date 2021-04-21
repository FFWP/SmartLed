<?php 
/**
 * linux workerman例子测试
 * 需要在Linux系统控制台进行启动，启动文件位于根目录的start.php文件中
 * Windows无法进行同时启动多个协议
 * 由于PHP-CLI在windows系统无法实现多进程以及守护进程，所以windows版本Workerman建议仅作开发调试使用。
 */
namespace app\chat\controller;
use Workerman\Worker;
use GatewayWorker\Gateway;
use GatewayWorker\Register;
use GatewayWorker\BusinessWorker;
class Espgateway
{
    /**
     * 构造函数
     * @access public
     */
    public function __construct(){
        
        $gateway_text = new Gateway("tcp://0.0.0.0:7273");
        // 进程名称，主要是status时方便识别
        $gateway_text->name = 'Espgateway';
        // 开启多少text协议的gateway进程
        $gateway_text->count = 4;
        // 本机ip（分布式部署时需要设置成内网ip）
        $gateway_text->lanIp = '127.0.0.1';
        // 设置服务注册地址(注意：这个地址是start_register.php中监听的地址)
        $gateway_text->registerAddress = '127.0.0.1:1236';
        // gateway内部通讯起始端口，起始端口不要重复
        $gateway_text->startPort = 2500;
        // 也可以设置心跳，这里省略
        // ##########新增端口支持Text协议 结束##########

        if(!defined('GLOBAL_START'))
        {
            Worker::runAll();
        }
    }
}