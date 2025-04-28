#include "../CryptoException.h"

namespace crypto
{
	namespace password
	{
		class PasswordException : public CryptoException
		{
		public:
			PasswordException(const std::string& message) : CryptoException(message) {}

		};
	} // namespace password
} // namespace crypto