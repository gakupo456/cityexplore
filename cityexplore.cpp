//============================================================================
// Name        : CityExplore++!
// Author      : Tomás Ayala
// Version     : 1.0
// Copyright   : Free to copy under my authorization
// Description : C++ recruitment task
//============================================================================
#include <fstream>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <iomanip>
#include <typeinfo>
#include <exception>
#include <regex>
#include "json.hpp"
#include "cityexplore.h"

using json = nlohmann::json;

namespace CITY {
	struct cities {
		std::string cityName;
		std::string cityCounty;
		std::string voivodeship;
		float area;
		int population;
		int populationDensity;
	};
}

namespace FILTER {
	struct values {
		int intVal;
		std::map<int, int> mapVal;
		std::string reg_ex;
	};

	struct filters {
		std::string path;
		std::string operation;
		values VALUE;
	};
}
// I really wanted to use structs, but the because of the array, I had to let it go.

int loaded = 0, nFilters, nCities;
std::ifstream inputFile;
std::ifstream filtersFile;
json jFile;
json jFilter;

std::vector<CITY::cities> eachCity2;
std::vector<FILTER::filters> eachFilter2;

std::vector<std::map<std::string, std::string>> eachCity3;
std::vector<std::map<std::string, std::string>> eachFilter3;
std::vector<int> inCase;

Fl_Input *filter, *resulInput;
Fl_Text_Buffer *resbuffer = 0;
Fl_File_Browser *mainFileSelected, *jsonBrowser;
Fl_File_Chooser *fileChooser;
Fl_Button *button, *loadFiltersJson, *loadFileJson;
Fl_Text_Editor *resultsDisplay;
Fl_Tree *newTree;
Fl_Double_Window *win;
Fl_Menu_Bar *bar;

void compare(Fl_Widget *, void*);

void  mainFilePicker(void) {
	int fileNum;
	char relative[FL_PATH_MAX];
	char absolute[FL_PATH_MAX];

	if (filter->value()[0])
		fileChooser->filter(filter->value());
  		fileChooser->show();

  	while (fileChooser->visible()) {
  		Fl::wait();
  	}

  	fileNum = fileChooser->count();

  	if (fileNum > 0) {
  		mainFileSelected->clear();
  		for (int i = 1; i <= fileNum; i ++) {
  			if (!fileChooser->value(i))
  				break;

  		fl_filename_absolute(absolute, sizeof(absolute), fileChooser->value(i));
  		mainFileSelected->add(absolute);

  		inputFile.open(absolute);

  		inputFile >> jFile;

  		if (jFile.contains("cities") == true ) {

  			json arrElements = jFile.at("cities");


  			int nElemensInArr = arrElements.size();
  			for (int k = 0; k < nElemensInArr; k++ ) {
  				std::map<std::string, std::string> lol2;
  				for (auto aaa : arrElements[k].items()) {

  					std::string KEY = aaa.key();
  					std::string VALUE;
  					if (aaa.value().is_number()) {
  						VALUE = std::to_string(aaa.value().get<int>());
  					}
  					if (aaa.value().is_string()) {
  						VALUE = aaa.value();
  					}
  					lol2.insert(std::pair<std::string, std::string>(KEY, VALUE));
  				}
  				eachCity3.push_back(lol2);
  			}
       newTree->redraw();
       for (int i = 0; i < nElemensInArr; i++) {
    	   CITY::cities newcity;
    	   newcity.cityName = arrElements[i].at("city").get<std::string>();
    	   newcity.cityCounty = arrElements[i].at("country").get<std::string>();
    	   newcity.voivodeship = arrElements[i].at("voivodeship").get<std::string>();
		   newcity.area = arrElements[i].at("area").get<float>();
		   newcity.population = arrElements[i].at("population").get<int>();
		   newcity.populationDensity = arrElements[i].at("population_density").get<int>();
		   eachCity2.push_back(newcity);
       }

      newTree->changed();

      std::string firstStr;
      for (int i = 0; i < nElemensInArr; i++) {
    	  std::string firstStr = eachCity2[i].cityName; int num1=firstStr.size(); char firstArr[num1 +1];
    	  std::string secondStr = firstStr + "/" + "County: " + eachCity2[i].cityCounty; int num2 = secondStr.size(); char secondArr[num2+1];
    	  std::string thirdStr = firstStr + "/" + "Voivodeship: " + eachCity2[i].voivodeship; int num3 = thirdStr.size(); char thirdArr[num3+1];
    	  std::string fourthStr = firstStr + "/" + "Area: " + std::to_string(eachCity2[i].area); int num4 = fourthStr.size(); char fourthArr[num4+1];
    	  std::string fifthStr = firstStr + "/" + "Population: " + std::to_string(eachCity2[i].population);int num5 = fifthStr.size(); char fifthArr[num5+1];
    	  std::string sixthStr = firstStr + "/" + "Population Density: " + std::to_string(eachCity2[i].populationDensity);int num6 = sixthStr.size(); char sixthArr[num6+1];

    	  strcpy(firstArr, firstStr.c_str());
    	  newTree->add(firstArr)->close();
    	  strcpy(secondArr, secondStr.c_str());
    	  newTree->add(secondArr);
    	  strcpy(thirdArr, thirdStr.c_str());
    	  newTree->add(thirdArr);
    	  strcpy(fourthArr, fourthStr.c_str());
    	  newTree->add(fourthArr);
    	  strcpy(fifthArr, fifthStr.c_str());
    	  newTree->add(fifthArr);
    	  strcpy(sixthArr, sixthStr.c_str());
    	  newTree->add(sixthArr);
      }

      loaded++;
      loadFileJson->deactivate();
      if(loaded == 2)
    	  compare(resultsDisplay, 0);
      newTree->redraw();

    } else if (jFile.contains("cities") == false) {
    	inputFile.close();
  	  fl_message("INVALID FILE, NO CITIES FOUND, PLEASE TRY AGAIN");
    }

	mainFileSelected->redraw();
  	}
  newTree->redraw();
  }
}



