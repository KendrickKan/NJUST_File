#include <bits/stdc++.h>
using namespace std;
vector<string> Keywords = {"char", "double", "float", "int", "long", "short",
                           "struct", "void",                                            //数据类
                           "for", "do", "while", "break", "continue",                   //循环类
                           "if", "else", "goto", "switch", "case", "default", "return", //条件类
                           "const", "include", "main", "iostream"};                     //其他类
struct State                 //状态
{
    char state;              //状态符号 非终结符
    vector<char> firsts;     // first集
    int ifInferEmpty;        //是否能推导出空 0代表不能 1代表能 -1代表还没确定
    vector<int> statesIndex; //所在的文法下标
};
vector<State> states;
vector<char> inputs;
struct Grammar
{
    string str;            //文法
    vector<char> searchCh; //向前搜索符
    int index;             //·的下标
    int num;               //第几个文法
    bool ifUse;            //·移动时，是否已经被使用了
    int ifInferEmpty;      //该文法能否推导出空 0代表不能 1代表能 -1代表还没确定
};
vector<Grammar> grammars; //文法集合
struct Item               //项目
{
    vector<Grammar> grammars;         //项目里面包含的文法
    vector<pair<int, char>> gotoItem; //第一个代表指向的Item的下标 第二个代表转换的符号S/r
    vector<pair<char, int>> Action;   // Action表
    vector<int> Goto;                 // Goto表
};
vector<Item> items;                          //项目集
int StrNum;                                  //输入符号串的长度
int isInStates(char ch);                     //判断是否在状态里 存在则返回标号
int isInInputs(char ch);                     //判断是否在输入字符里 存在则返回标号
void getGrammar();                           //读取文法
void showGrammars();                         //展示文法
void showStats();                            //展示状态
bool ifAllInferEmptyConfirm();               //是否所有的能否推导空都已经确定了
void ifInferEmpty();                         //是否能推导出空 运行这个函数 直接让所有的状态是否能推导出空填入状态属性里
bool insertFirst(int a, int b);              //将states[b]的first集添加到states[a],如果有变化返回1 没变化返回0
void getFirsts();                            //得到每个状态的first集
void showFirsts();                           //展示First集合
Item grammarInItem(Grammar g, Item it);      //这个文法是否在项目里 返回新的项目
Item getClosures(Item i);                    //得到一个项目闭包
bool ifItemAllGrammarUsed(Item it);          //该项目里是否所有的文法都遍历用过一遍了
bool cmpGrammar(Grammar g1, Grammar g2);     //用于sort函数 排序一个项目里面的文法
bool ifItemAEuqalsItemB(Item ita, Item itb); //判断ItemA是否等于ItemB
int ifTheItemInItems(Item it);               //判断这个Item是否已经存在 //返回-1不存在 否则返回下标
void LALR(Item tempIt, int iftempItInItems); // LALR（1）合并同心集
void getItems();                             //得到所有的项目集
void showItems();                            //展示所有的项目集
void initActionAndGoto();                    //初始化Action表和Goto表
void getActionAndGoto();                     //得到Action表和Goto表
void showActionAndGoto();                    //展示Action表和Goto表
void printChStack(vector<char> vec);         //打符号态栈
void printStateStack(vector<int> vec);       //打印状态栈
void printStrStack(vector<char> vec);        //打印输入符号串
void solveWrong(vector<char> vec);           //处理错误信息 在哪一行的什么后面出错
string soveWrongPrompt(char ch);             //处理错误提示 出错的具体原因
void solve();                                //进行判断是否acc
ofstream ofs("SyntaxAnalysisProcess.txt");
int main()
{
    getGrammar();
    cout << "--------------------------------\n";
    showGrammars();
    cout << "--------------------------------\n";
    showStats();
    ifInferEmpty();
    cout << "--------------------------------\n";
    showStats();
    getFirsts();
    cout << "--------------------------------\n";
    showFirsts();
    getItems();
    cout << "--------------------------------\n";
    showItems();
    getActionAndGoto();
    showActionAndGoto();
    cout << "--------------------------------\n";
    solve();
    ofs.close();
    system("pause");
}
int isInStates(char ch)
{
    for (int i = 0; i < states.size(); i++)
    {
        if (ch == states[i].state)
        {
            return i;
        }
    }
    return -1;
}
int isInInputs(char ch)
{
    for (int i = 0; i < inputs.size(); i++)
    {
        if (ch == inputs[i])
        {
            return i;
        }
    }
    return -1;
}
void getGrammar()
{
    ifstream fs("SyntaxAnalysisGrammar.txt");
    if (fs.is_open())
    {
        string line;
        int num = 0;
        while (getline(fs, line))
        {
            if (num == 0) //增加拓广文法
            {
                Grammar gtemp;
                string strtemp = "$->";
                strtemp += line[0]; //$代表拓广的初始状态
                gtemp.ifUse = false;
                gtemp.index = 3;
                gtemp.num = num;
                gtemp.searchCh.push_back('#');
                gtemp.str = strtemp;
                gtemp.ifInferEmpty = -1;
                grammars.push_back(gtemp);
                State stemp;             //将该状态存在状态集里
                stemp.state = '$';       //其实这里也就是没有拓广的初态
                stemp.ifInferEmpty = -1; //状态能否推导出空 置-1
                stemp.statesIndex.push_back(num);
                states.push_back(stemp);
            }
            num++;
            Grammar gm;
            gm.str = line;
            gm.num = num;
            gm.index = 3; //在下标为3的前面 eg:S->afsba 此时应该在a的前面
            gm.ifUse = false;
            gm.searchCh.push_back('#');
            gm.ifInferEmpty = -1;
            grammars.push_back(gm);
            int tempLoc = isInStates(line[0]); //判断是否存在在现有状态中
            if (tempLoc == -1)                 //不存在现有状态中
            {
                State stemp;
                stemp.state = line[0];
                stemp.ifInferEmpty = -1; //状态能否推导出空 置-1
                stemp.statesIndex.push_back(num);
                states.push_back(stemp); //如果这个状态是'A'~'Z'之间，那么就认为是一个状态不是输入}
            }
            else //存在现有状态
            {
                states[tempLoc].statesIndex.push_back(num);
            }
            for (int i = 3; i < line.size(); i++)
            {

                if ((isInInputs(line[i]) == -1) && (line[i] < 'A' || line[i] > 'Z'))
                    inputs.push_back(line[i]); //存输入
            }
        }
        fs.close();
    }
    return;
}
void showGrammars()
{
    for (int i = 0; i < grammars.size(); i++)
    {
        cout << left << setw(10) << grammars[i].str;
        cout << grammars[i].num << " " << grammars[i].index << " " << grammars[i].ifUse << endl;
        for (int j = 0; j < grammars[i].searchCh.size(); j++)
            cout << grammars[i].searchCh[j] << " ";
        cout << endl;
    }
}
void showStats()
{
    for (int i = 0; i < states.size(); i++)
    {
        cout << states[i].state << " ";
        cout << states[i].ifInferEmpty << " " << endl;
        for (int j = 0; j < states[i].statesIndex.size(); j++)
        {
            cout << states[i].statesIndex[j] << " ";
        }
        cout << endl;
    }
}
bool ifAllInferEmptyConfirm()
{
    for (int i = 0; i < states.size(); i++)
    {
        if (states[i].ifInferEmpty == -1)
            return false;
    }
    return true;
}
void ifInferEmpty()
{
    while (!ifAllInferEmptyConfirm())
    {
        for (int i = 0; i < states.size(); i++)
        {
            // bool notHaveInput = false;
            bool inferEmpty = false;
            // bool ifConfirmEveryNotInferEmpty = true;               //是否能确定每个都推不出空
            bool ifConfirm = false;                                //能否确定这个state 是 能推出 还是 不能推出,就是是否存在未知文法
            for (int j = 0; j < states[i].statesIndex.size(); j++) //遍历每个状态存在文法左边的文法下标
            {
                if (states[i].ifInferEmpty != -1) //这个状态能否推出空已经被确定了
                {
                    continue;
                }
                if (grammars[states[i].statesIndex[j]].ifInferEmpty == 1) //能直接通过文法判断  能推出空
                {
                    inferEmpty = true;
                }
                else if (grammars[states[i].statesIndex[j]].ifInferEmpty == 0) ////能直接通过文法判断  不能推出空 //感觉可以不用
                {
                }
                else if (grammars[states[i].statesIndex[j]].ifInferEmpty == -1) //不能通过文法判断能不能推出空
                {
                    ifConfirm = true;
                    bool ifTheGrammarInferEmpty = false;
                    bool ifTheGrammarNotConfirm = false;
                    for (int k = 3; k < grammars[states[i].statesIndex[j]].str.length(); k++)
                    {
                        char tt = grammars[states[i].statesIndex[j]].str[k];
                        if ((isInInputs(grammars[states[i].statesIndex[j]].str[k]) != -1) && grammars[states[i].statesIndex[j]].str[k] != '@')
                        {
                            grammars[states[i].statesIndex[j]].ifInferEmpty = 0; //这个文法不能推出空
                            break;
                        }
                        else if (isInStates(grammars[states[i].statesIndex[j]].str[k]) != -1)
                        {
                            if (states[isInStates(grammars[states[i].statesIndex[j]].str[k])].ifInferEmpty == -1)
                            {
                                ifTheGrammarNotConfirm = true;
                            }
                            else if (states[isInStates(grammars[states[i].statesIndex[j]].str[k])].ifInferEmpty == 0)
                            {
                                grammars[states[i].statesIndex[j]].ifInferEmpty = 0; //这个文法不能推出空
                                // ifTheGrammarInferEmpty = false;
                                break;
                            }
                            else
                            {
                                ifTheGrammarInferEmpty = true;
                            }
                        }
                        else if (grammars[states[i].statesIndex[j]].str[k] == '@')
                        {
                            inferEmpty = 1; //文法能推出空 那么状态就能推出空
                        }
                    }
                    if (ifTheGrammarInferEmpty == true && ifTheGrammarNotConfirm == false && grammars[states[i].statesIndex[j]].ifInferEmpty == -1) //有推出空的且没有不能推出空的和没有不确定的
                    {
                        grammars[states[i].statesIndex[j]].ifInferEmpty = 1;
                    }
                }
            }
            if (inferEmpty == true && states[i].ifInferEmpty == -1)
            {
                states[i].ifInferEmpty = 1;
                continue;
            }
            if (!ifConfirm && states[i].ifInferEmpty == -1)
            {
                states[i].ifInferEmpty = 0;
                continue;
            }
        }
    }
}
bool insertFirst(int a, int b, bool ifPBEmpty) //将states[b]的first集添加到states[a],如果有变化返回1 没变化返回0
{
    sort(states[a].firsts.begin(), states[a].firsts.end());
    sort(states[b].firsts.begin(), states[b].firsts.end());
    int x, y;
    x = states[b].firsts.size();
    y = states[a].firsts.size();
    bool flag = false;
    for (int i = 0; i < x; i++)
    {
        bool found = false;
        for (int j = 0; j < y; j++)
        {
            if (states[b].firsts[i] == states[a].firsts[j])
            {
                found = true;
            }
        }
        if (!found)
        {
            if (states[b].firsts[i] == '@' && ifPBEmpty == false) //如果是空 但是不需要压入空
                continue;
            flag = true;
            states[a].firsts.push_back(states[b].firsts[i]);
        }
    }
    return flag;
}
void getFirsts()
{
    bool flag = false;
    while (flag == false)
    {
        flag = true;
        for (int i = 0; i < states.size(); i++)
        {
            for (int j = 0; j < states[i].statesIndex.size(); j++)
            {
                string str = grammars[states[i].statesIndex[j]].str;
                for (int k = 3; k < str.length(); k++)
                {
                    if (isInInputs(str[k]) != -1) //如果是输入字符 那么直接压入first
                    {
                        bool ifPbInput = false;
                        for (int q = 0; q < states[i].firsts.size(); q++) //判断是否压入 程序简单 就不写函数了
                        {
                            if (str[k] == states[i].firsts[q])
                            {
                                ifPbInput = true;
                                break;
                            }
                        }
                        if (str[k] != '@') //如果不是空
                        {
                            if (ifPbInput == false)
                            {
                                states[i].firsts.push_back(str[k]);
                                flag = false;
                            }
                            break;
                        }
                        else //如果是空
                        {
                            if ((k == str.length() - 1) && ifPbInput == false) //如果是最后一位
                            {
                                states[i].firsts.push_back(str[k]);
                                flag = false;
                                break;
                            }
                        }
                    }
                    else if (isInStates(str[k]) != -1) //如果是状态
                    {
                        int stateIndex = isInStates(str[k]);
                        if (states[stateIndex].ifInferEmpty == 0) //如果该状态不能推出空
                        {
                            if (insertFirst(i, stateIndex, false)) // stateIndex 压入 i 如果有变化
                                flag = false;
                            break;
                        }
                        else if (states[stateIndex].ifInferEmpty == 1)
                        {
                            bool ifPBEmpty = false;
                            if (k == str.length() - 1)
                                ifPBEmpty = true;
                            if (insertFirst(i, stateIndex, ifPBEmpty))
                            {
                                flag = false;
                            }
                        }
                    }
                }
            }
        }
    }
}
void showFirsts()
{
    ofs << "--------------Firsts------------------\n";
    for (int i = 0; i < states.size(); i++)
    {
        ofs << "First(" << states[i].state << "): ";
        cout << "First(" << states[i].state << "): ";
        for (int j = 0; j < states[i].firsts.size(); j++)
        {
            ofs << states[i].firsts[j] << " ";
            cout << states[i].firsts[j] << " ";
        }
        ofs << endl;
        cout << endl;
    }
}
Item grammarInItem(Grammar g, Item it)
{
    for (int i = 0; i < it.grammars.size(); i++)
    {
        if (g.num == it.grammars[i].num && g.index == it.grammars[i].index)
        {
            sort(g.searchCh.begin(), g.searchCh.end());
            sort(it.grammars[i].searchCh.begin(), it.grammars[i].searchCh.end());
            for (int k = 0; k < g.searchCh.size(); k++)
            {
                bool found = false;
                for (int j = 0; j < it.grammars[i].searchCh.size(); j++)
                {
                    if (g.searchCh[k] == it.grammars[i].searchCh[j])
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    it.grammars[i].searchCh.push_back(g.searchCh[k]);
                }
            }
            return it;
        }
    }
    it.grammars.push_back(g);
    return it;
}
Item getClosures(Item it)
{
    for (int i = 0; i < it.grammars.size(); i++)
    {
        int tempGrammarYuanDianIndex = it.grammars[i].index;
        int tempstateIndex = isInStates(it.grammars[i].str[tempGrammarYuanDianIndex]);
        char tempCh;                                                 //用于计算向前搜索符
        if (tempGrammarYuanDianIndex >= it.grammars[i].str.length()) //圆点已经到最后了
        {
            tempCh = '#';
        }
        else if (tempGrammarYuanDianIndex == (it.grammars[i].str.length() - 1))
        {
            tempCh = '@';
        }
        else
        {
            tempCh = it.grammars[i].str[tempGrammarYuanDianIndex + 1];
        }
        if (tempstateIndex != -1)
        {
            for (int j = 0; j < states[tempstateIndex].statesIndex.size(); j++) //遍历该状态为左边的所有文法
            {
                //求向前搜索符
                Grammar tempg = grammars[states[tempstateIndex].statesIndex[j]];
                if (tempCh != '#')
                {
                    int tempstateIndex2 = isInStates(tempCh);       //判断这个符号是不是一个状态 也就是非终结符
                    if ((tempstateIndex2 == -1) && (tempCh != '@')) //如果不是非终结符 且不是空
                    {
                        vector<char> tempVec;
                        tempVec.push_back(tempCh);
                        tempg.searchCh = tempVec;
                    }
                    else if (tempCh == '@')
                    {
                        tempg.searchCh = it.grammars[i].searchCh; //直接继承上一个的向前搜索符
                    }
                    else if (tempstateIndex2 != -1) //是非终结符
                    {
                        if (states[tempstateIndex2].ifInferEmpty == false) //不能推出空
                        {
                            tempg.searchCh = states[tempstateIndex2].firsts; //那么直接替换就行啦
                        }
                        else //能推出空
                        {
                            for (int k = 0; k < states[tempstateIndex2].firsts.size(); k++)
                            {
                                if (states[tempstateIndex2].firsts[k] != '@') //这个first不为空的
                                {
                                    tempg.searchCh.push_back(states[tempstateIndex2].firsts[k]);
                                }
                            }
                        }
                    }
                }
                it = grammarInItem(tempg, it);
            }
        }
    }
    return it;
}
bool ifItemAllGrammarUsed(Item it)
{
    for (int i = 0; i < it.grammars.size(); i++)
    {
        if (it.grammars[i].ifUse == false)
            return false;
    }
    return true;
}
bool cmpGrammar(Grammar g1, Grammar g2)
{
    if (g1.num != g2.num)
    {
        return g1.num < g2.num;
    }
    return g1.index < g2.index;
}
bool ifItemAEuqalsItemB(Item ita, Item itb)
{
    if (ita.grammars.size() != itb.grammars.size())
        return false;
    sort(ita.grammars.begin(), ita.grammars.end(), cmpGrammar);
    sort(itb.grammars.begin(), itb.grammars.end(), cmpGrammar);
    for (int i = 0; i < ita.grammars.size(); i++)
    {
        if ((ita.grammars[i].num != itb.grammars[i].num) || (ita.grammars[i].index != itb.grammars[i].index) || (ita.grammars[i].searchCh.size() != itb.grammars[i].searchCh.size()))
            return false;
        sort(ita.grammars[i].searchCh.begin(), ita.grammars[i].searchCh.end());
        sort(itb.grammars[i].searchCh.begin(), itb.grammars[i].searchCh.end());
        for (int j = 0; j < ita.grammars[i].searchCh.size(); j++)
        {
            if (ita.grammars[i].searchCh[j] != itb.grammars[i].searchCh[j])
                return false;
        }
    }
    return true;
}
int ifTheItemInItems(Item it) //返回-1不存在 否则返回下标
{
    for (int i = 0; i < items.size(); i++)
    {
        if (ifItemAEuqalsItemB(it, items[i]))
            return i;
    }
    return -1;
}
void LALR(Item tempIt, int iftempItInItems)
{
    Item it = items[iftempItInItems];
    for (int i = 0; i < tempIt.grammars.size(); i++)
    {
        for (int j = 0; j < tempIt.grammars[i].searchCh.size(); j++)
        {
            bool found = false;
            for (int k = 0; k < it.grammars[i].searchCh.size(); k++)
            {
                if (tempIt.grammars[i].searchCh[j] == it.grammars[i].searchCh[k])
                {
                    found = true;
                    break;
                }
            }
            if (!found)
                it.grammars[i].searchCh.push_back(tempIt.grammars[i].searchCh[j]);
        }
    }
}
void getItems()
{
    //一开始Items为空的
    //初始项目
    Item it;
    it.grammars.push_back(grammars[0]); //第一步，将拓广文法添加到第一个项目中
    it = getClosures(it);
    items.push_back(it);
    for (int i = 0; i < items.size(); i++)
    {
        while (!ifItemAllGrammarUsed(items[i])) //当这个项目集所有的项目还没被遍历使用完
        {
            char tempCh = '@'; //可以充当一个判断的作用 同时还有移动的字符的作用
            Item tempIt;
            for (int j = 0; j < items[i].grammars.size(); j++)
            {
                int tempGrammarYuanDianIndex = items[i].grammars[j].index;
                if (tempGrammarYuanDianIndex == items[i].grammars[j].str.length())
                {
                    items[i].grammars[j].ifUse = true;
                }
                if (items[i].grammars[j].ifUse == false && tempGrammarYuanDianIndex < items[i].grammars[j].str.length())
                {
                    if (tempCh == '@')
                    {
                        tempCh = items[i].grammars[j].str[tempGrammarYuanDianIndex];
                    }
                    if (tempCh == items[i].grammars[j].str[tempGrammarYuanDianIndex])
                    {
                        items[i].grammars[j].ifUse = true;
                        Grammar tempg = items[i].grammars[j];
                        tempg.index++;
                        tempg.ifUse = false;
                        tempIt.grammars.push_back(tempg);
                    }
                }
            }
            tempIt = getClosures(tempIt);
            //这里需要判断这个tempIt是否已经存在了 而且是空的也不用加进去
            int iftempItInItems = ifTheItemInItems(tempIt);
            if ((tempIt.grammars.size() > 0) && (iftempItInItems == -1))
            {
                items.push_back(tempIt);
                items[i].gotoItem.push_back(make_pair(items.size() - 1, tempCh));
            }
            else if (iftempItInItems != -1)
            {
                //存在，需要合并同心集,向前搜索符求和
                LALR(tempIt, iftempItInItems);
                items[i].gotoItem.push_back(make_pair(iftempItInItems, tempCh));
            }
        }
    }
}
void showItems()
{
    ofs << "--------------Items------------------\n";
    for (int i = 0; i < items.size(); i++)
    {
        ofs << "I" << i << ":\n";
        cout << "I" << i << ":\n";
        for (int j = 0; j < items[i].grammars.size(); j++)
        {
            ofs << left << setw(9) << items[i].grammars[j].str << "  圆点的位置：" << items[i].grammars[j].index;
            ofs << "    向前搜索符：";
            cout << left << setw(9) << items[i].grammars[j].str << "  圆点的位置：" << items[i].grammars[j].index;
            cout << "    向前搜索符：";
            for (int k = 0; k < items[i].grammars[j].searchCh.size(); k++)
            {
                ofs << items[i].grammars[j].searchCh[k];
                cout << items[i].grammars[j].searchCh[k];
                if (k < items[i].grammars[j].searchCh.size() - 1)
                {
                    ofs << ",";
                    cout << ",";
                }
            }
            ofs << endl;
            cout << endl;
        }
        for (int j = 0; j < items[i].gotoItem.size(); j++)
        {
            ofs << "MoveTo I" << items[i].gotoItem[j].first << "  across: " << items[i].gotoItem[j].second << " \n";
            cout << "MoveTo I" << items[i].gotoItem[j].first << "  across: " << items[i].gotoItem[j].second << " \n";
        }
        ofs << "--------------------------------\n";
        cout << "--------------------------------\n";
    }
}
void initActionAndGoto()
{
    for (int i = 0; i < items.size(); i++)
    {
        // init Action
        for (int j = 0; j < inputs.size(); j++)
        {
            items[i].Action.push_back(make_pair('#', -1));
        }
        items[i].Action.push_back(make_pair('#', -1)); //添加'#'
        // init Goto
        for (int j = 0; j < states.size(); j++)
        {
            items[i].Goto.push_back(-1);
        }
    }
}
void getActionAndGoto()
{
    initActionAndGoto(); //初始化Action和Goto
    for (int i = 0; i < items.size(); i++)
    {
        int t = 1;
        for (int j = 0; j < items[i].grammars.size(); j++)
        {
            Grammar tempG = items[i].grammars[j];
            if (tempG.index < tempG.str.length()) //不是归约
            {
                char tempCh = tempG.str[tempG.index];
                int ifInInputs = isInInputs(tempCh);
                int ifInStates = isInStates(tempCh);
                int actionto;
                for (int k = 0; k < items[i].gotoItem.size(); k++) //寻找action到哪
                {
                    if (items[i].gotoItem[k].second == tempCh)
                    {
                        actionto = items[i].gotoItem[k].first;
                        break;
                    }
                }
                if (ifInInputs != -1) //是终结符
                {
                    items[i].Action[ifInInputs] = make_pair('S', actionto);
                }
                else if (ifInStates != -1) //是非终结符
                {
                    items[i].Goto[ifInStates] = actionto;
                }
            }
            else if (tempG.index == tempG.str.length()) //是归约
            {
                if (tempG.str[0] == '$') //如果是初态
                {
                    items[i].Action[items[i].Action.size() - 1] = make_pair('$', -2); //代表acc
                    continue;
                }
                for (int k = 0; k < tempG.searchCh.size(); k++) //对每一个向前搜索符
                {
                    char tempCh = tempG.searchCh[k];
                    if (tempCh != '#')
                    {
                        int ifInInputs = isInInputs(tempCh);
                        items[i].Action[ifInInputs] = make_pair('r', tempG.num);
                    }
                    else
                    {
                        items[i].Action[items[i].Action.size() - 1] = make_pair('r', tempG.num);
                    }
                }
            }
        }
    }
}
void showActionAndGoto()
{
    ofs << "    States     ||                                            Action                                             ||              Goto\n";
    ofs << "               || ";
    cout << "    States     ||                                            Action                                             ||              Goto\n";
    cout << "               || ";
    for (int i = 0; i < inputs.size(); i++)
    {
        ofs << inputs[i] << " | ";
        cout << inputs[i] << " | ";
    }
    ofs << "# ||  ";
    cout << "# ||  ";
    for (int i = 0; i < states.size(); i++)
    {
        if (states[i].state != '$')
        {
            ofs << states[i].state << " | ";
            cout << states[i].state << " | ";
        }
    }
    ofs << endl;
    cout << endl;
    for (int i = 0; i < items.size(); i++)
    {
        ofs << "      " << i << "       ";
        cout << "      " << i << "       ";
        if (i < 10)
        {
            ofs << " ";
            cout << " ";
        }
        ofs << "||";
        cout << "||";
        for (int j = 0; j < items[i].Action.size(); j++)
        {
            if (items[i].Action[j].second == -1)
            {
                ofs << "   ";
                cout << "   ";
            }
            else if (items[i].Action[j].second == -2)
            {
                ofs << "acc";
                cout << "acc";
            }
            else
            {
                ofs << items[i].Action[j].first << items[i].Action[j].second;
                cout << items[i].Action[j].first << items[i].Action[j].second;
                if (items[i].Action[j].second < 10)
                {
                    ofs << " ";
                    cout << " ";
                }
            }
            ofs << "|";
            cout << "|";
        }
        ofs << "| ";
        cout << "| ";
        for (int j = 1; j < items[i].Goto.size(); j++)
        {
            if (items[i].Goto[j] == -1)
            {
                ofs << "   ";
                cout << "   ";
            }
            else
            {
                if (items[i].Goto[j] < 10)
                {
                    ofs << " ";
                    cout << " ";
                }
                ofs << items[i].Goto[j] << " ";
                cout << items[i].Goto[j] << " ";
            }
            ofs << "|";
            cout << "|";
        }
        ofs << endl;
        cout << endl;
    }
}
void printChStack(vector<char> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        ofs << vec[i];
        cout << vec[i];
    }
    for (int i = 0; i < 24 - vec.size(); i++)
    {
        ofs << " ";
        cout << " ";
    }
}
void printStateStack(vector<int> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        ofs << vec[i] << " ";
        cout << vec[i] << " ";
        if (vec[i] < 10)
        {
            ofs << " ";
            cout << " ";
        }
    }
    for (int i = 0; i < 70 - vec.size() * 3; i++)
    {
        ofs << " ";
        cout << " ";
    }
}
void printStrStack(vector<char> vec)
{
    for (int i = vec.size() - 1; i >= 0; i--)
    {
        ofs << vec[i];
        cout << vec[i];
    }
    for (int i = 0; i < 51 - vec.size(); i++)
    {
        ofs << " ";
        cout << " ";
    }
}
void solveWrong(vector<char> vec)
{
    ifstream fs("LexicalAnalysisProcess.txt");
    int i = -1;
    int j = StrNum - vec.size();
    string line;
    while (i != j)
    {
        i++;
        getline(fs, line);
    }
    int k = 0;
    string ch;      //符号
    string lineNum; //行号
    bool flag = false;
    for (int q = 0; q < line.size(); q++)
    {
        if (line[q] != ' ')
        {
            flag = false;
            if (k == 0)
            {
                ch += line[q];
            }
            if (k == 2)
            {
                lineNum += line[q];
            }
        }
        else
        {
            if (!flag)
            {
                k++;
                flag = true;
            }
        }
    }
    ofs << "在行号为：" << lineNum << "  符号为：" << ch << " 出错  ";
    cout << "在行号为：" << lineNum << "  符号为：" << ch << " 出错  ";
}
string soveWrongPrompt(char ch)
{
    string str;
    if (ch >= 'a' && ch <= 'z')
    {
        if (ch == 'z')
        {
            str = "常量";
        }
        else if (ch == 'y')
        {
            str = "标识符";
        }
        else
        {
            str = Keywords[ch - 'a'];
        }
    }
    else
    {
        if (ch == '1')
        {
            str = "单目运算符";
        }
        else if (ch == '2')
        {
            str = "双目运算符";
        }
        else if (ch == '3')
        {
            str = "#";
        }
        else
        {
            str = ch;
        }
    }
    return str;
}
void solve()
{
    ofs << "         符号栈                                   状态栈                                                       输入符号串                           动作 " << endl;
    cout << "         符号栈                                   状态栈                                                       输入符号串                           动作 " << endl;
    ifstream fs("SyntaxAnalysisSourceProgram.txt");
    string line;
    if (fs.is_open())
    {
        getline(fs, line);
        fs.close();
    }
    vector<char> vecStr;  //输入符号串
    vector<char> vecCh;   //符号栈
    vector<int> vecState; //状态栈
    vecStr.push_back('#');
    vecCh.push_back('#');
    vecState.push_back(0);
    for (int i = line.size() - 1; i >= 0; i--)
    {
        vecStr.push_back(line[i]);
    }
    StrNum = vecStr.size(); //输入符号串长度
    bool flag = false;
    while (!flag)
    {
        char tempCh = vecStr[vecStr.size() - 1];
        int tempState = vecState[vecState.size() - 1];
        int ifInInputs = isInInputs(tempCh);
        if (tempCh == '#') //由于'#'不是inputs，所以我们这另判 因为之前把'#'加到了action的最后一项
            ifInInputs = items[tempState].Action.size() - 1;
        char ifCh = items[tempState].Action[ifInInputs].first;  // Action第一项 判断S还是r
        int ifInt = items[tempState].Action[ifInInputs].second; // Action第二项
        if (ifCh == 'S')
        {
            vecState.push_back(ifInt); //状态入栈
            vecCh.push_back(tempCh);   //符号入栈
            vecStr.pop_back();         //输入串弹栈
            printChStack(vecCh);
            printStateStack(vecState);
            printStrStack(vecStr);
            ofs << "移进\n";
            cout << "移进\n";
        }
        else if (ifCh == 'r')
        {
            Grammar tempG = grammars[ifInt];
            int tempk = tempG.str.length() - 3;
            char tempCha = tempG.str[0];          //产生式左部符号
            int tempState2 = isInStates(tempCha); //产生式左部在哪
            for (int i = 0; i < tempk; i++)
            {
                vecState.pop_back(); //弹出k个状态
                vecCh.pop_back();    //符号栈弹栈
            }
            int tempState3 = vecState[vecState.size() - 1];         //当前状态栈顶
            vecState.push_back(items[tempState3].Goto[tempState2]); //状态栈入栈
            vecCh.push_back(tempCha);                               //符号栈入栈
            printChStack(vecCh);
            printStateStack(vecState);
            printStrStack(vecStr);
            ofs << "归约:" << tempG.str;
            ofs << endl;
            cout << "归约:" << tempG.str;
            cout << endl;
        }
        else if (ifCh == '$' && ifInt == -2)
        {
            flag = true;
            ofs << "---------Acc----YES-------\n";
            cout << "---------Acc----YES-------\n";
        }
        else
        {
            flag = true;
            ofs << "---------Wrong----NO-------\n";
            ofs << "可能的错误原因：\n";
            cout << "---------Wrong----NO-------\n";
            cout << "可能的错误原因：\n";
            for (int i = 0; i < items[tempState].Action.size(); i++)
            {
                if (items[tempState].Action[i].second != -1)
                {
                    ofs << "                ";
                    cout << "                ";
                    solveWrong(vecStr);
                    // printChStack(vecCh);
                    string strPrompt = soveWrongPrompt(inputs[i]);
                    ofs << "可能是在此之后缺少: " << strPrompt << endl;
                    cout << "可能是在此之后缺少: " << strPrompt << endl;
                }
            }
        }
    }
}