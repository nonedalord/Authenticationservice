#include <string>
#include <memory>
#include <optional>
#include <thread>

#include <pqxx/pqxx>

#include "DataBaseCfg.h"

class DataBaseProcessor : public std::enable_shared_from_this<DataBaseProcessor>
{
public:
	struct Auth
	{
		std::string login;
		std::string hashed_password;
	};
	DataBaseProcessor(const DataBaseCfg& data_base_cfg);
	~DataBaseProcessor();

private:
	std::unique_ptr<pqxx::connection> m_connection;
};