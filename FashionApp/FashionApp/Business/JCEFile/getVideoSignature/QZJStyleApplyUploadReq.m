// **********************************************************************
// This file was generated by a TAF parser!
// TAF version 3.0.0 by WSRD Tencent.
// Generated from `/Users/a1/newApp/FeedOperation.jce'
// **********************************************************************

#import "QZJStyleApplyUploadReq.h"

@implementation QZJStyleApplyUploadReq

- (id)init
{
    if (self = [super init]) {
        JV2_PROP(feed_info) = [QZJStyleFeedInfo object];
    }
    return self;
}

+ (NSString*)jceType
{
    return @"Style.ApplyUploadReq";
}

@end