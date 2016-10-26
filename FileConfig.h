#include "IConfig.h"
#include <string>

class FileConfig:IConfig {
private:
	std::string _path;
public:
	static FileConfig read(std::string path);

	std::string getPath();
};

