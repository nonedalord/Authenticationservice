#include "Crypto.h"

const std::string crypto::GenerateRandomKey(const size_t bytes)
{
	std::vector<unsigned char> generated_array(bytes);
	if (RAND_bytes(generated_array.data(), bytes) != 1)
	{
		throw std::runtime_error("Failed to generate random bytes " + std::to_string(bytes));
	}
	return std::string(generated_array.begin(), generated_array.end());

}
