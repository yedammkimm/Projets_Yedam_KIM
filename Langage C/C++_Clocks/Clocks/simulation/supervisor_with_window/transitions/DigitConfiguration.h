#ifndef DIGITCONFIGURATION_H
#define DIGITCONFIGURATION_H

#include <map>
#include <vector>
#include <utility>
#include "Transition.h"


//Defines clock hand positions for digits 0–9
using namespace std;


// Static configurations for each digit/character
extern const map<string, vector<pair<float, float>>> digitConfigurations;

// Converts a string like "1234" into clock-hand angles for display
vector<vector<pair<float, float>>> getTextAngles(const std::string& text);



#endif // DIGITCONFIGURATION_H
