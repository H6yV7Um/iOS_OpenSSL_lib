//
//  FSHttpService.m
//  FashionApp
//
//  Created by 1 on 2018/4/10.
//  Copyright © 2018年 1. All rights reserved.
//

#import "FSNetWorkService.h"
#import "FSBaseNetworkService.h"

#import <WnsSDK4Cloud/WnsSDK.h>

@interface FSNetWorkService ()
{
    WnsSDK *_wnsSDK;
    
}
@property (nonatomic, assign)WnsSDKStatus gWnsSDKStatus;
@property (nonatomic, assign)int tryCount;
@end

@implementation FSNetWorkService

+ (FSNetWorkService *)shareInstance
{
    static FSNetWorkService *fshObject = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        fshObject = [[FSNetWorkService alloc] init];
    });
    return fshObject;
}

- (id)init
{
    if(self=[super init])
    {
        self.tryCount = 0;
    }
    return self;
}
#pragma mark  init wns
- (void)connectNetWns
{
    __weak FSNetWorkService *weakself = self;
    if(_wnsSDK)
    {
        [_wnsSDK reset:YES];
    }
    else
    {
        _wnsSDK = [[WnsSDK alloc] initWithAppID:[kFSAPPID intValue]
                    andAppVersion:kFSAPPSHORTVERSION
                    andAppChannel:[self appChannel]
                       andAppType:WnsSDKAppTypeThirdParty];
    }
    [_wnsSDK setStatusCallback:^(WnsSDKStatus status) {
        weakself.gWnsSDKStatus = status;
        if(weakself.gWnsSDKStatus == WnsSDKStatusDisconnected)
        {
            DebugLog(@"wns连接失败");
            if(weakself.tryCount<2)
            {
                [weakself resetConnection];
                weakself.tryCount++;
            }
        }
        else if(weakself.gWnsSDKStatus == WnsSDKStatusConnecting)
        {
            DebugLog(@"wns连接中......");
        }
        else if(weakself.gWnsSDKStatus == WnsSDKStatusConnected)
        {
            DebugLog(@"wns连接成功");
            weakself.tryCount = 0;
        }
        
    }];
    
}

- (void)resetConnection
{
    if(_wnsSDK)
    {
        [_wnsSDK reset:YES];
    }
    else
    {
        [self connectNetWns];
    }
}

- (NSString *)appChannel
{
    return @"appstore";
}

#pragma mark 封装tcp
- (long)sendRequestTCPData:(FSBaseNetworkService *)baseService
                completion:(void(^)(NSString *cmd, NSData *data, NSError *bizError, NSError *wnsError, NSError *wnsSubError))completion;
{
    if(self.gWnsSDKStatus == WnsSDKStatusConnected)
    {
        return [_wnsSDK sendRequest:[baseService requestData] cmd:[baseService wnsCmd] timeout:[baseService reqTimeOut] completion:^(NSString *cmd, NSData *data, NSError *bizError, NSError *wnsError, NSError *wnsSubError) {
            DDLogInfo(@"cmd=%@,bizeErrorCode=%ld,wnsErrorCode=%ld,wnsSubErrorCode=%ld",cmd,(long)bizError.code,(long)wnsError.code,(long)wnsSubError.code);
            completion(cmd,data,bizError,wnsError,wnsSubError);
        }];
    }
    else
    {
        [self resetConnection];
    }
    return -1;
}

- (void)cancelReq:(long)reqNum
{
    [_wnsSDK cancelRequest:reqNum];
}

@end
