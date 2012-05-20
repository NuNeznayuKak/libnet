#include "TcpSocket.hpp"
#include <errno.h>

using namespace Net;

TcpSocket::TcpSocket(int fd /* = -1 */) :
  _wBuf(1024, 12048),
  _rBuf(1024, 12048),
  _fd(fd)
{

}

TcpSocket::~TcpSocket()
{
  close (_fd);
}

int TcpSocket::fd() const
{
  return _fd;
}

bool TcpSocket::write(const char *source, int len)
{
  if (!_wBuf.write(source, len))
    return false;
  return true;
}

int TcpSocket::flush()
{
  char                  buffer[512];
  int			r;

  r = _wBuf.rAvailable();
  r = r > 512 ? 512 : r;
  if (r)
    {
      _wBuf.read(&buffer[0], r);
      if (::write(_fd, (char *) buffer, r) == -1)
        {
          if (errno != EWOULDBLOCK &&
              errno != EAGAIN)
            return (-1);
          _wBuf.readRollback();
        }
    }
  return (0);
}