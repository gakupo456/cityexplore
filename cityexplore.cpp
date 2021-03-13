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
//#include <type_traits>
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
int loaded = 0, nFilters, nCities;
std::ifstream inputFile;
std::ifstream filtersFile;
json jFile;
json jFilter;

std::vector<CITY::cities> eachCity2;
std::vector<FILTER::filters> eachFilter2;

std::vector<std::map<std::string, std::string>> eachCity3;
std::vector<std::map<std::string, std::string>> eachFilter3;

Fl_Input *filter, *resulInput;
Fl_Text_Buffer *resbuffer = 0;
Fl_File_Browser *mainFileSelected, *jsonBrowser;
Fl_File_Chooser *fileChooser;
Fl_Button *button, *loadFiltersJson, *loadFileJson;
Fl_Text_Editor *resultsDisplay;
//Fl_Text_Display *resultsDisplay;
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
      for (auto aaa : arrElements[k].items())
      {
    	  std::string KEY = aaa.key();
    	  std::string VALUE;
    	  if (aaa.value().is_number()) {
    		  VALUE = std::to_string(aaa.value().get<int>());
    	  }
    	  if (aaa.value().is_string()) {
    		  VALUE = aaa.value();
    	  }
    	  std::map<std::string, std::string> lol2;
    	  lol2[KEY]=VALUE;
    	  eachCity3.push_back(lol2);
//          std::cout << "key: " << aaa.key() << ", value:" << aaa.value() << '\n';
      }
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
            for (auto aaa : filterArrElements[k].items())
            {
          	  std::string KEY = aaa.key();
          	  std::string VALUE;

          	  if (aaa.value().is_number()) {
          		  VALUE = std::to_string(aaa.value().get<int>());
          	  }
          	  else if (aaa.value().is_string() && aaa.key() == "path") {
          		  VALUE = aaa.value();
          		  VALUE.erase(VALUE.begin());
          		  std::cout << VALUE << std::endl;
          	  }
          	  else if (aaa.value().is_string()) {
          		  VALUE = aaa.value();
          	  }
          	  else if(aaa.value().is_array()) {
          		 std::string replace;
          		 int first = aaa.value()[0].get<int>(); int second = aaa.value()[1].get<int>();
          		 VALUE = aaa.value()[0].get<int>() + ", " + aaa.value()[1].get<int>();
          		 std::cout << aaa.value()[0] << std::endl;
          		std::cout << aaa.value()[1] << std::endl;

//          		 std::cout << VALUE << std::endl;
          	  }
          	  std::map<std::string, std::string> lol2;
          	  lol2[KEY]=VALUE;
          	  eachFilter3.push_back(lol2);
//                std::cout << "key: " << aaa.key() << ", value:" << aaa.value() << '\n';
            }
           }

      for (int i = 0; i < nFilterElemensInArr; i++){

    	 json jVALUE = filterArrElements[i];

    	 std::map<int, int> vPlaceHolder;
    	 int intPlaceHolder;
    	 std::string sPLaceHolder;

    	 if (jVALUE["value"].is_array()) {
    		 int a = jVALUE["value"].at(0).get<int>();
    		 int b = jVALUE["value"].at(1).get<int>();
    		 vPlaceHolder.insert(std::pair<int, int>(a, b));
    		 FILTER::filters newFilter;
    		 newFilter.path = filterArrElements[i].at("path").get<std::string>();
			 newFilter.operation = filterArrElements[i].at("op").get<std::string>();
			 newFilter.VALUE.intVal = 0;
			 newFilter.VALUE.mapVal = vPlaceHolder;
			 newFilter.VALUE.reg_ex = "array";
			 eachFilter2.push_back(newFilter);
    	 } else if (jVALUE["value"].is_number()) {
    		 intPlaceHolder = jVALUE["value"] ;
    		 FILTER::filters newFilter;
    		 newFilter.path = filterArrElements[i].at("path").get<std::string>();
    	 	 newFilter.operation = filterArrElements[i].at("op").get<std::string>();
			 newFilter.VALUE.intVal = intPlaceHolder;
			 newFilter.VALUE.mapVal = vPlaceHolder;
			 newFilter.VALUE.reg_ex = "integer";
			 eachFilter2.push_back(newFilter);
    	 } else if (jVALUE["value"].is_string()) {
    		  sPLaceHolder = jVALUE["value"];
    		  FILTER::filters newFilter;
    		  newFilter.path = filterArrElements[i].at("path").get<std::string>();
    		  newFilter.operation = filterArrElements[i].at("op").get<std::string>();
			  newFilter.VALUE.intVal = 0;
			  newFilter.VALUE.mapVal =	vPlaceHolder;
			  newFilter.VALUE.reg_ex =	sPLaceHolder;
			  eachFilter2.push_back(newFilter);
    	  }
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

void compare(Fl_Widget *, void *) {

	// a) remove "/"s b) check if the filter key exists in city vector element
	// c) check to see if the filter operation includes this element
	//print if it does include the element

	std::string AAAAA;

	for (int i = 0; i < eachFilter3.size(); i++) {
		for (auto& k : eachFilter3[i]) {
			AAAAA += k.first + " : " + k.second + "\n";

			std::cout <<  k.first << "    " << k.second << std::endl;
		}
	}

	for (int m = 0; m < eachCity3.size(); m++) {
		for (auto& p : eachCity3[m]) {
			AAAAA += p.first + "     " + p.second + "\n";

			std::cout << p.first << "      " << p.second << std::endl;
		}
	}

	char ARR[AAAAA.size()+1];
	strcpy(ARR, AAAAA.c_str());
	resbuffer->append(ARR);
//	resultsDisplay->redraw();
//	resultsDisplay->buffer(resbuffer);
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
//	 resultsDisplay->activate();
//	 resultsDisplay->textsize(TS);
	 resultsDisplay->buffer(resbuffer);
//	 resultsDisplay->wrap();
//	 resultsDisplay->callback((Fl_Callback *)compare);
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
