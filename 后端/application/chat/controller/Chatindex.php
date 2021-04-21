<?php
namespace app\chat\controller;
use think\Controller;
use think\Request;
use GatewayWorker\Lib\Gateway;
class Chatindex extends Controller
{
	// 初始化registerAddress
     public function __construct(){
        Gateway::$registerAddress = '127.0.0.1:0629';
     }

    public function index()
    {
    	return view();
    }
    public function chat()
    {
    	$request = Request::instance();
		// 获取当前域名
		$toid = $request->only("toid");
		$name = $toid['toid'];
    	// $data = $this->resdata($request);
         //$toid = $request->toid;
        // var_dump($request->param(toid));
         var_dump($toid['toid']);
    	// 向当前client_id发送数据 
       // Gateway::sendToClient($client_id, "Hello $client_id\r\n");
        // 向所有人发送
        Gateway::sendToAll("$name 登录啦\r\n");
    
    }
     // 发送信息
    public function send(){
  
            Gateway::sendToUid($to_id,json_encode($data));

    }
       
}
