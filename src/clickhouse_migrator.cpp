#include <string>
#include <utility>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <clickhouse/client.h>
#include <loguru.hpp>

#include "clickhouse_migrator.hpp"

namespace {

	const std::string fieldsToCopy = "t_stamp, date, dow, hod, adv_id, terminal_id, content, business_unit, publisher, vast_uri, proxy_dsn, success, city";

}

namespace ch_cron_migrator {

	namespace ch = clickhouse;

	//
	// ClickHouseClientData impl
	//

	std::string ClickHouseMigrator::ClickHouseClientData::to_remote_str() const
	{
		std::string addressesExpr = host;
		if (!port.empty()) {
			addressesExpr += ":" + port;
		}

		return "'" + addressesExpr + "'," + database + "," + table + "," + user + "," + password + ",rand()";
	}


	//
	// ClickHouseMigrator impl
	//

	std::string ClickHouseMigrator::createLastMigrationTimestamp()
	{
		std::stringstream buffer;
		std::time_t currentTimestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		buffer << std::put_time(std::gmtime(&currentTimestamp), "%F %T");
		return buffer.str();
	}

	ClickHouseMigrator::ClickHouseMigrator(ClickHouseClientData&& from, ClickHouseClientData&& to) :
		m_from(std::move(from)),
		m_to(std::move(to)),
		m_lastMigrationTimestamp(createLastMigrationTimestamp())
	{}

	void ClickHouseMigrator::migrate()
	{
		// Initialize client connection.
		try {
			const std::string query = "INSERT INTO " + m_to.table +
				" SELECT " + fieldsToCopy +
				" FROM remote(" + m_from.to_remote_str() + ")" +
				" WHERE t_stamp >= " + m_lastMigrationTimestamp;
			
			ch::Client client(
				ch::ClientOptions()
				.SetHost(m_to.host)
				.SetPort(std::stoi(m_to.port))
				.SetDefaultDatabase(m_to.database)
				.SetUser(m_to.user)
				.SetPassword(m_to.password)
			);
			

			LOG_F(INFO, "Executing ClickHouse query: %s", query.c_str());
			client.Execute(query);
			LOG_F(INFO, "Executed.");

			m_lastMigrationTimestamp = createLastMigrationTimestamp();
		}
		catch (const std::system_error& e) {
			LOG_F(FATAL, "Failed to executed ClickHouse query");
		}
	}
}