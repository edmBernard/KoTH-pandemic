
#include <functional>
#include <array>

#pragma once

enum class Action {
  kDoNoThing,               // Do Nothing
  kResearchEpidemiology,  // Research Epidemiology [Effects: Reduce local Contagion Rate by 8%]
  kResearchImmunology,    // Research Immunology [Effects: Reduce local Lethality Rate by 4%]
  kResearchVaccination,   // Research Vaccination [Effects: Reduce local Contagion Rate by 4%, reduce local Lethality Rate by 2%]
  kGiveCure,    // Give Cure [Effects: Convert 10 local Infected to Healthy]
  kQuarantine,  // Quarantine [Effects: Isolate 30 local Infected for one Turn, they will be remove from the Contagion computation]
  kKill,        // Kill [Effects: Remove 30 local Infected]
};

using BotReturn = std::array<Action, 3>;
using BotFunction = std::function<BotReturn(int, int, int)>;

struct City {
  int healty = 99;
  int infected = 1;
  int isolated = 0;
  int contagionRate = 5;
  int lethalityRate = 5;
};
