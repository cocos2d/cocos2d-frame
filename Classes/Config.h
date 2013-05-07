//
//  Config.h
//  CocosPlayer
//
//  Created by Ricardo Quesada on 5/6/13.
//
//

#ifndef __CocosPlayer__Config__
#define __CocosPlayer__Config__

#include <iostream>

class Config {

	Config( const std::string& config_file) {};

	virtual ~Config() {};

	// non path
	std::string getString( const std::string &key) {
		return std::string("test");
	}
	double getDouble( const std::string &key ) {
		return 0.0;
	}
	long long getInt( const std::string &key ) {
		return 0;
	}

	// path
	std::string getStringFromPath( const std::string &path) {
		return std::string("");
	}
	double getDoubleFromPath( const std::string &path) {
		return 0.0;
	}
	long long getIntFromPath( const std::string &path) {
		return 0;
	}

};

#endif /* defined(__CocosPlayer__Config__) */
