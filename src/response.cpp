#include "response.h"
#include <sstream>

Response::Response(wsgi_request* r)
    : r_(r), done_(false)
{}

Response& Response::contentType(const std::string& val)
{
    headers_.push_back( header_t("content-type",val) );
    return *this;
}       

Response& Response::header(const std::string& key, const std::string& val)
{
    headers_.push_back( header_t(key,val) );
    return *this;
}

Response& Response::body(const std::string& b)
{
    body_.append(b);
    return *this;
}

Response& Response::cookie(const Cookie& c)
{
    cookies_.push_back(c);
    return *this;
}

Response& Response::flush()
{
    if ( !status_.empty() )
    {
        uwsgi_response_prepare_headers(
            r_, 
            (char*)status_.c_str(), status_.size()
        );
        status_ = "";
    }
    
    for ( size_t i = 0; i < headers_.size(); i++)
    {
        const std::string& key = headers_[i].first;
        const std::string& val = headers_[i].second;

        uwsgi_response_add_header(
            r_, 
            (char*)key.c_str(), key.size(), 
            (char*)val.c_str(), val.size()
        );    
        headers_.clear();
    }

    if ( !cookies_.empty())
    {
        for ( size_t i = 0; i < cookies_.size(); i++ )
        {    
            std::string key = "Set-Cookie";
            std::string val = cookies_[i].str();
            uwsgi_response_add_header(
                r_, 
                (char*)key.c_str(), key.size(), 
                (char*)val.c_str(), val.size()
            );        
        }        
        cookies_.clear(); 
    }    
    
    if ( !body_.empty() )
    {
        uwsgi_response_write_body_do(
            r_, 
            (char*)body_.c_str(), body_.size()
        );        
        body_ = "";
    }    
    return *this;
}

Response& Response::done()
{
    done_ = true;
    return *this;
}

Response& Response::reset()
{
    done_ = false;
    body_ = "";
    status_ = "";
    headers_.clear();
    return *this;
}

int Response::status(const std::string& s)
{
    status_ = s;
    return UWSGI_OK;
}

int Response::ok()
{
    return status("200 OK");
}

int Response::error()
{
    return status("500 internal error");
}

int Response::bad_request()
{
    return status("400 bad request");
}

int Response::unauthorized()
{
    return status("401 unauthorized");
}

int Response::forbidden()
{
    return status("403 forbidden");
}

int Response::not_found()
{
    return status("404 not found");
}

int Response::redirect(const std::string& s, int code)
{
    header("location",s);
    std::ostringstream oss;
    oss << code;
    
    return status(oss.str());
}


std::string Response::status()
{
    return status_;
}

std::string Response::body()
{
    return body_;
}

headers_t Response::headers()
{
    return headers_;
}

bool Response::isDone()
{
    return done_;
}

