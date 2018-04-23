//
//  FSUploadNetWorkService.h
//  FashionApp
//
//  Created by 1 on 2018/4/23.
//  Copyright © 2018年 1. All rights reserved.
//

#import "FSBaseNetworkService.h"

@interface FSUploadNetWorkService : FSBaseNetworkService

- (NSString *)getVideoPath;

- (UIImage *)coverImage;

- (BOOL)enableHTTPS;

- (BOOL)enableResume;

- (void)upLoadVideoPath:(NSString *)videoPath
                 result:(void(^)(NSDictionary *result, int retCode))resultBlock
                 procee:(void(^)(NSInteger bytesUpload, NSInteger bytesTotal))processBlock;

@end
