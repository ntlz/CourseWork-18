#include "split_merge.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <windows.h>
#include <memory>

using namespace std;

void save_img(string of)
{
	string pathToDot = "C:\\Program Files (x86)\\Graphviz2.38\\bin\\dot.exe";
    //string pathToDot = "D:\\Graphviz2.38\\bin\\dot.exe";
    string pathToFile = " \"" + of + "_out.dot\" ";
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
		cout << GetLastError() << endl;
}
int main(int argc, char** argv)
{
	int o;
	string file;
	string trace;
	vector<vector<string>> log;
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
	file = "..//..//logs//input3.txt";
    //file = "input4.txt";
    size_t ind = file.find_last_of(".");
    string out_file = file.substr(0, ind);
	//file = "input.csv";
	ifstream fin(file);
	if (!fin)
	{
		cout << "No such file." << endl;
		return 1;
	}
	/*while (fin >> trace)
		log.push_back(trace);
	fin.close();*/
	int k = 0;
    while (fin >> trace)
    {
        vector<string> tr;
        while (true)
        {
            size_t i = trace.find_first_of(";");
            if (i == string::npos)
            {
                log.push_back(tr);
                break;
            }
            tr.push_back(trace.substr(0, i));
            trace = trace.substr(i + 1);
        }
		k += tr.size();
    }
	if (log.size() == 0 || k == 0)
	{
		cout << "Log is empty or the format is incorrect." << endl;
		return 1;
	}
	cout << "Building FSM..." << endl;
	split_merge sm(log, o);
	sm.build(out_file);
	save_img(out_file);
	cout << "Data saved in the input file directory." << endl;
	system("PAUSE");

	return 0;
}