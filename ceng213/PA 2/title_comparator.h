#ifndef _title_h__
#define _title_h__

#include "movie.h"
#include <cstring>


class TitleComparator
{
  public:
    // IMPLEMENT BELOW (inline)
    bool operator( ) (const Movie::SecondaryKey & key1, 
                      const Movie::SecondaryKey & key2) const
    { 
        std::string title1 = key1.getTitle();
        std::string dir1 = key1.getDirector();
        std::string title2 = key2.getTitle();
        std::string dir2 = key2.getDirector();
        

        
        for (auto &w: title1)
        {
            std::tolower(w);
        }
        
        for (auto &x: dir1)
        {
            std::tolower(x);
        }
        
        for (auto &y: title2)
        {
            std::tolower(y);
        }
        
        for (auto &z: dir2)
        {
            std::tolower(z);
        }        
        
        
        if (title1 < title2)
        {
            return true;
        }
        
        else if (title1 == title2)
        {
            if (dir1 < dir2)
            {
                return true;
            }
            
            else
            {
                return false;
            }
        }
        
        else if (title1 > title2)
        {
            return false;
        }
        
        return false;
        
    }

};

#endif
