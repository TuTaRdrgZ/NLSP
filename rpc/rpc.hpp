#ifndef RPC_HPP
#define RPC_HPP
#include <json.hpp>
#include <optional>

using json = nlohmann::json;

// encode and decode message functions
std::string EncodeMessage(const nlohmann::json &msg);
std::tuple<std::string, std::vector<uint8_t>, std::optional<std::string>>
DecodeMessage(std::vector<uint8_t> msg);
std::optional<std::tuple<int, std::vector<uint8_t>, std::optional<std::string>>>
Split(std::vector<uint8_t> data);

// testing functions
void TestEncode();
void TestDecode();
void TestSplit();

#endif // RPC_HPP
