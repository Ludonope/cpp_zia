#pragma once

#include <string_view>
#include <string>
#include <sstream>
#include "Lexer.hpp"

namespace zia::http
{
	class Uri
	{
	public:
		Uri() = default;
		~Uri() noexcept = default;

		Uri(std::string_view uri);

		Uri(Uri const &) = default;
		Uri &operator=(Uri const &) = default;

		Uri(Uri &&) noexcept = default;
		Uri &operator=(Uri &&) noexcept = default;

		Uri &operator=(std::string_view uri);

		void set(std::string_view uri);

		std::string toString() const;

		auto const &scheme() const noexcept		{ return m_scheme; }
		auto const &user() const noexcept		{ return m_user; }
		auto const &password() const noexcept	{ return m_password; }
		auto const &host() const noexcept		{ return m_host; }
		auto const &port() const noexcept		{ return m_port; }
		auto const &path() const noexcept		{ return m_path; }
		auto const &query() const noexcept		{ return m_query; }
		auto const &fragment() const noexcept	{ return m_fragment; }

		void setScheme(std::string_view scheme)		{ m_scheme = scheme; }
		void setUser(std::string_view user)			{ m_user = user; }
		void setPassword(std::string_view password) { m_password = password; }
		void setHost(std::string_view host)			{ m_host = host; }
		void setPort(std::uint16_t port)			{ m_port = port; }
		void setPath(std::string_view path)			{ m_path = path; }
		void setQuery(std::string_view query)		{ m_query = query; }
		void setFragment(std::string_view fragment) { m_fragment = fragment; }

		static std::string decode(std::string_view s);

	private:
		void hostGroupToString(std::ostringstream &ss) const;
		void parseHostGroup(UriLexer &lex);
		void normalizeAll();
		static void normalize(std::string &e, bool percentOnly = false);
		void clear();
		static void decodeValue(std::string &s);

		std::string m_scheme = "";
		std::string m_user = "";
		std::string m_password = "";
		std::string m_host = "";
		std::uint16_t m_port = 0;
		std::string m_path = "";
		std::string m_query = "";
		std::string m_fragment = "";
	};
}