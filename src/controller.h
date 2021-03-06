
#include <functional>
#include <numeric>
#include <random>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <assert.h>

#include <spdlog/spdlog.h>
#include <fmt/core.h>


#include "engine.h"

#pragma once

class Controller {
public:
  Controller(int numberRuns, int steps)
    : m_numberRuns(numberRuns)
    , m_steps(steps)
    , bots(GetBotRegister())
    , botIndex(bots.size(), 0)
    , cumulCities(bots.size(), {0,0,0,0,0,0,0})
  {
    std::iota(botIndex.begin(), botIndex.end(), 0);

    assert(botIndex.size() == bots.size());
    assert(cumulCities.size() == bots.size());

    spdlog::info("Number of bots registered : {}", bots.size());
  }

  void run() {

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> mutationActionChoice(0, 2);

    for (int runNumber = 1; runNumber <= m_numberRuns; ++runNumber) {
      spdlog::debug("Run Start {}", runNumber);
      std::vector<City> cities(bots.size());

      for (int step = 1; step <= m_steps; ++step) {
        spdlog::debug("Iteration : {:05}", step);

        // Phase 1: Mutation
        const int mutationAction = mutationActionChoice(gen);
        for (auto &city : cities) {
          switch (mutationAction) {
            case 0:
              city.contagionRate += 5;
              break;
            case 1:
              city.infectionRate += 2;
              break;
            case 2:
              city.lethalityRate += 5;
              break;
            default:
              throw std::runtime_error("Mutation action out of range");
          }
        }

        // Phase 2: Reproduction
        if (step % 5 == 0) {
          for (auto &city : cities) {
            city.healthy += city.healthy / 2;
            city.infected += city.infected / 2;
          }
        }

        // Phase 3: Migration
        int healthyMigrant = 0;
        int infectedMigrant = 0;
        int totalMigrationRate = 0;
        for (auto &city : cities) {
          const int healthyToLeave = static_cast<int>(std::ceil(city.healthy * city.migrationRate / 100.f));
          city.healthy -= healthyToLeave;
          healthyMigrant += healthyToLeave;
          const int infectedToLeave = static_cast<int>(std::ceil(city.infected * city.migrationRate / 100.f));
          city.infected -= infectedToLeave;
          infectedMigrant += infectedToLeave;
          totalMigrationRate += city.migrationRate;
        }
        for (auto &city : cities) {
          const int healthyToCome = static_cast<int>(std::ceil(healthyMigrant * city.migrationRate / float(totalMigrationRate)));
          city.healthy += healthyToCome;
          const int infectedToCome = static_cast<int>(std::ceil(infectedMigrant * city.migrationRate / float(totalMigrationRate)));
          city.infected += infectedToCome;
        }


        // Phase 4: Infection
        for (auto &city : cities) {
          const int toInfect = std::min(city.healthy, city.infectionRate);
          city.infected += toInfect;
          city.healthy -= toInfect;
        }

        // Phase 5: Contagion
        for (auto &city : cities) {
          if (city.contagionRate == 0) {
            continue;
          }
          const int toInfect = std::min(city.healthy, static_cast<int>(std::ceil(city.infected * city.contagionRate / 100.f)));
          city.infected += toInfect;
          city.healthy -= toInfect;
        }

        // Phase 6: Extinction
        for (auto &city : cities) {
          if (city.lethalityRate == 0) {
            continue;
          }
          const int dead = static_cast<int>(std::floor(city.infected * city.lethalityRate / 100.f));
          decrease(city.infected, dead);
          city.dead += dead;
        }

        // Phase: Players turn
        // Local Action
        std::vector<Action> globalActions;
        std::shuffle(botIndex.begin(), botIndex.end(), gen);
        for (int id : botIndex) {
          const auto &[name, bot] = bots[id];
          for (auto action : bot(step, id, cities[id])) {
            if (!doLocalAction(action, cities[id])) {
              globalActions.push_back(action);
            };
          }
        }
        // Global Action
        doGlobalAction(globalActions, cities);
      }
      // show(cities, 1);
      for (int i = 0; i < cities.size(); ++i) {
        cumulCities[i] += cities[i];
      }
      spdlog::debug("Run End");
    }
    show(cumulCities, m_numberRuns);
  }


  void show(const std::vector<City> &cities, int normalization) {

    std::vector<int> cityIndex(cities.size());
    std::iota(cityIndex.begin(), cityIndex.end(), 0);

    std::sort(cityIndex.begin(), cityIndex.end(), [&](int a, int b) {
      const int healthyA = cities[a].healthy;
      const int healthyB = cities[b].healthy;

      if (healthyA == healthyB) {
        const int infectedA = cities[a].infected;
        const int infectedB = cities[b].infected;

        if (infectedA == infectedB) {
          return cities[a].dead < cities[b].dead;
        }

        return infectedA > infectedB;
      }

      return healthyA > healthyB;
    });


    fmt::print("+{:-^120}+\n", " Leader Board ");
    fmt::print("| {:>4} | {:>20} | {:>10} | {:>10} | {:>10} | {:>10} | {:>10} | {:>10} | {:>10} |\n", "Rank", "Bot Name", "Healthy", "Infected", "Dead", "Infection", "Contagion", "Lethality", "Migration");
    fmt::print("+{:-^120}+\n", "");
    for (int i = 0; i < cityIndex.size(); ++i) {
      const int id = cityIndex[i];
      fmt::print("| {:>4} | {:>20} | {:>10} | {:>10} | {:>10} | {:>10} | {:>9}% | {:>9}% | {:>9}% |\n",
            i,
            std::get<0>(bots[id]),
            cities[id].healthy / normalization,
            cities[id].infected / normalization,
            cities[id].dead / normalization,
            cities[id].infectionRate / normalization,
            cities[id].contagionRate / normalization,
            cities[id].lethalityRate / normalization,
            cities[id].migrationRate / normalization
            );
    }
    fmt::print("+{:-^120}+\n", "");
  }

  static std::vector<std::tuple<std::string, BotFunction>> &GetBotRegister() {
    static std::vector<std::tuple<std::string, BotFunction>> g_bots;
    return g_bots;
  }

private:
  int m_numberRuns;
  int m_steps;

  std::vector<std::tuple<std::string, BotFunction>> bots;
  std::vector<int> botIndex;  // Bot index to shuffle execution order for each step
  std::vector<City> cumulCities;  // Used for leader board
};


class RegisterBot {
public:
  RegisterBot(const std::string &name, BotFunction bot) {
    Controller::GetBotRegister().push_back({name, bot});
  }
};