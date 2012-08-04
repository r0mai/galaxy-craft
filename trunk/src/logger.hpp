#include <iostream>
#include <fstream>
namespace gc{

class logger{
public:
	enum levels{none, error, warning, all} threshold, messagelevel;
	

	logger(logger::levels threshold = logger::error ) : threshold(threshold) {}

	template<class T>
	logger& operator<<(T message){
		if(messagelevel >= threshold)
			std::cout << message;
		return *this;
	}


	logger& operator<<(logger::levels level){
		messagelevel = level;
		return *this;
	}
};





} // namespace gc
