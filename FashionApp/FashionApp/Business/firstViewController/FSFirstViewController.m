//
//  ViewController.m
//  FashionApp
//
//  Created by 1 on 2018/4/9.
//  Copyright © 2018年 1. All rights reserved.
//

#import "FSFirstViewController.h"
#import "FSCameraViewController.h"

@interface FSFirstViewController ()
{
    TestNetwork *t;
}
@end

@implementation FSFirstViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    self.view.backgroundColor = [UIColor redColor];
    
    UIButton *bt = [[UIButton alloc] initWithFrame:CGRectMake(100, 100, 100, 120)];
    bt.backgroundColor = [UIColor greenColor];
    [bt addTarget:self action:@selector(testbt) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:bt];
    
    UIButton *bt2 = [[UIButton alloc] initWithFrame:CGRectMake(100, 200, 100, 120)];
    bt2.backgroundColor = [UIColor yellowColor];
    [bt2 addTarget:self action:@selector(testCamera) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:bt2];
}

- (void)testbt
{
    DDLogDebug(@"ddlogdebug_111111");
    if(!t)
    {
        t = [[TestNetwork alloc] init];
    }
    [t sendMsg];
//    DDLogInfo(@"ddloginfo_111111");
//    DDLogVerbose(@"ddlogverbose_1111111");
//    DDLogError(@"ddlogerror_1111111");
}

- (void)testCamera{
    FSCameraViewController *vc = [[FSCameraViewController alloc] init];
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end

@implementation TestNetwork

//- (NSString *)serviceName
//{
//    return @"VAC.KiddyTestServer.KiddyTestObj";
//}
//
//- (NSString *)funcName
//{
//    return @"json";
//}
//
//- (NSString *)requestJceClass
//{
//    return @"QZJVacJsonReq";
//}
//
//- (NSString *)responseJceClass
//{
//    return @"QZJVacJsonReq";
//}

- (void)sendMsg
{

    [self upLoadVideoPath:@"/Users/a1/Library/Developer/CoreSimulator/Devices/11EC494F-91D6-4889-9F4D-58768B5BEBCD/data/Containers/Data/Application/131C1B54-3E3F-458D-98A1-3B4A64C87724/tmp/1234.mp4" result:^(NSDictionary *result, int retCode) {
        
    } procee:^(NSInteger bytesUpload, NSInteger bytesTotal) {
        
    }];
    
//    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
////    [dict setObject:@"VAC.KiddyTestServer.KiddyTestObj" forKey:@"servantName"];
////    [dict setObject:@"json" forKey:@"funcName"];
////    [dict setObject:@"QZJVacJsonReq" forKey:@"requestJceClass"];
////    [dict setObject:@"QZJVacJsonReq" forKey:@"responseJceClass"];
//    [dict setObject:@[@"121",@"23344"] forKey:@"input_list"];
//    [dict setObject:@"1213" forKey:@"input"];
//    [dict setObject:@{@"1111":@"22222"} forKey:@"input_map"];
//
//        NSMutableDictionary *mdict = [self packetReqParamSerName:@"VAC.KiddyTestServer.KiddyTestObj" funcName:@"json" reqJceName:@"QZJVacJsonReq" resposeJceName:@"QZJVacJsonReq" busDict:dict];
//
//    [self sendRequestDict:mdict completion:^(NSDictionary *busDict, NSError *bizError) {
//
//    }];
//    NSData *data = [@"hello" dataUsingEncoding:NSUTF8StringEncoding];
//    [self sendRequestData:data cmd:@"srf_proxy" completion:^(NSString *cmd, NSData *data, NSError *bizError) {
//
//    }];
    
//    [FSServiceRoute  syncCallService:@"FSLoginService" func:@"qqlogin" withParam:nil];
}
@end
