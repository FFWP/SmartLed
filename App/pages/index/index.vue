<template>
	<view>
			<cu-custom bgColor="bg-gradual-blue" :isBack="false">
			<block slot="content">主页</block>
		</cu-custom>
	
		<view class="margin radius bg-gradual-green shadow-blur" style="height:100upx;font-size: 40upx; text-align: center;padding-top: 16upx;">
			<text>点击进入物联网设备 </text>
		</view>
		
		<view class="margin flex">
			<view class=" bg-gradual-blue flex-sub margin-right radius shadow-lg cuIcon-creativefill" style="font-size: 240upx;text-align: center;" @click="tiaoledtem()">
				
			</view>
			<view class="bg-gradual-red flex-sub radius shadow-lg">
				<image src="../../static/door.png" mode="aspectFit" class="gif-white response" style="height:240upx; padding-top: 10px;" @click="tiaodoor()"></image>
			</view>
		</view>
		<view class="margin flex">
			<view class="bg-white flex-sub margin-right radius shadow-lg">
				<image src="../../static/rhomb-black.gif" mode="aspectFit" class="gif-black response" style="height:240upx"></image>
			</view>
			<view class="bg-white flex-sub radius shadow-lg">
				<image src="../../static/rhomb-black.gif" mode="aspectFit" class="gif-white response" style="height:240upx"></image>
			</view>
		</view>
		<view class="margin flex">
			<view class="bg-white flex-sub margin-right radius shadow-lg">
				<image src="../../static/rhomb-black.gif" mode="aspectFit" class="gif-white response" style="height:240upx"></image>
			</view>
			<view class="bg-black flex-sub radius shadow-lg">
				<image src="../../static/rhomb-black.gif" mode="aspectFit" class="gif-black response" style="height:240upx"></image>
			</view>
		</view>
	</view>
</template>

<script>
	export default {
		data() {
			return {
				
			

			};
		},
		methods: {
			tiaoledtem(){//跳转led
				uni.navigateTo({
					url:'../ledtmp/ledtmp',
				})
			},
			tiaodoor(){//跳转led
				uni.navigateTo({
					url:'../door/door',
				})
			},
	
		},
	}
</script>

<style>


</style>