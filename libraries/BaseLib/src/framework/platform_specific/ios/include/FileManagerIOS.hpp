/**************************************************************
 *                                                            * 
 *      Filename:   FileManagerIOS.hpp                        * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2016 lut 22 13:44:39                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#ifndef FILEMANAGERIOS_HPP
#define FILEMANAGERIOS_HPP 

#include <aex/FileManagerBase.hpp>

namespace aex
{
    class FileManagerIMPL : public IFileManagerIMPL
    {
    public:
        using path = IFileManagerIMPL::path;
        using path_list = IFileManagerIMPL::path_list;
        FileManagerIMPL(void);
        ~FileManagerIMPL(void);

        bool init(void);
        path resourceDir() const;
        path writeDir() const;
        path alternativeDir() const;
        path temporaryDir() const;
        path userDir() const;
        path logDir() const;
        path_list  getDirFileNames(const char* dir) const;
        path_list  getDirFileNames(const path& dir) const;
        size_t       file_size(const char* fName) const;
        size_t       file_size(const path& fName) const;

    private:
        void* self;
    };
}

#endif /* FILEMANAGERIOS_HPP */
