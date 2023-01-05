#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>

int delayTime;

void stringOutput(std::string s) {
	delayTime = 25;
	for (int i = 0; i < s.length(); i++) {
		std::cout << s[i];
		if (!(s[i] == ' ')) Sleep(delayTime);
	}
	std::cout << std::endl;
}