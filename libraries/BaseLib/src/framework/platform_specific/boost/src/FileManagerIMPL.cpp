#include <aex/FileManagerIMPL.hpp>
#include <aex/FileManager.hpp> //For exception 
#include <sys/stat.h>
#include <aex/Logger.hpp>
#include <SDL.h>

#include <unistd.h>
#include <dirent.h>
#include <vector>

namespace aex
{

    void FileManagerIMPL::init(void)
    {
        char* temp = (char*)malloc(1024 * sizeof(char));

        if(!(m_working_dir = getcwd(temp, 1024)))
        {
            LOG_ERR("Working directory error");
            //throw FileManagerInitFailed;
        }

        m_alternative_dir = temp;
#ifndef P_tmpdir
        char* tempDirPath = getenv("TMPDIR");

        if(!tempDirPath)
            m_temporary_dir = "/tmp"
                              m_temporary_dir = tempDirPath;

#else
        m_temporary_dir = P_tmpdir;
#endif

    }

    std::vector<aex::string> FileManagerIMPL::getDirFileNames(const char* dir)
    {
        std::vector<aex::string> ret;

        //LOG("The directory.");
        DIR* dp;
        struct dirent* ep;
        int  count = 0;
        dp = opendir(dir);

        if(dp != NULL)
        {
            while(ep = readdir(dp))
            {
                ret.push_back(ep->d_name);
                //LOG(ep->d_name);
                count++;
            }

            (void) closedir(dp);
        }
        else
        {
            LOG_ERR("Couldn't open the directory.");
        }

        //Refcount=count;
        return ret;
    }



    FileManagerIMPL::~FileManagerIMPL()
    {
        //free(m_write_dir);
        free(m_working_dir);
    }

}
