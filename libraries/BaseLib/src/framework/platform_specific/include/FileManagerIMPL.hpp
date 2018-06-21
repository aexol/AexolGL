/** @file FileManagerIMPL.hpp
 *  @brief Definition of FileManagerIMPL.hpp
 *
 *  @author Kamil Matysiewicz
 *  @bug No known bugs.
 */

#ifndef __FILEMANAGERIMPL_C_INTERFACE_H__
#define __FILEMANAGERIMPL_C_INTERFACE_H__

#include <aex/FileManagerBase.hpp>
#include <string>
#include <vector>

namespace aex
{
    class FileManagerIMPLFactory : public IFileManagerFactory
    {
        public:
            IFileManager getNewFileManagerImplementation() const;
    };
}

#endif
