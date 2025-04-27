#pragma once

#include <iostream>
#include <iomanip>

#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/core_names.h>
#include <openssl/params.h>
#include <openssl/thread.h>
#include <openssl/kdf.h>   

#include "./jwt_token/JwtToken.h"

namespace crypto
{
	// Hash the password using ARGON2ID
	const std::string HashPassword(const std::string& password);

	// Generates a cryptographically secure random key (256 bits)
	const std::string GenerateRandomKey();
} // namespace crypto
