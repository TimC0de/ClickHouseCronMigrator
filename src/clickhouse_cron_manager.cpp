#include <thread>
#include <chrono>
#include <utility>

#include <loguru.hpp>

#include "clickhouse_cron_manager.hpp"

namespace {
	void executeMigration(const libcron::TaskInformation& taskInformation)
	{
		LOG_F(INFO, "Executing '%s' task", taskInformation.get_name().data());
	}
}

namespace ch_cron_migrator {

	using namespace std::literals::chrono_literals;

	ClickHouseCronManager::ClickHouseCronManager(std::string& taskName, std::string& cronExpression, ClickHouseMigrator& migrator) :
		m_taskName(std::move(taskName)),
		m_cronExpression(std::move(cronExpression)),
		m_migrator(std::move(migrator))
	{
		initTask();
	}

	void ClickHouseCronManager::initTask()
	{
		m_task.add_schedule(m_taskName, m_cronExpression, [&](const auto& taskInfo) {
			LOG_F(INFO, "Executing '%s' task", taskInfo.get_name().c_str());

			// Executing migration
			m_migrator.migrate();

			LOG_F(INFO, "Task '%s' executed", taskInfo.get_name().c_str());
		});
	}

	void ClickHouseCronManager::start()
	{
		// Executing CRON
		LOG_F(INFO, "Starting cron '%s'...", m_taskName.c_str());
		while (1) {
			m_task.tick();
			std::this_thread::sleep_for(500ms);
		}
		LOG_F(INFO, "Cron ended");
	}
}