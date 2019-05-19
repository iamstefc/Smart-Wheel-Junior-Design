#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

using namespace std;

class GPIO {
private:
	string value;
	string direction;
	string gpionumber;
public:
	GPIO(string v, string d, string num) { 
		gpionumber = num; 
		setvalue(v);
		setdirection(d);

	}
	GPIO(string num) {
		value = "1";
		gpionumber = num;
		gpio_export();
	}
	GPIO() {}
	~GPIO() {}

	string getvalue();
	void setvalue(string v);
	string getdirection();
	void setdirection(string d);
	void gpio_export();
	void gpio_unexport();

};

