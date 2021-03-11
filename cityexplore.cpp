//============================================================================
// Name        : cityexplore.cpp
// Author      : Tomás Ayala
// Version     :0.02
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
#include "nlohmann/json.hpp"
#include "cityexplore.h"


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


using json = nlohmann::json;

//int loaded = 0;

namespace CITY {
	struct cities {
		std::string cityName;
		std::string cityCounty;
		std::string voivodeship;
		double area;
		long population;
		int populationDensity;
	};
}

//const unsigned int lengthFile = 8192;
//char * buffer[lengthFile];

std::ifstream inputFile("/home/spurdo/cppworkspace/cityexplore/src/cities.json");
std::filebuf * fileBuffer = inputFile.rdbuf();

void selectedTreeItem(Fl_Widget*, void*) {

	Fl_Choice* benis = new Fl_Choice(200, 200, 250, 250, "haha bepis!");

}



void cb(Fl_Widget *ob) {
  printf("Callback for %s '%s'\n",ob->label(),((Fl_Input*)ob)->value());
}

void searchLookUp(){



}

void resultShow(Fl_Widget *res);

void lookUp(Fl_Widget *o) {

	std::cout << "HAHA BEPIS!!!" << std::endl;

}

void abt(Fl_Widget *about) {

	fl_message("Author: Tomás Ayala\nFLTK is certainly great, simple and powerfully customizable\nBy the way, I love you, Oliwia,\n thank you for supporting me in all my endeavors");

}

void resultShow(Fl_Widget *res) {

	 printf("Callback for %s '%s'\n",res->label(),((Fl_Text_Display*)res));

}

int main() {

	std::ifstream file("/home/spurdo/cppworkspace/cityexplore/src/cities.json");
	json jFile;
	file >> jFile;

	json arrElements = jFile.at("cities");

	int nElemensInArr = arrElements.size();

	CITY::cities eachCity[nElemensInArr+1];

	for (int i = 0; i < nElemensInArr; i++){
		eachCity[i] = {
		arrElements[i].at("city").get<std::string>(),
		arrElements[i].at("country").get<std::string>(),
		arrElements[i].at("voivodeship").get<std::string>(),
		arrElements[i].at("area").get<double>(),
		arrElements[i].at("population").get<long>(),
		arrElements[i].at("population_density").get<int>()
		};
	}

	Fl_Double_Window* win = new Fl_Double_Window(50, 50, 1000, 740, "City-Explore++");
	Fl_Menu_Bar* bar = new Fl_Menu_Bar(0, 0, 1000, 20);
	bar->color(FL_LIGHT2);
	bar->callback(abt);
	bar->add("About");
	Fl_Tabs* mainTabs = new Fl_Tabs(350, 40, 775, 590);
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
	Fl_Text_Display* txtChoice = new Fl_Text_Display(325, 160, 150, 50, "Comparision: ");
	txtChoice->box(FL_NO_BOX);
//	Fl_Text_Display* benis = new Fl_Text_Display(X, Y, W, H, l);
	filterChoice->end();
	Fl_Button* searchBtn = new Fl_Button(555, 160, 60, 80, "Search");
	searchBtn->show();
	searchBtn->callback(lookUp);

//	Fl_Group* results = new Fl_Group(360, 260, 600, 350);
//	results->box(FL_DOWN_BOX);
//	results->end();


	 Fl_Text_Display* result = new Fl_Text_Display(360, 260, 600, 350);
	 result->callback(resultShow);

	 result->box(FL_DOWN_BOX);


	advTab->end();
	mainTabs->end();

	Fl_Tree* newTree = new Fl_Tree(20, 40, 325, 600);
	newTree->begin();

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
	newTree->end();
	win->show();

	return Fl::run();
}