void filterFilePicker(void) {
	int fileNum;
	char absolute[FL_PATH_MAX];
	if (filter->value()[0])
		fileChooser->filter(filter->value());
	fileChooser->show();
	while (fileChooser->visible()) {
		Fl::wait();
	}
	fileNum = fileChooser->count();
	if (fileNum > 0) {
		jsonBrowser->clear();
		for (int i = 1; i <= fileNum; i ++) {
			if (!fileChooser->value(i))
				break;
			fl_filename_absolute(absolute, sizeof(absolute), fileChooser->value(i));
			jsonBrowser->add(absolute);
			jsonBrowser->redraw();
			filtersFile.open(absolute);
			filtersFile >> jFilter;
			filtersFile.close();
			if(jFilter.contains("filters") == true) {
				json filterArrElements = jFilter.at("filters");
				int nFilterElemensInArr = filterArrElements.size();
				nFilters = nFilterElemensInArr;
				for (int k = 0; k < nFilters; k++) {
					std::map<std::string, std::string> lole;
					for (auto aaa : filterArrElements[k].items()){
						std::string KEY = aaa.key();
						std::string VALUE;
						if (aaa.value().is_number()) {
							VALUE = std::to_string(aaa.value().get<int>());
						}
						else if (aaa.value().is_string()) {
							VALUE = aaa.value();
							if (VALUE.at(0) == '/') {
								VALUE.erase(VALUE.begin());
							}
						}
						else if(aaa.value().is_array()) {
							std::string replace;
							int first = aaa.value()[0].get<int>(); int second = aaa.value()[1].get<int>(); inCase.push_back(first); inCase.push_back(second);
							VALUE = std::to_string(aaa.value()[0].get<int>()) + ", " + std::to_string(aaa.value()[1].get<int>());
						}
						lole.insert(std::pair<std::string, std::string>(KEY, VALUE));
					}
					eachFilter3.push_back(lole);
				}
				loaded++;
				loadFiltersJson->deactivate();
				if (loaded == 2)
					compare(resultsDisplay, 0);
			} else if (jFilter.contains("filters") == false) {
				fl_message("INVALID FILTERS FILE, NO COMPATIBLE FILTER FOUND");
			}
			jsonBrowser->redraw();
		}
	}
}

void abt(Fl_Widget *about) {
	fl_message("Author: Tomás Ayala\nFLTK is certainly great, simple and powerfully customizable\nBy the way, I love you, Oliwia,\n thank you for supporting me in all my endeavors");
}

void reset(Fl_Widget *btn){
	//all of these reset the tree, files loaded, information displayed, and it allows to pick again the files.
	loadFileJson->activate();loadFiltersJson->activate();inputFile.close();filtersFile.close();
	jsonBrowser->clear();jsonBrowser->redraw();mainFileSelected->clear();jFile = 0;jFilter = 0;
	newTree->clear();newTree->redraw();loaded = 0;resultsDisplay->clear_output();resultsDisplay->redraw();
	resbuffer->text("");
}

bool comparisonCase(std::string comp, std::string leftval, std::string rightval) {

	bool lole = false;
	if (comp == "gt") {
		if (std::stoi(leftval) < std::stoi(rightval)) {
			lole = true;
		}
	} else

	if (comp == "lt") {
		if (std::stoi(leftval) > std::stoi(rightval))
			lole = true;
	} else

	if (comp == "ge") {
		if (std::stoi(leftval) <= std::stoi(rightval))
			lole = true;
	} else

	if (comp == "le") {
		if (std::stoi(leftval) >= std::stoi(rightval))
			lole = true;
	} else

	if (comp == "eq") {
		if (std::stoi(leftval) == std::stoi(rightval))
			lole = true;
	}

	return lole;
}

