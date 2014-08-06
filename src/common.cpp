#include "common.h"

std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if(!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}


std::vector<std::string> glob(const std::string& f)
{
  DIR           *d;
  struct dirent *dir;
  
  std::vector<std::string> result;
  
  uwsgi_log("glob dir %s\n", f.c_str()); 
  d = opendir(f.c_str());
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
        if (dir->d_type == DT_REG)
        {   
            std::string n = std::string(dir->d_name);
            if ( n != "." && n != "..")
            {
                result.push_back(dir->d_name);
            }
        }
    }
    closedir(d);
  }
  return result;
}
