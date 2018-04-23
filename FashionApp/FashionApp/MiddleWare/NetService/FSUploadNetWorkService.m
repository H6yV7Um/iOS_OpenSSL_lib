//
//  FSUploadNetWorkService.m
//  FashionApp
//
//  Created by 1 on 2018/4/23.
//  Copyright © 2018年 1. All rights reserved.
//

#import "FSUploadNetWorkService.h"
#import "TXUGCPublish.h"
#import "ThreadSafeMutableDictionary.h"

#define kUPLoadVideoAPPID  @"1254126712"

@interface FSUploadNetWorkService()<TXVideoPublishListener>
{
    int uploadReq;
}
@property (nonatomic, strong) ThreadSafeMutableDictionary *cacheDict;
@property (nonatomic, strong) TXUGCPublish *pulish;
@property (nonatomic, copy) void(^resultBlock)(NSDictionary *dict, int retCode);
@property (nonatomic, copy) void(^processBlock)(NSInteger uploadBytes, NSInteger totalBytes);
@end

@implementation FSUploadNetWorkService
#pragma mark overload func
- (NSString *)getVideoPath
{
    return nil;
}

- (UIImage *)coverImage
{
    return nil;
}

- (BOOL)enableHTTPS
{
    return YES;
}

- (BOOL)enableResume
{
    return YES;
}

#pragma mark upload func
- (void)upLoadVideoPath:(NSString *)videoPath
                 result:(void(^)(NSDictionary *result, int retCode))resultBlock
                 procee:(void(^)(NSInteger bytesUpload, NSInteger bytesTotal))processBlock
{
    self.resultBlock = resultBlock;
    self.processBlock = processBlock;
    WeakSelf(self);
    [self getVideoSignature:^(NSString *signature) {
        weakself.pulish = [[TXUGCPublish alloc] initWithUserID:kUPLoadVideoAPPID];
        weakself.pulish.delegate = weakself;
        TXPublishParam *upParm = [self getVideoParam];
        if(videoPath)
        {
            upParm.videoPath = videoPath;
        }
        upParm.signature = signature;
        [weakself.pulish publishVideo:upParm];
    }];
}

- (TXPublishParam *)getVideoParam
{
    TXPublishParam *upParm = [TXPublishParam new];
    if([self coverImage])
    {
        upParm.coverImage = [self coverImage];
    }
    if([self getVideoPath])
    {
        upParm.videoPath = [self getVideoPath];
    }
    upParm.enableHTTPS = [self enableHTTPS];
    upParm.enableResume = [self enableResume];
    return upParm;
}

- (void)getVideoSignature:(void (^)(NSString *signature))block
{
    NSMutableDictionary *dict = [self packetReqParamSerName:@"Style.FeedOperationServer.FeedOperationObj" funcName:@"ApplyUpload" reqJceName:@"QZJStyleApplyUploadReq" resposeJceName:@"QZJStyleApplyUploadRsp" busDict:nil];
    NSMutableDictionary *feedDict = [NSMutableDictionary dictionaryWithCapacity:0];
    [feedDict setValue:[NSNumber numberWithInt:1] forKey:@"type"];
    [dict setObject:feedDict forKey:@"feed_info"];
    [self sendRequestDict:dict completion:^(NSDictionary *busDict, NSError *bizError) {
        DDLogDebug(@"视频签名%@",busDict);
        block([busDict objectForKey:@"signature"]);
    }];
}

- (void)cancelUpload:(int)seqNum
{
    if(self.pulish)
    {
        [self.pulish canclePublish];
    }
}

#pragma mark delegate txvideoPublishListerer

-(void) onPublishProgress:(NSInteger)uploadBytes totalBytes: (NSInteger)totalBytes
{
    if(self.processBlock)
    {
        self.processBlock(uploadBytes, totalBytes);
    }
}

/**
 * 短视频发布完成
 */
-(void) onPublishComplete:(TXPublishResult*)result
{
    if(result)
    {
        self.resultBlock([self responseToDict:result], result.retCode);
    }
}

#pragma mark tool func
- (NSDictionary *)responseToDict:(TXPublishResult *)response
{
    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithCapacity:5];
    [dict setValue:[NSNumber numberWithInt:response.retCode] forKey:@"retCode"];
    [dict setValue:response.descMsg forKey:@"descMsg"];
    [dict setValue:response.videoId forKey:@"videoId"];
    [dict setValue:response.videoURL forKey:@"videoURL"];
    [dict setValue:response.coverURL forKey:@"coverURL"];
    return dict;
}

- (ThreadSafeMutableDictionary *)cacheDict
{
    if(!_cacheDict)
    {
        _cacheDict = [[ThreadSafeMutableDictionary alloc] initWithCapacity:0];
    }
    return _cacheDict;
}

@end
