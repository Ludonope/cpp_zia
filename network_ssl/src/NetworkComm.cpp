#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include "NetworkComm.hpp"

namespace zia::network
{
	std::uint32_t	NetworkComm::m_nbSockets = 0;

#if defined(_WIN32)
	bool		NetworkComm::m_WSAInited = false;
#endif

	NetworkComm::NetworkComm(zia::api::Net::Callback &&cb,
				std::uint16_t const port,
				std::string const &cert,
				std::string const &key) :
		m_respCallback(std::move(cb)),
		m_port(port), m_cert(cert), m_key(key),
		m_sslctx(SSL_CTX_new(TLS_server_method())),
		m_clients{}
	{
#if defined(_WIN32)
		initWindows();
#endif
		if (!m_nbSockets)
		{
			initSSL();
		}
		m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
		if (m_socket == -1)
		{
			throw std::runtime_error("Cannot create socket");
		}
		m_sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		m_sockAddr.sin_family = AF_INET;
		m_sockAddr.sin_port = htons(m_port);
		if (::bind(m_socket,
			reinterpret_cast<sockaddr_t *>(&m_sockAddr),
			sizeof(m_sockAddr)) == -1)
		{
			throw std::runtime_error("Cannot bind socket");
		}
		if (::listen(m_socket, 1) == -1)
		{
			throw std::runtime_error("Cannot listen on socket");
		}
		startSSL();
		++m_nbSockets;
		std::cout << "Socket: " << m_socket << std::endl;
	}

	void NetworkComm::startSSL()
	{
		if (!m_sslctx)
		{
			throw std::runtime_error("SSL Context not set");
		}
		SSL_CTX_set_options(m_sslctx, SSL_OP_SINGLE_DH_USE);
		auto const useCert =
				SSL_CTX_use_certificate_file(m_sslctx, m_cert.c_str(), SSL_FILETYPE_PEM);
		if (useCert != 1)
		{
			throw std::runtime_error("Cannot load certificate \"" + m_cert + "\"");
		}
		auto const useKey =
				SSL_CTX_use_PrivateKey_file(m_sslctx, m_key.c_str(), SSL_FILETYPE_PEM);
		if (useKey != 1)
		{
			throw std::runtime_error("Cannot load key \"" + m_key + "\"");
		}
		if (!SSL_CTX_check_private_key(m_sslctx))
		{
			throw std::runtime_error(
				"Private key does not match the certificate public key");
		}
		m_socketSSL = SSL_new(m_sslctx);
		if (!m_socketSSL)
		{
			throw std::runtime_error("Cannot create secure socket");
		}
		SSL_set_fd(m_socketSSL, m_socket);
	}

	NetworkComm::~NetworkComm()
	{
		if (m_socketSSL != nullptr)
		{
			--m_nbSockets;
			std::cout << "Closing socket..." << m_socket << std::endl;
			SSL_shutdown(m_socketSSL);
			SSL_free(m_socketSSL);
			m_socketSSL = nullptr;
			m_socket = -1;
			if (!m_nbSockets)
			{
				deinitSSL();
			}
		}
#if defined(_WIN32)
		deinitWindows();
#endif
	}

	NetworkComm::NetworkComm(NetworkComm &&other) :
		m_respCallback(std::move(other.m_respCallback)),
		m_port(std::move(other.m_port)),
		m_cert(std::move(other.m_cert)),
		m_key(std::move(other.m_key)),
		m_sslctx(std::move(other.m_sslctx)),
		m_socketSSL(std::move(other.m_socketSSL)),
		m_socket(std::move(other.m_socket)),
		m_sockAddr(std::move(other.m_sockAddr))
	{
		other.m_socketSSL = nullptr;
		other.m_socket = -1;
	}

	NetworkComm &NetworkComm::operator=(NetworkComm &&other)
	{
		if (&other != this)
		{
			m_respCallback = std::move(other.m_respCallback);
			m_port = std::move(other.m_port);
			m_cert = std::move(other.m_cert);
			m_key = std::move(other.m_key);
			m_sslctx = std::move(other.m_sslctx);
			m_socketSSL = std::move(other.m_socketSSL);
			m_socket = std::move(other.m_socket);
			m_sockAddr = std::move(other.m_sockAddr);
			other.m_socketSSL = nullptr;
			other.m_socket = -1;
		}
		return *this;
	}

