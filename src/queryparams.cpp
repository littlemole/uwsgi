#include "queryparams.h"
#include "urlencode.h"

namespace mol {
namespace whiskey {


QueryParams::QueryParams(const std::string& s)
{
    std::vector<std::string> v = split(s,'&');
    for ( size_t i = 0; i < v.size(); i++ )
    {
        if ( !v[i].empty() )
        {
            std::vector<std::string> p = split(v[i],'=');
            if ( p.size() > 1 )
            {
                params_[Urlencode::decode(p[0])] = Urlencode::decode(p[1]);
            }
        }
    }
}

std::string QueryParams::get(const std::string& key)
{
    if ( params_.count(key) == 0 )
    {
        return "";
    }
    
    return params_[key];
}

} // end namespace whiskey
} // end namespace mol

