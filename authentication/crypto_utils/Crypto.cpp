#include "Crypto.h"

const std::string crypto::HashPassword(const std::string& password)
{
    EVP_KDF* kdf = NULL;
    EVP_KDF_CTX* kctx = NULL;
    OSSL_PARAM params[6];
    OSSL_PARAM *p = params;

    uint32_t lanes = 2;
    uint32_t threads = 2;
    uint32_t memcost = 65536;
    char pwd[] = "1234567890";
    char salt[] = "saltsalt";

    size_t outlen = 128;
    unsigned char result[outlen];

    if (OSSL_set_max_threads(NULL, threads) != 1)
    {
        EVP_KDF_free(kdf);
        EVP_KDF_CTX_free(kctx);
        OSSL_set_max_threads(NULL, 0);
        return std::string();
    }

    p = params;
    *p++ = OSSL_PARAM_construct_uint32(OSSL_KDF_PARAM_THREADS, &threads);
    *p++ = OSSL_PARAM_construct_uint32(OSSL_KDF_PARAM_ARGON2_LANES, &lanes);
    *p++ = OSSL_PARAM_construct_uint32(OSSL_KDF_PARAM_ARGON2_MEMCOST, &memcost);
    *p++ = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT, salt, strlen((const char*)salt));
    *p++ = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_PASSWORD, pwd, strlen((const char*)pwd));
    *p++ = OSSL_PARAM_construct_end();

    if ((kdf = EVP_KDF_fetch(NULL, "ARGON2ID", NULL)) == NULL)
    {
        EVP_KDF_free(kdf);
        EVP_KDF_CTX_free(kctx);
        OSSL_set_max_threads(NULL, 0);
        return std::string();
    }
    if ((kctx = EVP_KDF_CTX_new(kdf)) == NULL)
    {
        EVP_KDF_free(kdf);
        EVP_KDF_CTX_free(kctx);
        OSSL_set_max_threads(NULL, 0);
        return std::string();
    }
    if (EVP_KDF_derive(kctx, &result[0], outlen, params) != 1)
    {
        EVP_KDF_free(kdf);
        EVP_KDF_CTX_free(kctx);
        OSSL_set_max_threads(NULL, 0);
        return std::string();
    }

    std::clog << "<crypto::HashPassword> Generated hash password: " << OPENSSL_buf2hexstr(result, outlen) << std::endl; //NEED TO DELETE THIS LOG

    return std::string(OPENSSL_buf2hexstr(result, outlen));
}

const std::string crypto::GenerateRandomKey()
{
	std::vector<unsigned char> generated_array(32);
	if (RAND_bytes(generated_array.data(), 32) != 1)
	{
		throw std::runtime_error("<GenerateRandomKey> Failed to generate random bytes");
	}

    std::clog << "<crypto::GenerateRandomKey> Generated random key" << std::endl;

	return std::string(generated_array.begin(), generated_array.end());
}
