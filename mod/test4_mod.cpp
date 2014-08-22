#include "handler.h"
#include "response.h"
#include "base64.h"
#include "cryptics.h"
#include "json.h"
#include "jwt.h"

using namespace mol::whiskey;


static const char* TEST_SECRET = "betterworldapps";
static const char* TEST_KEY    = "worldofgoodapps";

static auto start_handler = Get( "/cpp/start" )(
[] ( Request& req, Response& res ) {

    std::string token = req.attr<std::string>("token");
    JWT jwt = req.attr<JWT>("jwt");

    res << "JWT: " << token << "\r\n";
    res << "header: " << JSON::stringify(jwt.header()) << "<br>\r\n";
    res << "claim: " << JSON::stringify(jwt.claim()) << "<br>\r\n";
    res << "signature: " << jwt.signature() << "<br>\r\n";  
    res << "<a href='http://localhost:3032/cpp/start'>start</a>\r\n";

    return res.contentType("text/html").ok();
});

static auto jwt_interceptor = interceptor( "GET", "/cpp/.*", 
[] (Request& req, Response& res) {

    static SymCrypt encrypt(EVP_bf_cbc(), TEST_KEY);
    static SymCrypt decrypt(EVP_bf_cbc(), TEST_KEY, encrypt.iv());

    std::string token = req.queryParams().get("jwt");
    if ( token.empty() ) {

        Cookies& cookies = req.cookies();
        if ( cookies.exists("cpp-cookie") ) {
            const Cookie& cookie = cookies.get("cpp-cookie");
            token = decrypt.decrypt(fromHex(cookie.value()));
        }
    }

    if ( token.empty() ) {
        return res.done().redirect("https://ssl-id1.de/oha7.org/auth/?app=cpp");
    }
    
    mol::whiskey::JWT jwt;
    bool verified = jwt.verify(token,TEST_SECRET);
                
    if ( !verified ) {
        return res.done().redirect("https://ssl-id1.de/oha7.org/auth/?app=cpp");
    }

    Cookie cookie("cpp-cookie",toHex(encrypt.encrypt(token)));
    cookie.maxAge(10);
    res.cookie(cookie);
    
    req.attr("jwt",jwt);
    req.attr("token",token);
    return 0;
});



