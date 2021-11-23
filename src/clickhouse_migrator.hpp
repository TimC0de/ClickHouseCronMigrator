#ifndef CLICKHOUSE_MIGRATOR_H
#define CLICKHOUSE_MIGRATOR_H

#include "config_parser.hpp"

namespace ch_cron_migrator {

	class ClickHouseMigrator {
	public:

		struct ClickHouseClientData {
			std::string host;
			std::string port;
			std::string database;
			std::string table;
			std::string user;
			std::string password;

			std::string to_remote_str() const;
		};

		ClickHouseMigrator(ClickHouseClientData&& from, ClickHouseClientData&& to);
		ClickHouseMigrator(ClickHouseMigrator&& migrator) = default;
		
		void migrate();

	private:

		std::string createLastMigrationTimestamp();

		ClickHouseClientData m_from;
		ClickHouseClientData m_to;
		std::string m_lastMigrationTimestamp;
	};
}

#endif // !defined(CLICKHOUSE_MIGRATOR_H)