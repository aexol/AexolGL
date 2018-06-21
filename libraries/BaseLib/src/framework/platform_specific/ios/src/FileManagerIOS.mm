//
//  FileManagerIOS.m
//  aexolGL
//
//  Created by Kamil Matysiewicz on 17.07.2013.
//  Copyright (c) 2013 Kamil Matysiewicz. All rights reserved.
//
#include <aex/platform_definitions.hpp>
#include <aex/FileManager.hpp>
#include <vector>
#include <boost/filesystem.hpp>

#import "../include/FileManagerIOS.hpp"
#import "../include/FileManagerIOS.hh"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

using cpp_path = aex::IFileManagerIMPL::path;

@implementation FileManagerIOS

aex::FileManagerIMPL::FileManagerIMPL(void)
    : self(nil)
{}

aex::FileManagerIMPL::~FileManagerIMPL(void)
{
    if(self != nil)
        [(id)self release];
}

bool aex::FileManagerIMPL::init(void)
{
    self = (void*)[[FileManagerIOS alloc] init];
    return self != nil;
}

cpp_path aex::FileManagerIMPL::resourceDir() const
{
    return cpp_path([(id)self resourceDir]);
}

cpp_path aex::FileManagerIMPL::writeDir() const
{
    return cpp_path([(id)self writeDir]);
}
cpp_path aex::FileManagerIMPL::alternativeDir() const
{
    return cpp_path([(id)self writeDir]);
}

cpp_path aex::FileManagerIMPL::temporaryDir() const
{
    return cpp_path([(id)self temporaryDir]);
}

size_t aex::FileManagerIMPL::file_size(const char* fName) const
{
    return [(id)self file_size:fName];
}

size_t aex::FileManagerIMPL::file_size(const path& fName) const
{
    return file_size(fName.native().c_str());
}

- (const char*) resourceDir
{
    const char* cfilename = [ [ [ NSBundle mainBundle ] resourcePath ] UTF8String];
    return cfilename ;
}
- (const char*) writeDir
{
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSString* filePath = [paths objectAtIndex:0];
    const char* cfilename = [ filePath UTF8String];
    BOOL is_dir;
    if(!([[NSFileManager defaultManager] fileExistsAtPath:filePath isDirectory:&is_dir]==YES))
    {
        NSError *err = nil;
        if([[NSFileManager defaultManager] createDirectoryAtPath:filePath withIntermediateDirectories:YES attributes:nil error:&err]==NO)
        {
            throw aex::FileManagerError{
                [[err localizedDescription] UTF8String]
            };
        }
    } else if(is_dir == NO) {
        throw aex::FileManagerError{
            std::string{cfilename}+" exists but is not a directory."
        };
    }
    return cfilename ;
}
- (const char*) temporaryDir
{
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString* filePath = [paths objectAtIndex:0];
    const char* cfilename = [ filePath UTF8String];
    return cfilename ;
}
- (size_t) file_size:(const char*) fName
{

    NSFileManager* fileManager = [NSFileManager defaultManager];

    NSDictionary* fileAttributes = [fileManager attributesOfItemAtPath:[NSString stringWithUTF8String:fName]
                                    error:NULL];

    if(fileAttributes != nil)
    {
        NSNumber* fileSize;

        if((fileSize = [fileAttributes objectForKey:NSFileSize]))
        {
            return [fileSize unsignedLongLongValue];
        }
    }

    return 0;
}

aex::FileManagerIMPL::path_list aex::FileManagerIMPL::getDirFileNames(const char* dir) const
{
    NSString* path = [[NSString alloc] initWithCString:dir encoding:NSASCIIStringEncoding];
    NSFileManager* fm = [NSFileManager defaultManager];
    NSArray* dirContents = [fm contentsOfDirectoryAtPath:path error:nil];
    aex::FileManagerIMPL::path_list fileList;
    fileList.reserve([dirContents count]);

    for(id object in dirContents)
    {
        fileList.push_back(cpp_path([object UTF8String]));
    }

    return fileList;
}

aex::FileManagerIMPL::path_list aex::FileManagerIMPL::getDirFileNames(const path& dir) const
{
    return getDirFileNames(dir.native().c_str());
}

cpp_path aex::FileManagerIMPL::logDir() const
{
    NSFileManager* fm = [NSFileManager defaultManager];
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString* libraryDirectory = [paths objectAtIndex:0];
    NSString* logDir = [libraryDirectory stringByAppendingString:@"/Logs/"];
    BOOL isDir;

    if([fm fileExistsAtPath:logDir isDirectory:&isDir] && isDir)
    {
        return cpp_path([logDir UTF8String]);
    }
    else
    {
        if(![fm fileExistsAtPath:logDir])
        {
            NSLog(@"Creating Log directory");
            NSError* error = nil;
            [fm createDirectoryAtPath:logDir withIntermediateDirectories:YES attributes:nil error:&error];
            NSLog(@"%@", [error description]);
            return cpp_path([logDir UTF8String]);
        }
        else
        {
            NSLog(@"Path Library/Logs exists and it is not a directory");
            return cpp_path("");
        }
    }
}

cpp_path aex::FileManagerIMPL::userDir() const
{
    return cpp_path([NSHomeDirectory() UTF8String]);
}

@end