	std::int32_t NetworkComm::multiplex(fd_set &readfds,
				fd_set &writefds,
				fd_set &exceptfds) const noexcept
	{
		auto	rc = -1;

		do
		{
			std::int32_t	fd_max = m_socket;

			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			FD_ZERO(&exceptfds);
			FD_SET(m_socket, &readfds);

			for (auto const &cli : m_clients)
			{
				auto const sock = cli->getSocket();
				if (cli->canWrite())
				{
					FD_SET(sock, &writefds);
				}
				FD_SET(sock, &readfds);
				FD_SET(sock, &exceptfds);
				if (sock > fd_max)
				{
					fd_max = sock;
				}
			}
			rc = select(fd_max + 1, &readfds, &writefds,
				&exceptfds, nullptr);
		} while (rc == -1 && errno == EINTR);
		return rc;
	}

	void NetworkComm::handle(fd_set const &readfds,
			fd_set const &writefds,
			fd_set const &exceptfds)
	{
		if (FD_ISSET(m_socket, &readfds))
		{
			try
			{
				acceptClient();
			}
			catch (std::exception const &e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
		for (auto ite = std::begin(m_clients); ite != std::end(m_clients);)
		{
			auto clientStatus = Client::Status::OK;
			auto const sock = (*ite)->getSocket();

			if (FD_ISSET(sock, &readfds))
			{
				clientStatus = (*ite)->handleInput();
				if (clientStatus != Client::Status::ERR &&
					(*ite)->hasRequest()) {
					auto const &raw = (*ite)->getRaw();
					auto const &infos = (*ite)->getInfos();

					// TODO: Call in async ?
					m_respCallback(raw, infos);
				}
			}
			if (FD_ISSET(sock, &writefds))
			{
				clientStatus = (*ite)->handleOutput();
			}
			if (FD_ISSET(sock, &exceptfds))
			{
				clientStatus = Client::Status::ERR;
			}

			if (clientStatus == Client::Status::DONE ||
				clientStatus == Client::Status::ERR ||
				(*ite)->hasTimedOut())
			{
				ite = m_clients.erase(ite);
			}
			else
			{
				++ite;
			}
		}
	}

	void NetworkComm::send(api::ImplSocket *sock, api::Net::Raw &&data) noexcept
	{
		auto ite = std::find_if(std::begin(m_clients), std::end(m_clients),
			[&](auto const &o) {
			return (*o).getInfos().sock == sock;
		});
		if (ite != std::end(m_clients))
		{
			(*ite)->send(std::move(data));
		}
	}

	void NetworkComm::acceptClient()
	{
		sockaddr_in_t	sockAddr = {};
		sock_t		rc = 0;

		do
		{
			socklen_t len = sizeof(sockAddr); 
			rc = ::accept(m_socket,
				reinterpret_cast<sockaddr_t *>(&sockAddr), &len);
		} while (rc == -1 && errno == EINTR);
		if (rc > 0)
		{
			m_clients.push_back(std::make_unique<Client>(rc, m_sslctx, sockAddr));
		}
	}

	void NetworkComm::initSSL() noexcept
	{
		SSL_load_error_strings();
		SSL_library_init();
		OpenSSL_add_all_algorithms();
		SSLeay_add_ssl_algorithms();
	}

	void NetworkComm::deinitSSL() noexcept
	{
		ERR_free_strings();
		EVP_cleanup();
	}

#if defined _WIN32
	void NetworkComm::initWindows() const
	{
		if (!m_nbSockets && !m_WSAInited)
		{
			WSADATA wsa;

			assert(m_WSAInited == false);
			if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
			{
				throw std::runtime_error("Cannot load network DLL");
			}
			m_WSAInited = true;
		}
	}

	void NetworkComm::deinitWindows() const noexcept
	{
		if (m_socketSSL != nullptr)
		{
			if (!m_nbSockets)
			{
				assert(m_WSAInited == true);
				WSACleanup();
				m_WSAInited = false;
			}
		}
	}
#endif
}
