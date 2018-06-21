#include <aex/FileManagerIMPL.hpp>
#include <aex/Logger.hpp>
#include <vector>
namespace aex
{

    void FileManagerIMPL::init(void)
    {
        char* temp = SDL_GetBasePath();

        if(temp)
        {
            LOG_ERR("Working directory error");

        }
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
        //free(m_working_dir);
    }

}
