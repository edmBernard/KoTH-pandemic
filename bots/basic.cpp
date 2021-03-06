#include "controller.h"

RegisterBot passive("passive", [](int step, int id, const City &) -> BotReturn {
  return {Action::kDoNoThing, Action::kDoNoThing, Action::kDoNoThing};
});

RegisterBot lockDown("killer", [](int step, int id, const City &) -> BotReturn {
  return {Action::kQuarantine, Action::kQuarantine, Action::kQuarantine};
});

RegisterBot bot01("bot01", [](int step, int id, const City &) -> BotReturn {
  return {Action::kResearchVaccination, Action::kResearchEpidemiology, Action::kGiveCure};
});