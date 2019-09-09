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

constexpr const char* test_string_valid = "\
    {\
     \"item1\" : [ 1, 2, 368, 4], \
     \"item2\" : { \"a\" : 1, \"b\" : 2, \"c\" : 3}, \
     \"item3\" : \"An Item\",\
     \"item4\" : \"Another Item\",\
    \"item5\" : \"An Item\",\
    \"item6\" : \"Another Item\"\
    }";

constexpr const char* big_sample =
"[{\"_id\":\"5d74c8e938ff849f41adeaf5\",\"index\":0,\"guid\":\"88891256-2315-48ed-872c-49097d818973\",\"isActive\":\"true\",\"balance\":\"$3,235.01\",\"picture\":\"http:\/\/placehold.it\/32x32\",\"age\":28,\"eyeColor\":\"brown\",\"name\":\"Pitts King\",\"gender\":\"male\",\"company\":\"XYQAG\",\"email\":\"pittsking@xyqag.com\",\"phone\":\"+1 (920) 441-3096\",\"address\":\"883 Vandervoort Avenue, Takilma, Oklahoma, 8989\",\"about\":\"Nisi sint mollit laboris proident voluptate eiusmod qui commodo in est amet ex id. Pariatur laboris amet occaecat nostrud eiusmod. In esse qui nostrud qui magna id occaecat ea. Quis amet consectetur cillum nisi nulla Lorem voluptate tempor irure reprehenderit culpa. Duis non aliquip incididunt duis non pariatur ea voluptate.\\r\\n\",\"registered\":\"2016-11-11T10:06:40 -06:-30\",\"latitude\":-83.355881,\"longitude\":-91.773318,\"tags\":[\"sit\",\"veniam\",\"ut\",\"irure\",\"exercitation\",\"ut\",\"tempor\"],\"friends\":[{\"id\":0,\"name\":\"Larson Duran\"},{\"id\":1,\"name\":\"Butler Chapman\"},{\"id\":2,\"name\":\"Mcleod Warner\"}],\"greeting\":\"Hello, Pitts King! You have 4 unread messages.\",\"favoriteFruit\":\"strawberry\"},{\"_id\":\"5d74c8e97b3ab00cb842d7ee\",\"index\":1,\"guid\":\"05555925-621e-47ca-aa8e-5b9dc6de8930\",\"isActive\":\"false\",\"balance\":\"$1,695.86\",\"picture\":\"http:\/\/placehold.it\/32x32\",\"age\":31,\"eyeColor\":\"green\",\"name\":\"Ruby Mccarthy\",\"gender\":\"female\",\"company\":\"PERMADYNE\",\"email\":\"rubymccarthy@permadyne.com\",\"phone\":\"+1 (909) 523-3817\",\"address\":\"128 Krier Place, Oasis, Marshall Islands, 1890\",\"about\":\"Elit adipisicing commodo do sint sint laborum aute irure ipsum adipisicing esse sunt. Dolore id proident incididunt nulla laboris fugiat consequat magna est incididunt veniam consectetur. Veniam ullamco pariatur amet ut ex elit non nisi reprehenderit ipsum esse. Ut Lorem quis eu incididunt nisi dolore voluptate sint id. Officia eu non adipisicing adipisicing. Quis commodo culpa reprehenderit sit ex sit eu officia culpa quis et. Magna consectetur deserunt aliquip esse irure voluptate.\\r\\n\",\"registered\":\"2015-05-08T04:56:33 -06:-30\",\"latitude\":-50.755509,\"longitude\":-106.695786,\"tags\":[\"ex\",\"eiusmod\",\"reprehenderit\",\"dolore\",\"ut\",\"minim\",\"adipisicing\"],\"friends\":[{\"id\":0,\"name\":\"Lorraine Acevedo\"},{\"id\":1,\"name\":\"York Romero\"},{\"id\":2,\"name\":\"Lana Bates\"}],\"greeting\":\"Hello, Ruby Mccarthy! You have 2 unread messages.\",\"favoriteFruit\":\"apple\"},{\"_id\":\"5d74c8e9f93fbed810d60cb5\",\"index\":2,\"guid\":\"892b961c-5359-4b15-b023-a3f0b14403d8\",\"isActive\":\"false\",\"balance\":\"$3,460.84\",\"picture\":\"http:\/\/placehold.it\/32x32\",\"age\":23,\"eyeColor\":\"brown\",\"name\":\"Mathews Sherman\",\"gender\":\"male\",\"company\":\"MIRACLIS\",\"email\":\"mathewssherman@miraclis.com\",\"phone\":\"+1 (877) 455-2968\",\"address\":\"272 Judge Street, Guilford, Idaho, 4016\",\"about\":\"Et culpa commodo sint officia mollit ea aute exercitation in. Ea enim eiusmod consequat excepteur dolore incididunt irure. Enim laboris cillum nulla et consectetur consequat. Aliqua deserunt deserunt ipsum enim excepteur cillum exercitation officia. Proident dolor ad eu ipsum nisi ipsum laboris in. Culpa reprehenderit id id irure qui adipisicing. Et labore sint Lorem laboris nostrud amet consequat eu est.\\r\\n\",\"registered\":\"2015-11-10T01:58:03 -06:-30\",\"latitude\":-54.721056,\"longitude\":13.329974,\"tags\":[\"velit\",\"quis\",\"magna\",\"ex\",\"labore\",\"duis\",\"elit\"],\"friends\":[{\"id\":0,\"name\":\"Duffy Blanchard\"},{\"id\":1,\"name\":\"Burks Marshall\"},{\"id\":2,\"name\":\"Holland Carey\"}],\"greeting\":\"Hello, Mathews Sherman! You have 9 unread messages.\",\"favoriteFruit\":\"banana\"},{\"_id\":\"5d74c8e99cc45d1c72f1c44f\",\"index\":3,\"guid\":\"0118dac5-9ca2-459c-ad92-f2b93e6001f6\",\"isActive\":\"true\",\"balance\":\"$2,654.42\",\"picture\":\"http:\/\/placehold.it\/32x32\",\"age\":23,\"eyeColor\":\"blue\",\"name\":\"Laura Phelps\",\"gender\":\"female\",\"company\":\"APEX\",\"email\":\"lauraphelps@apex.com\",\"phone\":\"+1 (855) 541-2357\",\"address\":\"294 Cumberland Street, Wollochet, American Samoa, 6180\",\"about\":\"Voluptate quis in magna elit cupidatat dolor pariatur. Elit ullamco cillum ad est laboris do nisi elit do officia eu. Proident sint voluptate qui duis amet incididunt consequat exercitation in nostrud mollit adipisicing consectetur. Exercitation incididunt aute reprehenderit mollit Lorem elit. Sint ea fugiat enim ad eiusmod nulla aute voluptate fugiat est dolor mollit. Fugiat elit do adipisicing amet irure aute veniam irure do.\\r\\n\",\"registered\":\"2015-06-07T11:27:16 -06:-30\",\"latitude\":-74.805493,\"longitude\":-72.430626,\"tags\":[\"nulla\",\"do\",\"ipsum\",\"velit\",\"dolor\",\"proident\",\"et\"],\"friends\":[{\"id\":0,\"name\":\"Pat Miles\"},{\"id\":1,\"name\":\"West Vega\"},{\"id\":2,\"name\":\"Tammy Rogers\"}],\"greeting\":\"Hello, Laura Phelps! You have 10 unread messages.\",\"favoriteFruit\":\"banana\"},{\"_id\":\"5d74c8e9a2d3e9a5a4a1bdd3\",\"index\":4,\"guid\":\"b577717f-e2ad-4f70-b0bd-628f9e4b7588\",\"isActive\":\"true\",\"balance\":\"$3,439.55\",\"picture\":\"http:\/\/placehold.it\/32x32\",\"age\":33,\"eyeColor\":\"brown\",\"name\":\"Bonner Randolph\",\"gender\":\"male\",\"company\":\"FUTURIZE\",\"email\":\"bonnerrandolph@futurize.com\",\"phone\":\"+1 (883) 552-3610\",\"address\":\"793 Gunnison Court, Caspar, Tennessee, 3451\",\"about\":\"Fugiat mollit deserunt eu incididunt anim adipisicing dolor laboris ex ut non ipsum laboris. Aliqua sunt cillum cupidatat fugiat. Lorem irure veniam ullamco in do quis deserunt aliquip esse occaecat mollit. Voluptate velit velit ea esse voluptate. Do fugiat esse Lorem laborum deserunt non eu veniam irure nisi exercitation nisi nulla. Mollit nostrud labore elit consectetur ea eu ea irure.\\r\\n\",\"registered\":\"2018-02-25T05:39:39 -06:-30\",\"latitude\":36.261331,\"longitude\":13.024399,\"tags\":[\"aliquip\",\"aliquip\",\"sunt\",\"proident\",\"id\",\"esse\",\"quis\"],\"friends\":[{\"id\":0,\"name\":\"Katrina Kent\"},{\"id\":1,\"name\":\"Kelly Middleton\"},{\"id\":2,\"name\":\"Kenya Caldwell\"}],\"greeting\":\"Hello, Bonner Randolph! You have 8 unread messages.\",\"favoriteFruit\":\"apple\"}]";

