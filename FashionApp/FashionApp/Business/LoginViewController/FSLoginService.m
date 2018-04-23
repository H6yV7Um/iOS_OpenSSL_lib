//
//  FSLoginManager.m
//  FashionApp
//
//  Created by 1 on 2018/4/13.
//  Copyright © 2018年 1. All rights reserved.
//

#import "FSLoginService.h"
#import "WXLoginService.h"
#import "QQLoginService.h"

@implementation FSLoginService
//+(FSLoginService *)shareInstance
//{
//    static FSLoginService *loginM = nil;
//    static dispatch_once_t onceToken;
//    dispatch_once(&onceToken, ^{
//        loginM = [FSLoginService new];
//    });
//    return loginM;
//}
#pragma mark 微信登录
- (void)registerWXLogin
{
    [[WXLoginService shareWXLoginData] registerWXApi];
}

- (void)wxLogin
{
    [[WXLoginService shareWXLoginData] loginWXAccount];
}

- (void)handWXLoginUrl:(NSDictionary *)aDict
{
    [[WXLoginService shareWXLoginData] handleWXLoginDelegateUrl:[aDict objectForKey:@"url"]];
}

- (id)getWXLoginDelegate
{
    return [WXLoginService shareWXLoginData];
}

#pragma mark QQ登录
- (void)registerQQLogin
{
    [[QQLoginService shareQQLoginService] registerQQLogin];
}

- (void)qqlogin
{
    [[QQLoginService shareQQLoginService] qqLogin];
}

- (void)handQQLoginUrl:(NSDictionary *)aDict
{
     [[QQLoginService shareQQLoginService] handCallbackUrl:[aDict objectForKey:@"url"]];
}
@end
