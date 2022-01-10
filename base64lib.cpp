#include <iostream>
#include <vector>
#include "base64lib.h"
const char* base64_ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/;";
//конвертирует строку в массив с кодами символов
std::vector<byte> stroke_to_bytes(std::string stroke){
    std::vector<byte> bytes;
    bytes.resize(stroke.length());
    for (int i=0;i<bytes.size();i++){
        bytes[i] = static_cast<byte>(stroke[i]);
    }
    return bytes;
}
//конвертирует строку base64 в массив с индексами символов в base64
std::vector<byte> stroke_to_bytes64(std::string stroke){
    std::vector<byte> bytes64;
    for (int i=0;i<stroke.length();i++){
        for (int j=0;j<64;j++) if (stroke[i] == base64_ABC[j]) bytes64.push_back(j);
    }
    return bytes64;
}
//конвертирует массив с кодами символов в массив с кодами символов base64
std::vector<byte> bytes_to_base64(std::vector<byte> bytes){
    std::vector<byte> bytes64;
    if ((bytes.size()*8) % 6 == 0) bytes64.resize((bytes.size()*8)/6);
    else bytes64.resize((bytes.size()*8)/6 + 1);

    byte ostatok = 0;
    int sdvig=0;
    bool skip = false;
    for (int i=0;i<bytes.size();i++){
        if ((i % 3) == 0) {
            bytes64[i+sdvig] = bytes[i] >> 2;
            ostatok = bytes[i] & 3; //00000011 mask
            ostatok = ostatok << 6;

            //if (i == ((bytes.size() - 1))) {
            //bytes64[i + sdvig + 1] = ostatok >> 2;
            //}
        } else if ((i % 3) == 1) {
            ostatok = ostatok >> 2;
            ostatok += bytes[i] >> 4;
            bytes64[i + sdvig] = ostatok;
            ostatok = bytes[i] & 15;
            ostatok = ostatok << 2;
            if (i == ((bytes.size() - 1))) bytes64[i + sdvig + 1] = ostatok;
        }

        else if ((i % 3) == 2) {
            ostatok += bytes[i] >> 6;
            bytes64[i + sdvig] = ostatok;
            sdvig += 1;
            bytes64[i + sdvig] = bytes[i] & 63;
            ostatok = 0;
        }
    }
    return bytes64;
}
//encoder base64
std::string base64_encode(std::string stroke){

    std::vector<byte> bytes64 = stroke_to_bytes(stroke);
    bytes64 = bytes_to_base64(bytes64);

    int eqCount=0;
    if ((stroke.length() % 3) == 1)eqCount+=2;
    else if ((stroke.length() % 3) == 2)eqCount+=1;

    stroke = "";

    for (int i=0;i<bytes64.size();i++){
        stroke+=base64_ABC[bytes64[i]];
    }

    for (int i=0;i<eqCount;i++){
        stroke+='=';
    }
    return stroke;
}
//конвертирует массив с индексами символов base64 в массив с кодами обычных символов
std::vector<byte> base64_to_bytes(std::vector<byte> bytes64){
    std::vector<byte> bytes;
    int Rsdvig = 0;

    //учитываю знаки '='
    if (bytes64[bytes64.size()-1] == '='){
        if (bytes64[bytes64.size()-2] == '=') Rsdvig = 4;
        else Rsdvig = 2;
    }

    bool skip = false;
    int sdvig = 0;
    for (int i=0;i<bytes64.size()-1;i++) {
        if (skip == false) {
            if ((i + sdvig) % 3 == 0) {
                bytes.push_back((bytes64[i] << 2) + (bytes64[i + 1] >> 4));
                //std::cout<<(int)(bytes64[i+1]>>4)<<std::endl;
            } else if ((i + sdvig) % 3 == 1) {
                bytes.push_back(((bytes64[i] & 15) << 4) + (bytes64[i + 1] >> 2));
            } else {
                bytes.push_back(((bytes64[i] & 3) << 6) + bytes64[i + 1]);
                sdvig-=1;
                skip = true;
            }
        }
        else skip = false;
    }
    return bytes;
}
//decoder base64
std::string base64_decode(std::string stroke){

    std::vector<byte> bytes64 = stroke_to_bytes64(stroke);
    std::vector<byte> bytes = base64_to_bytes(bytes64);

    stroke = "";

    for (int i=0;i<bytes.size();i++)stroke+=bytes[i];
    return stroke;
}