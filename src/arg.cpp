#include "request.h"



Args::Args( const patharguments_t& args)
    : args_(args)
{}

std::string Args::get(const std::string& key)
{
    std::vector<std::string> entries = keys();
    for ( size_t i = 0; i < entries.size(); i++ ) 
    {
        if ( entries[i] == key )
        {
            return args_[i].second;
        }
    }
    return "";
}

std::vector<std::string> Args::keys()
{
    std::vector<std::string> v;
    for ( size_t i = 0; i < args_.size(); i++ ) 
    {
        v.push_back( args_[i].first );
    }
    return v;
}    


