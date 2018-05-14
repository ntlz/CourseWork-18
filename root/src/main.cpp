#include "split_merge.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <windows.h>
#include <memory>

using namespace std;

void save_img(string of, int o)
{
	string pathToDot = "C:\\Program Files (x86)\\Graphviz2.38\\bin\\dot.exe";
    //string pathToDot = "D:\\Graphviz2.38\\bin\\dot.exe";
    string pathToFile = " \"" + of + "_out_" + to_string(o) + ".dot\" ";
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
	}
	o = 2;
	file = "..//..//logs//imaginary.txt";*/
	ifstream fin;
	string out_file;
	while (true)
	{
		cout << "Enter log filename: ";
		cin >> file;
		if (file == "exit")
			return 0;
		size_t ind = file.find_last_of(".");
		out_file = file.substr(0, ind);
		fin = ifstream(file);
		if (!fin)
		{
			cout << "No such file." << endl;
			continue;
		}
		if (file.substr(ind) != ".txt")
		{
			cout << ".txt file is required" << endl;
			continue;
		}

		break;
	}
	while (true)
	{
		cout << "Enter algorithm parameter: ";
		string c;
		cin >> c;
		try
		{
			if (c == "exit")
				return 0;
			o = stoi(c);
		}
		catch (invalid_argument e)
		{
			cout << "Invalid argument." << endl;
			continue;
		}
		if (o < 1)
		{
			cout << "Parameter must be 2 or more." << endl;
			continue;
		}
		break;
	}
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
	fin.close();
	if (log.size() == 0 || k == 0)
	{
		cout << "Log is empty or the format is incorrect." << endl;
		return 1;
	}
	//ifstream d("..//..//logs//dict_log05.txt");
	map<string, string> dict;
	string line;
	/*while (getline(d, line))
	{
		size_t i = line.find_first_of(":");
		dict[line.substr(0, i)] = line.substr(i + 1);
	}*/
	cout << "Building FSM..." << endl;
	split_merge sm(log, o, dict);
	sm.build(out_file);
	save_img(out_file, o);
	cout << "Data saved in the input file directory." << endl;

	return 0;
}