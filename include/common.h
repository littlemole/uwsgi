#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_COMMON_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_COMMON_DEF_GUARD_

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <boost/any.hpp>
#include "boost/regex.hpp"

extern "C" struct wsgi_request;

namespace mol {
namespace whiskey {

std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

std::vector<std::string> glob(const std::string& f);

typedef std::pair<std::string,std::string> header_t;
typedef std::vector<header_t> headers_t;

typedef std::pair<std::string,std::string> pathargument_t;
typedef std::vector<pathargument_t> patharguments_t;

template<class T>
class Buffer
{
public:

    Buffer(size_t s)
        : buf_(s,0)
    {}
    
    T* operator&()
    {
        return &(buf_[0]);
    }
    
    T& operator[](size_t i)
    {
        return buf_[i];
    }
    
    std::string toString()
    {
        return std::string( (char*)&(buf_[0]), buf_.size()*sizeof(T) );
    }
    
    
    std::string toString(size_t n)
    {
        return std::string( (char*)&(buf_[0]), n*sizeof(T) );
    }    
    
private:
    std::vector<T> buf_;

};

typedef Buffer<unsigned char> uchar_buf;
typedef Buffer<char> char_buf;

} // end namespace whiskey
} // end namespace mol

#endif

