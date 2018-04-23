//
//  FSServiceRoute.m
//  ProtocalClass
//
//  Created by 1 on 2018/4/3.
//  Copyright © 2018年 1. All rights reserved.
//

#import "FSServiceRoute.h"
typedef void (^routerCallbackBlock)(NSDictionary *info);

@interface FSServiceRoute()
@property (nonatomic, strong) NSMapTable *cachedTarget;
@end

@implementation FSServiceRoute

+ (FSServiceRoute *)shareInstance
{
    static FSServiceRoute *rObject = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        rObject = [[FSServiceRoute alloc] init];
    });
    return rObject;
}

- (instancetype)init
{
    if(self=[super init])
    {
        self.cachedTarget = [NSMapTable mapTableWithKeyOptions:NSMapTableCopyIn valueOptions:NSPointerFunctionsWeakMemory];
    }
    return self;
}
////调用单例service
//- (id)singletonCallService:(NSString *)className
//                      func:(NSString *)actionName
//                 withParam:(NSDictionary *)param
//                completion:(void(^)(NSDictionary *info))completion
//{
//    NSAssert(className, @"className not nil");
//    NSAssert(actionName, @"actionName not nil");
//    if(className==nil && !actionName)
//    {
//        return nil;
//    }
//    Class targetClass = NSClassFromString(className);
//    if([targetClass respondsToSelector:@selector(shareInstance)])
//    {
//        id targetObject = [targetClass shareInstance];
//        SEL sel = NSSelectorFromString(actionName);
//        if([targetObject respondsToSelector:sel])
//        {
//            return [self safePerformAction:sel target:targetObject withParam:param completion:completion];
//        }
//        else
//        {
//            DDLogInfo(@"调用失败：%@",[NSString stringWithFormat:@"%@中没有找到shareInstance@方法，检查参数及方法名是否正确",className]);
//            NSAssert(NO, ([NSString stringWithFormat:@"%@中没有找到shareInstance方法，检查参数及方法名是否正确",className]));
//        }
//    }
//    else
//    {
//        DDLogInfo(@"调用失败：%@",[NSString stringWithFormat:@"%@中没有找到shareInstance方法，检查参数及方法名是否正确",className]);
//        NSAssert(NO, ([NSString stringWithFormat:@"%@中没有找到shareInstance方法，检查参数及方法名是否正确",className]));
//    }
//    return nil;
//}
//调用非单例service
+ (id)syncCallService:(NSString *)className
                 func:(NSString *)actionName
            withParam:(NSDictionary *)param
{
    NSAssert(className, @"className not nil");
    NSAssert(actionName, @"actionName not nil");
    if(className==nil && !actionName)
    {
        return nil;
    }
    
    id targetObject = [[FSServiceRoute shareInstance].cachedTarget objectForKey:className];
    if(targetObject == nil)
    {
        Class targetClass = NSClassFromString(className);
        targetObject = [targetClass new];
        [[FSServiceRoute shareInstance].cachedTarget setObject:targetObject forKey:className];
    }
    SEL sel = NSSelectorFromString(actionName);
    if([targetObject respondsToSelector:sel])
    {
        return [[FSServiceRoute shareInstance] safePerformAction:sel target:targetObject withParam:param completion:nil];
    }
    else
    {
        DDLogInfo(@"调用失败：%@",[NSString stringWithFormat:@"%@中没有找到%@方法，检查参数及方法名是否正确",className,actionName]);
        NSAssert(NO, ([NSString stringWithFormat:@"%@中没有找到%@方法，检查参数及方法名是否正确",className,actionName]));
    }
    return nil;
}
//异步调用要临时持有住 对象
+ (id)asyncCallService:(NSString *)className
                  func:(NSString *)actionName
             withParam:(NSDictionary *)param
            completion:(void(^)(NSDictionary *info))completion
{
    NSAssert(className, @"className not nil");
    NSAssert(actionName, @"actionName not nil");
    if(className==nil && actionName==nil)
    {
        return nil;
    }

    
    id targetObject = [[FSServiceRoute shareInstance].cachedTarget objectForKey:className];
    if(targetObject == nil)
    {
        Class targetClass = NSClassFromString(className);
        targetObject = [targetClass new];
        [[FSServiceRoute shareInstance].cachedTarget setObject:targetObject forKey:className];
    }
    SEL sel = NSSelectorFromString(actionName);
    if([targetObject respondsToSelector:sel])
    {
        return [[FSServiceRoute shareInstance] safePerformAction:sel target:targetObject withParam:param completion:completion];
    }
    else
    {
        DDLogInfo(@"调用失败：%@",[NSString stringWithFormat:@"%@中没有找到%@方法，检查参数及方法名是否正确",className,actionName]);
        NSAssert(NO, ([NSString stringWithFormat:@"%@中没有找到%@方法，检查参数及方法名是否正确",className,actionName]));
    }
    return nil;
}



- (id)safePerformAction:(SEL)action target:(NSObject *)target withParam:(NSDictionary *)params completion:(void(^)(NSDictionary *info))completion
{
    NSMethodSignature* methodSig = [target methodSignatureForSelector:action];
    if(methodSig == nil) {
        return nil;
    }
    const char* retType = [methodSig methodReturnType];

    NSInvocation *invoke = [NSInvocation invocationWithMethodSignature:methodSig];
    [invoke setTarget:target];
    [invoke setSelector:action];
    if(params)
    {
        [invoke setArgument:&params atIndex:2];
        if(completion)
        {
            [invoke setArgument:&completion atIndex:3];
        }
    }
    else
    {
        if(completion)
        {
            [invoke setArgument:&completion atIndex:2];
        }
    }

    [invoke invoke];
    if(strcmp(retType, @encode(void))!=0)
    {
        id retuvalue = nil;
        [invoke getReturnValue:&retuvalue];
        return retuvalue;
    }
    else
    {
        return nil;
    }
}


@end
