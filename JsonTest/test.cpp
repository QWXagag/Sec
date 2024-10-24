#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h>
using namespace std;
using namespace Json;


void writeJson()
{
    // [12, 13.45, "hello world", true, false, [1, 2, "aa"], {"a":"b"}];
    Value root;
    root.append(12);
    root.append(13.45);
    root.append("hello world");
    root.append(true);
    root.append(false);
    Value arr;
    arr.append(1);
    arr.append(2);
    arr.append("aa");
    root.append(arr);
    Value obj;
    obj["a"] = "b";
    root.append(obj);
    string text = root.toStyledString();

    ofstream ofs("test.json");
    ofs << text;
    ofs.close();
}

void readJson()
{
    ifstream ifs("test.json");
    Value root;
    Reader r;
    r.parse(ifs, root);
    for (int i = 0; i < root.size(); i++)
    {
        Value temp = root[i];
        if (temp.isInt())
        {
            cout << temp.asInt() << " ";
        }
        else if (temp.isDouble())
        {
            cout << temp.asDouble() << " ";
        }
        else if (temp.isBool())
        {
            cout << temp.asBool() << " ";
        }
        else if (temp.isString())
        {
            cout << temp.asString() << " ";
        }
        else if (temp.isArray())
        {
            for (int i = 0; i < temp.size(); i++)
            {
                Value ttemp = temp[i];
                if (ttemp.isInt())
                {
                    cout << ttemp.asInt() << " ";
                }
                else if (ttemp.isString())
                {
                    cout << ttemp.asString() << " ";
                }
            }
            
        }
        else if (temp.isObject())
        {
            cout << "temp[a]" << temp["a"] << " ";
        }
    }

    ifs.close();
}



int main()
{
    writeJson();
    readJson();
    return 0;
}