#include <cstring>
#include <string>
#include "GPIO.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
using namespace std;


void GPIO::setvalue(string v) {
	value = v;
	string fullgpio = "gpio" + gpionumber;
	ofstream gpio;
	gpio.open(("/sys/class/gpio/" + fullgpio + "/value"));
	gpio << v;
	gpio.close();
}

void GPIO::setdirection(string d) {
	direction = d;
	//string setdir = "/sys/class/gpio/gpio" + gpionumber + "/direction";
	ofstream gpio;
	gpio.open(("/sys/class/gpio/gpio" + gpionumber + "/direction")); //converts c++ string to c string
	gpio << d;
	gpio.close();
}

void GPIO::gpio_export() {

	ofstream gpio;
	gpio.open("/sys/class/gpio/export");
	gpio << gpionumber;
	gpio.close();
	sleep(1);
}

void GPIO::gpio_unexport() {
	ofstream gpio;
	gpio.open("/sys/class/gpio/unexport");
	gpio << gpionumber;
	gpio.close();
}

string GPIO::getvalue() { 
	ifstream gpio;
	string s;
	gpio.open(("/sys/class/gpio/gpio" + gpionumber + "/value"));
	gpio >> s; 
	gpio.close();
	return s;
}

string GPIO:: getdirection() {
	ifstream gpio;
	string s;
	gpio.open(("/sys/class/gpio/gpio" + gpionumber + "/direction"));
	gpio >> s;
	gpio.close();
	return s;
}