void TestDataDeserializationTime()
{
    cout<< "Test started ====== DataDeserialization Time" << endl;
    jsonhandler::JsonValue jsonVal;
    jsonhandler::JsonParseError jsonError;
    
    auto end = chrono::steady_clock::now();
    auto start = chrono::steady_clock::now();
    if(jsonhandler::JsonDecode(jsonVal, big_sample, jsonError))
    {
        end = chrono::steady_clock::now();
    }

    flexbuffers::Builder flexbuilder;
    std::string result;
    
    auto countJsonTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    
    auto startFlex = chrono::steady_clock::now();
    if (flexit::JsonToFlexBuffer(big_sample, flexbuilder))
    {
        auto endFlex = chrono::steady_clock::now();
        auto countFlexTime = chrono::duration_cast<chrono::nanoseconds>(endFlex - startFlex).count();
        cout<< "RapidJson Time elapsed = " << countJsonTime << " ns" << endl;
        cout<< "FlexIT Time elapsed = " << countFlexTime << " ns" << endl;
        
    }
    else
    {
        cout<< "FlexIT flexbuffer creation failed" << endl;
    }
    cout<< "Test ended ===== DataDeserialization Time" << endl << endl;
}

void DataSerializationTimeOverHead()
{
    cout<< "Test started ====== DataSerialization (overhead) Time" << endl;
    jsonhandler::JsonValue jsonVal;
    jsonhandler::JsonParseError jsonError;
    
    std::string result, backToJson;

    auto start = chrono::steady_clock::now();
    jsonhandler::JsonDecode(jsonVal, big_sample, jsonError);
    jsonhandler::JsonEncode(jsonVal, result);
    auto end = chrono::steady_clock::now();
    
    auto startConvert = chrono::steady_clock::now();
    jsonhandler::JsonEncode(jsonVal, result);
    auto endConvert = chrono::steady_clock::now();
    
    auto countJsonTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    auto countJsonTimeConvert = chrono::duration_cast<chrono::nanoseconds>(endConvert - startConvert).count();

    flexbuffers::Builder flexbuilder;
    
    auto startOverHead = chrono::steady_clock::now();
    flexit::JsonToFlexBuffer(big_sample, flexbuilder);
    flexit::FlexBufferToJson(flexbuilder, backToJson);
    auto endOverHead = chrono::steady_clock::now();
    
    auto startOverHeadConvert = chrono::steady_clock::now();
    flexit::FlexBufferToJson(flexbuilder, backToJson);
    auto endOverHeadConvert = chrono::steady_clock::now();
    
    auto countOverHeadTime = chrono::duration_cast<chrono::nanoseconds>(endOverHead - startOverHead).count();
    auto countOverHeadTimeConvert = chrono::duration_cast<chrono::nanoseconds>(endOverHeadConvert - startOverHeadConvert).count();

    cout<< "FlexIT Time elapsed (to json) = " << countOverHeadTimeConvert << " ns" << endl;
    cout<< "Rapidjson Time elapsed (to json) = " << countJsonTimeConvert << " ns" << endl;
    
    cout<< "Rapidjson Time elapsed (from json to jsonObject & again back to json) = " << countJsonTime << " ns" << endl;
    
    cout<< "FlexIT Time elapsed (from json to flexbuffer & again back to json) = " << countOverHeadTime << " ns" << endl;
    cout<< "Test ended ====== DataSerialization (overhead) Time" << endl << endl;
}
void TestDataSerializationTime()
{
    cout<< "Test started ====== DataSerialization Time" << endl;
    jsonhandler::JsonValue jsonVal;
    jsonhandler::JsonParseError jsonError;
    
    jsonhandler::JsonDecode(jsonVal, test_string_valid, jsonError);
    
    flexbuffers::Builder flexbuilder;
    std::string result;
    
    auto start = chrono::steady_clock::now();
    jsonhandler::JsonEncode(jsonVal, result);
    auto end = chrono::steady_clock::now();
    
    auto countJsonTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    
    size_t sz = flexbuilder.GetSize();
    
    if (flexit::JsonToFlexBuffer(test_string_valid, flexbuilder))
    {
        auto startFlex = chrono::steady_clock::now();
        result.assign(flexbuilder.GetBuffer().data(), flexbuilder.GetBuffer().data() + flexbuilder.GetSize());
        auto endFlex = chrono::steady_clock::now();
        
        auto countFlexTime = chrono::duration_cast<chrono::nanoseconds>(endFlex - startFlex).count();
        cout<< "RapidJson Time elapsed = " << countJsonTime << " ns" << endl;
        cout<< "FlexIT Time elapsed (to flexbuffer) = " << countFlexTime << " ns" << endl;
        
    }
    else
    {
        cout<< "FlexIT flexbuffer creation failed" << endl;
    }
    cout<< "Test ended ===== DataSerialization Time" << endl << endl;
}

