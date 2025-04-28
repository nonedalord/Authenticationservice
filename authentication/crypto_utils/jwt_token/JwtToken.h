#pragma once

#include <iostream>

#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/json.hpp>
#include <openssl/hmac.h>

namespace crypto
{
	namespace jwt_token
	{
		namespace json = boost::json;

		class JwtToken
		{
		public:
			JwtToken() = default;
			~JwtToken() = default;
			static const std::string GenerateJWT(const std::string& secret, const json::object& header, const json::object& payload);
		private:
			static std::string EncodeBase64Url(const std::string& data);
		};
	} // namespace jwt_token
} // namespace crypto