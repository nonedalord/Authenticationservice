#include "EnvCfg.h"

void env_cfg::EnvCfg::InitEnv(std::unordered_map<std::string, std::any>& env_map)
{
	std::unordered_map<std::string, std::any> result;
	for (const auto& entry : env_map)
	{
		ProcessEntry(entry, result);
	}
	env_map.swap(result);
}

std::string env_cfg::EnvCfg::GetEnv(const std::string& env)
{
	if (char* env_value = std::getenv(env.c_str()))
	{
		return env_value;
	}
	else
	{
		return std::string();
	}
}

void env_cfg::EnvCfg::ProcessEntry(const std::pair<std::string, std::any>& entry, std::unordered_map<std::string, std::any>& result)
{
	const auto& [env_name, value] = entry;

	if (value.type() == typeid(const char*))
	{
		std::string value_str = std::any_cast<const char*>(value);
		if (TryHandleType<std::string>(value_str, env_name, result))
			return;
	}

	if (value.type() == typeid(EnvCfgTypes)) 
	{
		HandleEnumType(value, env_name, result);
	}
	else 
	{
		if (TryHandleType<int>(value, env_name, result))
			return;
		if (TryHandleType<std::string>(value, env_name, result))
			return;
		if (TryHandleType<double>(value, env_name, result))
			return;
		if (TryHandleType<long long>(value, env_name, result))
			return;
		if (TryHandleType<bool>(value, env_name, result))
			return;
		std::string type = typeid(value.type()).name();
		throw std::runtime_error("Unhandled type" + type);
	}
}

void env_cfg::EnvCfg::HandleEnumType(const std::any& value, const std::string& env_name, std::unordered_map<std::string, std::any>& result)
{
	const auto type = std::any_cast<EnvCfgTypes>(value);
	switch (type) 
	{
		case EnvCfgTypes::string_:  
			HandleType<std::string>(value, env_name, result); 
			break;
		case EnvCfgTypes::int_:     
			HandleType<int>(value, env_name, result); 
			break;
		case EnvCfgTypes::double_:  
			HandleType<double>(value, env_name, result); 
			break;
		case EnvCfgTypes::longlong_:
			HandleType<long long>(value, env_name, result); 
			break;
		case EnvCfgTypes::bool_:    
			HandleType<bool>(value, env_name, result); 
			break;
		default: 
			throw std::runtime_error("Unknown enum type");
	}
}