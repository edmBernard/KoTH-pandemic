
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
        const int toInfect = static_cast<int>((city.infected * city.contagionRate + 99.f) / 100.f);
        const int converted = city.healthy >= toInfect ? toInfect : city.healthy;
        city.infected += converted;
        city.healthy -= converted;
      }

      // Phase 6: Extinction
      for (auto &city : cities) {
        if (city.lethalityRate == 0) {
          continue;
        }
        const int dead = static_cast<int>((city.infected * city.lethalityRate) / 100.f);
        decrease(city.infected, dead);
        const int deadInIsolation = static_cast<int>((city.infectedIsolated * city.lethalityRate) / 100.f);
        decrease(city.infectedIsolated, deadInIsolation);
      }

      // Phase 7: Free from quarantine
      for (auto &city : cities) {
        city.healthy += city.healthyIsolated;
        city.infected += city.infectedIsolated;
        city.healthyIsolated = 0;
        city.infectedIsolated = 0;
      }

      // Phase: Players turn
      // Local Action
      std::vector<Action> globalActions;
      std::shuffle(botIndex.begin(), botIndex.end(), gen);
      for (int id : botIndex) {
        const auto &[name, bot] = bots[id];
        for (auto action : bot(step, id, 1)) {
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

    fmt::print("\n");
    fmt::print("| {:>20} | {:>10} | {:>10} | {:>10} | {:>10} | {:>10} |\n", "Bot Name", "Healthy", "Infected", "Infection", "Contagion", "Lethality");
    for (int id : botIndex) {
      fmt::print("| {:>20} | {:>10} | {:>10} | {:>10} | {:>9}% | {:>9}% |\n",
            std::get<0>(bots[id]),
            allHealthy(cities[id]),
            allInfected(cities[id]),
            cities[id].infectionRate,
            cities[id].contagionRate,
            cities[id].lethalityRate
            );
    }
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