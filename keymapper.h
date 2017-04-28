#pragma once
#include <string>
#include <fstream>

class KeyMapper
{
	static const std::string noModFilename_;
	static const std::string shiftFilename_;
	static const std::string altFilename_;
	static const std::string shiftAltFilename_;
	char noModMap_[256];
	char shiftMap_[256];
	char altMap_[256];
	char shiftAltMap_[256];

	const std::pair<const std::string*, char*> assignMap_[4];
	void load_();

public:
	KeyMapper();

	char convert(size_t ch,	
		bool shift = false,	
		bool alt = false, 
		bool ctrl = false);
};