void TestContainersTime()
{
    cout<< "Test started ====== Containers Time" << endl;
    {
        std::map<std::string, std::string> sample {{"test1", "abc"}, {"test2", "xyz"}};
    
        flexbuffers::Builder flexbuilder;
        std::string result;
    
        auto start = chrono::steady_clock::now();
        sample.insert(std::make_pair("test3", "a result"));
        auto end = chrono::steady_clock::now();
    
        auto countJsonTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    
        flexbuilder.Add(sample);
        auto startFlex = chrono::steady_clock::now();
        flexbuilder.Add("test4", "a result");
        auto endFlex = chrono::steady_clock::now();
        auto countFlexTime = chrono::duration_cast<chrono::nanoseconds>(endFlex - startFlex).count();
        cout<< "STL map Time elapsed = " << countJsonTime << " ns" << endl;
        cout<< "FlexIT map Time elapsed = " << countFlexTime << " ns" << endl;
    }
    
    {
        std::vector<int> sample { 10, 20, 30, 40};
        
        flexbuffers::Builder flexbuilder;
        std::string result;
        
        auto start = chrono::steady_clock::now();
        sample.push_back(50);
        auto end = chrono::steady_clock::now();
        
        auto countJsonTime = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
        
        flexbuilder.Add(sample);
        auto startFlex = chrono::steady_clock::now();
        flexbuilder.Add(60);
        auto endFlex = chrono::steady_clock::now();
        auto countFlexTime = chrono::duration_cast<chrono::nanoseconds>(endFlex - startFlex).count();
        cout<< "STL vector Time elapsed = " << countJsonTime << " ns" << endl;
        cout<< "FlexIT vector Time elapsed = " << countFlexTime << " ns" << endl;
    }
    
    cout<< "Test ended ===== Containers Time" << endl << endl;
}

