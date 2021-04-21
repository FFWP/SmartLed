<?php
namespace app\chat\controller;
use think\Controller;
use think\Request;
use think\Db;
use app\chat\model\Stm32Temp;
use GatewayWorker\Lib\Gateway;
class Xieru extends Controller
{
   public function inwendu()
    {
     
 	(new Stm32Temp())->wendu();
  
    }
  
  
    public function outwendu()
    {
      $data=Db::name('stm32_temp')->order('create_time','desc')->limit(10)->select();
    	//$data=(new Stm32Temp())->outwendu();
      return json($data);
    }
  

   
       
}
