#include "PasswordArgon2id.h"

crypto::password::PasswordArgon2id::PasswordArgon2id() : PasswordArgon2id(crypto::password::default_threads, crypto::password::default_memcost, crypto::password::default_lanes)
{
}

crypto::password::PasswordArgon2id::PasswordArgon2id(uint32_t threads) : PasswordArgon2id(threads, crypto::password::default_memcost, crypto::password::default_lanes)
{
}

crypto::password::PasswordArgon2id::PasswordArgon2id(uint32_t threads, uint32_t memcost, uint32_t lanes) : m_threads(threads),
m_memcost(memcost), m_lanes(lanes), m_kdf(nullptr, EVP_KDF_free), m_ctx(OSSL_LIB_CTX_new(), OSSL_LIB_CTX_free)
{
    if (!m_ctx)
    {
        throw PasswordException("Failed creating context");
    }
    if (OSSL_set_max_threads(m_ctx.get(), m_threads) != 1)
    {
        throw PasswordException("Failed to set OpenSSL threads");
    }
    EVP_KDF* kdf = nullptr;
    if ((kdf = EVP_KDF_fetch(m_ctx.get(), crypto::password::algoritm_name.c_str(), nullptr)) == nullptr)
    {
        throw PasswordException("Failed to reinitialize KDF");
    }
    m_kdf.reset(kdf);
}

crypto::password::PasswordArgon2id::~PasswordArgon2id()
{
    OSSL_set_max_threads(nullptr, 0);
}

std::string crypto::password::PasswordArgon2id::HashPassword(const std::string& password)
{
    char* hex_str = NULL;
    std::unique_ptr<EVP_KDF_CTX, decltype(&EVP_KDF_CTX_free)> kctx(EVP_KDF_CTX_new(m_kdf.get()), EVP_KDF_CTX_free);
    if (!kctx)
    {
        throw PasswordException("Failed to initialize KDF CTX");
    }

    std::string psw(password);

    unsigned char result[m_outlen];
    std::string salt = crypto::GenerateRandomKey(crypto::bytes::salt_size);
    OSSL_PARAM params[6];
    OSSL_PARAM* p = params;
    p = params;
    *p++ = OSSL_PARAM_construct_uint32(OSSL_KDF_PARAM_THREADS, &m_threads);
    *p++ = OSSL_PARAM_construct_uint32(OSSL_KDF_PARAM_ARGON2_LANES, &m_lanes);
    *p++ = OSSL_PARAM_construct_uint32(OSSL_KDF_PARAM_ARGON2_MEMCOST, &m_memcost);
    *p++ = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_SALT, salt.data(), salt.size());
    *p++ = OSSL_PARAM_construct_octet_string(OSSL_KDF_PARAM_PASSWORD, psw.data(), psw.size());
    *p++ = OSSL_PARAM_construct_end();

    if (EVP_KDF_derive(kctx.get(), &result[0], m_outlen, params) != 1)
    {
        throw PasswordException("Failed to derive");
    }

    hex_str = OPENSSL_buf2hexstr(result, m_outlen);

    std::string hash(hex_str);

    OPENSSL_free(hex_str);
    return hash;
}