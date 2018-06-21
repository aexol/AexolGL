#include "../../include/App/Connection.hpp"
#include <cstdlib>

namespace aex
{
  namespace con
  {
    static inline std::string& ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }
 
    static inline std::string& rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    static inline std::string& trim(std::string &s )
    {
        return ltrim(rtrim(s));
    }
    using boost::asio::ip::tcp;

    Request::Request(const std::string& url) :
    m_url(url),
    m_empty("")
    {
      // set default header params
      (*this)["Accept"] = "*/*";
      (*this)["Accept-Language"] = "en-us";
      (*this)["User-Agent"] = "aex_con_connection";
      (*this)["Connection"] = "close";
      (*this)["Content-Type"] = "application/json; charset=UTF-8";
    }
    Request::~Request()
    {
    }

    void Request::setBody(const std::string& body)
    {
      m_body = body;
    }

    void Request::setType(REQUEST_TYPE type)
    {
      m_type = type;
    }

    const std::string& Request::getBody() const
    {
      return m_body;
    }

    REQUEST_TYPE Request::getType() const
    {
      return m_type;
    }

    std::string Request::getTypeStr() const
    {
      switch(m_type)
      {
        case REQUEST_TYPE::POST:
          return "POST";
        case REQUEST_TYPE::GET:
          return "GET";
        default:
          return "GET";
      }
    }

    // "https://api.syncano.io/v1.1/instances/laundry-guide-free/endpoints/scripts/p/fc47e2b4160abe0424166d95b3615e42ebfe4bfc/potato-endpoint/";
    std::string Request::getProtocolStr() const
    {
      std::string url = m_url;
      trim(url);
      std::size_t firstIndex = 0;
      std::size_t lastIndex = url.find_first_of(':');
      std::string protocol = url.substr(0, lastIndex - firstIndex);
      return protocol;
    }

    std::string Request::getHostStr() const
    {
      std::string url = m_url;
      trim(url);
      std::size_t index = url.find("//");
      if (index!=std::string::npos)
      {
        url.erase(0, index+2);
        index = url.find("/");
        return url.substr(0, index);
      }

      return "";
    }

    std::string Request::getPathStr() const
    {
      std::string url = m_url;
      trim(url);
      std::size_t index = url.find("//");
      if (index!=std::string::npos)
      {
        url.erase(0, index+2);
        index = url.find("/");
        url.erase(0, index);
        return url;
      }

      return "";
    }

    std::string& Request::operator[](const std::string& key)
    {
      if ( m_header.find(key) == m_header.end() )
      {
        m_header[key] = "";
      }

      return m_header.at(key);
    }

    const std::string& Request::operator[](const std::string& key) const
    {
      if ( m_header.find(key) == m_header.end() )
      {
        return m_empty;
      }

      return m_header.at(key);
    }

    Response::Response(const std::string& fullRespondStr)
    {
      std::size_t index = fullRespondStr.find("\r\n\r\n"); // header end
      if(index != std::string::npos){
        m_header = fullRespondStr.substr(0, index+2);
        index += 2;
        m_body = fullRespondStr.substr(index, fullRespondStr.size() - index);
      }
    }

    Response::~Response()
    {
    }

    const std::string& Response::getHeader() const
    {
      return m_header;
    }

    const std::string& Response::getBody() const
    {
      return m_body;
    }

    int Response::statusCode() const
    {
      if(m_header.size() > 10){
        std::stringstream ss;
        ss << m_header;
        std::string param;
        ss >> param; // HTTP...
        ss >> param; // CODE (eg. 200 or 404)
        return atoi(param.c_str());
      }
      return -1;
    }

    Connection::Connection()
    {
    }

    Connection::~Connection()
    {
    }

    void Connection::addCertPemFileContent(const std::string& fileContent)
    {
      std::stringstream contentStream;
      contentStream << fileContent << std::endl;

      bool inCert = false;
      std::stringstream ss;
      std::string line;
      while(std::getline(contentStream, line))
      {
        std::size_t found = line.find("-----BEGIN CERTIFICATE-----");
        if (found!=std::string::npos)
        {
          inCert = true;
        }
        else
        {
          found = line.find("-----END CERTIFICATE-----");
          if (found!=std::string::npos)
          {
            inCert = false;
            ss << line << std::endl;
            m_pemCerts.push_back(ss.str());
            ss.str("");
            ss.clear();
            ss.str().clear();
          }
        }
        if(inCert)
          ss << line << std::endl;
      }
    }

    int Connection::syncRequest(const Request& request, const std::function<void( const Response& response )> callback)
    {
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(request.getHostStr(), request.getProtocolStr());
        boost::asio::ip::tcp::socket socket(io_service);
        //socket.set_option(tcp::no_delay(true));

        // Create a context that uses the default paths for finding CA certificates and provided pem certs
        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
        ctx.set_default_verify_paths();
        for(int i = 0 ; i < m_pemCerts.size() ; i++)
        {
          try{
            boost::asio::const_buffer cBuffer(m_pemCerts[i].c_str(), m_pemCerts[i].size());
            ctx.add_certificate_authority(cBuffer);
          } catch(std::exception& exc) {
              // same cert exist, do nothing
          }
        }
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket&> sslSocket(socket, ctx);

        boost::asio::connect(socket, resolver.resolve(query));

        if(request.getProtocolStr() == "https")
        {
          // Perform SSL handshake and verify the remote host's certificate.
          typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
          sslSocket.set_verify_mode(boost::asio::ssl::verify_peer);
          sslSocket.set_verify_callback(boost::asio::ssl::rfc2818_verification("api.syncano.io"));
          sslSocket.handshake(ssl_socket::client);
        }

        boost::asio::streambuf requestBuf;
        std::ostream requestStream(&requestBuf);

        requestStream << request.getTypeStr() << " " << request.getPathStr() << " HTTP/1.1\r\n";
        requestStream << "Host: " << request.getHostStr() << "\r\n";
        for(auto& pair : request.m_header)
        {
          requestStream << pair.first << ": " << pair.second << "\r\n";
        }
        requestStream << "Content-Length: " << request.getBody().length() << "\r\n\r\n";
        requestStream << request.getBody(); // body

        if(request.getProtocolStr() == "https")
          boost::asio::write(sslSocket, requestBuf);
        else
          boost::asio::write(socket, requestBuf);

        boost::asio::streambuf respond;
        boost::system::error_code ec;

        if(request.getProtocolStr() == "https")
          boost::asio::read(sslSocket, respond, completion, ec);
        else
          boost::asio::read(socket, respond, completion, ec);

        std::stringstream respondStream;
        respondStream << &respond << std::endl;
        Response respObj(respondStream.str());
        callback(respObj);

        return 0; // TODO how to check request id here ? not needed i guess
    }

    int Connection::asyncRequest(const Request& request, const std::function<void( const Response& response )> callback)
    {
      // TODO do
    }

    std::size_t completion(const boost::system::error_code& error, std::size_t bytes_transfered)
    {
        return !error;
    }

  }
}
