
<template>
	<view >
		<cu-custom bgColor="bg-gradual-blue" :isBack="false">
		    <block slot="content">智能配网</block>
		</cu-custom>
		
		<view class="content">
			<view style="font-size: 50upx;padding-bottom: 30upx;padding-top: 300upx;"> WIFI名为: {{ssid}}</view>
			<view class="text-area">
					<button class="cu-btn round lg bg-gradual-red" @tap="getssid">获取WIFI</button>
			</view>
			<view style="font-size: 50upx;padding-top: 30upx;"> 密码：</view>
			 <input @input="inputpassword" style="height: 80upx;padding-top: 20upx;" type="text" placeholder="输入密码" :value="password2" />
			  <button class="cu-btn round lg bg-gradual-purple" style="margin-top: 30upx;" @tap="startwifi">开始配网</button>
		</view>
	</view>
	
</template>

<script>
	const hkSC = uni.requireNativePlugin('HKing-SmartConfig'); 
	export default {
		data() {
			return {
				ssid:"",
				password:0,
		
			}
		},
		onLoad() {

		},
		methods: {
			inputpassword(e) {
			                this.password = e.detail.value;
							  console.log(this.password)
			            },
			getssid(){
				var apSsid = "";
				hkSC.getWifiSsid(result=>{
				    if(result.apSsid==""){
				        console.log("当前手机未连接wifi");
				        return;
				    }
				   this.ssid = result.apSsid;
				    console.log("apSsid："+this.ssid);
				});	
			},
			startwifi(){
				console.log('ccccccc'+this.ssid)
				if(this.ssid!=""){
					
					if(this.password==0)
					{
						uni.showToast({
						    title: '密码不能为空',
						
						    duration: 2000
						});
						
						
					}
					if(this.password!=0)
					{
						uni.showLoading({
						    title: '配网中请稍后..'
						});
						hkSC.startSmaerConfig(this.ssid,this.password,result=>{
						    uni.hideLoading();
						   uni.showToast({
						       title: '配网成功',
						   
						       duration: 2000
						   });
						  this.ssid="";
						});
						
					
					}
					
				}
				if(this.ssid=="")
				{
					uni.showToast({
					    title: 'WIFI名未知',
					    duration: 2000
					});
				}
				
				
			}
			
		}
	}
</script>

<style>
	.content {
		display: flex;
		flex-direction: column;
		align-items: center;
		justify-content: center;
	}



</style>
