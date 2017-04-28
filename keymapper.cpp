#include "keymapper.h"

void KeyMapper::load_()
{
	std::fstream keymapFile;
	for (size_t i = 0; i<4; ++i) {
		keymapFile.open(*assignMap_[i].first, std::fstream::in);
		if (!keymapFile.is_open()) {
			throw(0);
		}
		size_t k = 0;
		while (keymapFile.get(assignMap_[i].second[k++]));
		keymapFile.close();
	}
}

KeyMapper::KeyMapper() :
	assignMap_{
		{&KeyMapper::noModFilename_,	noModMap_},
		{&KeyMapper::shiftFilename_,	shiftMap_},
		{&KeyMapper::altFilename_,		altMap_},
		{&KeyMapper::shiftAltFilename_,	shiftAltMap_}
}
{
	try {
		load_();
	}
	catch (int error) {
		throw(error);
	}
}

char KeyMapper::convert(size_t ch, bool shift, bool alt, bool ctrl)
{
	if (shift) {
		return shiftMap_[ch];
		if (alt) return shiftAltMap_[ch];
	}
	else if (alt) return altMap_[ch];
	else if (!ctrl) return noModMap_[ch];
	return 0;
}

const std::string KeyMapper::noModFilename_ = "./nomod.km";
const std::string KeyMapper::shiftFilename_ = "./shift.km";
const std::string KeyMapper::altFilename_ = "./alt.km";
const std::string KeyMapper::shiftAltFilename_ = "./shiftalt.km";