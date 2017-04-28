#ifndef ARG_H
#define ARG_H

#include "xvector.h"
#include "range.h"
#include "fn.h"

class Arg
{
public:
	const std::string name_;
	Arg(std::string const& name):
		name_{name}
	{}
	virtual bool pass(std::string const& arg)
	{
		return arg==name_;
	}
	virtual ~Arg() {}
};

class BasicArg final: public Arg
{
public:
	Fn<void()> action_;

	template<typename F>
	BasicArg(std::string const& name, F&& fn):
		Arg(name),
		action_{std::forward<F>(fn)}
	{}
	bool pass(std::string const& str) final override
	{
		if (Arg::pass(str)) {
			action_();
			return true;
		}
		return false;
	}
};

class StringArg final: public Arg
{
public:
	Fn<void(std::string const&)> action_;
	const char assignCh_;
	std::string str_;

	template<typename F>
	StringArg(std::string const& name, char assignCh, F&& fn):
		Arg(name),
		assignCh_{assignCh},
		action_{std::forward<F>(fn)}
	{}
	bool pass(std::string const& arg) final override
	{
		try {
			size_t ascPos = arg.find_first_of(assignCh_);
			if (Arg::pass(arg.substr(0, ascPos)) &&
				(str_ = arg.substr(ascPos+1)).size()) {
				action_(str_);
				return true;
			}
		}
		catch (...) {}
		return false;
	}
};

template<typename T>
class ValueArg final: public Arg
{
public:
	const char assignCh_;
	T value_;
	x::range<T> range_;
	Fn<void(T)> action_;

	template<typename F>
	ValueArg(std::string const& name, char assignCh, x::range<T>&& range, F&& fn):
		Arg(name),
		assignCh_{assignCh},
		range_{range},
		action_{std::forward<F>(fn)}
	{}

	template<typename F>
	ValueArg(std::string const& name, char assignCh, F&& fn):
		Arg(name),
		assignCh_{assignCh},
		range_{std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max()},
		action_{std::forward<F>(fn)}
	{}

	bool pass(std::string const& arg) final override
	{
		try {
			size_t ascPos = arg.find_first_of(assignCh_);
			if (Arg::pass(arg.substr(0, ascPos)) &&
				range_.contains(value_ =
					T(std::stod(arg.substr(ascPos+1))))) {
				action_(value_);
				return true;
			}
		}
		catch (...) {}
		return false;
	}
};


class ArgHandler
{
	x::vector<Arg*> args_;
public:
	ArgHandler() {}

	void process(int argc, char* argv[])
	{
		for (size_t i = 0; i<argc; ++i) {
			std::cout<<argv[i]<<std::endl;
			for (Arg*& a : args_) {
				if (a->pass(std::string(argv[i])))
					break;
			}
		}
	}

	void add(Arg* arg)
	{
		args_.push_back(arg);
	}

	~ArgHandler()
	{
		args_.erase<x::vector_opt::PTR_DELETE>();
	}
};

#endif //ARG_H
