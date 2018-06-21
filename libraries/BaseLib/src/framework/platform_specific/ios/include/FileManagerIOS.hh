//
//  FileManagerIOS.h
//  aexolGL
//
//  Created by Kamil Matysiewicz on 17.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//

#include <aex/platform_definitions.hpp>
#import "../../include/FileManagerIMPL.hpp"
#import <Foundation/Foundation.h>

// An Objective-C class that needs to be accessed from C++
@interface FileManagerIOS : NSObject
{
}

// The Objective-C member function you want to call from C++
- (const char*) resourceDir;
- (const char*) writeDir;
- (const char*) temporaryDir;
- (size_t)      file_size:(const char*) fName;


@end
//#endif
