#include "JwtToken.h"

const std::string crypto::jwt_token::JwtToken::GenerateJWT(const std::string& secret, const json::object& header, const json::object& payload)
{
	std::string header_str = json::serialize(header);
	std::string payload_str = json::serialize(payload);
	
	std::string header_b64 = EncodeBase64Url(header_str);
	std::string payload_b64 = EncodeBase64Url(payload_str);

	std::string data = header_b64 + "." + payload_b64;

	unsigned char digest[EVP_MAX_MD_SIZE];
	unsigned int digest_len;

	HMAC(
		EVP_sha256(),
		secret.data(),
		secret.size(),
		reinterpret_cast<const unsigned char*>(data.data()),
		data.size(),
		digest,
		&digest_len
	);

	std::string signature(reinterpret_cast<char*>(digest), digest_len);
	std::string signature_b64 = EncodeBase64Url(signature);

	std::clog << "<crypto::jwt_token::JwtToken::GenerateJWT> Generated key" << std::endl;

	return data + "." + signature_b64;
}

std::string crypto::jwt_token::JwtToken::EncodeBase64Url(const std::string& data)
{
	std::size_t base64_len = boost::beast::detail::base64::encoded_size(data.size());
	std::vector<unsigned char> dest(base64_len);

	std::size_t encoded_len = boost::beast::detail::base64::encode(dest.data(), data.c_str(), data.size());
	std::string encoded(dest.begin(), dest.begin() + encoded_len);

	// Replacing "+" and "-" for Base64Url
	std::replace(encoded.begin(), encoded.end(), '+', '-');
	std::replace(encoded.begin(), encoded.end(), '/', '_');

	// Deleting padding "="
	encoded.erase(std::remove(encoded.end() - 2, encoded.end(), '='), encoded.end());
	return encoded;
}