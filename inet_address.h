#ifndef __COMMON_INET_ADDRESS_H__
#define __COMMON_INET_ADDRESS_H__

#include <sys/socket.h>
#include <memory>

namespace Network
{
  
  class InetAddress;
  typedef std::unique_ptr<InetAddress> InetAddressPtr;
  class InetAddress    
  {
  
  public:
    virtual ~InetAddress();
	sockaddr const* GetAddr() const;
	int GetSize() const;
  protected:
	
    InetAddress(sockaddr const *addr, int size);
    
  private:
    sockaddr *Addr;
    int Size;
    
    void Init(sockaddr const *addr, int size);
  };
  
  // typedef std::unique_ptr<InetAddress> InetAddressPtr;

}

#endif  // !__COMMON_INET_ADDRESS_H__
