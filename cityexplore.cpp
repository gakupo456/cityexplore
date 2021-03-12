//============================================================================
// Name        : cityexplore.cpp
// Author      : Tomás Ayala
// Version     : 0.7
// Copyright   : Free to copy under my authorization
// Description : c++ recruitment task
//============================================================================
#include <fstream>
#include <map>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <iomanip>
#include <type_traits>
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
		long population;
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
int loaded = 0;
std::ifstream inputFile;
std::ifstream filtersFile;
json jFile;
json jFilter;

Fl_Input *filter;
Fl_File_Browser *mainFileSelected, *jsonBrowser;
Fl_File_Chooser *fileChooser;
Fl_Button *button;
Fl_Text_Display *mainFileDisplay, *filterFileDisplay;
Fl_Tree *newTree;
Fl_Double_Window *win;
Fl_Menu_Bar *bar;

void choice_cb(Fl_Widget *w, void *userdata) {
  // Show info about the picked item
  Fl_Input_Choice *choice = (Fl_Input_Choice*)w;
  const Fl_Menu_Item *item = choice->menubutton()->mvalue();
  printf("*** Choice Callback:\n");
  printf("    item label()='%s'\n", item ? item->label() : "(No item)");
  printf("    item value()=%d\n", choice->menubutton()->value());
  printf("    input value()='%s'\n", choice->input()->value());
  printf("    The user %s\n", choice->menubutton()->changed()
                                  ? "picked a menu item"
                                  : "typed text");
}

void fileChooserCallback(Fl_File_Chooser *fileChooser, void *data) {
  const char *filename;
  printf("fileChooser_callback(fileChooser = %p, data = %p)\n", fileChooser, data);
  filename = fileChooser->value();
  printf("    filename = \"%s\"\n", filename ? filename : "(null)");
}
void compare();

