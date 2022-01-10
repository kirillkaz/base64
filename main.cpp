#include <iostream>
#include <vector>
#include "base64lib.h"
#include <gtest/gtest.h>

int main() {
    std::string stroke = "hello world!";
    std::cout<<stroke<<std::endl;
    stroke = base64_encode(stroke);
    std::cout<<stroke<<std::endl;
    stroke = base64_decode(stroke);
    std::cout<<stroke<<std::endl;
    RUN_ALL_TESTS();
    return 0;
}

TEST(base64Test,stroke_to_bytes){
    std::vector<byte> bytesTest;
    std::vector<byte> bytes;

    std::string stroke = "1234567890-=!@#$%^&*()_+qwertyuiop[]\\QWERTYUIOP{}|asdfghjkl;'ASDFGHJKL:zxcvbnm,./ZXCVBNM<>?\"zxcvbnm,./ZXCVBNM<>?";
    stroke+="йцукенгшщзхъ\\ЙЦУКЕНГШЩЗХЪ/фывапролджэФЫВАПРОЛДЖЭячсмитьбю.ЯЧСМИТЬБЮ,!№;%:?*()_+";
    bytesTest = stroke_to_bytes(stroke);

    for (int i = 0;i<stroke.length();i++)bytes.push_back(static_cast<byte>(stroke[i]));
    EXPECT_EQ(bytesTest,bytes);
}

TEST(base64Test,stroke_to_bytes64){
    std::vector<byte> bytesTest;
    std::vector<byte> bytes;
    const char* base64_ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/;";
    std::string stroke = "1234567890-=!@#$%^&*()_+qwertyuiop[]\\QWERTYUIOP{}|asdfghjkl;'ASDFGHJKL:zxcvbnm,./ZXCVBNM<>?\"zxcvbnm,./ZXCVBNM<>?";
    stroke+="йцукенгшщзхъ\\ЙЦУКЕНГШЩЗХЪ/фывапролджэФЫВАПРОЛДЖЭячсмитьбю.ЯЧСМИТЬБЮ,!№;%:?*()_+";
    for (int i=0;i<stroke.length();i++){
        for (int j=0;j<64;j++) if (stroke[i] == base64_ABC[j]) bytes.push_back(j);
    }
    bytesTest = stroke_to_bytes64(stroke);
    EXPECT_EQ(bytesTest,bytes);
}

TEST(base64Test,bytes_to_base64){
    std::vector<byte> bytes64Test;
    std::vector<byte> bytes64;
    std::vector<byte> bytes;
    const char* base64_ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/;";
    std::string stroke = "1234567890-=!@#$%^&*()_+qwertyuiop[]\\QWERTYUIOP{}|asdfghjkl;'ASDFGHJKL:zxcvbnm,./ZXCVBNM<>?\"zxcvbnm,./ZXCVBNM<>?";
    stroke+="йцукенгшщзхъ\\ЙЦУКЕНГШЩЗХЪ/фывапролджэФЫВАПРОЛДЖЭячсмитьбю.ЯЧСМИТЬБЮ,!№;%:?*()_+";

    for (int i = 0;i<stroke.length();i++)bytes.push_back(static_cast<byte>(stroke[i]));
    bytes64Test = bytes_to_base64(bytes);

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

    EXPECT_EQ(bytes64Test,bytes64);
}

TEST(base64Test,base64_to_bytes){
    std::vector<byte> bytesTest;
    std::vector<byte> bytes64;
    std::vector<byte> bytes;
    const char* base64_ABC = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/;";
    std::string stroke = "1234567890-=!@#$%^&*()_+qwertyuiop[]\\QWERTYUIOP{}|asdfghjkl;'ASDFGHJKL:zxcvbnm,./ZXCVBNM<>?\"zxcvbnm,./ZXCVBNM<>?";
    stroke+="йцукенгшщзхъ\\ЙЦУКЕНГШЩЗХЪ/фывапролджэФЫВАПРОЛДЖЭячсмитьбю.ЯЧСМИТЬБЮ,!№;%:?*()_+";

    for (int i=0;i<stroke.length();i++){
        for (int j=0;j<64;j++) if (stroke[i] == base64_ABC[j]) bytes64.push_back(j);
    }
    bytesTest = base64_to_bytes(bytes64);

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
    EXPECT_EQ(bytesTest,bytes);
}

TEST(base64Test,base64_encode){
    std::string stroke = "1234567890-=!@#$%^&*()_+qwertyuiop[]\\QWERTYUIOP{}|asdfghjkl;'ASDFGHJKL:zxcvbnm,./ZXCVBNM<>?\"zxcvbnm,./ZXCVBNM<>?";
    stroke+="йцукенгшщзхъ\\ЙЦУКЕНГШЩЗХЪ/фывапролджэФЫВАПРОЛДЖЭячсмитьбю.ЯЧСМИТЬБЮ,!№;%:?*()_+";
    std::string encoded = "MTIzNDU2Nzg5MC09IUAjJCVeJiooKV8rcXdlcnR5dWlvcFtdXFFXRVJUWVVJT1B7fXxhc2RmZ2hqa2w7J0FTREZHSEpLTDp6eGN2Ym5tLC4vWlhDVkJOTTw+PyJ6eGN2Ym5tLC4vWlhDVkJOTTw+P1wvLiwhOyU6PyooKV8r";
    std::string encodedTest = base64_encode(stroke);
    EXPECT_EQ(encodedTest,encodedTest);

}

TEST(base64Test,base64_decode){
    std::string encoded = "MTIzNDU2Nzg5MC09IUAjJCVeJiooKV8rcXdlcnR5dWlvcFtdXFFXRVJUWVVJT1B7fXxhc2RmZ2hqa2w7J0FTREZHSEpLTDp6eGN2Ym5tLC4vWlhDVkJOTTw+PyJ6eGN2Ym5tLC4vWlhDVkJOTTw+Pw==";
    std::string stroke = "1234567890-=!@#$%^&*()_+qwertyuiop[]\\QWERTYUIOP{}|asdfghjkl;'ASDFGHJKL:zxcvbnm,./ZXCVBNM<>?\"zxcvbnm,./ZXCVBNM<>?";
    std::string strokeTest = base64_decode(encoded);
    EXPECT_EQ(strokeTest,stroke);
}