#include "arg.h"


Args::Args( const std::string& tpl, const std::string& path_info)
{
    std::vector<std::string> p = split(path_info,'/');
    std::vector<std::string> t = split(tpl,'/');    
    for ( size_t i = 0; i < t.size(); i++ )
    {
        const std::string& s = t[i];
        if ( !s.empty() && i < p.size() )
        {
            if ( s[0] == '{' && s[s.size()-1] == '}' ) 
            {
                std::string key = s.substr(1,s.size()-2);
                std::string val = p[i];
                args_[key] = val;
            }            
        }
    }
}

std::string Args::get(const std::string& key)
{
    if ( args_.count(key) == 0 )
    {
        return "";
    }
    
    return args_[key];
}

