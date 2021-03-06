#include "controller.h"

RegisterBot passive("passive", [](int step, int id, int input) -> BotReturn {
  return {Action::kDoNoThing, Action::kDoNoThing, Action::kDoNoThing};
});

RegisterBot killer("killer", [](int step, int id, int input) -> BotReturn {
  return {Action::kKill, Action::kKill, Action::kKill};
});

RegisterBot lockDown("lockDown", [](int step, int id, int input) -> BotReturn {
  return {Action::kQuarantine, Action::kQuarantine, Action::kQuarantine};
});

RegisterBot bot01("bot01", [](int step, int id, int input) -> BotReturn {
  return {Action::kResearchVaccination, Action::kResearchEpidemiology, Action::kQuarantine};
});