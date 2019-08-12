//
//  main.cpp
//  FlexBufferVsJson
//
//  Created by Pranay Kumar on 22/07/19.
//  Copyright © 2019 Pranay Kumar. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <map>

#include "flexit.h"

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    std::string jsonPath = "{name:\"Maxim\", birthday:{\"year\": 1981, month: 6, day: 12}}";
    const char* test_string_valid = " \
    { item1 : [1, 2, 368, 4], \
    \"item2\" : { \"a\" : 1, \"b\" : 2, \"c\" : 3 }, \
    \"item3\" : \"An Item\" \
    }";
    
    flexbuffers::Builder flexbuilder;
    
    if (flexit::JsonToFlexBuffer(test_string_valid, flexbuilder))
    {
        jsonhandler::JsonValue jsonVal;
        jsonhandler::JsonParseError jsonError;
        
        jsonhandler::JsonDecode(jsonVal, test_string_valid, jsonError);
        /*
        auto map = flexbuffers::GetRoot(flexbuilder.GetBuffer()).AsMap();
        auto map2 = map["birthday"].AsMap();
        auto mon = map2["month"].AsInt64();
         std::cout<<mon;
         */
        
        const jsonhandler::JsonValueMap jmap = jsonVal.GetMap();
        auto start1 = chrono::steady_clock::now();
        auto strjson = jmap.at("item3").GetString();
        auto start2 = chrono::steady_clock::now();

        auto map = flexbuffers::GetRoot(flexbuilder.GetBuffer()).AsMap();
        auto start3 = chrono::steady_clock::now();
        //size_t sz = flexbuilder.GetSize();
        //size_t ss = strlen(test_string_valid);
        auto str = map["item3"].AsString().str();
        //int i = vec[2].AsInt32();
        auto start4 = chrono::steady_clock::now();
        
        cout << "Elapsed time in nanoseconds : " << chrono::duration_cast<chrono::nanoseconds>(start4 - start3).count()<< " ns" << endl;
        
        cout << "Elapsed time in nanoseconds : " << chrono::duration_cast<chrono::nanoseconds>(start2 - start1).count()<< " ns" << endl;
        auto map2 = map["item2"].AsMap();
        auto l = map2["c"].AsInt32();
        
        auto vec = map["item1"].AsVector();
        auto size = vec.size();
        
        auto temp = vec[2].AsInt64();
        vec[2].MutateInt(369);
        
        temp = vec[2].AsInt64();
        std::cout<<str<<"-" <<l<<"-"<<temp;
        
        std::string ret;
        
        flexit::FlexBufferToJson(flexbuilder, ret);

#if !defined(_WIN32)  
        flexit::FlushFlexbufferToFile("/tmp/.123", flexbuilder);
#else
		flexit::FlushFlexbufferToFile("C:\\ProgramData\\.kfdjkff", flexbuilder);
#endif
        flexbuffers::Reference x = flexit::utils::CreateFlexBufferReference(nullptr, 0);
		std::vector<char> *buffer = nullptr;
#if !defined(_WIN32)  
		auto f = flexit::GetMemMappedFlexBufferReferenceFromFile("/tmp/.123", x);
#else
		auto f = flexit::GetMemMappedFlexBufferReferenceFromFile("C:\\ProgramData\\.kfdjkff", x);
#endif
        
        auto map3 = x.AsMap();
        
        //size_t sz = flexbuilder.GetSize();
        //size_t ss = strlen(test_string_valid);
        auto str3 = map["item3"].AsString().str();
        
        flexit::FreeFlexBufferReference(buffer);
        
        if(ret.empty())
        {
            
        }
    }
    return 0;
}
