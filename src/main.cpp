/*
By Luis Morales, Dec-2017
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "HYDAText.h"

using namespace std;


int main(int argc, char const *argv[]) {

  (void) argc;

  // Check existence of HYDAT.in file
  fstream file(argv[1], ios::in);
  if(!file.is_open()){
    cout << "File not found!\n";
    return 1;
  }

  string HYDATdir, outdir;

  string line;
  // Reading header file
  getline(file, line);
  getline(file, HYDATdir);
  //cout << HYDATdir << '\n';
  getline(file, line);
  getline(file, outdir);
  //cout << outdir << '\n';
  getline(file, line);


  vector<string> STATION_NUMBER;

  string element, STATION;
  while( getline(file, line) ){
    istringstream stream(line);
    vector<string> column;

    getline(stream, STATION, ',');
    //cout << STATION << '\n';
    // Check is station is in station vector
    //if (find(STATION_NUMBER.begin(), STATION_NUMBER.end(), STATION) != STATION_NUMBER.end()){
    STATION_NUMBER.push_back(trim(STATION));
    //}

    // Save varariable names for each station
    while( getline(stream, element, ',') ){
      column.push_back(trim(element));
    }
    // Write time series for each station number
    for(vector<string>::iterator VARIA = column.begin(); VARIA != column.end(); ++VARIA){
      cout << "Recording " << *VARIA <<" for " << STATION << '\n';
      extractTS(HYDATdir, outdir, STATION, *VARIA);
    }
  }

  // Write station information
  statInfo(HYDATdir, outdir, STATION_NUMBER);

  return 0;
}
