
<template>
	<view >
		<cu-custom bgColor="bg-gradual-blue" :isBack="true">
	
		    <block slot="content">灯温度</block>
		</cu-custom>
		<!-- <view v-if="wengdu==null" class="cu-bar bg-white">
			<view class="action">
				<text class="cuIcon-title text-red"></text>
				<text>未检测到设备请检查连接！</text>
			</view>
		</view> -->
		<!-- LED -->
		<view class="flex solid-bottom padding justify-around">
			<view @tap="senddata1">
				<view v-if="led1zt==0" class="iconfont cuIcon-creative" style="font-size: 100upx;">
					<view style="font-size: 30upx;font-weight: bold;color:#ED1C24; padding-left: 18upx;">LED1</view>
				</view>
				<view v-if="led1zt==1" class="iconfont cuIcon-creativefill" style="font-size: 100upx;color: #FBBD08;">
					<view style="font-size: 30upx;font-weight: bold;color:#ED1C24; padding-left: 18upx;">LED1</view>
				</view>
			</view >
			<view @tap="senddata2">
				<view v-if="led2zt==0" class="iconfont cuIcon-creative" style="font-size: 100upx;">
						<view style="font-size: 30upx;font-weight: bold;color:#ED1C24;padding-left: 18upx; ">LED2</view>
					
				</view>
				<view v-if="led2zt==1" class="iconfont cuIcon-creativefill" style="font-size: 100upx;color: #FBBD08;">
						<view style="font-size: 30upx;font-weight: bold;color:#ED1C24;padding-left: 18upx; ">LED2</view>
				</view>
			</view>
		</view>
		<view class="qiun-columns">
				<view class="qiun-bg-white qiun-title-bar qiun-common-mt" >
					<view class="qiun-title-dot-light">当前温度：</view>
				</view>
				<view class="qiun-charts" style="padding-left: 100upx;" >
					<canvas canvas-id="canvasGauge" id="canvasGauge" class="charts"></canvas>
				</view>
		</view>
		<view class="qiun-columns">
				<view class="qiun-bg-white qiun-title-bar qiun-common-mt" >
					<view class="qiun-title-dot-light">温度折线：</view>
				</view>
				<view class="qiun-charts2" >
					<canvas canvas-id="canvasLineA" id="canvasLineA" class="charts2" @touchstart="touchLineA"></canvas>
				</view>
		</view>

	
	</view>
	
</template>

