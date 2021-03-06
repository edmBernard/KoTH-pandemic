
#include <functional>
#include <numeric>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include <spdlog/spdlog.h>

#include "engine.h"

#pragma once

class Controller {
public:
  Controller(int numberRuns) : m_numberRuns(numberRuns) {
    spdlog::info("Number of bots registered : {}", GetBotRegister().size());
    botIndex = std::vector<int>(GetBotRegister().size(), 0);
    std::iota(botIndex.begin(), botIndex.end(), 0);
  }

  void run() {

    std::random_device rd;
    std::mt19937 gen(rd());
    const auto botList = GetBotRegister();

    spdlog::info("Run Start");
    for (int step = 1; step <= m_numberRuns; ++step) {
      spdlog::debug("Iteration : {:05}", step);

      std::shuffle(botIndex.begin(), botIndex.end(), gen);
      for (int id : botIndex) {
        const auto &[name, bot] = botList[id];
        BotReturn actions = bot(step, id, 1);
      }
    }
    spdlog::info("Run End");
  }

  static std::vector<std::tuple<std::string, std::function<std::array<Action, 3>(int, int, int)>>> &GetBotRegister() {
    static std::vector<std::tuple<std::string, std::function<std::array<Action, 3>(int, int, int)>>> g_bots;
    return g_bots;
  }

private:
  std::vector<int> botIndex;  // Bot index to shuffle execution order for each step
  int m_numberRuns;
};


class RegisterBot {
public:
  RegisterBot(const std::string &name, std::function<std::array<Action, 3>(int, int, int)> bot) {
    Controller::GetBotRegister().push_back({name, bot});
  }
};