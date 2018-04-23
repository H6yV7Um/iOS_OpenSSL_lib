//
//  BaseHttpService.m
//  FashionApp
//
//  Created by 1 on 2018/4/10.
//  Copyright © 2018年 1. All rights reserved.
//

#import "FSBaseNetworkService.h"
#import "FSNetWorkService.h"
#import "ThreadSafeMutableArry.h"
#import "ThreadSafeMutableDictionary.h"
#import "JCEObjectConverter.h"

@interface FSBaseNetworkService()
{
    NSString *_reqCmd;
    NSData *_reqData;
    ThreadSafeMutableArry *_reqCacheArry;
    NSString *_servantName;
    NSString *_funcName;
    NSString *_requestJceClass;
    NSString *_responseJceClass;
    ThreadSafeMutableDictionary *_jceMap;
}
@end

@implementation FSBaseNetworkService

- (instancetype)init
{
    if(self = [super init])
    {
        _reqCacheArry = [[ThreadSafeMutableArry alloc] init];
        _jceMap = [[ThreadSafeMutableDictionary alloc] initWithCapacity:0];
    }
    return self;
}

#pragma mark TCP
- (NSString *)wnsCmd
{
    if(!_reqCmd)
    {
#ifdef DEBUG
        _reqCmd = @"srf_proxy_test";
#else
        _reqCmd = @"srf_proxy";
#endif
    }
    return _reqCmd;
}

#pragma mark overLoad func

- (NSString *)serviceName
{
    return nil;
}

- (NSString *)funcName
{
    return nil;
}

- (NSString *)requestJceClass
{

    return nil;
}

- (NSData *)requestData
{
    if(_reqData)
    {
        return _reqData;
    }
    return nil;
}

- (NSString *)responseJceClass
{
    return nil;
}

- (NSTimeInterval)reqTimeOut
{
    return 10000;
}

#pragma mark request network func

- (long)sendRequestDict:(NSDictionary *)dict completion:(void(^)(NSDictionary *busDict, NSError *bizError))completion
{
    if([self checkParam:dict])
    {
        JceObjectV2 *jceObject = convertDicToJceObject(dict, NSClassFromString(_requestJceClass));
        _reqData = creatCommonPacketWithServantName(_servantName, _funcName, @"req", jceObject);
        if(_reqData.length>0)
        {
            _reqCmd = [self wnsCmd];
            NSAssert(_reqCmd, @"cmd不能为空");
            WeakSelf(self);
            NSMutableString *strSeq = [[NSMutableString alloc] init];
            long reqNum = [[FSNetWorkService shareInstance] sendRequestTCPData:self completion:^(NSString *cmd, NSData *data, NSError *bizError, NSError *wnsError, NSError *wnsSubError) {
                if(!wnsError && !wnsSubError && !bizError)
                {
                    NSString *respoJceClass = [self->_jceMap objectForKey:strSeq];
                    NSDictionary *dict = parseWUPData(data, @"rsp", respoJceClass);
                    completion(dict,bizError);
                    DDLogDebug(@"请求成功seq=%@,dict=%@",strSeq,dict);
                }
                [weakself removeSeqNum:strSeq];
            }];
            
            if(reqNum != -1)
            {
                [strSeq appendFormat:@"%ld",reqNum];
                [_reqCacheArry addObject:strSeq];
                [_jceMap setObject:_responseJceClass forKey:strSeq];
            }
            else
            {
#warning 处理网络请求为-1的情况
            }
            return reqNum;
        }
    }
    return -1;
}

#pragma mark  tool func

- (NSMutableDictionary *)packetReqParamSerName:(NSString *)servantName
                                      funcName:(NSString *)funcName
                                    reqJceName:(NSString *)reqJce
                                resposeJceName:(NSString *)responseJce
                                       busDict:(NSDictionary *)busDict
{
    if(!funcName || !reqJce || !responseJce || !servantName)
    {
        NSAssert(NO, @"funcName,cmd,servantName,requestJceClass,responseJceClass是必须填写的");
    }
    NSMutableDictionary *mutDict = [NSMutableDictionary dictionary];
    [mutDict setObject:servantName forKey:@"servantName"];
    [mutDict setObject:funcName forKey:@"funcName"];
    [mutDict setObject:reqJce forKey:@"requestJceClass"];
    [mutDict setObject:responseJce forKey:@"responseJceClass"];
    if(busDict)
    {
        [mutDict addEntriesFromDictionary:busDict];
    }
    _servantName = servantName;
    _funcName = funcName;
    _requestJceClass = reqJce;
    _responseJceClass = responseJce;
    return mutDict;
}

- (BOOL)checkParam:(NSDictionary *)dict
{
    if(!_servantName)
    {
        _servantName = [self serviceName]?:[dict objectForKey:@"servantName"];
    }
    if(!_funcName)
    {
        _funcName = [self funcName]?:[dict objectForKey:@"funcName"];
    }
    if(!_requestJceClass)
    {
        _requestJceClass = [self requestJceClass]?:[dict objectForKey:@"requestJceClass"];
    }
    if(!_responseJceClass)
    {
        _responseJceClass = [self responseJceClass]?:[dict objectForKey:@"responseJceClass"];
    }

    if(!_funcName || !_requestJceClass || !_responseJceClass || !_servantName)
    {
        NSAssert(NO, @"funcName,cmd,servantName,requestJceClass,responseJceClass是必须填写的");
        DDLogInfo(@"%@",dict);
        return NO;
    }
    return YES;
}

- (void)removeSeqNum:(NSString *)reqNum
{
    if(reqNum)
    {
        [_reqCacheArry removeObject:reqNum];
        [_jceMap removeObjectForKey:reqNum];
    }
}

- (void)cancelRequest:(long)seq
{
    [[FSNetWorkService shareInstance] cancelReq:seq];
}

- (void)cancelAllReq
{
    if(_reqCacheArry.count>0)
    {
        int count = (int)_reqCacheArry.count;
        [_reqCacheArry enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [[FSNetWorkService shareInstance] cancelReq:[(NSString *)obj longLongValue]];
            if(idx == count-1)
            {
                *stop = YES;
            }
        }];
        [_reqCacheArry removeAllObjects];
    }
}

#pragma mark http
- (NSString *)getBaseUrl
{
    return @"http://";
}

- (NSString *)httpMethod
{
    return @"GET";
}

- (NSString *)httpContentType
{
    return @"application/x-www-form-urlencoded";
}

- (NSString *)httpContentEncoding
{
    return @"gzip";
}


- (void)dealloc
{
    [self cancelAllReq];
}

@end
