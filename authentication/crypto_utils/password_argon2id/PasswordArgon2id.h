#pragma once

#include <iostream>
#include <iomanip>
#include <memory>

#include <openssl/core_names.h>
#include <openssl/params.h>
#include <openssl/thread.h>
#include <openssl/kdf.h>
#include <openssl/hmac.h>
#include <openssl/err.h>

#include "../Crypto.h"
#include "PasswordException.h"

namespace crypto
{
	namespace password
	{
		static const uint32_t default_threads = 2;
		static const uint32_t default_lanes = 2;
		static const uint32_t default_memcost = 65536;
		static const std::string algoritm_name = "ARGON2ID";

		class PasswordArgon2id
		{
		public:
			PasswordArgon2id();
			explicit PasswordArgon2id(uint32_t threads);
			PasswordArgon2id(uint32_t threads, uint32_t memcost, uint32_t lanes);
			~PasswordArgon2id();

			std::string HashPassword(const std::string& password);
		private:
			uint32_t m_memcost;
			uint32_t m_lanes;
			uint32_t m_threads;
			size_t m_outlen{ 128 };

			std::unique_ptr<OSSL_LIB_CTX, decltype(&OSSL_LIB_CTX_free)> m_ctx;
			std::unique_ptr<EVP_KDF, decltype(&EVP_KDF_free)> m_kdf;
		};
	} // namespace password
} // namespace crypto