#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>

#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/options_description.hpp>

namespace ch_cron_migrator {

	namespace po = boost::program_options;

	class ConfigParser {
	public:

		ConfigParser();

		std::string get(const char* optionName) const;
		std::string get(const std::string& optionName) const;

	private:

		void init();
		void initOptions();

		po::options_description m_options;
		po::variables_map m_content;
	};
}

#endif // !defined(CONFIG_PARSER_H)