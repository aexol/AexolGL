

#ifndef CELLURALAUTOMATA_HPP
#define	CELLURALAUTOMATA_HPP

AEX_PUBLIC_API int countNeighboursWrap(std::vector<std::vector<bool>>& image, int x, int y, int size)
{

    int ret = 0;
    int tempx = 0, tempy = 0,

        for(int dx = -1; dx <= 1; dx++)
    {
        for(int dy = -1; dy <= 1; dy++)
        {
            if(dx != 0 && dy != 0)
            {
                tempx = (x + dx) % size;
                tempy = (y + dy) % size;

                if(tempx < 0)
                {
                    tempx += size;
                }

                if(tempy < 0)
                {
                    tempy += size;
                }

                if(image[tempx][tempy])
                {
                    ret++;
                }
            }
        }
    }
}

AEX_PUBLIC_API void cellularAuto_B678_S345678(std::vector<std::vector<bool>>& image)
{
    int size = image.size();

    for(int x = 0; x < image.size(); x++)
    {
        for(int y = 0; y < image[0].size(); y++)
        {

            if(image[x][y] == true)   //if this pixel is currently dead...
            {
                switch(countNeighboursWrap(image, x, y))    //get number of neigbours
                {
                case 6:
                case 7:
                case 8: //BORN!
                    image[x][y] = false;
                    break;

                default:
                    break;
                }
            }
            else   //pixel must be alive...
            {
                if(countNeighboursWrap(image, x, y) < 3)    //DIED!
                {
                    for(int c = 0; c < 3; c++)
                    {
                        image[x][y] = true;
                    }
                }
            }
        }
    }
}

AEX_PUBLIC_API void cellularAutomata(std::vector<std::vector<bool>>& image, std::vector<int> born, std::vector<int> survive)
{
    for(int x = 0; x < image.width(); x++)
    {
        for(int y = 0; y < image.height(); y++)
        {
            if(x == 0 || x == image.width() - 1
                    || y == 0 || y == image.height() - 1)   //set-up border up
            {
                for(int c = 0; c < 3; c++)
                {
                    image(x, y, c) = 0;
                }

                continue;
            }

            if(image(x, y, 0) == 255)   //if this pixel is currently dead...
            {
                switch(countNeighboursWrap(image, x, y))    //get number of neigbours
                {
                case 6:
                case 7:
                case 8: //BORN!

                    image[x][y] = 0;
                    break;

                default:
                    break;
                }
            }
            else   //pixel must be alive...
            {
                if(countNeighboursWrap(image, x, y) < 3)    //DIED!
                {
                    for(int c = 0; c < 3; c++)
                    {
                        image(x, y, c) = 255;
                    }
                }
            }
        }
    }
}
#endif	/* CELLURALAUTOMATA_HPP */

