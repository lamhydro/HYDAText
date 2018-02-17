
#include <iostream>
#include <string>
/*
#include <fstream>
#include <sstream>
#include <vector>
*/
using namespace std;

#ifndef ADD_HEADER
#define ADD_HEADER

/*
Trim white space of a string.
*/
string trim(const string& str);

/*
Return the column number from where daily values are recorded.
*/
int getStartCol(string &VARIA);

/*
Return the column number from where daily values are recorded.
*/
int getNO_DAYSCol(string &VARIA);

/*
Record station information
*/
int statInfo(string &HYDATdir, string &outdir, vector<string> &STATION_NUMBER);

/*
Fetch time series
*/
int extractTS(string &HYDATdir, string &outdir, string &STATION_NUMBER, string &VARIA);

/*
Extract station regulation information.
*/
int read_STN_REGULATION(string &HYDATdir, string &STATION_NUMBER, string &YEAR_FROM, string &YEAR_TO, int &REGULATED);

#endif // ADD_HEADER
