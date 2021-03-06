
#include <functional>
#include <array>

#pragma once

enum class Action {
  kDoNoThing,             // Do Nothing
  kResearchMicrobiology,  // Research Microbiology [Effects: Reduce local Infection Rate by 4]
  kResearchEpidemiology,  // Research Epidemiology [Effects: Reduce local Contagion Rate by 8%]
  kResearchImmunology,    // Research Immunology [Effects: Reduce local Lethality Rate by 4%]
  kResearchVaccination,   // Research Vaccination [Effects: Reduce local Infection Rate by 2, Reduce local Contagion Rate by 4%, reduce local Lethality Rate by 2%]
  kGiveCure,      // Give Cure [Effects: Convert 10 local Infected to Healthy]
  kQuarantine,    // Kill [Effects: Remove 30 local Infected]
  kOpenBorder,    // Open Borders [Effects: Increase local Migration Rate by 10%]
  kCloseBorder,   // Close Borders [Effects: Decrease local Migration Rate by 10%]
  kBioTerrorism,  // BioTerrorism [Effects: Convert 4 global Healthy to Infected]
  kDissemination, // Dissemination [Effects: Increase global Infection Rate by 1, increase global Contagion Rate by 2%]
  kWeaponization, // Weaponization [Effects: Increase global Infection Rate by 1, increase global Lethality Rate by 2%]
  kPacification,  // Pacification [Effects: Decrease global Infection Rate by 1, decrease global Contagion Rate by 1%, decrease global Lethality Rate by 1%]
};

// Local modifier
constexpr int kLocalResearchMicrobiology = 4;
constexpr int kLocalResearchEpidemiology = 8;
constexpr int kLocalResearchImmunology = 4;
constexpr int kLocalGiveCure = 10;
constexpr int kLocalQuarantine = 30;
constexpr int kLocalKill = 30;
constexpr int kLocalOpenBorder = 10;
constexpr int kLocalCloseBorder = 10;

// Global modifier
constexpr int kGlobalInfectionRate = 1;
constexpr int kGlobalContagionRate = 2;
constexpr int kGlobalLethalityRate = 2;


struct City {
  int healthy = 99;
  int infected = 1;
  int infectionRate = 5;
  int contagionRate = 5;
  int lethalityRate = 5;
  int migrationRate = 5;
};


using BotReturn = std::array<Action, 3>;
using BotFunction = std::function<BotReturn(int, int, const City&)>;


inline void decrease(int &variable, int value) {
  if (variable > value) {
    variable -= value;
  } else {
    variable = 0;
  }
}


[[nodiscard]]
inline bool doLocalAction(Action action, City &city) {

  switch (action) {
    case Action::kResearchEpidemiology:
      decrease(city.contagionRate, kLocalResearchEpidemiology);
      break;

    case Action::kResearchImmunology:
      decrease(city.lethalityRate, kLocalResearchImmunology);
      break;

    case Action::kResearchVaccination:
      decrease(city.contagionRate, kLocalResearchMicrobiology / 2);
      decrease(city.lethalityRate, kLocalResearchEpidemiology / 2);
      decrease(city.infectionRate, kLocalResearchImmunology / 2);
      break;

    case Action::kGiveCure: {
        const int cured = city.infected >= kLocalGiveCure ? kLocalGiveCure : city.infected;
        city.infected -= cured;
        city.healthy += cured;
      }
      break;
    case Action::kQuarantine:
      decrease(city.infected, kLocalKill);
      break;
    case Action::kOpenBorder:
      city.migrationRate += kLocalOpenBorder;
      break;
    case Action::kCloseBorder:
      decrease(city.migrationRate, kLocalCloseBorder);
      break;
    case Action::kDoNoThing:
      break;
    default:
      // if it's a global action
      return false;
  }
  return true;
}

inline void doGlobalAction(const std::vector<Action> &actions, std::vector<City> &cities) {
  int bioTerrorismCounter = 0;
  for (auto action : actions) {
    for (auto &city : cities) {
      switch(action) {
        case Action::kBioTerrorism: {
            const int toInfect = city.healthy >= kGlobalInfectionRate * 4 ? kGlobalInfectionRate * 4 : city.healthy;
            city.infected += toInfect;
            city.healthy -= toInfect;
          }
          break;
        case Action::kWeaponization:
          city.infectionRate += kGlobalInfectionRate;
          city.lethalityRate += kGlobalLethalityRate;
          break;
        case Action::kDissemination:
          city.infectionRate += kGlobalInfectionRate;
          city.contagionRate += kGlobalContagionRate;
          break;
        case Action::kPacification:
          decrease(city.infectionRate, kGlobalInfectionRate);
          decrease(city.contagionRate, kGlobalContagionRate / 2);
          decrease(city.lethalityRate, kGlobalLethalityRate / 2);
          break;
      }
    }
  }
}