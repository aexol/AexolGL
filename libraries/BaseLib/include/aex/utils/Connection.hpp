#ifndef CONNECTION_BOOST_AEXOL_GL_HEADER
#define CONNECTION_BOOST_AEXOL_GL_HEADER

#include <aex/AexolGL.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <boost/array.hpp>
#include <boost/asio.hpp> 
#include <boost/asio/ssl.hpp>
#include <memory>


namespace aex
{
  namespace con
  {

    enum REQUEST_TYPE
    {
      POST = 0,
      GET
      // HEAD,
      // PUT,
      // DELETE,
      // OPTIONS,
      // TRACE,
      // CONNECT,
      // PATCH
    };

    class Request
    {
    public:
      friend class Connection;
    public:
      Request(const std::string& url);
      ~Request();
      void setBody(const std::string& body);
      void setType(REQUEST_TYPE type);

      const std::string& getBody() const;
      REQUEST_TYPE getType() const;
      std::string getTypeStr() const;

      std::string getProtocolStr() const;
      std::string getHostStr() const;
      std::string getPathStr() const;

      std::string& operator[](const std::string& key);
      const std::string& operator[](const std::string& key) const;

    protected:
      std::string m_url;
      std::string m_body;
      std::map<std::string, std::string> m_header;
      std::string m_empty;
      REQUEST_TYPE m_type;
    };

    class Response
    {
    public:
      friend class Connection;
    protected:
      Response(const std::string& fullRespondStr);
    public:
      ~Response();

      const std::string& getHeader() const;
      const std::string& getBody() const;
      int statusCode() const;

    protected:
      std::string m_header;
      std::string m_body;
    };

    class Connection
    {
    public:
      Connection();
      ~Connection();

      void addCertPemFileContent(const std::string& fileContent);
      int syncRequest(const Request& request, const std::function<void( const Response& response )> callback);
      int asyncRequest(const Request& request, const std::function<void( const Response& response )> callback);

    protected:
      std::vector<std::string> m_pemCerts;
    };

    std::size_t completion(const boost::system::error_code& error, std::size_t bytes_transfered);
  }
}

#endif
