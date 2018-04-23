//
//  VACReportService.h
//  QQMSFContact
//
//  Created by Eric on 15/9/1.
//
//

#import <Foundation/Foundation.h>


@interface VACReportService : NSObject{
    NSTimer *_checkTimer;
}

+ (instancetype)sharedInstance;

/**
 上报ReportInfo到后台，一次可以上报多个
 */
- (void)sendReportInfos:(NSArray *)infos;

@end
