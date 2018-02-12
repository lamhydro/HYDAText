#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "HYDAText.h"

using namespace std;

/*
Trim white space of a string.
*/
string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

/*
Return the column number from where daily values are recorded.
*/
int getStartCol(string &VARIA){
  if (VARIA.compare("DLY_FLOWS") == 0){
    return int(11);
  }
  if (VARIA.compare("DLY_LEVELS") == 0 ){
    return int(12);
  }
  if (VARIA.compare("SED_DLY_SUSCON") == 0){
    return int(10);
  }

  return 0;
}

/*
Return the column number from where daily values are recorded.
*/
int getNO_DAYSCol(string &VARIA){
  if (VARIA.compare("DLY_FLOWS") == 0 || VARIA.compare("SED_DLY_SUSCON") == 0){
    return int(4);
  }
  if (VARIA.compare("DLY_LEVELS") == 0){
    return int(5);
  }

  return 0;
}

/*
Record station information
*/
//int statInfo(string &STATION_NUMBER){
int statInfo(string &HYDATdir, string &outdir, vector<string> &STATION_NUMBER){

  fstream file(HYDATdir+"STATIONS.csv", ios::in);
  if(!file.is_open()){
    cout << "File not found!\n";
    return 1;
  }

  fstream fileout(outdir+"STATION_INFO.csv",ios::out);
  if(!fileout.is_open()){
    cout << "File not found!\n";
    return 1;
  }

  string csvLine;
  int j = 0;
  string STATION_NUM;
  for(vector<string>::iterator i = STATION_NUMBER.begin(); i != STATION_NUMBER.end(); ++i){
    STATION_NUM = "\""+*i+"\"";
    //cout << STATION_NUM << "\n";

    while( getline(file, csvLine) ){
    //for (size_t i = 0; i < 3; i++) {
      //getline(file, csvLine);
      istringstream csvStream(csvLine);
      //vector<string> csvColumn;
      string csvElement;

      // Write the header
      if (j++ == 0){
        fileout << csvLine << "\n";
      }

      // Write the station information
      getline(csvStream, csvElement, ',');
      if (csvElement.compare(STATION_NUM) == 0){
        fileout << csvLine << "\n";
        break;
      }

    }
    // Rewind the STATIONS.csv file
    file.clear();
    file.seekg(0);
  }
  return 0;

}


int extractTS(string &HYDATdir, string &outdir, string &STATION_NUMBER, string &VARIA) {

  fstream file(HYDATdir+VARIA+".csv", ios::in);
  if(!file.is_open()){
    cout << "File not found!\n";
    return 1;
  }

  string csvLine;

  fstream fileout(outdir+STATION_NUMBER+"_"+VARIA+".csv",ios::out);
  if(!fileout.is_open()){
    cout << "File not found!\n";
    return 1;
  }
  // Write header of file
  fileout << "YEAR" << ", ";
  fileout << "MONTH" << ", ";
  fileout << "DAY" << ", ";
  fileout << "VAR" << ", ";
  fileout << "VAR_SYMBOL" << "\n";

  string STATION_NUM = "\""+STATION_NUMBER+"\"";

  int startCol = getStartCol(VARIA);
  int NO_DAYSCol = getNO_DAYSCol(VARIA);

  // Start reading through file line by line
  while( getline(file, csvLine) ){
  		istringstream csvStream(csvLine);
  		vector<string> csvColumn;
  		string csvElement;

  		// read every element from the line that is seperated by commas
  		// and put it into the vector or strings
      getline(csvStream, csvElement, ',');
      if (csvElement.compare(STATION_NUM) == 0){
        csvColumn.push_back(csvElement);
        while( getline(csvStream, csvElement, ',') ){
          csvColumn.push_back(csvElement);
        }
        // This checks for a trailing comma with no data after it.
        if (!csvStream && csvElement.empty()){
                // If there was a trailing comma then add an empty element.
                csvColumn.push_back("");
        }

        string YEAR, MONTH;
        int NO_DAYS = 0;
        int j = 0, k = 0, j1 = 0;
        vector<string> VAR, VAR_SYMBOL;
        for(vector<string>::iterator i = csvColumn.begin(); ; ++i ){

          if (j == 1) YEAR = *i;
          if (j == 2) MONTH = *i;
          if (j == NO_DAYSCol) NO_DAYS = atoi((*i).c_str());

          if (j == startCol+k){
            VAR.push_back(*i);
            j1++;
          }
          if (j == startCol+1+k){
            VAR_SYMBOL.push_back(*i);
            if (j1 == NO_DAYS) break;
            k += 2;
          }
          j++;

        }

        // Save into file
        int DAY = 1;
        vector<string>::iterator i1, ii;
        //cout << VAR.size()<<','<<VAR_SYMBOL.size() <<','<<k<< '\n';
        for( i1 = VAR.begin(), ii = VAR_SYMBOL.begin(); DAY <=NO_DAYS; ++i1, ++ii){
          fileout << YEAR << ", ";
          fileout << MONTH << ", ";
          //cout << YEAR<<','<<MONTH<<','<<NO_DAYS<<',' <<DAY<<','<<*i1<<','<<*ii << '\n';
          fileout << DAY++ << ", ";
          fileout << *i1 << ", ";
          fileout << *ii << "\n";
        }
      }
  }
  return 0;
}
