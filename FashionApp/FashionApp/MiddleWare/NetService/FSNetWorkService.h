//
//  FSHttpService.h
//  FashionApp
//
//  Created by 1 on 2018/4/10.
//  Copyright © 2018年 1. All rights reserved.
//

#import <Foundation/Foundation.h>

@class FSBaseNetworkService;

@interface FSNetWorkService : NSObject
+ (FSNetWorkService *)shareInstance;

- (void)connectNetWns;

- (long)sendRequestTCPData:(FSBaseNetworkService *)baseService
                completion:(void(^)(NSString *cmd, NSData *data, NSError *bizError, NSError *wnsError, NSError *wnsSubError))completion;

- (void)cancelReq:(long)reqNum;
@end