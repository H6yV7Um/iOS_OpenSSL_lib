//
//  FSLoginManager.h
//  FashionApp
//
//  Created by 1 on 2018/4/13.
//  Copyright © 2018年 1. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface FSLoginService : NSObject
//+(FSLoginService *)shareInstance;

- (void)registerWXLogin;

- (void)wxLogin;

- (id)getWXLoginDelegate;

- (void)handWXLoginUrl:(NSDictionary *)aDict;
@end
