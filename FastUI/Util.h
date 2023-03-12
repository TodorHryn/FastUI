#pragma once
#include <cstdint>
#include <vector>
#include <string>

void SleepInMs(uint32_t ms);
void SleepInUs(uint32_t us);

void LoadResource(int name, std::vector<uint8_t>& data);
std::string LoadResourceAsString(int name);
