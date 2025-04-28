//TO DO add crypto exceptions
#pragma once

#include <stdexcept>
#include <openssl/err.h>

namespace crypto
{
	class CryptoException : public std::runtime_error
	{
	public:
		CryptoException(const std::string& message) : std::runtime_error(FormatErrorMessage(message)) {}
	private:
		static std::string FormatErrorMessage(const std::string& message)
		{
			char err[256];
			unsigned long error_code = ERR_get_error();
			if (error_code == 0) 
			{
				return message + ": No OpenSSL error";
			}
			ERR_error_string_n(ERR_get_error(), err, sizeof(err));
			return message + ": " + err;
		}
	};
} // namespace crypto