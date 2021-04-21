<?php

namespace app\chat\model;
use app\chat\controller\Events;
use think\Model;
use think\Db;

class Stm32Temp extends Model
{
    
    // 自动写入时间戳
 protected $autoWriteTimestamp = true;
  //关闭 updata
  protected $updateTime = false;


  
    public function wendu(){
    //  $c=(new Events())->fanhui();
     // include("app/chat/controller/Events.php");
   
      
     // $temp = new Stm32Temp;//数据库表
     // $temp->mingzi= '温度';
     // $temp->temp= '123';
     // $temp->color= '#2fc25b';
    //$temp->save();
    	//$result = Db::table('stm32_temp')->insert(['temp'=>'26.3','shijian'=>'23:12']);
         var_dump($data);
    }

  

}