#include "split_merge.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <windows.h>
#include <memory>

using namespace std;

void save_img() 
{
	string pathToDot = "D:\\Graphviz2.38\\bin\\dot.exe";
	string pathToFile = " \"output.dot\" ";
	/*char buf[256];
	GetCurrentDirectoryA(256, buf);
	pathToFile = " \"" + string(buf) + '\\' + pathToFile;*/
	string args = " -Tpng -O ";
	string str = "\"" + pathToDot + "\" -Tpng \"" + pathToFile + "\" -O";

	STARTUPINFOA info;
	PROCESS_INFORMATION pi;

	memset(&info, 0, sizeof(info));
	memset(&pi, 0, sizeof(pi));

	info.wShowWindow = false;
	str = pathToFile + args;
	if (!CreateProcessA((LPSTR)pathToDot.c_str(), (LPSTR)str.c_str(), nullptr, nullptr, false, 0, nullptr, nullptr, &info, &pi))
		cerr << GetLastError() << endl;
}
int main(int argc, char** argv)
{
	int o;
	string file;
	string trace;
	vector<string> log;
	/*if (argc < 3)
	{
		cerr << "Usage: " << argv[0] << " order path_to_file" << endl;
		return 1;
	}
	else
	{
		o = atoi(argv[1]);
		file = argv[2];
		ifstream fin(file);
		if (!fin)
		{
			cerr << "File doesn't exist!" << endl;
			return 1;
		}
		while (fin >> trace)
			log.push_back(trace);
		fin.close();

		split_merge sm(log, o);
		sm.build();
	}*/
	o = 3;
	file = "input.txt";
	//file = "input.csv";
	ifstream fin(file);
	while (fin >> trace)
		log.push_back(trace);
	fin.close();

	split_merge sm(log, o);
	sm.build();
	save_img();
	system("PAUSE");
}