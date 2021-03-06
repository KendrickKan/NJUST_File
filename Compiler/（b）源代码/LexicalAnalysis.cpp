#include <bits\stdc++.h>
using namespace std;
//列举24个关键字、运算符、限定符写全，不一定会用完
//把关键字映射到24个字母上 剩下两个字母 y，z分别代表标识符 常量 1代表单目运算符 2代表双目运算符 3代表#
vector<string> Keywords = {"char", "double", "float", "int", "long", "short",
                           "struct", "void",                                                                                                           //数据类
                           "for", "do", "while", "break", "continue",                                                                                  //循环类
                           "if", "else", "goto", "switch", "case", "default", "return",                                                                //条件类
                           "const", "include", "main", "iostream"};                                                                                    //其他类
vector<string> MonocularOperators = {"+", "-", "*", "/", "!", "%", "~", "&", "|", "^", "="};                                                           //单目运算符
vector<string> BinocularOperators = {"++", "--", "&&", "||", "<=", "!=", "==", ">=", "+=", "-=", "*=", "/="};                                          //双目运算符
vector<string> Delimiters = {",", "(", ")", "{", "}", ";", "<", ">", "#"};                                                                             //限定符
char Letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'}; //限定符
//初始状态字符
char beginch;
// NFA当输入一个的时候，所转向的是不确定的
// 所以将NFA的每一个状态所转向的状态当作一个结构
struct NFA_Point
{
    vector<char> NFA_Point_State;
};
// NFA存是将其转化为ASCALL码十进制值存在数组中
struct NFA
{
    NFA_Point NFAMove[200][200];
    vector<string> NFAState; //非终结符 状态 "Z"为终态
    vector<string> NFAInput; //终结符
};
NFA kNFA;
//一个子集
//用@代表空
struct Closure
{
    vector<char> cSet;   //需要将其升序排序，以方便比对是否存在
    bool haveOver;       //用于判断是否含有终态
    int dfa[200] = {-1}; //下标就是输入的ASCAII的值，存的就是clousers的下标
};
vector<Closure> clousers;                 //子集集合
bool isInteger(char a);                   //判断是否是数字
bool isLetter(char a);                    //判断是否是字母
int isKeyword(string a);                  //判断是否是关键字
bool isMonocularOperator(string a);       //判断是否是单目运算符
bool isBinocularOperator(string a);       //判断是否是双目运算符
bool isDelimiter(string a);               //判断是否是限定符
bool is_in_NFAState(string str);          //判断该状态是否存在于NFA中
bool is_in_NFAInput(string str);          //判断该输入字母是否存在于NFA中
bool CreateNFA();                         //建立NFA
void ShowNFA();                           //建立DFA
bool is_in_closure(char a, Closure c);    //判断该元素是否在子集
Closure get_closure(Closure c);           //得到闭包
int is_in_closures(Closure c);            //判断这个子集是否已经存在 不存在返回-1 存在返回标号
Closure colouse_move(Closure c, char ch); //子集Move
void NFA_to_DFA();                        // NFA转化为DFA
void ShowDFA();                           //展示DFA
bool DFA(string str);                     // DFA判断
void slove();                             //程序运行函数
ofstream Processfs("LexicalAnalysisProcess.txt");
int main()
{
    if (!CreateNFA())
    {
        cout << "Error:构建NFA失败";
    }
    ShowNFA();
    NFA_to_DFA();
    for (int i = 0; i < clousers.size(); i++)
    {
        cout << "T_Index:" << i << " ";
        for (int j = 0; j < clousers[i].cSet.size(); j++)
        {
            cout << clousers[i].cSet[j] << " ";
        }
        cout << clousers[i].haveOver;
        cout << endl;
    }
    ShowDFA();
    slove();
    // cout << 'a' - 0;
    Processfs.close();
    system("pause");
    return 0;
}
bool isInteger(char a)
{
    if (a >= '0' && a <= '9')
        return true;
    return false;
}
bool isLetter(char a)
{
    if (a >= 'a' && a <= 'z')
        return true;
    if (a >= 'A' && a <= 'Z')
        return true;
    return false;
}
int isKeyword(string a)
{
    for (int i = 0; i < Keywords.size(); i++)
    {
        if (a == Keywords[i])
            return i;
    }
    return -1;
    //也可以用迭代器和find函数，但是没必要。
}
bool isMonocularOperator(string a)
{
    for (int i = 0; i < MonocularOperators.size(); i++)
    {
        if (a == MonocularOperators[i])
            return true;
    }
    return false;
}
bool isBinocularOperator(string a)
{
    for (int i = 0; i < BinocularOperators.size(); i++)
    {
        if (a == BinocularOperators[i])
            return true;
    }
    return false;
}
bool isDelimiter(string a)
{
    for (int i = 0; i < Delimiters.size(); i++)
    {
        if (a == Delimiters[i])
            return true;
    }
    return false;
}
bool is_in_NFAState(string str)
{
    for (int i = 0; i < kNFA.NFAState.size(); i++)
    {
        if (str == kNFA.NFAState[i])
            return true;
    }
    return false;
}
bool is_in_NFAInput(string str)
{ //判断该状态是否存在于NFA中
    for (int i = 0; i < kNFA.NFAInput.size(); i++)
    {
        if (str == kNFA.NFAInput[i])
            return true;
    }
    return false;
}
bool CreateNFA()
{
    ifstream fs("LexicalAnalysis.txt");
    if (fs.is_open())
    {
        bool fbegin = false;
        // cout << "YES";
        string line;
        while (getline(fs, line))
        {
            // cout << line << "YES" << endl;
            string strL;
            strL = line[0];
            char ch1 = line[0];
            string strR = line.substr(3); //产生式右边部分
            // cout << strL << "" << strR << endl;
            if (!is_in_NFAState(strL)) //如果状态不存在，那就把它放进去
            {
                if (!fbegin) //第一个输入的就是初态
                {
                    beginch = ch1;
                    fbegin = true;
                }
                kNFA.NFAState.push_back(strL);
            }
            string strR0;
            strR0 = strR[0];            // String类型的
            if (!is_in_NFAInput(strR0)) //填入输入 也就是终态
            {
                kNFA.NFAInput.push_back(strR0);
            }
            if (strR.size() >= 2) //说明右边还有递归
            {
                kNFA.NFAMove[ch1 - 0][strR[0] - 0].NFA_Point_State.push_back(strR[1]);
            }
            else
            {
                kNFA.NFAMove[ch1 - 0][strR[0] - 0].NFA_Point_State.push_back('Z'); //终态既包含了直接到字母或者数字的那种，又包含了空
            }
        }
        fs.close();
        return true;
    }
    return false;
}
void ShowNFA()
{
    for (int i = 0; i < 200; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            for (int k = 0; k < kNFA.NFAMove[i][j].NFA_Point_State.size(); k++)
            {
                cout << char(i) << "->" << char(j) << kNFA.NFAMove[i][j].NFA_Point_State[k] << endl;
            }
        }
    }
}
bool is_in_closure(char a, Closure c)
{
    for (int i = 0; i < c.cSet.size(); i++)
    {
        if (a == c.cSet[i])
            return true;
    }
    return false;
}
Closure get_closure(Closure c)
{
    //这里的c.size()是动态变化的
    for (int i = 0; i < c.cSet.size(); i++)
    {
        for (int j = 0; j < kNFA.NFAMove[c.cSet[i] - 0]['@' - 0].NFA_Point_State.size(); j++)
        {
            //从该状态经过空弧走到temp
            char temp = kNFA.NFAMove[c.cSet[i] - 0]['@' - 0].NFA_Point_State[j];
            if (!is_in_closure(temp, c))
            {
                c.cSet.push_back(temp);
            }
        }
    }
    return c; //加入子集 集合
}
int is_in_closures(Closure c)
{
    for (int i = 0; i < clousers.size(); i++)
    {
        if (clousers[i].cSet.size() == c.cSet.size())
        {
            //已经提前对其进行排序了，只需要一个一个比对就行了
            bool flag = false;
            for (int j = 0; j < clousers[i].cSet.size(); j++)
            {
                if (clousers[i].cSet[j] != c.cSet[j])
                {
                    flag = true;
                    break;
                }
            }
            if (!flag)
                return i;
        }
    }
    return -1;
}
Closure colouse_move(Closure c, char ch)
{
    Closure tempClosure;
    for (int i = 0; i < c.cSet.size(); i++)
    {
        for (int j = 0; j < kNFA.NFAMove[c.cSet[i] - 0][ch - 0].NFA_Point_State.size(); j++)
        {
            char temp = kNFA.NFAMove[c.cSet[i] - 0][ch - 0].NFA_Point_State[j];
            if (!is_in_closure(temp, tempClosure))
            {
                tempClosure.cSet.push_back(temp);
            }
        }
    }
    return tempClosure;
}
Closure chaveOver(Closure c)
{
    c.haveOver = false;
    for (int i = 0; i < c.cSet.size(); i++)
    {
        if (c.cSet[i] == 'Z')
            c.haveOver = true;
    }
    return c;
}
void NFA_to_DFA()
{
    Closure cbegin;
    cbegin.cSet.push_back(beginch); //初态 这里是定死了的 //但是后期可以改
    cbegin = get_closure(cbegin);   //得到第一个子集
    cbegin.haveOver = false;
    sort(cbegin.cSet.begin(), cbegin.cSet.end()); //排序，方便比对
    cbegin = chaveOver(cbegin);                   //判断是否含有终结
    clousers.push_back(cbegin);
    for (int i = 0; i < clousers.size(); i++)
    {
        for (int j = 0; j < kNFA.NFAInput.size(); j++)
        {
            char tempch = kNFA.NFAInput[j][0];
            if (kNFA.NFAInput[j][0] == '@') //这里的kNFA.NFAInput[j]其实是含有一个字符的字符串，所以只需要提起第一个就行啦
                continue;
            Closure temp = colouse_move(clousers[i], kNFA.NFAInput[j][0]);
            Closure temp2 = get_closure(temp);
            sort(temp2.cSet.begin(), temp2.cSet.end()); //排序，方便比对
            temp2 = chaveOver(temp2);                   //判断是否含有终结
            int tempIndex = is_in_closures(temp2);
            if (tempIndex == -1)
            {
                clousers.push_back(temp2);
                clousers[i].dfa[kNFA.NFAInput[j][0] - 0] = clousers.size() - 1; //加入新的就把最新的下标存
            }
            else
            {
                clousers[i].dfa[kNFA.NFAInput[j][0] - 0] = tempIndex;
            }
        }
    }
}
void ShowDFA()
{
    for (int i = 0; i < clousers.size(); i++)
    {
        cout << "T_Index:" << i << "\nMoveToTable: \n";
        for (int j = 0; j < kNFA.NFAInput.size(); j++)
        {
            if (kNFA.NFAInput[j][0] == '@') //这里的kNFA.NFAInput[j]其实是含有一个字符的字符串，所以只需要提起第一个就行啦
                continue;
            char temp = kNFA.NFAInput[j][0];
            cout << "            corss " << temp << " to " << clousers[i].dfa[temp - 0] << "; \n";
        }
        cout << endl;
    }
}
bool DFA(string str)
{
    int tempIndex = 0;
    for (int i = 0; i < str.length(); i++)
    {
        tempIndex = clousers[tempIndex].dfa[str[i] - 0];
        if (tempIndex == 0)
            return false;
    }
    if (clousers[tempIndex].haveOver)
        return true;
    return false;
}
void slove()
{
    Processfs << "  token内容      类别          行号\n";
    cout << "\n  token内容      类别          行号\n";
    ofstream ofs("SyntaxAnalysisSourceProgram.txt");
    ifstream fs("LexicalAnalysisSourceProgram.txt");
    if (fs.is_open() && ofs.is_open())
    {
        string line;
        int lineNum = 0; //代码所在行数
        while (getline(fs, line))
        {
            lineNum++;
            for (int i = 0; i < line.length();)
            {
                string checkStr = "";
                if (isInteger(line[i]))
                {
                    checkStr += line[i];
                    i++;
                    // while (isInteger(line[i]) || isLetter(line[i]) || line[i] == '.' || line[i] == '+' || line[i] == '-')
                    while (line[i] != ' ' && line[i] != '\n' && line[i] != '\t' && line[i] != '\0')
                    {
                        checkStr += line[i];
                        i++;
                    }
                    if (DFA(checkStr))
                    {
                        ofs << 'z';
                        cout << left << setw(15) << checkStr << " 常量            " << lineNum << endl;
                        Processfs << left << setw(15) << checkStr << " 常量            " << lineNum << endl;
                    }
                    else
                    {
                        cout << left << setw(15) << checkStr << " 非法常量        " << lineNum << endl;
                        Processfs << left << setw(15) << checkStr << " 非法常量        " << lineNum << endl;
                    }
                }
                else if (isLetter(line[i]) || line[i] == '_')
                {
                    checkStr += line[i];
                    i++;
                    // while (isLetter(line[i]) || isInteger(line[i]) || line[i] == '_')
                    while (line[i] != ' ' && line[i] != '\n' && line[i] != '\t' && line[i] != '\0')
                    {
                        checkStr += line[i];
                        i++;
                    }
                    if (isKeyword(checkStr) != -1)
                    {
                        ofs << Letters[isKeyword(checkStr)];
                        cout << left << setw(15) << checkStr << " 关键字          " << lineNum << endl;
                        Processfs << left << setw(15) << checkStr << " 关键字          " << lineNum << endl;
                    }
                    else
                    {
                        if (DFA(checkStr))
                        {
                            ofs << 'y';
                            cout << left << setw(15) << checkStr << " 标识符          " << lineNum << endl;
                            Processfs << left << setw(15) << checkStr << " 标识符          " << lineNum << endl;
                        }
                        else
                        {
                            cout << left << setw(15) << checkStr << " 非法标识符      " << lineNum << endl;
                            Processfs << left << setw(15) << checkStr << " 非法标识符      " << lineNum << endl;
                        }
                    }
                }
                checkStr = line[i]; //这样能将char转化为string
                if (isDelimiter(checkStr))
                {
                    i++;
                    if (checkStr == "#")
                        ofs << 3;
                    else
                        ofs << checkStr;
                    cout << left << setw(15) << checkStr << " 限定符          " << lineNum << endl;
                    Processfs << left << setw(15) << checkStr << " 限定符          " << lineNum << endl;
                }
                else if (isMonocularOperator(checkStr))
                {
                    i++;
                    string tempCheckStr = checkStr;
                    tempCheckStr += line[i];
                    if (isBinocularOperator(tempCheckStr))
                    {
                        i++;
                        ofs << 2;
                        cout << left << setw(15) << tempCheckStr << " 双目运算符      " << lineNum << endl;
                        Processfs << left << setw(15) << tempCheckStr << " 双目运算符      " << lineNum << endl;
                    }
                    else
                    {
                        if (checkStr == "=")
                            ofs << '=';
                        else
                            ofs << 1;
                        cout << left << setw(15) << checkStr << " 单目运算符      " << lineNum << endl;
                        Processfs << left << setw(15) << checkStr << " 单目运算符      " << lineNum << endl;
                    }
                }
                if (line[i] == ' ' || line[i] == '\n' || line[i] == '\t')
                    i++;
            }
        }
        fs.close();
        ofs.close();
    }
}