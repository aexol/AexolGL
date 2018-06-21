#include "aex/fb/FBGraphRequest.hpp"
#include <sstream>

#import <UIKit/UIKit.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>

namespace aex
{
    namespace fb
    {
        void FBGraphRequest::nativeRequestGET(
            const std::string& graphPath,
            const std::map<std::string,std::string>& parameters,
            const NativeCallback& natCB)
        {
            NativeCallback* tmpCB = new NativeCallback(natCB);
            NSMutableDictionary* mutDictParams = [[NSMutableDictionary alloc] init];

            for(const auto& elem : parameters)
            {
                [mutDictParams
                    setObject: [NSString stringWithUTF8String: elem.second.c_str()]
                    forKey: [NSString stringWithUTF8String: elem.first.c_str()]];
            }

            [[[FBSDKGraphRequest alloc]
                initWithGraphPath:[NSString stringWithUTF8String: graphPath.c_str()]
                parameters:mutDictParams]
                startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error)
            {
                NSLog(@"fetched user:%@", result);
                //NSLog(@"USER Email is :%@",[result valueForKey:@"email"]);

                int resultCode = FBResult::RESULT_SUCCEED;
                std::string jsonResult;
                std::string errorMsg;

                NSError *error2;
                NSData *jsonData = [NSJSONSerialization dataWithJSONObject:result
                                                                   options:NSJSONWritingPrettyPrinted // Pass 0 if you don't care about the readability of the generated string
                                                                     error:&error2];
                if (! jsonData) {
                    NSLog(@"Got an error: %@", error);
                    errorMsg = std::string([[error description] UTF8String]);
                } else {
                    NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
                    jsonResult = std::string([jsonString UTF8String]);
                    [jsonString release];
                }

                // std::map<std::string, std::string> resMap;
                // NSArray* keys = [result allKeys];
                // if(keys)
                // {
                //     for(int i = 0 ; i < [keys count] ; i++)
                //     {
                //         resMap[ std::string( [[keys[i] description] UTF8String] ) ] = std::string([[result valueForKey:keys[i]] UTF8String]);
                //     }
                // }

                if(error)
                {
                    resultCode = FBResult::RESULT_ERROR;
                    errorMsg = std::string( [[error description] UTF8String] );
                }

                if(tmpCB)
                {
                    (*tmpCB)(resultCode, jsonResult, errorMsg);
                    delete tmpCB;
                }
            }];

            [mutDictParams release];
        }

        void FBGraphRequest::nativeRequestDELETE(const std::string& graphPath, const std::function<void(bool success, const std::string& errMsg)>& callback)
        {
            std::function<void(bool success, const std::string& errMsg)>* tmpCb = new std::function<void(bool success, const std::string& errMsg)>(callback);
            [[[FBSDKGraphRequest alloc] initWithGraphPath:[NSString stringWithUTF8String: graphPath.c_str()]
            parameters:nil
            HTTPMethod:@"DELETE"]
            startWithCompletionHandler:^(FBSDKGraphRequestConnection *connection, id result, NSError *error) {
                      std::string errMsg;
                      bool resultVal = false;
                      if (!error) {
                          resultVal = true;
                      } else {
                        errMsg = std::string( [[error description] UTF8String] );
                      }

                      (*tmpCb)(resultVal, errMsg);
                      delete tmpCb;
            }];
        }
    }
}
