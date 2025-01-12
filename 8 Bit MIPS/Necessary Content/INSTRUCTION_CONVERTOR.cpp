#include <bits/stdc++.h>
using namespace std;

string createOpcodeValue(string s)
{ // Returns opcode value with instruction type
    // if (s == "j")
    //     return "0J";
    // else if (s == "add")
    //     return "5R";
    // else if (s == "or")
    //     return "fR";
    // else if (s == "sll")
    //     return "aR";
    // else if (s == "andi")
    //     return "8I";
    // else if (s == "bneq" || s == "bne")
    //     return "4I";
    // else if (s == "and")
    //     return "bR";
    // else if (s == "ori")
    //     return "dI";
    // else if (s == "srl")
    //     return "eR";
    // else if (s == "beq")
    //     return "9I";
    // else if (s == "lw")
    //     return "7I";
    // else if (s == "subi")
    //     return "3I";
    // else if (s == "sub")
    //     return "1R";
    // else if (s == "sw")
    //     return "6I";
    // else if (s == "nor")
    //     return "0R";
    // else if (s == "addi")
    //     return "cI";
    // else
    //     return "$x";

    if (s == "nor")
        return "0R";
    else if (s == "sub")
        return "1R";
    else if (s == "j")
        return "2J";
    else if (s == "subi")
        return "3I";
    else if (s == "bneq" || s == "bne")
        return "4I";
    else if (s == "add")
        return "5R";
    else if (s == "sw")
        return "6I";
    else if (s == "lw")
        return "7I";
    else if (s == "andi")
        return "8I";
    else if (s == "beq")
        return "9I";
    else if (s == "sll")
        return "aR";
    else if (s == "and")
        return "bR";
    else if (s == "addi")
        return "cI";
    else if (s == "ori")
        return "dI";
    else if (s == "srl")
        return "eR"; /// double F in instructions set /// J for "srl" is missing. So using here
    else if (s == "or")
        return "fR";
    else
        return "$x";
}

char createValue(string s)
{
    int pos = s.find(',');
    if (pos < s.size())
        s.replace(pos, 1, "");

    if (s == "$t0")
        return '0';
    else if (s == "$t1")
        return '1';
    else if (s == "$t2")
        return '2';
    else if (s == "$t3")
        return '3';
    else if (s == "$t4")
        return '4';
    else if (s == "$zero")
        return '5';
    else if (s == "$sp")
        return '6';
    else
        return '$';
}

string intToHexString(int val)
{
    stringstream hex_;
    hex_ << std::hex << val;
    string ret;
    hex_ >> ret;
    return ret;
}

int main()
{
    ifstream infile;
    infile.open("MIPS_INPUT.txt");

    ofstream outfile;
    outfile.open("hexdata.hex");

    string operation, source1, source2, destination, shamt;
    int h;
    string str, word;
    vector<string> words, instructions;
    map<string, int> mp;

    int count = 0;

    if (infile.is_open())
    {
        while (getline(infile, str))
        {
            // ignoring the line comments
            if (str[0] == '#' || (str[0] == '/' && str[1] == '/'))
            {
                continue;
            }
            // if (count == 0 && str != "addi $sp, $zero, 255" && instructions.size() == 0)
            // {
            //     instructions.push_back("addi $sp, $zero, 255");
            // }
            if (str.size() <= 1)
                continue;
            replace(str.begin(), str.end(), ',', ' ');
            stringstream ss(str);
            while (ss >> word)
            {
                // ignoring the comment after the instruction
                if (word[0] == '#' || (word[0] == '/' && word[1] == '/'))
                    break;
                if (word[(int)word.size() - 1] == ':')
                {
                    word.replace((int)word.size() - 1, 1, "");
                    mp.insert(pair<string, int>(word, instructions.size()));
                    break;
                }
            }
            instructions.push_back(str);
            // count++;
        }
        infile.close();
    }
    else
    {
        std::cout << "Unable to open input file" << endl;
    }

    for (int i = 0; i < instructions.size(); i++)
    {
        cout << i + 1 << " " << instructions[i] << endl;
        stringstream ss(instructions[i]);
        while (ss >> word)
        {
            if (word[0] == '/' && word[1] == '/')
                break;
            words.push_back(word);
        }
        if (words.size() >= 1)
        {
            operation = createOpcodeValue(words[0]);
            if (operation[1] == 'R')
            {
                destination = createValue(words[1]);
                source1 = createValue(words[2]);
                source2 = createValue(words[3]);
                if (source2 == "$")
                {
                    stringstream int_(words[3]);
                    int_ >> h;
                    source2 = intToHexString(h);
                }
                shamt = "0";
                if (operation[0] == 'a' || operation[0] == 'e')
                {
                    shamt = source2;
                    source2 = source1;
                    source1 = "5";
                }
                outfile << operation[0] << source1 << source2 << destination << shamt << " ";
            }
            else if (operation[1] == 'I')
            {
                string src, dest, immd;
                int imd;
                dest = createValue(words[1]);
                if (operation[0] == '7' || operation[0] == '6')
                { // lw/sw
                    // cout << words[2] << endl;
                    int l = (int)words[2].find('(') + 1;
                    src = createValue(words[2].substr(l, 3));
                    immd = words[2].substr(0, l - 1);
                    // cout << immd << endl;
                    stringstream int_(immd);
                    int_ >> imd;
                }
                else if (operation[0] == '4' || operation[0] == '9')
                { // beq/bneq
                    src = dest;
                    dest = createValue(words[2]);
                    imd = mp[words[3]] - (i + 1);
                }
                else
                {
                    src = createValue(words[2]);
                    stringstream int_(words[3]);
                    int_ >> imd;
                }
                immd = intToHexString(imd);
                int len = immd.size();
                outfile << operation[0] << src << dest;
                if (len > 1)
                    outfile << immd[len - 2] << immd[len - 1] << " ";
                else
                    outfile << "0" << immd[0] << " ";
            }
            else if (operation[1] == 'J')
            {
                int Target_Jump_Address = mp[words[1]];
                // cout << "Target_Jump_Address " << Target_Jump_Address << endl;
                outfile << operation[0];
                // Target_Jump_Address--;
                //  cout << "Printing Operation[0] " << operation[0] << endl;
                if (Target_Jump_Address < 16)
                    outfile << "0" << std::hex << Target_Jump_Address;
                else
                    outfile << std::hex << Target_Jump_Address;
                outfile << "00 ";
            }
            else
            {
                if ((i + 1) < 16)
                    outfile << "20" << std::hex << (i + 1);
                else
                    outfile << "2" << std::hex << (i + 1);
                outfile << "00 ";
            }
        }
        words.clear();
        // outfile << endl;
    }
    outfile.close();
    return 0;
}