<script>
	import uCharts from '@/js_sdk/u-charts/u-charts/u-charts.js';
	var _self;

	var canvaGauge=null;
   var canvaLineA=null;
	export default {
		data() {
			return {
				tempcnm:[],
				tutu2:[],//具体数据
				tutu:[],//轴
				wengdu:'',
				frist:'',
				second:'',
				getshuju:'',
				led1zt:0,
				led2zt:0,
				cWidth:'',
				cHeight:'',
				zWidth:'',
				zHeight:'',
				pixelRatio:1,
				gaugeWidth:15,
				
				chartData: {
					categories: [{
						value: 0.3,
						color: '#2fc25b'
					}, {
						value: 0.7,
						color: '#1890ff'
					}, {
						value: 1,
						color: '#f04864'
					}],
					series: [{
						name: '温度',
						data:0,
					}]
				},
				chartData2: {
					//categories: ["23:11", '4点', '8点', '12点', '16点', '20点'],
					//categories: [],
					series: [{
						name: '温度',
						data: [],
						color: '#2fc25b'
					},
			
					]
				},
			}
		},
		onLoad() {
			_self = this;
			this.cWidth=uni.upx2px(550);
			this.cHeight=uni.upx2px(400);
			this.zWidth=uni.upx2px(750);
			this.zHeight=uni.upx2px(500);
			this.getServerData();
	
			this.connectws();
			this.getTmpData();
			
	
		},
	
		methods: {
			getTmpData(){
				var j ,len,tu;
				uni.request({
					url: 'http://smart.lxyy.me/outwendu',
					data:{
					},
					success: function(res) {
						console.log(res.data)
						let h=res.data;
						for(j = 0,len=h.length; j < len; j++) {
							let cucu =h[j].shijian;
							_self.tutu.push(cucu);//时间
							  
							  let cnm;//数据
							  tu=[{name:'温度',data:[],color: '#2fc25b'}];
							  let wendu=h[j].temp;
							  _self.tempcnm.unshift(wendu);
							  
							  tu[0].data=_self.tempcnm;
			
							  _self.tutu2=tu;
							  
						
						}
					
						let LineA={categories:[],series:[]};
						//这里我后台返回的是数组，所以用等于，如果您后台返回的是单条数据，需要push进去
						LineA.categories=_self.tutu.reverse();
						_self.tutu=[];
						LineA.series=_self.tutu2;
				
						_self.tutu2=[];
						_self.showLineA("canvasLineA",LineA);
					},
					
					fail: () => {
						_self.tips="网络错误，小程序端请检查合法域名";
					},
				});
				
			},
			senddata1(){
	
				let bian;
					if(this.led1zt==0)
					{
						bian="LED1_1";
					}else{
						bian="LED1_0";
					}
				 uni.sendSocketMessage({
				   data: bian,
				    });
				uni.sendSocketMessage({
				  data: bian,
				   });
				   uni.sendSocketMessage({
				      data: bian,
				       });
			
			},
			senddata2(){
				
			let bian;
				if(this.led2zt==0)
				{
					bian="LED2_1";
				}else{
					bian="LED2_0";
				}
			 uni.sendSocketMessage({
			   data: bian,
			    });
			uni.sendSocketMessage({
			  data: bian,
			   });
			   uni.sendSocketMessage({
			      data: bian,
			       });
			
				
			},
			//数据分离
			//led1_1%led2_1!tep_30
		datafl(){
		
			var qian;
			var hou;
			var qian2;
			var hou2;
			var qian3;
			var hou3;
				if(this.getshuju.includes("%"))//true
				{	
				
					let subString =String(this.getshuju).split("%");
					qian=subString[0];//led1_1
					this.led1zt=this.getCaption(qian);
					hou=subString[1];//中转
					console.log('qq'+qian)
						if(this.getshuju.includes("!"))//true
						{
							let subString2 =String(hou).split("!");
							qian2=subString2[0];//led2_1
							this.led2zt=this.getCaption(qian2);
							hou2=subString2[1];//tep_30
							this.wengdu=this.getCaption(hou2)/100;
							
							this.chartData.series[0].data =this.wengdu;
							this.getServerData();
							console.log('rr'+this.chartData.series[0].data)
						}
					
				}else{
					let bian =String(this.getshuju).split("_");//中转处理 单数据
					qian3=bian[0];
					hou3=bian[1];
					switch(qian3) {
					     case 'LED1':
						 console.log('bbb22：'+hou3);
					        this.led1zt=hou3;
					        break;
					     case 'LED2':
					        this.led2zt=hou3;
							break;
						case 'TEP':
						   this.wengdu=hou3/100;
						   this.chartData.series[0].data =this.wengdu;
						   console.log(this.wengdu)
						   this.getServerData();
						 
						break;
					     // default:
					     //    默认代码块
					} 
					
				}
				
			},
			//数据处理方法
			getCaption(obj){
			
			    var index=obj.lastIndexOf("\_");
			    obj=obj.substring(index+1,obj.length);
			//  console.log(obj);
			    return obj;
			},
			//数据温度
			// getwengdu(){
			// 	var wengdu;
			// 	wengdu=this.getCaption(this.getshuju);
				
			// },
			//数据led
			// chooseled(){
			
			// 	console.log('vv：'+rr);
			// 	let subString =String(this.getshuju).split("_");
			// 	var qian;
			// 	var hou;
			// 	qian=subString[0];
			// 	hou=subString[1]
				
			// 	switch(qian) {
			// 	     case 'LED1':
			// 		 console.log('bbb：'+hou);
			// 	        this.led1zt=hou;
			// 	        break;
			// 	     case 'LED2':
			// 	        this.led2zt=hou;
			// 			break;
			// 	     // default:
			// 	     //    默认代码块
			// 	} 
			// },
			connectws(){
				
				var shuju2;
				uni.connectSocket({
				  url: 'ws://149.129.103.5:7272'
				});
				
				uni.onSocketMessage(function (res) {
				_self.getshuju = res.data;
				 console.log('服务器：'+_self.getshuju);
				_self.datafl();
				let subString =String(_self.getshuju).split("_");
				let ceshi='TEP'
				if(subString[0]==ceshi)
				{
					_self.getTmpData();
					
				}
				
				
				
				});
				
				 //this.getshuju=shuju
				//let subString =String(shuju).split("_");
				//this.getshuju=subString;
				
				//console.log('vv：'+_self.getshuju);
				
			},
			
			getServerData(){
				
				let Gauge={categories:[],series:[]};
				Gauge.categories=this.chartData.categories;
				Gauge.series=this.chartData.series;
				_self.showGauge("canvasGauge",Gauge);
		
			},
			showGauge(canvasId,chartData){
				var num ;
				num= (this.wengdu*100).toFixed(1);//保留2位小数
				canvaGauge = new uCharts({
					$this:_self,
					canvasId: canvasId,
					type: 'gauge',
					fontSize:11,
					legend:false,
					title: {
						//name: Math.round((chartData.series[0].data*100))+'℃',
						name:num+'℃',
						color: chartData.categories[1].color,
						fontSize: 20*_self.pixelRatio,
						offsetY:50*_self.pixelRatio,//新增参数，自定义调整Y轴文案距离
					},
					subtitle: {
						name: chartData.series[0].name,
						color: '#666666',
						fontSize: 15*_self.pixelRatio,
						offsetY:-50*_self.pixelRatio,//新增参数，自定义调整Y轴文案距离
					},
					extra: {
						gauge:{
							type:'default',
							width: _self.gaugeWidth*_self.pixelRatio,//仪表盘背景的宽度
							startAngle:0.75,
							endAngle:0.25,
							startNumber:0,
							endNumber:100,
							splitLine:{
								fixRadius:0,
								splitNumber:10,
								width: _self.gaugeWidth*_self.pixelRatio,//仪表盘背景的宽度
								color:'#FFFFFF',
								childNumber:5,
								childWidth:_self.gaugeWidth*0.4*_self.pixelRatio,//仪表盘背景的宽度
							},
							pointer:{
								width: _self.gaugeWidth*0.8*_self.pixelRatio,//指针宽度
								color:'auto'
							}
						}
					},
					background:'#FFFFFF',
					pixelRatio:_self.pixelRatio,
					categories: chartData.categories,
					series: chartData.series,
					animation: false,
					width: _self.cWidth*_self.pixelRatio,
					height: _self.cHeight*_self.pixelRatio,
					dataLabel: true,
				});
			},
	
			showLineA(canvasId,chartData2){
				canvaLineA=new uCharts({
					$this:_self,
					canvasId: canvasId,
					type: 'line',
					fontSize:11,
					legend:{show:true},
					dataLabel:false,
					dataPointShape:true,
					background:'#FFFFFF',
					pixelRatio:_self.pixelRatio,
					categories: chartData2.categories,
					series: chartData2.series,
					animation: false,
					xAxis: {
						type:'grid',
						gridColor:'#CCCCCC',
						gridType:'dash',
						dashLength:8
					},
					yAxis: {
						gridType:'dash',
						gridColor:'#CCCCCC',
						dashLength:8,
						splitNumber:10,
						min:0,
						max:100,
						format:(val)=>{return val.toFixed(0)+'℃'}
					},
					width: _self.zWidth*_self.pixelRatio,
					height: _self.zHeight*_self.pixelRatio,
					extra: {
						line:{
							type: 'straight'
						}
					}
				});
				
			},
			touchLineA(e) {
				canvaLineA.showToolTip(e, {
					format: function (item, category) {
						return category + ' ' + item.name + ':' + item.data 
					}
				});
			},
		}
	}
</script>

<style>
	/*样式的width和height一定要与定义的cWidth和cHeight相对应*/
	.qiun-charts {
		width: 550upx;
		height: 360upx;
		background-color: #F1F1F1;
	}
	
	.charts {
		width: 550upx;
		height: 360upx;
		background-color: #F1F1F1;
	}
	.qiun-charts2 {
		width: 750upx;
		height: 500upx;
		background-color: #F1F1F1;
	}
	
	.charts2 {
		width: 750upx;
		height: 500upx;
		background-color: #F1F1F1;
	}
</style>
