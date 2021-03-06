
#include <functional>
#include <iostream>
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

  }

  void run() {

    spdlog::info("Run Start");
    for (int step = 1; step <= m_numberRuns; ++step) {
      spdlog::debug("Iteration : {:05}", step);

      for (int id = 0; id < GetBotRegister().size(); ++id) {

        const auto &[name, bot] = GetBotRegister()[id];
        m_state = bot(step, id, m_state);
      }
    }
    spdlog::info("Run End");
  }

  static std::vector<std::tuple<std::string, std::function<int(int, int, int)>>> &GetBotRegister() {
    static std::vector<std::tuple<std::string, std::function<int(int, int, int)>>> g_bots;
    return g_bots;
  }

private:
  int m_state = 0;
  int m_numberRuns;
};


class RegisterBot {
public:
  RegisterBot(const std::string &name, std::function<int(int, int, int)> bot) {
    Controller::GetBotRegister().push_back({name, bot});
  }
};