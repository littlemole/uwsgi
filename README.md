uwsgi
=====

fun with wsgi modules in raw c++11 running on the uwsgi stack.

see example http modules in ./mod for usage.

see tests in /t for available apis.

features:

- HTTP handlers with access to HTTP Request and Response
- path mathcing incl path arguments
- query params support
- urlencode, base64
- http client for upstream requests on the server (based on libcurl)
- http interceptors for cross cutting concerns (aspect or filter)
- crypto helpers (openssl):
    - md5,sha1,sha256
    - symmetric encryption, ie blowfish,r4
    - HMAC
    - pk based message signing
    - pk based envelopes
    - dh key exchange
    

