#ifndef UTIL_H
#define UTIL_H

#include <map>
#include <string>

#define PAIRTYPE(t1, t2)    std::pair<t1, t2>

namespace i2p
{
namespace util
{
	namespace config
	{
		extern std::map<std::string, std::string> mapArgs;
		extern std::map<std::string, std::vector<std::string> > mapMultiArgs;
		void OptionParser(int argc, const char* const argv[]);
		int GetArg(const std::string& strArg, int nDefault);
		std::string GetArg(const std::string& strArg, const std::string& strDefault);
		const char* GetCharArg(const std::string& strArg, const std::string& nDefault);
	}

	namespace filesystem
	{
		boost::filesystem::path GetDefaultDataDir();
		boost::filesystem::path GetConfigFile();
		void ReadConfigFile(std::map<std::string, std::string>& mapSettingsRet,
                std::map<std::string, std::vector<std::string> >& mapMultiSettingsRet);
	}

	namespace http
	{
		std::string httpRequest(const std::string& address);
		struct url {
    			url(const std::string& url_s); // omitted copy, ==, accessors, ...
			private:
    			void parse(const std::string& url_s);
			public:
    			std::string protocol_, host_, path_, query_;
		};
	}
}
}


#endif
