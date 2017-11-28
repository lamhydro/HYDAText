#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
//#include <algorithm>

using namespace std;

/*
Return the column number from where daily values are recorded.
*/
int getStartCol(std::string &VARIA){
  if (VARIA.compare("DLY_FLOWS") == 0){
    return int(11);
  }
  if (VARIA.compare("DLY_LEVELS") == 0 || VARIA.compare("SED_DLY_SUSCON") == 0){
    return int(12);
  }

  return 0;
}

/*
Return the column number from where daily values are recorded.
*/
int getNO_DAYSCol(std::string &VARIA){
  if (VARIA.compare("DLY_FLOWS") == 0){
    return int(4);
  }
  if (VARIA.compare("DLY_LEVELS") == 0 || VARIA.compare("SED_DLY_SUSCON") == 0){
    return int(5);
  }

  return 0;
}

/*
Record station information
*/
//int statInfo(std::string &STATION_NUMBER){
int statInfo(std::string &HYDATdir, std::string &outdir, std::vector<std::string> &STATION_NUMBER){

  std::fstream file(HYDATdir+"STATIONS.csv", ios::in);
  if(!file.is_open()){
    std::cout << "File not found!\n";
    return 1;
  }

  std::fstream fileout(outdir+"STATION_INFO.csv",ios::out);
  if(!fileout.is_open()){
    std::cout << "File not found!\n";
    return 1;
  }

  std::string csvLine;
  int j = 0;
  std::string STATION_NUM;
  for(std::vector<std::string>::iterator i = STATION_NUMBER.begin(); i != STATION_NUMBER.end(); ++i){
    STATION_NUM = "\""+*i+"\"";

    while( std::getline(file, csvLine) ){
    //for (size_t i = 0; i < 3; i++) {
      //std::getline(file, csvLine);
      std::istringstream csvStream(csvLine);
      //std::vector<std::string> csvColumn;
      std::string csvElement;

      // Write the header
      if (j++ == 0){
        fileout << csvLine << "\n";
      }

      // Write the station information
      std::getline(csvStream, csvElement, ',');
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



int extractTS(std::string &HYDATdir, std::string &outdir, std::string &STATION_NUMBER, std::string &VARIA) {

  std::fstream file(HYDATdir+VARIA+".csv", ios::in);
  if(!file.is_open()){
    std::cout << "File not found!\n";
    return 1;
  }

  std::string csvLine;

  std::fstream fileout(outdir+STATION_NUMBER+"_"+VARIA+".csv",ios::out);
  if(!fileout.is_open()){
    std::cout << "File not found!\n";
    return 1;
  }
  // Write header of file
  fileout << "YEAR" << ", ";
  fileout << "MONTH" << ", ";
  fileout << "DAY" << ", ";
  fileout << "VAR" << ", ";
  fileout << "VAR_SYMBOL" << "\n";

  std::string STATION_NUM = "\""+STATION_NUMBER+"\"";

  int startCol = getStartCol(VARIA);
  int NO_DAYSCol = getNO_DAYSCol(VARIA);

  // Start reading through file line by line
  while( std::getline(file, csvLine) ){
  		std::istringstream csvStream(csvLine);
  		std::vector<std::string> csvColumn;
  		std::string csvElement;

  		// read every element from the line that is seperated by commas
  		// and put it into the vector or strings
      std::getline(csvStream, csvElement, ',');
      if (csvElement.compare(STATION_NUM) == 0){
        csvColumn.push_back(csvElement);
        while( std::getline(csvStream, csvElement, ',') ){
          csvColumn.push_back(csvElement);
        }
        // This checks for a trailing comma with no data after it.
        if (!csvStream && csvElement.empty()){
                // If there was a trailing comma then add an empty element.
                csvColumn.push_back("");
        }

        std::string YEAR, MONTH;
        int NO_DAYS = 0;
        int j = 0, k = 0, j1 = 0;
        std::vector<std::string> VAR, VAR_SYMBOL;
        for(std::vector<std::string>::iterator i = csvColumn.begin(); ; ++i ){

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
        std::vector<std::string>::iterator i1, ii;
        //std::cout << VAR.size()<<','<<VAR_SYMBOL.size() <<','<<k<< '\n';
        for( i1 = VAR.begin(), ii = VAR_SYMBOL.begin(); DAY <=NO_DAYS; ++i1, ++ii){
          fileout << YEAR << ", ";
          fileout << MONTH << ", ";
          //std::cout << YEAR<<','<<MONTH<<','<<NO_DAYS<<',' <<DAY<<','<<*i1<<','<<*ii << '\n';
          fileout << DAY++ << ", ";
          fileout << *i1 << ", ";
          fileout << *ii << "\n";
        }
      }
  }
  return 0;
}



int extractTS_SED(std::string &HYDATdir, std::string &outdir, std::string &STATION_NUMBER, std::string &VARIA) {

  std::fstream file(HYDATdir+VARIA+".csv", ios::in);
  if(!file.is_open()){
    std::cout << "File not found!\n";
    return 1;
  }

  std::fstream fileout(outdir+STATION_NUMBER+"_"+VARIA+".csv",ios::out);
  if(!fileout.is_open()){
    std::cout << "File not found!\n";
    return 1;
  }
  // Write header of file
  fileout << "YEAR" << ", ";
  fileout << "MONTH" << ", ";
  fileout << "DAY" << ", ";
  fileout << "VAR" << "\n";

  std::string STATION_NUM = "\""+STATION_NUMBER+"\"";

  int startCol = getStartCol(VARIA);
  int NO_DAYSCol = getNO_DAYSCol(VARIA);

  // Start reading through file line by line
  std::string csvLine;
  while( std::getline(file, csvLine) ){
  		std::istringstream csvStream(csvLine);
  		std::vector<std::string> csvColumn;
  		std::string csvElement;

  		// read every element from the line that is seperated by commas
  		// and put it into the vector or strings
      std::getline(csvStream, csvElement, ',');
      if (csvElement.compare(STATION_NUM) == 0){
        csvColumn.push_back(csvElement);
        while( std::getline(csvStream, csvElement, ',') ){
          csvColumn.push_back(csvElement);
        }
        // This checks for a trailing comma with no data after it.
        if (!csvStream && csvElement.empty()){
                // If there was a trailing comma then add an empty element.
                csvColumn.push_back("");
        }

        std::string YEAR, MONTH;
        int NO_DAYS = 0;
        int j = 0, j1 = 0;
        std::vector<std::string> VAR;
        for(std::vector<std::string>::iterator i = csvColumn.begin(); i != csvColumn.end(); ++i){

          if (j == 1) YEAR = *i;
          if (j == 2) MONTH = *i;
          if (j == NO_DAYSCol) NO_DAYS = atoi((*i).c_str());

          if (j == startCol+j1){
            VAR.push_back(*i);
            j1++;
          }
          j++;

        }

        // Save into file
        int DAY = 1;
        std::vector<std::string>::iterator i1;
        //std::cout << VAR.size()<<','<<VAR_SYMBOL.size() <<','<<k<< '\n';
        for( i1 = VAR.begin(); DAY <=NO_DAYS; ++i1){
          fileout << YEAR << ", ";
          fileout << MONTH << ", ";
          //std::cout << YEAR<<','<<MONTH<<','<<NO_DAYS<<',' <<DAY<<','<<*i1 << '\n';
          fileout << DAY++ << ", ";
          fileout << *i1 << "\n";
        }
      }
  }
  return 0;
}



int main(int argc, char const *argv[]) {

  (void) argc;

  // Check existence of HYDAT.in file
  std::fstream file(argv[1], ios::in);
  if(!file.is_open()){
    std::cout << "File not found!\n";
    return 1;
  }

  std::string HYDATdir, outdir;

  std::string line;
  // Reading header file
  std::getline(file, line);
  std::getline(file, HYDATdir);
  std::cout << HYDATdir << '\n';
  std::getline(file, line);
  std::getline(file, outdir);
  std::cout << outdir << '\n';
  std::getline(file, line);


  std::vector<std::string> STATION_NUMBER;

  std::string element, STATION;
  while( std::getline(file, line) ){
    std::istringstream stream(line);
    std::vector<std::string> column;

    std::getline(stream, STATION, ',');
    std::cout << STATION << '\n';
    // Check is station is in station vector
    //if (std::find(STATION_NUMBER.begin(), STATION_NUMBER.end(), STATION) != STATION_NUMBER.end()){
    STATION_NUMBER.push_back(STATION);
    //}

    // Save varariable names for each station
    while( std::getline(stream, element, ',') ){
      column.push_back(element);
    }
    // Write time series for each station number
    for(std::vector<std::string>::iterator VARIA = column.begin(); VARIA != column.end(); ++VARIA){
      if ((*VARIA).compare("SED_DLY_SUSCON") == 0){
        extractTS_SED(HYDATdir, outdir, STATION, *VARIA);
      }else{
        extractTS(HYDATdir, outdir, STATION, *VARIA);
      }
      extractTS(HYDATdir, outdir, STATION, *VARIA);
    }
  }
  // Write station information
  statInfo(HYDATdir, outdir, STATION_NUMBER);

  return 0;
}
