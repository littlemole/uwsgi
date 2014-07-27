#ifndef _MOL_DEF_GUARD_DEFINE_MOD_HTTP_COMMON_DEF_GUARD_
#define _MOL_DEF_GUARD_DEFINE_MOD_HTTP_COMMON_DEF_GUARD_
#include <dlfcn.h>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <boost/any.hpp>
#include "uwsgi.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif

