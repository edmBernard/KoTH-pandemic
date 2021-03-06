
#include <functional>
#include <array>

#pragma once

enum class Action {
  kDoNoThing,             // Do Nothing
  kResearchEpidemiology,  // Research Epidemiology [Effects: Reduce local Contagion Rate by 8%]
  kResearchImmunology,    // Research Immunology [Effects: Reduce local Lethality Rate by 4%]
  kResearchVaccination,   // Research Vaccination [Effects: Reduce local Contagion Rate by 4%, reduce local Lethality Rate by 2%]
  kGiveCure,    // Give Cure [Effects: Convert 10 local Infected to Healthy]
  kQuarantine,  // Quarantine [Effects: Isolate 30 local people for one Turn, they will be remove from the Contagion computation]
  kKill,        // Kill [Effects: Remove 30 local Infected]
};

constexpr int kValueResearchEpidemiology = 8;
constexpr int kValueResearchImmunology = 4;
constexpr int kValueResearchVaccination = 2;
constexpr int kValueGiveCure = 10;
constexpr int kValueQuarantine = 30;
constexpr int kValueKill = 30;

using BotReturn = std::array<Action, 3>;
using BotFunction = std::function<BotReturn(int, int, int)>;

struct City {
  int healthy = 99;
  int infected = 1;
  int healthyIsolated = 0;
  int infectedIsolated = 0;
  int contagionRate = 5;
  int lethalityRate = 5;
};

inline void decrease(int &variable, int value) {
  if (variable > value) {
    variable -= value;
  } else {
    variable = 0;
  }
}

inline void doAction(Action action, City &city) {

  switch (action) {
    case Action::kResearchEpidemiology:
      decrease(city.contagionRate, kValueResearchEpidemiology);
      break;

    case Action::kResearchImmunology:
      decrease(city.lethalityRate, kValueResearchImmunology);
      break;

    case Action::kResearchVaccination:
      decrease(city.contagionRate, kValueResearchVaccination * 2);
      decrease(city.lethalityRate, kValueResearchVaccination);
      break;

    case Action::kGiveCure: {
        const int cured = city.infected >= kValueGiveCure ? kValueGiveCure : city.infected;
        city.infected -= cured;
        city.healthy += cured;
      }
      break;
    case Action::kQuarantine: {
        if (city.infected + city.healthy == 0) {
          break;
        }
        const int healthyIsolated = kValueQuarantine * city.healthy / (city.infected + city.healthy);
        decrease(city.healthy, healthyIsolated);
        city.healthyIsolated = healthyIsolated;
        const int infectedIsolated = kValueQuarantine - healthyIsolated;
        decrease(city.infected, infectedIsolated);
        city.infectedIsolated = infectedIsolated;
      }
      break;
    case Action::kKill:
      decrease(city.infected, kValueKill);
      break;
  }
}
