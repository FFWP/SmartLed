
<template>
	<view >
		<cu-custom bgColor="bg-gradual-blue" :isBack="true">
	
		    <block slot="content">门控制</block>
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
				<view v-if="door1==0" class="iconfont cuIcon-exit" style="font-size: 200upx;">
					<view style="font-size: 30upx;font-weight: bold;color:#ED1C24; padding-left: 70upx;">门1</view>
				</view>
				<view v-if="door1==1" class="lg text-gray cuIcon-exit" style="font-size: 200upx;color: #FBBD08;">
					<view style="font-size: 30upx;font-weight: bold;color:#ED1C24; padding-left: 70upx;">门1</view>
				</view>
			</view >
			<view @tap="senddata2">
				<view v-if="door2==0" class="iconfont cuIcon-exit" style="font-size: 200upx;">
						<view style="font-size: 30upx;font-weight: bold;color:#ED1C24;padding-left: 70upx; ">门2</view>
					
				</view>
				<view v-if="door2==1" class="iconfont cuIcon-exit" style="font-size: 200upx;color: #FBBD08;">
						<view style="font-size: 30upx;font-weight: bold;color:#ED1C24;padding-left: 70upx; ">门2</view>
				</view>
			</view>
		</view>
		<view class="flex solid-bottom padding justify-around">
			<view @tap="senddata3">
				<view v-if="door3==0" class="iconfont cuIcon-exit" style="font-size: 200upx;">
					<view style="font-size: 30upx;font-weight: bold;color:#ED1C24; padding-left: 70upx;">门3</view>
				</view>
				<view v-if="door3==1" class="iconfont cuIcon-exit" style="font-size: 200upx;color: #FBBD08;">
					<view style="font-size: 30upx;font-weight: bold;color:#ED1C24; padding-left: 70upx;">门3</view>
				</view>
			</view >
			<view @tap="senddata4">
				<view v-if="door4==0" class="iconfont cuIcon-exit" style="font-size: 200upx;">
						<view style="font-size: 30upx;font-weight: bold;color:#ED1C24;padding-left: 70upx; ">门4</view>
					
				</view>
				<view v-if="door4==1" class="iconfont cuIcon-exit" style="font-size: 200upx;color: #FBBD08;">
						<view style="font-size: 30upx;font-weight: bold;color:#ED1C24;padding-left: 70upx; ">门4</view>
				</view>
			</view>
		</view>
		

	
	</view>
	
</template>

<script>
	
	export default {
		data() {
			return {
				door1:0,
				door2:0,
				door3:0,
				door4:0,
				
			}
		},
		onLoad() {
			this.connectws();
			this.door1=0;
			this.door2=0;
			this.door3=0;
			this.door4=0;
		},
	
		methods: {
			connectws(){
				
				//var shuju2;
				uni.connectSocket({
				  url: 'ws://149.129.103.5:7272'
				});
				
		// 		uni.onSocketMessage(function (res) {
		// 		_self.getshuju = res.data;
		// 		 console.log('服务器：'+_self.getshuju);
		// 		_self.datafl();
		// 		let subString =String(_self.getshuju).split("_");
		// 		let ceshi='TEP'
		// 		if(subString[0]==ceshi)
		// 		{
		// 			_self.getTmpData();
					
		// 		}
		
				
		// 		});

			},
			
				senddata1(){
					this.door1=1;
					
					this.door2=0;
					this.door3=0;
					this.door4=0;
				 uni.sendSocketMessage({
				   data: "CardC:3D9EB4AE",
					});
			
				},
				senddata2(){
					this.door1=0;
					this.door2=1;
					this.door3=0;
					this.door4=0;
				 uni.sendSocketMessage({
				   data: "CardB",
					});
							
				},
				senddata3(){
					this.door1=0;
					this.door2=0;
					this.door3=1;
					this.door4=0;
				 uni.sendSocketMessage({
				   data: "CardC",
					});
							
				},
				
		}
	}
</script>

<style>
	
</style>
