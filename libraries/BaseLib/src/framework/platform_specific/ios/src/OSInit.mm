#include <aex/Logger.hpp>
#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <objc/objc.h>
#import <SDL.h>


namespace aex
{
    typedef IMP* IMPPointer;
    BOOL class_swizzleInstanceAndStore(Class clazz, SEL original, IMP replacement, IMPPointer store)
    {
        IMP imp = NULL;
        Method method = class_getInstanceMethod(clazz, original);

        if(method)
        {
            const char* type = method_getTypeEncoding(method);
            imp = class_replaceMethod(clazz, original, replacement, type);

            if(!imp)
            {
                imp = method_getImplementation(method);
            }
        }

        if(imp && store)
        {
            *store = imp;
        }

        return (imp != NULL);
    }
    BOOL class_swizzleClassMethodAndStore(Class clazz, SEL original, IMP replacement, IMPPointer store)
    {
        IMP imp = NULL;
        Method method = class_getClassMethod(clazz, original);

        if(method)
        {
            const char* type = method_getTypeEncoding(method);
            imp = class_replaceMethod(clazz, original, replacement, type);

            if(!imp)
            {
                imp = method_getImplementation(method);
            }
        }

        if(imp && store)
        {
            *store = imp;
        }

        return (imp != NULL);
    }
}
