#include "rpc.hpp"
#include <cstdint>
#include <iostream>
#include <optional>

using json = nlohmann::json;

std::string EncodeMessage(const nlohmann::json &msg);

std::tuple<std::string, std::vector<uint8_t>, std::optional<std::string>>
DecodeMessage(std::vector<uint8_t> msg);
std::optional<std::tuple<int, std::vector<uint8_t>, std::optional<std::string>>>
Split(std::vector<uint8_t> data);

void TestEncode() {
  std::string expected = "Content-Length: 16\r\n\r\n{\"testing\":true}";
  std::string actual = EncodeMessage({{"testing", true}});
  if (expected != actual) {
    std::cout << "Expected: " << expected << std::endl
              << "Got: " << actual << std::endl;
    throw std::runtime_error("TestEncode failed");
  }
  std::cout << "TestEncode passed" << std::endl;
}

void TestDecode() {
  std::string incomingMsg = "Content-Length: 15\r\n\r\n{\"method\":\"hi\"}";
  std::vector<uint8_t> incomingMsgVec(incomingMsg.begin(), incomingMsg.end());
  int expectedLength = 15;
  auto [method, content, err] = DecodeMessage(incomingMsgVec);
  std::vector<int> contentVec(content.begin(), content.end());
  int contentLength = contentVec.size();
  // check if method length is correct
  if (expectedLength != contentLength) {
    std::cout << "Expected: " << expectedLength << std::endl
              << "Got: " << contentLength << std::endl;
    throw std::runtime_error("TestDecode failed");
  }

  // check if the actual method is correct
  if (method != "hi") {
    std::cout << "Expected: hi" << std::endl << "Got: " << method << std::endl;
    throw std::runtime_error("TestDecode failed");
  }
  std::cout << "TestDecode passed" << std::endl;
}

void TestSplit() {
  std::string incomingMsg = "Content-Length: 15\r\n\r\n{\"method\":\"hi\"}";
  std::vector<uint8_t> incomingMsgVec(incomingMsg.begin(), incomingMsg.end());
  int expectedLength = 15;
  auto splitResult = Split(incomingMsgVec);
  if (!splitResult)
    return;
  auto [length, contentBytes, err] = *splitResult;
  int contentLength = contentBytes.size();
  // check if method length is correct
  if (expectedLength != contentLength) {
    std::cout << "Expected: " << expectedLength << std::endl
              << "Got: " << contentLength << std::endl;
    throw std::runtime_error("TestSplit failed");
  }
  std::cout << "TestSplit passed" << std::endl;
}

/* This function encodes the json object into a string
 * @param msg: json object to be encoded
 * @return: string representation of the json object
 */
std::string EncodeMessage(const nlohmann::json &msg) {
  std::string content = msg.dump();

  std::stringstream ss;
  ss << "Content-Length: " << content.size() << "\r\n\r\n" << content;

  return ss.str();
}

/* This function decodes the vector of bytes into a tuple of method and
 * content length on error, it throws a runtime error
 * @param msg: vector of bytes to be decoded
 * @return: tuple of method and content length
 */
std::tuple<std::string, std::vector<uint8_t>, std::optional<std::string>>
DecodeMessage(std::vector<uint8_t> msg) {
  // convert bytes message into a string
  std::string msg_str(msg.begin(), msg.end());

  // search for separator header and content
  size_t header_end = msg_str.find("\r\n\r\n");
  if (header_end == std::string::npos) {
    return {0, std::vector<uint8_t>(), "Did not find separator"};
  }

  // extract header and content
  std::string header = msg_str.substr(0, header_end);
  std::string content = msg_str.substr(header_end + 4); // +4 to skip \r\n\r\n

  // get value of Content-Length
  size_t content_length_pos = header.find("Content-Length: ");
  if (content_length_pos == std::string::npos) {
    return {0, std::vector<uint8_t>(), "Content-Length not found"};
  }

  size_t content_length = std::stoi(header.substr(content_length_pos + 16));
  if (content_length != content.size()) {
    return {0, std::vector<uint8_t>(), "Content-Length does not match"};
  }

  // Deserialize json
  json baseMessage;
  try {
    baseMessage = json::parse(content);
  } catch (const std::exception &e) {
    return {0, std::vector<uint8_t>(), e.what()};
  }

  // Extract method from baseMessage
  std::string method = baseMessage["method"];
  std::vector<uint8_t> contentBytes(content.begin(), content.end());
  return {method, contentBytes, ""};
}

std::optional<std::tuple<int, std::vector<uint8_t>, std::optional<std::string>>>
Split(std::vector<uint8_t> data) {
  const std::string delimiter = "\r\n\r\n";
  auto it =
      std::search(data.begin(), data.end(), delimiter.begin(), delimiter.end());
  if (it == data.end()) {
    return std::nullopt; // Delimiter not found
  }

  size_t headerLength = std::distance(data.begin(), it) + delimiter.size();
  std::string header(data.begin(), data.begin() + headerLength);

  // Check for "Content-Length: " header
  const std::string contentLengthPrefix = "Content-Length: ";
  auto pos = header.find(contentLengthPrefix);
  if (pos == std::string::npos) {
    return std::make_tuple(0, std::vector<uint8_t>(),
                           "Missing Content-Length header");
  }

  pos += contentLengthPrefix.size();
  size_t contentLength = 0;
  try {
    contentLength = std::stoul(header.substr(pos));
  } catch (const std::exception &e) {
    return std::make_tuple(0, std::vector<uint8_t>(),
                           "Invalid Content-Length value");
  }

  // Ensure the buffer has enough data for the entire message
  size_t totalLength = headerLength + contentLength;
  if (data.size() < totalLength) {
    return std::nullopt; // Incomplete message
  }

  // Extract the message
  std::vector<uint8_t> message(data.begin(), data.begin() + totalLength);
  return std::make_tuple(totalLength, message, "");
}
