#include <string>
#include <csignal>
#include <memory>
#include <chrono>
#include <thread>
#include <vector>
#include <filesystem>
#include <iostream>

#include <boost/json.hpp>

#include "./crypto_utils/Crypto.h"

namespace json = boost::json;
using Token = crypto::jwt_token::JwtToken;

static std::sig_atomic_t g_terminated = false;

/*void CreateLogsDirectory()
{
	std::filesystem::path logs_path = "logs";

	if (!std::filesystem::exists(logs_path))
	{
		std::filesystem::create_directory(logs_path);
	}
}*/

void SignalHandler(int s)
{
	g_terminated = true;
}

int main()
{
	//CreateLogsDirectory();

	std::signal(SIGINT, SignalHandler);
	std::signal(SIGTERM, SignalHandler);
	json::object header = {
		{"alg", "HS256"},
		{"typ", "JWT"}
	};
	json::object payload = {
		{"sub", "534543"},
		{"name", "Pi Riby"},
		{"iat", 1516239022}
	};
	std::clog << "<main> Authenticationservice started!" << std::endl;
	std::string secret_key = crypto::GenerateRandomKey();
	std::cout << "JWT: " << Token::GenerateJWT(secret_key, header, payload) << std::endl;
	//crypto::HashPassword("!234");
	/*while (!g_terminated)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}*/
	std::clog << "<main> Authenticationservice stopped!" << std::endl;
	return EXIT_SUCCESS;
}