#ifndef BASE64_BASE64LIB_H
#define BASE64_BASE64LIB_H
using byte = uint8_t;

//конвертирует строку в массив с кодами символов
std::vector<byte> stroke_to_bytes(std::string stroke);
//конвертирует строку base64 в массив с индексами символов в base64
std::vector<byte> stroke_to_bytes64(std::string stroke);
//конвертирует массив с кодами символов в массив с кодами символов base64
std::vector<byte> bytes_to_base64(std::vector<byte> bytes);
//encoder base64
std::string base64_encode(std::string stroke);
//конвертирует массив с индексами символов base64 в массив с кодами обычных символов
std::vector<byte> base64_to_bytes(std::vector<byte> bytes64);
//decoder base64
std::string base64_decode(std::string stroke);
#endif //BASE64_BASE64LIB_H