std::string comparisonStr(std::string comp) {

	if (comp == "gt")
		return " greater than ";
	else if (comp == "lt")
		return " lesser than ";
	else if (comp == "ge")
		return " greater than and/or equal to ";
	else if (comp == "le")
		return " lesser than and/or equal to ";
	else if (comp == "eq")
		return " equal to ";
}



void compare(Fl_Widget *, void *) {

	std::string AAAAA;

	for (int c = 0; c < eachCity3.size(); c++){
		for (int f = 0; f < eachFilter3.size(); f++) { //for each city in the vector
			if (eachCity3[c].count(eachFilter3[f]["path"]) > 0) {
				std::string operation = eachFilter3[f]["op"];
				if (operation == "gt" || operation == "lt" || operation == "le" || operation == "ge" || operation == "eq"){
					if (comparisonCase(operation, eachFilter3[f]["value"] , eachCity3[c].find(eachFilter3[f]["path"])->second) == true)
					{
						AAAAA += "Found result for: " + eachFilter3[f]["path"] + " " + comparisonStr(operation) + " " + eachFilter3[f]["value"]+ ": " + "\n" ;
						AAAAA += eachCity3[c]["city"] +  " --> "  + eachFilter3[f]["path"] + ": " + eachCity3[c].find(eachFilter3[f]["path"])->second + "\n";
					}
				}
				else if (operation == "in") {
					std::string str = eachCity3[c].find(eachFilter3[f]["path"])->second;
					int num = std::stoi(str);
					if (inCase[0] < num && inCase[1] > num) {
						AAAAA += "Found result for: " + eachFilter3[f]["path"] + " in between " + std::to_string(inCase[0]) + " and " + std::to_string(inCase[1]);
						AAAAA += "\n" + eachCity3[c]["city"] + " --> " + eachFilter3[f]["path"] + ": " + eachCity3[c].find(eachFilter3[f]["path"])->second + "\n";
					}
				}
				else if (operation == "re") {
					std::string lole = eachFilter3[f]["value"];
					std::regex regexp;
					regexp = lole;
					std::string strToFind = eachCity3[c].find(eachFilter3[f]["path"])->second;
					if (std::regex_match(strToFind, regexp)) {
						AAAAA+= "Found result for: " + eachFilter3[f]["path"] + " for the regular expression " + "\"" +
								lole + "\" \n" + eachCity3[c]["city"] + " --> " + eachFilter3[f]["path"] + ": " + eachCity3[c].find(eachFilter3[f]["path"])->second + "\n";
					}
				}
			}
		}
	}
	char ARR[AAAAA.size()+1];
	strcpy(ARR, AAAAA.c_str());
	resbuffer->append(ARR);
}

void closeCallBack(Fl_Widget*, void*) {exit(0);}

int main(int  argc,char *argv[]) {

	win = new Fl_Double_Window(50, 50, 1000, 740, "City-Explore++");
	bar = new Fl_Menu_Bar(0, 0, 1000, 20);
	bar->color(FL_LIGHT2);
	bar->add("About");
	bar->callback(abt);

	newTree = new Fl_Tree(15, 55, 300, 500, "Items in file: ");

	Fl_Tabs* mainTabs = new Fl_Tabs(350, 40, 775, 590);
	Fl_Group* startTab = new Fl_Group(320, 65, 770, 565, "Main");

	loadFiltersJson = new Fl_Button(520, 75, 120, 30, "Load Filters File");
	loadFiltersJson->callback((Fl_Callback*)filterFilePicker);

	loadFileJson = new Fl_Button(360, 75, 140, 30, "Load main JSON file");
	loadFileJson->callback((Fl_Callback *)mainFilePicker);

	resbuffer = new Fl_Text_Buffer;

	resultsDisplay = new Fl_Text_Editor(360, 200, 550, 420, "Results" );
	resultsDisplay->textfont(FL_COURIER);
	resultsDisplay->buffer(resbuffer);
	resultsDisplay->show();

	Fl_Button* resetBtn = new Fl_Button(925, 180, 50, 40, "Reset");
	resetBtn->callback(reset);

	Fl_Box* instructions = new Fl_Box(650, 75, 340, 100);
	instructions->box(FL_DOWN_BOX);
	instructions->label("Welcome. You must press first the \nLOAD MAIN JSON FILE button, to load the elements,\n then press the LOAD FILTERS FILE button,\n to find the compatible elements");

	fileChooser = new Fl_File_Chooser(".", "*", Fl_File_Chooser::SINGLE, "Pick JSON file");

	mainFileSelected = new Fl_File_Browser(360, 115, 125, 40, "Main file");

	jsonBrowser = new Fl_File_Browser(515, 115, 125, 40, "Filter file");

	filter = new Fl_Input(50, 10, 315, 25, "Filter:");
	filter->hide();
	int argn = 1;
	if (argc > argn)
		filter->value(argv[argn]);
	else
		filter->value("JSON Files (*.json)");

	startTab->end();

	win->show();
	return Fl::run();
}
