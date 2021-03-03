
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

#pragma once

class Engine {
public:
  Engine() {
  }

  void run() {
    for (const auto &[name, bot] : GetBotRegister()) {
      m_state = bot(m_state);
      std::cout << name << " m_state: " << m_state << std::endl;
    }
  }

  static std::vector<std::tuple<std::string, std::function<int(int)>>> &GetBotRegister() {
    static std::vector<std::tuple<std::string, std::function<int(int)>>> g_bots;
    return g_bots;
  }

private:
  int m_state = 0;
};


class RegisterBot {
public:
  RegisterBot(const std::string &name, std::function<int(int)> bot) {
    Engine::GetBotRegister().push_back({name, bot});
  }
};