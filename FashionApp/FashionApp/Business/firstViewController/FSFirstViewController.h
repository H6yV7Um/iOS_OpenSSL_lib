//
//  ViewController.h
//  FashionApp
//
//  Created by 1 on 2018/4/9.
//  Copyright © 2018年 1. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "FSUploadNetWorkService.h"

@interface FSFirstViewController : UIViewController


@end

@interface TestNetwork :FSUploadNetWorkService
- (void)sendMsg;
@end