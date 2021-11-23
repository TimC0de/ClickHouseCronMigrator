#include <filesystem>
#include <vector>
#include <functional>
#include <stdlib.h>

#include <boost/program_options/parsers.hpp>

#include <loguru.hpp>

#include "config_parser.hpp"

namespace {

	namespace fs = std::filesystem;

	std::vector<fs::path> ls(const fs::path& directoryPath)
	{
		assert(fs::is_directory(directoryPath));

		std::vector<fs::path> fileNames;
		fs::directory_iterator directory(directoryPath);

		std::copy_if(
			fs::begin(directory),
			fs::end(directory),
			std::back_inserter(fileNames),
			[](const auto& directoryEntry) {
				return directoryEntry.path().string().ends_with(".ini");
			}
		);
		return fileNames;
	}
}

namespace ch_cron_migrator {

	ConfigParser::ConfigParser()
	{
		initOptions();
		init();
	}

	void ConfigParser::initOptions()
	{
		m_options.add_options()
			("old_mid_cron_expression", po::value<std::string>())
			("mid_new_cron_expression", po::value<std::string>())

			// New host info
			("new_host", po::value<std::string>())
			("new_port", po::value<std::string>()->default_value("9000"))
			("new_database", po::value<std::string>())
			("new_table", po::value<std::string>())
			("new_user", po::value<std::string>())
			("new_password", po::value<std::string>())

			// Mid host info
			("mid_host", po::value<std::string>())
			("mid_port", po::value<std::string>()->default_value("9000"))
			("mid_database", po::value<std::string>())
			("mid_table", po::value<std::string>())
			("mid_user", po::value<std::string>())
			("mid_password", po::value<std::string>())

			// Old host info
			("old_host", po::value<std::string>())
			("old_port", po::value<std::string>()->default_value("9000"))
			("old_database", po::value<std::string>())
			("old_table", po::value<std::string>())
			("old_user", po::value<std::string>())
			("old_password", po::value<std::string>());
	}

	void ConfigParser::init()
	{
		fs::path kConfigPath = fs::current_path() / std::string("config");
		const auto configFileNames = ls(kConfigPath);
		assert(!configFileNames.empty());

		for (const auto& configFileName : configFileNames) {
			const auto parsedConfigOptions = po::parse_config_file(configFileName.c_str(), m_options, true);
			po::store(parsedConfigOptions, m_content);
		}

		assert(!m_content.empty());
	}

	std::string ConfigParser::get(const char* optionName) const
	{
		return get(std::string(optionName));
	}

	std::string ConfigParser::get(const std::string& optionName) const
	{
		try {
			return m_content[optionName].as<std::string>();
		}
		catch (const boost::bad_any_cast&) {
			ABORT_F("Failed to get %s option", optionName.data());
		}
	}
}