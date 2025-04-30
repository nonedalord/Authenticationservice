#pragma once

#include <iostream>
#include <iomanip>

#include <openssl/sha.h>
#include <openssl/rand.h>

#include "./jwt_token/JwtToken.h"
#include "./password_argon2id/PasswordArgon2id.h"

namespace crypto
{
	enum bytes
	{
		sha256 = 32,
		salt_size = 16
	};
	// Generates a cryptographically secure random key
	const std::string GenerateRandomKey(const size_t bytes);
} // namespace crypto