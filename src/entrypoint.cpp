#include <chrono>
#include <filesystem>
#include <string>
#include <utility>
#include <thread>

#include <loguru.hpp>

#include "config_parser.hpp"
#include "clickhouse_migrator.hpp"
#include "clickhouse_cron_manager.hpp"


using namespace std::literals::chrono_literals;
using namespace ch_cron_migrator;
using ClickHouseClientData = ClickHouseMigrator::ClickHouseClientData;

inline void configure_logging(int argc, char* argv[])
{
	const auto timestampSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
	const auto millisSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(timestampSinceEpoch).count();
	const auto logfilePath = std::filesystem::current_path() / "log" / (std::to_string(millisSinceEpoch) + ".log");

	loguru::init(argc, argv);
	loguru::add_file(
		logfilePath.c_str(),
		loguru::Append,
		loguru::Verbosity_MAX);
}

inline ClickHouseClientData client_from_config(const std::string& client_prefix, const ConfigParser& parser)
{
	return ClickHouseClientData{
		parser.get(client_prefix + "_host"),
		parser.get(client_prefix + "_port"),
		parser.get(client_prefix + "_database"),
		parser.get(client_prefix + "_table"),
		parser.get(client_prefix + "_user"),
		parser.get(client_prefix + "_password")
	};
}

inline void start_cron(std::string& taskName, std::string& cronExpression, ClickHouseMigrator& migrator)
{
	loguru::set_thread_name(taskName.c_str());

	ClickHouseCronManager cronManager(taskName, cronExpression, migrator);
	cronManager.start();
}

int main(int argc, char* argv[])
{
	configure_logging(argc, argv);

	ConfigParser parser;

	// Start old -> mid cron
	std::string oldMidTaskName = "old_mid_clickhouse_cron";
	std::string oldMidCronExpression = parser.get("old_mid_cron_expression");
	ClickHouseMigrator oldMidMigrator(
		client_from_config("old", parser),
		client_from_config("mid", parser)
	);

	std::thread oldMidCronThread(
		start_cron,
			std::ref(oldMidTaskName),
			std::ref(oldMidCronExpression),
			std::ref(oldMidMigrator)
		);

	// Start mid -> new cron
	std::string midNewTaskName = "mid_new_clickhouse_cron";
	std::string midNewCronExpression = parser.get("mid_new_cron_expression");
	ClickHouseMigrator midNewMigrator(
		client_from_config("mid", parser),
		client_from_config("new", parser)
	);

	std::thread midNewCronThread(
		start_cron,
			std::ref(midNewTaskName),
			std::ref(midNewCronExpression),
			std::ref(midNewMigrator)
	);

	oldMidCronThread.join();
	midNewCronThread.join();
}
