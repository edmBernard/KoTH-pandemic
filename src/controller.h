
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
    spdlog::info("Run Start");
    for (int step = 1; step <= m_numberRuns; ++step) {
      spdlog::debug("Iteration : {:05}", step);

      // Phase 1: Mutation

      // Phase 2: Reproduction

      // Phase 3: Migration

      // Phase 4: Infection

      // Phase 5: Contagion

      // Phase 6: Extinction

      // Phase 7: Players turn
      std::shuffle(botIndex.begin(), botIndex.end(), gen);
      for (int id : botIndex) {
        const auto &[name, bot] = bots[id];
        for (auto action : bot(step, id, 1)) {
          doAction(action, cities[id]);
        }
      }

    }
    show();
    spdlog::info("Run End");
  }

  void show() {

    std::sort(botIndex.begin(), botIndex.end(), [&](int a, int b) {
      if (cities[a].healthy > cities[b].healthy) {
        return true;
      }
      return false;
    });

    fmt::print("| {:>20} | {:>10} | {:>10} |\n", "Bot Name", "Healthy", "Infected");
    for (int id : botIndex) {
      fmt::print("| {:>20} | {:>10} | {:>10} |\n", std::get<0>(bots[id]), cities[id].healthy, cities[id].infected);
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