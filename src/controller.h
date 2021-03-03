
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
  }

  void run() {
    for (int i = 1; i <= m_numberRuns; ++i) {
      spdlog::debug("Iteration : {:05}", i);
      for (const auto &[name, bot] : GetBotRegister()) {
        m_state = bot(m_state);
        std::cout << name << " m_state: " << m_state << std::endl;
      }
    }
  }

  static std::vector<std::tuple<std::string, std::function<int(int)>>> &GetBotRegister() {
    static std::vector<std::tuple<std::string, std::function<int(int)>>> g_bots;
    return g_bots;
  }

private:
  int m_state = 0;
  int m_numberRuns;
};


class RegisterBot {
public:
  RegisterBot(const std::string &name, std::function<int(int)> bot) {
    Controller::GetBotRegister().push_back({name, bot});
  }
};