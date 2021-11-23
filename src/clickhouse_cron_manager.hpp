#ifndef CLICKHOUSE_CRON_MANAGER_H
#define CLICKHOUSE_CRON_MANAGER_H

#include <libcron/Cron.h>

#include "clickhouse_migrator.hpp"

namespace ch_cron_migrator {

	class ClickHouseCronManager {
	public:

		ClickHouseCronManager(std::string& taskName, std::string& cronExpression, ClickHouseMigrator& migrator);

		void start();

	private:

		void initTask();

		std::string m_taskName;
		std::string m_cronExpression;
		
		ClickHouseMigrator m_migrator;

		libcron::Cron<> m_task;
	};
}

#endif // !defined(CLICKHOUSE_CRON_MANAGER_H)