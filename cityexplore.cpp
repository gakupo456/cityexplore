//============================================================================
// Name        : cityexplore.cpp
// Author      : Tomás Ayala
// Version     :0.02
// Copyright   : Free to copy under my authorization
// Description : c++ recruitment task
//============================================================================
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <iomanip>
#include "nlohmann/json.hpp"
#include "Windowbuild.h"

using json = nlohmann::json;

int main() {

	std::ifstream file("/home/spurdo/cppworkspace/cityexplore/src/cities.json");
	json jFile;
	file >> jFile;

	json arrElements = jFile.at("cities");

	for (int i = 0; i < 22; ++i) {
//	[i] =  arrElements[i].at("city");
		std::cout << "city: " << arrElements[i].at("city") << std::endl;
		std::cout << "voivodeship:  " << arrElements[i].at("voivodeship") << std::endl;
	}

	 Fl_Double_Window* win = new Fl_Double_Window(50, 50, 1000, 740, "City-Explore++");
	 Fl_Menu_Bar* bar = new Fl_Menu_Bar(0, 0, 1000, 20);
	      bar->color(FL_LIGHT2);
	 bar->add("Load");
	 bar->add("About");
	 Fl_Group* grp = new Fl_Group(240, 60, 730, 565);
	      grp->box(FL_GTK_DOWN_BOX);
	      grp->color(FL_LIGHT2);
	      grp->end();

	 Fl_Choice* feature = new Fl_Choice(290, 35, 125, 20, "Feature:");
	      feature->add("lole");
	      feature->add("BEPIS!!!");
	      feature->down_box(FL_BORDER_BOX);
	 std::string benis = " ";
	 Fl_Tree* tree = new Fl_Tree(10, 40, 200, 350);
	  	    tree->begin();
	  	    int count = 0;
	  	    for (auto iter = arrElements.begin(); iter != arrElements.end(); ++iter) {

//	  	    	std::vector<std::string> lolxd = {
//	  	    			arrElements[count].at("city"),
//						arrElements[count].at("country"),
//						arrElements[count].at("voivodeship"),
//						arrElements[count].at("area").get<std::string>(),
//						arrElements[count].at("population").get<std::string>(),
//						arrElements[count].at("population_density").get<std::string>(),
//	  	    	};
	  	    	std::string lole = arrElements[count].at("voivodeship");
//	  	    	std::string lole1 = arrElements[count].at("country");
//	  	    	std::string lole2 = arrElements[count].at("voivodeship");
//	  	    	std::string lole3 = arrElements[count].at("area");
//	  	    	std::string lole4 = arrElements[count].at("population");
//	  	    	std::string lole5 = arrElements[count].at("population_density");

//	  	    	for(int i = 0; i < 5; ++i) {
	  	    	int n = lole.length();

	  	    	char label[n+1];

	  	    	strcpy(label, lole.c_str());


	  	    	tree->add(label);
//	  	    	tree->add("benis");
//	  	    	tree->add("benis", label);
	  	    	++count;
//	  	    	}
	  	    }

	  	    tree->end();

	 win->show();

	std::cout << "epic" << std::endl;

//	std::cout << jCities.at(1) << std::endl;
	return Fl::run();
//	return 0;
}
