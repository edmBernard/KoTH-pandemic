
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
  Controller(int numberRuns)
    : m_numberRuns(numberRuns)
    , bots(GetBotRegister())
    , botIndex(bots.size(), 0)
    , cities(bots.size())
  {
    std::iota(botIndex.begin(), botIndex.end(), 0);

    assert(botIndex.size() == bots.size());
    assert(cities.size() == bots.size());

    spdlog::info("Number of bots registered : {}", bots.size());
  }

  void run() {

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> mutationActionChoice(0, 2);

    spdlog::info("Run Start");
    for (int step = 1; step <= m_numberRuns; ++step) {
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
          city.healthy += allHealthy(city) / 2;
          city.infected += allInfected(city) / 2;
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
        const int toInfect = city.healthy >= city.infectionRate ? city.infectionRate : city.healthy;
        city.infected += toInfect;
        city.healthy -= toInfect;
      }

      // Phase 5: Contagion
      for (auto &city : cities) {
        if (city.contagionRate == 0) {
          continue;
        }
        const int toInfect = static_cast<int>(std::ceil(city.infected * city.contagionRate / 100.f));
        const int converted = city.healthy >= toInfect ? toInfect : city.healthy;
        city.infected += converted;
        city.healthy -= converted;
      }

      // Phase 6: Extinction
      for (auto &city : cities) {
        if (city.lethalityRate == 0) {
          continue;
        }
        const int dead = static_cast<int>(std::floor(city.infected * city.lethalityRate / 100.f));
        decrease(city.infected, dead);
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
    show();
    spdlog::info("Run End");
  }

  void show() {

    std::sort(botIndex.begin(), botIndex.end(), [&](int a, int b) {
      const int healthyA = allHealthy(cities[a]);
      const int healthyB = allHealthy(cities[b]);

      if (healthyA == healthyB) {
        if (allInfected(cities[a]) > allInfected(cities[b])) {
          return true;
        }
        return false;
      }

      if (healthyA > healthyB) {
        return true;
      }
      return false;
    });


    fmt::print("\n+{:-^107}+\n", " Leader Board ");  // use '*' as a fill char
    fmt::print("| {:>4} | {:>20} | {:>10} | {:>10} | {:>10} | {:>10} | {:>10} | {:>10} |\n", "Rank", "Bot Name", "Healthy", "Infected", "Infection", "Contagion", "Lethality", "Migration");
    fmt::print("+{:-^107}+\n", "");  // use '*' as a fill char
    for (int i = 0; i < botIndex.size(); ++i) {
      const int id = botIndex[i];
      fmt::print("| {:>4} | {:>20} | {:>10} | {:>10} | {:>10} | {:>9}% | {:>9}% | {:>9}% |\n",
            i,
            std::get<0>(bots[id]),
            allHealthy(cities[id]),
            allInfected(cities[id]),
            cities[id].infectionRate,
            cities[id].contagionRate,
            cities[id].lethalityRate,
            cities[id].migrationRate
            );
    }
    fmt::print("+{:-^107}+\n", "");
  }

  static std::vector<std::tuple<std::string, BotFunction>> &GetBotRegister() {
    static std::vector<std::tuple<std::string, BotFunction>> g_bots;
    return g_bots;
  }

private:
  std::vector<std::tuple<std::string, BotFunction>> bots;
  std::vector<int> botIndex;  // Bot index to shuffle execution order for each step
  std::vector<City> cities;
  int m_numberRuns;
};


class RegisterBot {
public:
  RegisterBot(const std::string &name, BotFunction bot) {
    Controller::GetBotRegister().push_back({name, bot});
  }
};