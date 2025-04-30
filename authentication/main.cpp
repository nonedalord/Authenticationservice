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
#include "./env_cfg/EnvCfg.h"

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
	auto env_conf = std::unordered_map<std::string, std::any>{
			{"WEB_SERVER_IP", "543543"},
			{"WEB_SERVER_PORT", 4343}
	};
	env_cfg::EnvCfg::InitEnv(env_conf);
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
	std::string secret_key = crypto::GenerateRandomKey(32);
	std::cout << "JWT: " << Token::GenerateJWT(secret_key, header, payload) << std::endl;
	try
	{
		crypto::password::PasswordArgon2id pass;
		std::cout << pass.HashPassword("dsafdasvxc321das") << std::endl;
		std::cout << pass.HashPassword("dsbvc3DASdasas") << std::endl;
		std::cout << pass.HashPassword("321DSAfdsfdsdsa3vfdg") << std::endl;
		std::cout << pass.HashPassword("3231-#@!^&&^%1DSAfdsfdsdsa3vfdg") << std::endl;
	}
	catch (const crypto::password::PasswordException& err)
	{
		std::cerr << err.what() << std::endl;
	}
	/*while (!g_terminated)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}*/
	std::clog << "<main> Authenticationservice stopped!" << std::endl;
	return EXIT_SUCCESS;
}