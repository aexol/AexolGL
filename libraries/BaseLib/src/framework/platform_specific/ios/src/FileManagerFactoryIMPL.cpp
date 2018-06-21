/**************************************************************
 *                                                            * 
 *      Filename:   FileManagerIMPL.cpp                       * 
 *                                                            * 
 *   Description:                                             * 
 *                                                            * 
 *       Version:   1.0                                       * 
 *       Created:   2016 lut 24 12:37:37                      * 
 *                                                            * 
 *        Author:   Kamil Matysiewicz                         * 
 *          Mail:   kamil@aexol.com                           * 
 *  Organization:   Aexol                                     * 
 *                                                            * 
 **************************************************************/
#include "../../include/FileManagerIMPL.hpp"
#include "../include/FileManagerIOS.hpp"

namespace aex
{
    IFileManager FileManagerIMPLFactory::getNewFileManagerImplementation() const
    {
        return IFileManager(new FileManagerIMPL);
    }
}
