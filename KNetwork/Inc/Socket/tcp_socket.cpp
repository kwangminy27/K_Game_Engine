#include "KNetwork.h"
#include "tcp_socket.h"

#include "socket_address.h"

void K::TCPSocket::Bind(SocketAddress const& _address)
{
	if (SOCKET_ERROR == bind(socket_, &_address.sockaddr_, _address.GetSize()))
		throw std::exception{ "TCPSocket::Bind" };
}

void K::TCPSocket::Listen(int _backlog)
{
	if (SOCKET_ERROR == listen(socket_, _backlog))
		throw std::exception{ "TCPSocket::Listen" };
}

std::shared_ptr<K::TCPSocket> K::TCPSocket::Accept(SocketAddress& _from)
{
	int size = _from.GetSize();

	SOCKET s = accept(socket_, &_from.sockaddr_, &size);

	if (INVALID_SOCKET == s)
		throw std::exception{ "TCPSocket::Accept" };

	return std::shared_ptr<TCPSocket>{ new TCPSocket{ s }, [](TCPSocket* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::TCPSocket::Connect(SocketAddress const& _to)
{
	if (SOCKET_ERROR == connect(socket_, &_to.sockaddr_, _to.GetSize()))
		throw std::exception{ "TCPSocket::Connect" };
}

int K::TCPSocket::Send(void const* _buffer, int _size)
{
	auto result = send(socket_, static_cast<char const*>(_buffer), _size, NULL);

	if (SOCKET_ERROR == result)
		throw std::exception{ "TCPSocket::Send" };

	return result;
}

int K::TCPSocket::Receive(void* _buffer, int _size)
{
	auto result = recv(socket_, static_cast<char*>(_buffer), _size, NULL);

	if (SOCKET_ERROR == result)
		throw std::exception{ "TCPSocket::Receive" };

	return result;
}

void K::TCPSocket::ShutDown()
{
	if (SOCKET_ERROR == shutdown(socket_, SD_BOTH))
		throw std::exception{ "TCPSocket::ShutDown" };
}

K::TCPSocket::TCPSocket(SOCKET _socket)
{
	socket_ = _socket;
}

K::TCPSocket::TCPSocket(TCPSocket&& _other) noexcept
{
	*this = std::move(_other);
}

void K::TCPSocket::_Finalize()
{
	if (SOCKET_ERROR == closesocket(socket_))
		std::exception{ "TCPSocket::Finalize" };
}
