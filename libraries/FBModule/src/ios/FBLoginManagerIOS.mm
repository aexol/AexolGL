#include "aex/fb/FBLoginManager.hpp"
#include <aex/Logger.hpp>
#include <sstream>
#include <queue>

#import <UIKit/UIKit.h>
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>

namespace aex
{
    namespace fb
    {
        void FBLoginManager::nativeLogin(
            const NativeCallback& nativeCb)
        {
            NSMutableArray *permArray = [NSMutableArray array];
            for(int i = 0 ; i < m_permissions.size() ; i++)
            {
                std::string permStr = permissionToString(m_permissions[i]);
                [permArray addObject: [NSString stringWithUTF8String:permStr.c_str()]];
            }

            NativeCallback* natCallback = new NativeCallback(nativeCb); // will be deleted in cb invocation some lines below

            UIViewController* rootViewController = [[[UIApplication sharedApplication] keyWindow] rootViewController];
            FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
              [login  logInWithReadPermissions: permArray
                            fromViewController: rootViewController
                                       handler: ^(FBSDKLoginManagerLoginResult *result, NSError *error) {

                int resultCode;
                std::stringstream declinedPermsStream;
                std::stringstream grantedPermsStream;
                std::string errorMsg;

                if (error) {
                    resultCode = FBLoginResult::RESULT_ERROR;
                } else if (result.isCancelled) {
                  resultCode = FBLoginResult::RESULT_CANCELED;
                } else {
                  resultCode = FBLoginResult::RESULT_SUCCEED;
                }

                if(result.declinedPermissions != nil)
                {
                    LOG("declined count: ", [result.declinedPermissions count]);
                    NSArray* declinedPermsArray = [result.declinedPermissions allObjects];
                    if(declinedPermsArray != nil)
                    for(int i = 0 ; i < [declinedPermsArray count] ; i++ )
                    {
                        std::string declinedPerm([[declinedPermsArray objectAtIndex:i]  UTF8String]);
                        declinedPermsStream << declinedPerm;
                        if( (i+1) < [declinedPermsArray count])
                            declinedPermsStream << " ";
                    }
                }

                if(result.grantedPermissions != nil)
                {
                    LOG("granted count: ", [result.grantedPermissions count]);
                    NSArray* grantedPermsArray = [result.grantedPermissions allObjects];
                    if(grantedPermsArray != nil)
                    for(int i = 0 ; i < [grantedPermsArray count] ; i++ )
                    {
                        std::string grantedPerm([[grantedPermsArray objectAtIndex:i]  UTF8String]);
                        grantedPermsStream << grantedPerm;
                        if( (i+1) < [grantedPermsArray count])
                            grantedPermsStream << " ";
                    }
                }

                if(error)
                    errorMsg = std::string( [[error description] UTF8String] );

                // callback invokation
                if(natCallback)
                {
                    (*natCallback)(resultCode, declinedPermsStream.str(), grantedPermsStream.str(), errorMsg);
                    delete natCallback;
                }
              }];

            [permArray release];
        }

        bool FBLoginManager::nativeIsLogged() const
        {
            return [FBSDKAccessToken currentAccessToken];
        }
    }
}
