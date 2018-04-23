//
//  FSHttpService.m
//  FashionApp
//
//  Created by 1 on 2018/4/13.
//  Copyright © 2018年 1. All rights reserved.
//

#import "FSHttpService.h"
#import "AFNetworking.h"
#include "innercosHead.h"

@implementation FSHttpService
+ (FSHttpService *)shareInstance
{
    static FSHttpService *fsHttpObject = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        fsHttpObject = [[FSHttpService alloc] init];
    });
    return fsHttpObject;
}



@end