void mainFilePicker(void) {
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

       CITY::cities eachCity[nElemensInArr+1];

       newTree->redraw();

       for (int i = 0; i < nElemensInArr; i++){
       	eachCity[i] = {
       	arrElements[i].at("city").get<std::string>(),
       	arrElements[i].at("country").get<std::string>(),
       	arrElements[i].at("voivodeship").get<std::string>(),
       	arrElements[i].at("area").get<float>(),
       	arrElements[i].at("population").get<long>(),
       	arrElements[i].at("population_density").get<int>()
       	};
       }

      newTree->changed();

      std::string firstStr;
        for (int i = 0; i < nElemensInArr; i++) {
        std::string firstStr = eachCity[i].cityName; int num1=firstStr.size(); char firstArr[num1 +1];
        std::string secondStr = firstStr + "/" + "County: " + eachCity[i].cityCounty; int num2 = secondStr.size(); char secondArr[num2+1];
        std::string thirdStr = firstStr + "/" + "Voivodeship: " + eachCity[i].voivodeship; int num3 = thirdStr.size(); char thirdArr[num3+1];
        std::string fourthStr = firstStr + "/" + "Area: " + std::to_string(eachCity[i].area); int num4 = fourthStr.size(); char fourthArr[num4+1];
        std::string fifthStr = firstStr + "/" + "Population: " + std::to_string(eachCity[i].population);int num5 = fifthStr.size(); char fifthArr[num5+1];
        std::string sixthStr = firstStr + "/" + "Population Density: " + std::to_string(eachCity[i].populationDensity);int num6 = sixthStr.size(); char sixthArr[num6+1];
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
        if(loaded == 2)
       compare();
       newTree->redraw();
//       newTree->changed();
    } else if (jFile.contains("cities") == false) {
    	inputFile.close();
  	  fl_message("INVALID FILE, NO CITIES FOUND, PLEASE TRY AGAIN");
//  	  throw "INVALID FILE; NO CITIES FOUND!";

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

      FILTER::filters eachFilter[nFilterElemensInArr+1];

      for (int i = 0; i < nFilterElemensInArr; i++){

    	 json jVALUE = filterArrElements[i];

    	 std::map<int, int> vPlaceHolder;
    	 int intPlaceHolder;
    	 std::string sPLaceHolder;

    	 if (jVALUE["value"].is_array()) {
    		 int a = jVALUE["value"].at(0).get<int>();
    		 int b = jVALUE["value"].at(1).get<int>();
    		 vPlaceHolder.insert(std::pair<int, int>(a, b));
    		 eachFilter[i] = {
    		   	filterArrElements[i].at("path").get<std::string>(),
				filterArrElements[i].at("op").get<std::string>(),
				{
					0,
					vPlaceHolder,
					"v"
				}
    		 };
    	 } else if (jVALUE["value"].is_number()) {
    		 intPlaceHolder = jVALUE["value"] ;
    		 eachFilter[i] = {
    	    	filterArrElements[i].at("path").get<std::string>(),
    	 		filterArrElements[i].at("op").get<std::string>(),
				{
					intPlaceHolder,
					vPlaceHolder,
					"i"
				}
    		 };
    	 } else if (jVALUE["value"].is_string()) {
    		  sPLaceHolder = jVALUE["value"];
    		  eachFilter[i] = {
    		   	filterArrElements[i].at("path").get<std::string>(),
    		 	filterArrElements[i].at("op").get<std::string>(),
				{
					0,
					vPlaceHolder,
					sPLaceHolder
				}
    		 };
    	  }
      }

      loaded++;
      if (loaded == 2)
      compare();
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

void compare() {



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

	Fl_Button* loadFiltersJson = new Fl_Button(520, 75, 120, 30, "Load Filters File");
	loadFiltersJson->callback((Fl_Callback*)filterFilePicker);

	Fl_Button* loadFileJson = new Fl_Button(360, 75, 140, 30, "Load main JSON file");
	loadFileJson->callback((Fl_Callback *)mainFilePicker);

	mainFileDisplay = new Fl_Text_Display(360, 200, 550, 420, "Results" );
	  mainFileDisplay->box(FL_DOWN_BOX);
	  mainFileDisplay->show();

	Fl_Box* instructions = new Fl_Box(650, 75, 340, 100);
	instructions->box(FL_DOWN_BOX);
	instructions->label("Welcome. You must press first the \nLOAD MAIN JSON FILE button, to load the elements,\n then press the LOAD FILTERS FILE button,\n to find the compatible elements");
	//unfortunately FLTK doesn't allow me to either concatenate or divide the string, so here's a long one

	fileChooser = new Fl_File_Chooser(".", "*", Fl_File_Chooser::SINGLE, "Pick JSON file");
//	fileChooser->callback(fileChooserCallback);

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

	Fl_Group* advTab = new Fl_Group(350, 65, 770, 565, "Advanced Search");
	Fl_Input_Choice* firstChoice = new Fl_Input_Choice(355, 105, 250, 25);

	firstChoice->callback(choice_cb, 0);		//add("Voivodeship"); //(e.g.: Poznan)
	firstChoice->add("County");		//add("County"); //(e.g.: Wielkopolskie)
	firstChoice->add("Voivodeship");		//add("Area"); // (in KMs, e.g.: 200)
	firstChoice->add("Area");		//add("Population");// (e.g.: 123456)
	firstChoice->add("Population");		//add("Population Density");// (e.g.: 2315)
	firstChoice->add("Population Density");		//callback(cb);
	Fl_Input* txtInput = new Fl_Input(355, 220, 150, 25, "txtInput");
	txtInput->labeltype(FL_NO_LABEL);
	Fl_Input_Choice* filterChoice = new Fl_Input_Choice(355, 160, 160, 25);
	filterChoice->callback(choice_cb, 0);
	filterChoice->add("Equal To (=)");
	filterChoice->add("Greater than (>)");
	filterChoice->add("Greater or equal (>=)");
	filterChoice->add("Less than (<)");
	filterChoice->add("Less or equal (<=)");
	filterChoice->add("Within this value");

	Fl_Text_Display* txtOption = new Fl_Text_Display(310, 105, 150, 25, "Filter by: ");
	 txtOption->box(FL_NO_BOX);
	Fl_Text_Display* txtValue = new Fl_Text_Display(302, 220, 150, 25, "Value: ");
	txtValue->box(FL_NO_BOX);
	Fl_Text_Display* txtChoice = new Fl_Text_Display(325, 160, 150, 50, "Comparison: ");
	txtChoice->box(FL_NO_BOX);

	filterChoice->end();
	Fl_Button* searchBtn = new Fl_Button(555, 160, 60, 80, "Search");
	searchBtn->show();

	 Fl_Text_Display* result = new Fl_Text_Display(360, 260, 600, 350);

	 result->box(FL_DOWN_BOX);
	 advTab->end();
	mainTabs->end();

	win->show();
	return Fl::run();
}
