#include "FileConfig.h"

FileConfig FileConfig::read(std::string path) {
	FileConfig fileConfig;
	fileConfig._path = path;

	return fileConfig;
}

std::string FileConfig::getPath() {
	return _path;
}
