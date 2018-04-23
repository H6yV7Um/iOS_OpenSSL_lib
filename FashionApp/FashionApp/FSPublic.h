//
//  FSPublic.h
//  FashionApp
//
//  Created by 1 on 2018/4/10.
//  Copyright © 2018年 1. All rights reserved.
//

#ifndef FSPublic_h
#define FSPublic_h

#define  kFSAPPID @"203864"
#define  kWEIXINLoginAppid @"wxa4c083e3fcf06c80"
#define  kWEXINAppSecret @"2de3d3a83645f9882747971ffb64203b"

#define  kQQLoginAPPID @"222222"//@"1105859480"

#define  kFSAPPSHORTVERSION  [[NSBundle mainBundle] infoDictionary][@"CFBundleShortVersionString"]

#ifdef DEBUG
#define DebugLog(...)  printf("\n\t<%s line%d>\n%s\n", __FUNCTION__,__LINE__,[[NSString stringWithFormat:__VA_ARGS__] UTF8String])

#else
#define NSLog(...) {}
#define DebugLog(...) {}
#endif

#define kMainScreenHeight [UIScreen mainScreen].bounds.size.height
#define kMainScreenWidth   [UIScreen mainScreen].bounds.size.width

#define WeakSelf(type)  __weak typeof(type) weak##type = type;

#define StrongSelf(type)  __strong typeof(type) type = weak##type;

#define isNull(a) [a isKindOfClass:[NSNull class]]
#define kShortColor(r) [UIColor colorWithRed:r/256.0 green:r/256.0 blue:r/256.0 alpha:1]
#define RGBCOLOR(r,g,b) [UIColor colorWithRed:r/256.0 green:g/256.0 blue:b/256.0 alpha:1]
#define RGBACOLOR(r,g,b,a) [UIColor colorWithRed:r/256.0 green:g/256.0 blue:b/256.0 alpha:a]
#define ShortColor(c)   [UIColor colorWithRed:(c)/255.0f green:(c)/255.0f blue:(c)/255.0f alpha:(1)]

#define QLog_InfoP(...) {}
#define QLog_Event(...) {}
#define QLog_Info(...) {}

#endif /* FSPublic_h */