void TestLoadFromFile() {
    auto log = [](const std::string& input) {
        cout<< "[TestLoadFromFile] " << input << endl;
    };
    
    using namespace flexbuffers;
    using namespace flexit;
    
    if (auto reference = JsonToFlexBuffer("/tmp/.memoryMappedFile", test_string_valid)) {
        if (reference->IsMap()) {
            auto lookUpMap = reference->AsMap();
            log(lookUpMap["item3"].AsString().str());
        }
        
        std::string jsonEncodedString;
        if (FlexBufferToJson(reference, jsonEncodedString)) {
            log(jsonEncodedString);
        }
    }
}

int main(int argc, const char * argv[]) {

    TestLoadFromFile();
    return 1;
    TestDataDeserializationTime();
    TestDataSerializationTime();
    TestContainersTime();
    DataSerializationTimeOverHead();
    
    flexbuffers::Builder flexbuilder;

    
    if (flexit::JsonToFlexBuffer(big_sample, flexbuilder))
    {
        
        jsonhandler::JsonValue jsonVal;
        jsonhandler::JsonParseError jsonError;
        
        
        jsonhandler::JsonDecode(jsonVal, big_sample, jsonError);
        
        cout<< "Test started ====== Data Access Time" << endl;

        auto startJson1 = chrono::steady_clock::now();
        const jsonhandler::JsonValueMap jmap = jsonVal.GetList()[0].GetMap();
        auto startJson2 = chrono::steady_clock::now();
        //auto strjson = jmap.at("item1").GetList();
        //auto tempjson = strjson[2].GetInt64();
        auto strjson = jmap.at("_id").GetString();
        auto startJson3 = chrono::steady_clock::now();
        auto strjson2 = jmap.at("index").GetInt64();
        auto startJson4 = chrono::steady_clock::now();
        auto strjson3 = jmap.at("guid").GetString();
        auto startJson5 = chrono::steady_clock::now();

        auto startFlex1 = chrono::steady_clock::now();
        auto map = flexbuffers::GetRoot(flexbuilder.GetBuffer()).AsVector()[0].AsMap();
        auto startFlex2 = chrono::steady_clock::now();
        //auto vec = map["item1"].AsVector();
        //auto temp = vec[2].AsInt64();
        auto str = map["_id"].AsString().str();
        auto startFlex3 = chrono::steady_clock::now();
        auto str2 = map["index"].AsInt64();
        auto startFlex4 = chrono::steady_clock::now();
        auto string3 = map["guid"].AsString().str();
        auto startFlex5 = chrono::steady_clock::now();
        
        cout << "RapidJson Time elapsed in getting map = " << chrono::duration_cast<chrono::nanoseconds>(startJson2 - startJson1).count()<< " ns" << endl;
        cout << "RapidJson Time elapsed in getting value = " << chrono::duration_cast<chrono::nanoseconds>(startJson3 - startJson2).count()<< " ns" << endl;

        cout << "FlexIT Time elapsed in getting map = " << chrono::duration_cast<chrono::nanoseconds>(startFlex2 - startFlex1).count()<< " ns" << endl;
        cout << "FlexIT Time elapsed in getting value = " << chrono::duration_cast<chrono::nanoseconds>(startFlex3 - startFlex2).count()<< " ns" << endl;
        
        cout << "RapidJson Time elapsed in getting another value = " << chrono::duration_cast<chrono::nanoseconds>(startJson4 - startJson3).count()<< " ns" << endl;
        cout << "FlexIT Time elapsed in getting another value = " << chrono::duration_cast<chrono::nanoseconds>(startFlex4 - startFlex3).count()<< " ns" << endl;
        
        cout << "RapidJson Time elapsed in getting another value = " << chrono::duration_cast<chrono::nanoseconds>(startJson5 - startJson4).count()<< " ns" << endl;
        cout << "FlexIT Time elapsed in getting another value = " << chrono::duration_cast<chrono::nanoseconds>(startFlex5 - startFlex4).count()<< " ns" << endl;
        
        cout<< "Test ended ====== Data Access Time" << endl;

        /*
        auto map2 = map["item2"].AsMap();
        
        flexbuilder.Add("hello","hal");
        {
        auto l = map2["c"].AsInt32();
        
        auto vec = map["item1"].AsVector();
        auto size = vec.size();
        
        auto temp = vec[2].AsInt64();
        vec[2].MutateInt(369);
        
        temp = vec[2].AsInt64();
        //std::cout<<str<<"-" <<l<<"-"<<temp;
        }
        std::string ret;
        
        flexit::FlexBufferToJson(flexbuilder, ret);
         */
        
        auto sz = strlen(big_sample);
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
        
        
    }
    
    while(true);
    return 0;
}
