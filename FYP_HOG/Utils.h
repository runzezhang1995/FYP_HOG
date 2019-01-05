#pragma once

#include<stdio.h>
#include<io.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void findFile(const string &str,  vector<string> *res);
void SplitString(const string& s, vector<string>& v, const string& c);
void stringReplace(std::string &strBig, const std::string &strsrc, const std::string &strdst);
