#pragma once

#include <string>
#include <memory>
#include <optional>
#include <any>
#include <unordered_map>
#include <type_traits>

#include "EnvCfgTypes.h"

namespace env_cfg
{
	class EnvCfg
	{
	public:
		static void InitEnv(std::unordered_map<std::string, std::any>& env_map);
	private:
		template <class T>
		static std::optional<T> GetEnvByType(const std::string& env);
		static std::string GetEnv(const std::string& env);
		static void ProcessEntry(const std::pair<std::string, std::any>& entry, std::unordered_map<std::string, std::any>& result);
		template <typename T>
		static void HandleType(const std::any& value, const std::string& env_name, std::unordered_map<std::string, std::any>& result);
		static void HandleEnumType(const std::any& value, const std::string& env_name, std::unordered_map<std::string, std::any>& result);
		template<typename T>
		static bool TryHandleType(const std::any& value, const std::string& key, std::unordered_map<std::string, std::any>& result) 
		{
			if (value.type() == typeid(T))
			{
				HandleType<T>(value, key, result);
				return true;
			}
			return false;
		}
	};

	template<class T>
	inline std::optional<T> EnvCfg::GetEnvByType(const std::string& env)
	{
		std::string result = GetEnv(env);
		if (result.empty())
		{
			return std::nullopt;
		}
		else if constexpr (std::is_same<T, std::string>::value && std::is_same<T, char*>::value)
		{
			return result;
		}
		else if constexpr (std::is_same<T, int>::value)
		{
			try
			{
				T result_int = std::stoi(result);
				return result_int;
			}
			catch (const std::invalid_argument&)
			{
				throw std::invalid_argument("Expected int: " + result);
			}
			catch (const std::out_of_range&)
			{
				throw std::out_of_range("Int overflow: " + result);
			}
			catch (const std::exception& err)
			{
				throw std::runtime_error("Catch unhandled exception: " + static_cast<std::string>(err.what()));
			}
		}
		else if constexpr (std::is_same<T, double>::value)
		{
			try
			{
				T result_double = std::stod(result);
				return result_double;
			}
			catch (const std::invalid_argument&)
			{
				throw std::invalid_argument("Expected double: " + result);
			}
			catch (const std::exception& err)
			{
				throw std::runtime_error("Catch unhandled exception: " + static_cast<std::string>(err.what()));
			}
		}
		else if constexpr (std::is_same<T, long long>::value)
		{
			try
			{
				T result_longlong = std::stoll(result);
				return result_longlong;
			}
			catch (const std::invalid_argument&)
			{
				throw std::invalid_argument("Expected long long: " + result);
			}
			catch (const std::out_of_range&)
			{
				throw std::out_of_range("Long long overflow: " + result);
			}
			catch (const std::exception& err)
			{
				throw std::runtime_error("Catch unhandled exception" + static_cast<std::string>(err.what()));
			}
		}
		else if constexpr (std::is_same<T, bool>::value)
		{
			std::transform(result.begin(), result.end(), result.begin(),
				[](unsigned char a) {return std::tolower(a); });
			if (result == "true")
			{
				T result_bool = true;
				return result_bool;
			}
			else if (result == "false")
			{
				T result_bool = false;
				return result_bool;
			}
			else
			{
				throw std::logic_error("Expected bool: " + result);
			}
		}
		else
		{
			std::string type = typeid(T).name();
			throw std::runtime_error("Unhandled type " + type);
		}
	}

	template<typename T>
	inline void EnvCfg::HandleType(const std::any& value, const std::string& env_name, std::unordered_map<std::string, std::any>& result)
	{
		if (std::optional<T> val = GetEnvByType<T>(env_name)) 
		{
			result[env_name] = val.value();
		}
		else if (value.has_value())
		{
			const T* val = std::any_cast<T>(&value);
			if (val)
			{
				result[env_name] = *val;
			}
			else
			{
				result[env_name] = std::any();
			}
		}
		else
		{
			result[env_name] = std::any();
		}
	}

} // namespace env_cgf