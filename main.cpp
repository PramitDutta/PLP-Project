// RPAL_Interpreter.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
#include <vector>
#include <list>
#include <utility>
#include <map>
#include <cmath>
using namespace std;

class TreeNode {
private:
    string token;
    TreeNode* leftChild;
    TreeNode* rightSibling;
    string type;
    bool isTuple;
    int deltaNumber;
    int lambdaNum;
    int envNum;
    int betaIfNum;
    int betaElseNum;
    string lambdaParam;
    std::vector<TreeNode*> tuple;
    int lambdaEnv;
    int tauCount;

public:
    void setToken(string token) {
        this->token = token;
    }
    void setChild(TreeNode* leftChild) {
        this->leftChild = leftChild;
    }
    void setSibling(TreeNode* rightSibling) {
        this->rightSibling = rightSibling;
    }
    void setTokenType(string typeN) {
        type = typeN;
    }
    void setDeltaNum(int deltaNum) {
        deltaNumber = deltaNum;
    }
    void setLambdaNum(int lamNum) {
        lambdaNum = lamNum;
    }
    void setEnvNum(int envNum) {
        this->envNum = envNum;
    }
    void setBetaIfNum(int bNum1) {
        betaIfNum = bNum1;
    }
    void setBetaElseNum(int bNum2) {
        betaElseNum = bNum2;
    }
    void setTuple(vector<TreeNode*> tuple) {
        this->tuple = tuple;
    }
    void setLambdaEnv(int lambEnv) {
        lambdaEnv = lambEnv;
    }
    void setIsTuple(bool value) {
        isTuple = value;
    }
    void setLambdaParam(string lambParam) {
        lambdaParam = lambParam;
    }
    void setTauCount(int count) {
        tauCount = count;
    }

    string getToken() {
        return token;
    }
    TreeNode* getChild() {
        return leftChild;
    }
    TreeNode* getSibling() {
        return rightSibling;
    }
    string getTokenType() {
        return type;
    }
    bool getIsTuple() {
        return isTuple;
    }
    int getDeltaNum() {
        return deltaNumber;
    }
    int getLambdaNum() {
        return lambdaNum;
    }
    int getEnvNum() {
        return envNum;
    }
    int getBetaIfNum() {
        return betaIfNum;
    }
    int getBetaElseNum() {
        return betaElseNum;
    }
    vector<TreeNode*> getTuple() {
        return tuple;
    }
    int getLambdaEnv() {
        return lambdaEnv;
    }
    string getLambdaParam() {
        return lambdaParam;
    }
    int getTauCount() {
        return tauCount;
    }

};

//global variable to store each line of the code
string line;
//global variable to store the type of token
string type;
//global variable to store the token
string token;
//array to store the program
string codeLines[20000];
//file stream
ifstream myfile;
//global index on the string array
int index = 0;
//stack of tree nodes to store for push and pop
//int tauToken_Count;
std::stack<TreeNode*> tstack;
void read(string s);
string tokenNodeBuilder(string root, int num);
void buildTree(string root, int num);
void traversal(TreeNode* n, int level);
void printAst();
void printNodes(TreeNode* node, int level);


void E();
void Ew();
void T();
void Ta();
void Tc();
void B();
void Bt();
void Bs();
void Bp();
void A();
void At();
void Af();
void Ap();
void R();
void Rn();
void D();
void Da();
void Dr();
void Db();
void Vb();
void Vl();

string readfile();
string parse();
string getToken();
string getTokenType();

bool isLetters(string s);
bool isDigits(string s);
bool isOperators(string s);
bool isPunc(string s);
bool isKeyword(string s);
bool isMisc(string s);

void startOp();
void fillArray(string f);

TreeNode* transformTree(TreeNode* root);
TreeNode* transformLET(TreeNode* letNode);
TreeNode* transformWHERE(TreeNode* whereNode);
TreeNode* transformFCNFORM(TreeNode* fcnNode);
TreeNode* transformWITHIN(TreeNode* withinNode);
TreeNode* transformAND(TreeNode* andNode);
TreeNode* transformAT(TreeNode* atNode);
TreeNode* transformLAMBDA(TreeNode* lambdaNode);
TreeNode* transformREC(TreeNode* recNode);

typedef map<string, string> envMap;
typedef pair<int, string> keyPair;

map<int, vector<TreeNode*>> deltaMap;
std::queue<TreeNode*> pDeltaQueue;
map<keyPair, TreeNode*> parameterMap;
map<int, int> environmentMap;
stack<int> environmentStack;

bool printCalled = false;
int currentDeltaNumber = 0;
int deltaCounter = 0;
int currentEnv = 0;
int envCounter = 0;


ostringstream oss;
TreeNode* inputTree;


void createControlStruc(TreeNode* root);
void preOrderTraversal(TreeNode* root, vector<TreeNode*> &currentDelta);
string intToString(int intValue);
vector<string> split(string inputString, char delimiter);
bool isParameter(TreeNode* currNode);
TreeNode* applyOperator(TreeNode* first, TreeNode* second, TreeNode* current);
void processCurrentNode(TreeNode* &currNode, stack<TreeNode*> &controlStack, stack<TreeNode*> &executionStack);
void evaluateTree(TreeNode* input);



int i = 0;

int main(int arg, char*argc[])

{
    
    if (arg <= 1) return 0;

    *argc++;

    string argument1(*argc++);

    if (argument1 != "-ast")
    return 0;

    string f(*argc);
    
    //string f = "C:\\Users\\Anonymous\\Desktop\\rpal\\tests\\fn2";

    fillArray(f);

    startOp();
    //cout << "------------" << endl;
    TreeNode* root = tstack.top();

    TreeNode* newroot = new TreeNode();
    newroot = transformTree(root);
    //printAst();
    //printNodes(newroot,0);
    environmentStack.push(0);
    evaluateTree(newroot);
    cout << endl;
    //createControlStruc(root);

    /*
    for (std::map<int, vector<TreeNode*>>::iterator it = deltaMap.begin(); it != deltaMap.end(); ++it) {
    std::cout << it->first << " => " << '\n';
    for (int i = 0; i < it->second.size(); i++)
    {
    cout << it->second.at(i)->getToken() << endl;
    }
    } */

    return 0;

}

bool isLetters(string s) {

    string alphabets[52];

    int x = 0;

    for (int i = 0; i<26; i++) { alphabets[i] = 'a' + i; }



    for (int i = 26; i<52; i++) { alphabets[i] = 'A' + x++; }

    for (int i = 0; i<52; i++) {

        if (s == alphabets[i]) {

            return true;

        }



    }



    return false;

}

bool isDigits(string s) {

    string numbers[10];

    for (int i = 0; i<10; ++i) { numbers[i] = '0' + i; }

    for (int i = 0; i<10; i++) {

        if (s == numbers[i])

            return true;

        //else

        //return false;

    }

    return false;

}

bool isOperators(string s) {

    string oper[26] = { "+", "-", "*", "<", ">", "&", ".", "@", "/", ":", "=", "~", "|", "$", "!", "#", "%",

        "^", "_", "[", "]", "{", "}", "\"", "`", "?" };



    for (int i = 0; i<26; i++) {

        if (s == oper[i])

            return true;

        //else

        //return false;

    }

    return false;

}

bool isPunc(string s) {

    string punc[4] = { "(", ")", ";", "," };

    for (int i = 0; i<4; i++) {

        if (s == punc[i])

            return true;

        //else

        //return false;

    }

    return false;

}

bool isKeyword(string s) {

    string key[16] = { "let", "in", "fn", "where", "aug", "or", "not", "gr", "ge", "ls", "le", "eq", "ne", "within", "and", "rec" };

    for (int i = 0; i<16; i++) {

        if (s == key[i])

            return true;

        //else

        //return false;

    }

    return false;

}

bool isMisc(string s) {

    string misc[4] = { "true", "false", "nil", "dummy" };

    for (int i = 0; i<4; i++) {

        if (s == misc[i])

            return true;

        //else

        //return false;

    }

    return false;

}

void fillArray(string f) {

    //string f="C:\\Users\\Hp\\PLP\\rpal\\tests\\picture";

    myfile.open(f.c_str());

    if (myfile.is_open()) {

        while (getline(myfile, line)) {

            codeLines[index++] = line + " ";

        }

    }

    myfile.close();

}

string readfile() {

    string l2;

    if (index>0) {

        l2 = codeLines[i++];

    }

    return l2;

}

string parse() {

    string parString;

    bool flag;

    string str;

    do {

        flag = false;

        char c = codeLines[i][0];

        str = "";

        str = str + c;

        if (isLetters(str) == true) {

            type = "<IDENTIFIER>";

            while (true) {

                parString = parString + c;

                codeLines[i] = codeLines[i].substr(1);

                c = codeLines[i][0];

                str = "";

                str = str + c;

                if (codeLines[i].length() == 0) {

                    codeLines[i] = "";

                    break;

                }

                if (isLetters(str) == true)

                    continue;

                if (isDigits(str) == true)

                    continue;

                else if (c == '_')

                    continue;

                else

                    break;

            }

        }

        else if (isDigits(str) == true) {

            type = "<INTEGER>";

            while (true) {

                parString = parString + c;

                codeLines[i] = codeLines[i].substr(1);

                c = codeLines[i][0];

                str = "";

                str = str + c;

                if (codeLines[i].length() == 0) {

                    codeLines[i] = "";

                    break;

                }

                if (isDigits(str) == true) {

                    continue;

                }

                else {

                    break;

                }

            }

        }

        else if (codeLines[i][0] == '/' && codeLines[i][1] == '/') {

            type = "<DELETE>";

            i++;

            if (codeLines[i] == " ") {

                parString = "";

            }

            flag = true;

        }

        else if (isOperators(str) == true) {

            type = "<OPERATOR>";

            while (true) {

                parString = parString + c;

                codeLines[i] = codeLines[i].substr(1);

                c = codeLines[i][0];

                str = "";

                str = str + c;

                if (codeLines[i].length() == 0) {

                    codeLines[i] = "";

                    break;

                }

                if (isOperators(str) == true) {

                    continue;

                }

                else {

                    break;

                }

            }

        }

        else if (codeLines[i][0] == '\'') {

            type = "<STRING>";

            while (true) {

                parString = parString + c;

                codeLines[i] = codeLines[i].substr(1);

                c = codeLines[i][0];

                str = "";

                str = str + c;

                if (codeLines[i].length() == 0) {

                    codeLines[i] = "";

                    break;

                }

                if (codeLines[i][0] == '\\') {

                    c = codeLines[i][0];

                    codeLines[i] = codeLines[i].substr(1);

                    parString = parString + c;

                    c = codeLines[i][0];

                    codeLines[i] = codeLines[i].substr(1);

                    parString = parString + c;

                    c = codeLines[i][0];
                }

                if (codeLines[i][0] == '\'') {

                    c = codeLines[i][0];

                    codeLines[i] = codeLines[i].substr(1);

                    parString = parString + c;

                    if (codeLines[i].length() == 0)

                        codeLines[i] = "";

                    break;

                }

            }

        }

        else if (c == ' ' || c == '\t' || c == '\n') {

            type = "<DELETE>";

            parString = parString + c;

            codeLines[i] = codeLines[i].substr(1);

            if (codeLines[i].length() == 0) {

                i++;

            }

            if (myfile.eof()) {

                parString = "";

            }

            flag = true;

        }

        else if (isPunc(str) == true) {

            type = "<PUNCTIONS>";

            parString = parString + c;

            codeLines[i] = codeLines[i].substr(1);

            c = codeLines[i][0];

            str = "";

            str = str + c;

            if (codeLines[i].length() == 0) {

                codeLines[i] = "";

            }

        }

        if (c == '\0') {

            i++;

            if (i == (index - 1) && parString.length() == 0) {

                parString = "";

            }

        }

    } while (flag == true);

    return parString;

}

string getToken() {

    token = "";

    token = parse();

    return token;

}

string getTokenType() {

    if (token == "")

        type = "<EOF>";

    return type;

}

void read(string s) {


    if ((isKeyword(token) == false) && (getTokenType() == "<IDENTIFIER>" ||

        getTokenType() == "<INTEGER>" || getTokenType() == "<STRING>"))

        buildTree(token, 0);


    token = getToken();
    //type = getTokenType();


}

string tokenNodeBuilder(string token, int num) {

    string str;

    if (num == 0 && getTokenType() == "<IDENTIFIER>" && (isKeyword(token) == false)) {

        if (isMisc(token) == true) {

            str = "<" + token + ">"; // changes
            //str = token;

        }

        else {

            str = "<ID:" + token + ">"; //changes
            //str = token;

        }

    }

    else if (num == 0 && getTokenType() == "<INTEGER>")

        str = "<INT:" + token + ">"; //changes
        //str = token;

    else if (num == 0 && getTokenType() == "<STRING>" && (isKeyword(token) == false))

        str = "<STR:" + token + ">"; //changes
        //str = token;

    else str = token;

    return str;

}

void buildTree(string token, int num) {

    int i = 0;

    TreeNode* temp = new TreeNode();

    TreeNode* sibling = new TreeNode();

    string str = tokenNodeBuilder(token, num);

    string type = getTokenType(); //changes

    while (i<num) {


        sibling = tstack.top();

        tstack.pop();

        //temp = sibling;

        sibling->setSibling(temp);

        temp = sibling;

        i++;


    }

    TreeNode* res = new TreeNode();

    res->setToken(str);

    //res->setTokenType(type); //changes

    res->setChild(temp);

    res->setSibling(NULL);

    tstack.push(res);

}

void E() {

    if (token == "let") {

        read("let");

        D();

        read("in");

        E();

        buildTree("let", 2);

    }

    else if (token == "fn") {

        read("fn");

        int num = 0;

        do {

            Vb();

            num++;

        } while (getTokenType() == "<IDENTIFIER>" || token == "(");

        read(".");

        E();

        buildTree("lambda", num + 1);

    }

    else {

        Ew();

    }

}

void Ew() {

    T();

    if (token == "where") {

        read("where");

        Dr();

        buildTree("where", 2);

    }



}

void T() {

    Ta();

    int num = 0;

    while (token == ",") {

        read(",");

        Ta();

        num++;

    }

    if (num>0) {

        buildTree("tau", num + 1);

    }

}

void Ta() {

    Tc();

    while (token == "aug") {

        read("aug");

        Tc();

        buildTree("aug", 2);

    }

}

void Tc() {

    B();

    if (token == "->") {

        read("->");

        Tc();

        read("|");

        Tc();

        buildTree("->", 3);

    }

}

void B() {

    Bt();

    while (token == "or") {

        read("or");

        Bt();

        buildTree("or", 2);

    }

}

void Bt() {

    Bs();

    while (token == "&") {

        read("&");

        Bs();

        buildTree("&", 2);

    }

}

void Bs() {

    if (token == "not") {

        read("not");

        Bp();

        buildTree("not", 1);

    }

    else {

        Bp();

    }

}

void Bp() {

    A();

    if (token == "gr" || token == ">") {

        read(token);

        A();

        buildTree("gr", 2);

    }

    else if (token == "ge" || token == ">=") {

        read(token);

        A();

        buildTree("ge", 2);

    }

    else if (token == "ls" || token == "<") {

        read(token);

        A();

        buildTree("ls", 2);

    }

    else if (token == "le" || token == "<=") {

        read(token);

        A();

        buildTree("le", 2);

    }

    else if (token == "eq") {

        read("eq");

        A();

        buildTree("eq", 2);

    }

    else if (token == "ne") {

        read("ne");

        A();

        buildTree("ne", 2);

    }

}

void A() {

    if (token == "+") {

        read("+");

        At();

    }

    else if (token == "-") {

        read("-");

        At();

        buildTree("neg", 1);

    }

    else {

        At();

    }

    string temp;

    while (token == "+" || token == "-") {

        temp = token;

        read(temp);

        At();

        buildTree(temp, 2);

    }

}

void At() {

    Af();

    string temp;

    while (token == "*" || token == "/") {

        temp = token;

        read(temp);

        Af();

        buildTree(temp, 2);

    }

}

void Af() {

    Ap();

    if (token == "**") {

        read("**");

        Af();

        buildTree("**", 2);

    }

}

void Ap() {

    R();

    while (token == "@") {

        read("@");

        read(token);

        R();

        buildTree("@", 3);

    }

}

void R() {

    Rn();

    while ((isKeyword(token) == false) && (getTokenType() == "<IDENTIFIER>" || getTokenType() == "<INTEGER>" || getTokenType() == "<STRING>" || token == "true" || token == "false" || token == "nil" || token == "(" || token == "dummy")) {

        Rn();

        buildTree("gamma", 2);

    }

}

void Rn() {

    if (getTokenType() == "<IDENTIFIER>" || getTokenType() == "<INTEGER>" || getTokenType() == "<STRING>") {

        read(token);

    }

    else if (token == "true") {

        read("true");

        buildTree("true", 0);

    }

    else if (token == "false") {

        read("false");

        buildTree("true", 0);

    }

    else if (token == "nil") {

        read("nil");

        buildTree("nil", 0);

    }

    else if (token == "(") {

        read("(");

        E();

        read(")");

    }

    else if (token == "dummy") {

        read("dummy");

        buildTree("dummy", 0);

    }



}

void D() {

    Da();

    if (token == "within") {

        read("within");

        D();

        buildTree("within", 2);

    }

}

void Da() {

    Dr();

    int num = 0;

    while (token == "and") {

        read("and");

        Dr();

        num++;

    }

    if (num>0) {

        buildTree("and", num + 1);

    }

}

void Dr() {

    if (token == "rec") {

        read("rec");

        Db();

        buildTree("rec", 1);

    }

    else

        Db();

}

void Db() {

    if (getTokenType() == "<IDENTIFIER>") {

        Vl();

        if (token == "=") {

            read("=");

            E();

            buildTree("=", 2);

        }

        else {

            int num = 0;

            while (getTokenType() == "<IDENTIFIER>" || token == "(") {

                Vb();

                num++;

            }

            read("=");

            E();

            buildTree("function_form", num + 2);

        }

    }

    else if (token == "(") {

        read("(");

        D();

        read(")");

    }

}

void Vb() {

    if (getTokenType() == "<IDENTIFIER>")

        read(token);

    else if (token == "(") {

        read("(");

        if (token == ")") {

            read(")");

            buildTree("(", 2);

        }

        else {

            Vl();

            read(")");

        }

    }

}

void Vl() {

    if (getTokenType() == "<IDENTIFIER>") {

        read(token);

    }

    int num = 0;

    while (token == ",") {

        read(",");

        read(token);

        num++;

    }

    if (num>0)

        buildTree(",", num + 1);

}

void traversal(TreeNode* n, int level) {

    string str = "";

    for (int i = 0; i<level; i++)

        str = str + ".";

    if (n != NULL && n->getToken().length() != 0) {
        //if (n->getTokenType() == "<INTEGER>") {
            //str = "<ID:" + token + ">";
            //cout << str + "<INT:" + n->getToken() + ">" << endl;
        //}
        //else if (n->getTokenType() == "<IDENTIFIER>") {
           // cout << str + "<ID:" + n->getToken() + ">" << endl;
        //}
        //else if (n->getTokenType() == "<STRING>") {
           // cout << str + "<STR:" + n->getToken() + ">" << endl;
        //}

        //else
            cout << str + "" +n->getToken() << endl;

    }

    else {

        return;

    }

    //cout << str+""+n->getToken() << endl;

    if (n->getChild() != NULL) {

        traversal(n->getChild(), level + 1);

    }

    if (n->getSibling() != NULL) {

        traversal(n->getSibling(), level);

    }

}

void printAst() {

    TreeNode* AST = new TreeNode();

    AST = tstack.top();

    tstack.pop();
    //tstack.push(AST);

    traversal(AST, 0);

}

void startOp() {

    token = getToken();

    E();

    if (getTokenType() == "<EOF>") {

        printAst();

    }

}

void changeNodeTypes(TreeNode* n, int level) {
    for (int i = 0; i < level; i++) {
        if (n != NULL && n->getToken().length() != 0) {
            string str = n->getToken();
            string set;
            if (str == "let") set = "let";
            else if (str == "lambda") set = "lambda";
        }
    }
}

/*-------------------------------Project 2 Start-------------------- */

/************************Standardize Tree************************************/

TreeNode* transformTree(TreeNode* root) {

    TreeNode* tRoot = root;
    TreeNode* nextChild = tRoot->getChild();
    bool firstChild = true;
    while (nextChild != NULL && nextChild->getToken() != "") {
        TreeNode* transformedChild = transformTree(nextChild);
        if (firstChild == true) {
            tRoot->setChild(transformedChild);
            tRoot = tRoot->getChild();
            firstChild = false;
        }
        else {
            tRoot->setSibling(transformedChild);
            tRoot = tRoot->getSibling();
        }
        nextChild = nextChild->getSibling();
    }

    string rootToken = root->getToken();
    if (rootToken == "let") {
        root = transformLET(root);
    }

    else if (rootToken == "where") {
        root = transformWHERE(root);
    }

    else if (rootToken == "function_form") {
        root = transformFCNFORM(root);
    }

    else if (rootToken == "within") {
        root = transformWITHIN(root);
    }

    else if (rootToken == "and") {
        root = transformAND(root);
    }

    else if (rootToken == "@") {
        root = transformAT(root);
    }

    else if (rootToken == "lambda") {
        root = transformLAMBDA(root);
    }
    else if (rootToken == "rec") {
        root = transformREC(root);
    }

    return root;
}

TreeNode* transformLET(TreeNode* letNode) {
    TreeNode* lambdaNode = new TreeNode();
    TreeNode* gammaNode = new TreeNode();
    lambdaNode->setToken("lambda");
    lambdaNode->setTokenType("lambda"); //changes
    gammaNode->setToken("gamma");
    gammaNode->setTokenType("gamma"); //changes
    gammaNode->setChild(lambdaNode);
    lambdaNode->setSibling(letNode->getChild()->getChild()->getSibling());
    lambdaNode->setChild(letNode->getChild()->getChild());
    lambdaNode->getChild()->setSibling(letNode->getChild()->getSibling());

    return gammaNode;

}

TreeNode* transformWHERE(TreeNode* whereNode) {
    TreeNode* lambdaNode = new TreeNode();
    TreeNode* gammaNode = new TreeNode();
    lambdaNode->setToken("lambda");
    lambdaNode->setTokenType("lambda"); //changes
    gammaNode->setToken("gamma");
    gammaNode->setTokenType("gamma"); //changes
    gammaNode->setChild(lambdaNode);
    gammaNode->getChild()->setSibling(whereNode->getChild()->getSibling()->getChild()->getSibling());
    gammaNode->getChild()->setChild(whereNode->getChild()->getSibling()->getChild());
    gammaNode->getChild()->getChild()->setSibling(whereNode->getChild());
    gammaNode->getChild()->getChild()->getSibling()->setSibling(NULL);
    return gammaNode;

}

TreeNode* transformFCNFORM(TreeNode* fcnNode) {
    TreeNode* equalNode = new TreeNode();
    equalNode->setToken("=");
    int count = 0;
    std::stack<TreeNode*> stack;
    //TreeNode* first = new TreeNode();
    //TreeNode* second = new TreeNode();
    stack.push(fcnNode->getChild());
    TreeNode* child = fcnNode->getChild()->getSibling();
    while (child != NULL && child->getToken() != "") {
        count++;
        stack.push(child);
        child = child->getSibling();
    }
    count = count - 1;
    while (count != 0) {
        TreeNode* first = stack.top();
        stack.pop();
        TreeNode* second = stack.top();
        stack.pop();
        second->setSibling(first);
        TreeNode* lambdaNode = new TreeNode();
        lambdaNode->setToken("lambda");
        lambdaNode->setTokenType("lambda"); //changes
        lambdaNode->setChild(second);
        stack.push(lambdaNode);
        count--;
    }
    TreeNode* first = stack.top();
    stack.pop();
    TreeNode* second = stack.top();
    stack.pop();
    second->setSibling(first);
    equalNode->setChild(second);
    return equalNode;
}

TreeNode* transformWITHIN(TreeNode* withinNode) {
    TreeNode* lambdaNode = new TreeNode();
    TreeNode* gammaNode = new TreeNode();
    TreeNode* equalNode = new TreeNode();
    lambdaNode->setToken("lambda");
    lambdaNode->setTokenType("lambda"); //changes
    gammaNode->setToken("gamma");
    gammaNode->setTokenType("gamma"); //changes
    equalNode->setToken("=");
    gammaNode->setChild(lambdaNode);
    lambdaNode->setSibling(withinNode->getChild()->getChild()->getSibling());
    lambdaNode->setChild(withinNode->getChild()->getChild());
    lambdaNode->getChild()->setSibling(withinNode->getChild()->getSibling()->getChild()->getSibling());
    equalNode->setChild(withinNode->getChild()->getSibling()->getChild());
    equalNode->getChild()->setSibling(gammaNode);
    return equalNode;
}

TreeNode* transformAND(TreeNode* andNode) {
    TreeNode* equalNode = new TreeNode();
    TreeNode* commaNode = new TreeNode();
    TreeNode* tauNode = new TreeNode();
    tauNode->setToken("tau");
    commaNode->setToken(",");
    equalNode->setToken("=");
    equalNode->setChild(commaNode);
    commaNode->setSibling(tauNode);
    TreeNode* child = andNode->getChild();
    std::queue<TreeNode*> eQueue;
    int tau_Count = 0;
    while (child != NULL && child->getToken() != "") {
        eQueue.push(child);
        child = child->getSibling();
        tau_Count++;
    }
    //tauToken_Count = tau_Count;
    tauNode->setTauCount(tau_Count);
    tauNode->setToken("tau");
    TreeNode* currentPara = NULL;
    TreeNode* currentVal = NULL;
    while (!eQueue.empty()) {
        TreeNode* eq = eQueue.front();
        eQueue.pop();
        TreeNode* para = eq->getChild();
        TreeNode* val = eq->getChild()->getSibling();
        para->setSibling(NULL);
        if (currentPara == NULL) {
            currentPara = para;
            currentVal = val;
            commaNode->setChild(currentPara);
            tauNode->setChild(currentVal);

        }
        else {
            currentPara->setSibling(para);
            currentVal->setSibling(val);
            currentPara = para;
            currentVal = val;
        }
    }
    return equalNode;
}

TreeNode* transformAT(TreeNode* node) {
    TreeNode* gammaNode1 = new TreeNode();
    TreeNode* gammaNode2 = new TreeNode();
    gammaNode1->setToken("gamma");
    gammaNode1->setTokenType("gamma"); //changes
    gammaNode2->setToken("gamma");
    gammaNode2->setTokenType("gamma"); //changes
    gammaNode1->setChild(gammaNode2);
    TreeNode* n1 = node->getChild()->getSibling()->getSibling();
    TreeNode* m = node->getChild()->getSibling();
    m->setSibling(NULL);
    TreeNode* n2 = node->getChild();
    n2->setSibling(NULL);
    gammaNode2->setChild(m);
    m->setSibling(n2);
    gammaNode2->setSibling(n1);
    return gammaNode1;
}

TreeNode* transformLAMBDA(TreeNode* node) {
    if (node->getChild()->getSibling()->getSibling() == NULL) return node;
    std::stack<TreeNode*> stack;
    TreeNode* child = node->getChild();
    while (child != NULL && child->getToken() != "") {
        stack.push(child);
        TreeNode* nextChild = child->getSibling();
        child->setSibling(NULL);
        child = nextChild;
    }
    while (stack.size() != 1) {
        TreeNode* right = stack.top();
        stack.pop();
        TreeNode* left = stack.top();
        stack.pop();
        TreeNode* lambdaNode = new TreeNode();
        lambdaNode->setToken("lambda");
        lambdaNode->setTokenType("lambda"); //changes
        lambdaNode->setChild(left);
        left->setSibling(right);
        stack.push(lambdaNode);
    }
    return stack.top();
    stack.pop();
}

TreeNode* transformREC(TreeNode* recNode) {
    TreeNode* lambdaNode = new TreeNode();
    TreeNode* gammaNode = new TreeNode();
    TreeNode* equalNode = new TreeNode();
    TreeNode* ystarNode = new TreeNode();
    lambdaNode->setToken("lambda");
    lambdaNode->setTokenType("lambda");
    gammaNode->setToken("gamma");
    gammaNode->setTokenType("gamma");
    equalNode->setToken("=");
    ystarNode->setToken("YSTAR");
    ystarNode->setTokenType("YSTAR");
    TreeNode* m = recNode->getChild()->getChild()->getSibling();
    TreeNode* n1 = recNode->getChild()->getChild();
    n1->setSibling(NULL);
    TreeNode* n2 = new TreeNode();
    n2->setToken(n1->getToken());
    equalNode->setChild(n1);
    n1->setSibling(gammaNode);
    gammaNode->setChild(ystarNode);
    ystarNode->setSibling(lambdaNode);
    lambdaNode->setChild(n2);
    n2->setSibling(m);
    return equalNode;
}

void printNodes(TreeNode* node, int level) {
    string str = "";

    for (int i = 0; i<level; i++)

        str = str + ".";

    if (node != NULL && node->getToken().length() != 0) {

        cout << str + "" + node->getToken() << endl;

    }

    else {

        return;

    }
    if (node->getChild() != NULL)
        printNodes(node->getChild(), level + 1);
    if (node->getSibling() != NULL)
        printNodes(node->getSibling(), level);
}



/***********************************CSE MACHINE **********************************************/


void createControlStruc(TreeNode* root) {
    /*if (root->getTokenType() == "<EOF>") {
    root->setTokenType(root->getToken());
    }*/
    pDeltaQueue.push(root);
    while (!pDeltaQueue.empty()) {
        vector<TreeNode*> currentDelta;
        TreeNode* currentStartNode = pDeltaQueue.front();
        pDeltaQueue.pop();
        preOrderTraversal(currentStartNode, currentDelta);
        deltaMap[currentDeltaNumber++] = currentDelta;
    }
}

void preOrderTraversal(TreeNode* root, vector<TreeNode*> &currentDelta) {
    if (root->getToken() == "lambda" && root->getToken() != "") { //changes
        if (root->getChild()->getToken() != "," &&  root->getChild()->getToken() != "") { //changes
            TreeNode* lambdaclosure = new TreeNode();
            lambdaclosure->setToken(root->getToken()); //changes
            lambdaclosure->setTokenType("lambdaclosure");
            lambdaclosure->setLambdaParam(root->getChild()->getToken());
            lambdaclosure->setLambdaNum(++deltaCounter);
            currentDelta.push_back(lambdaclosure);
        }
        else {
            TreeNode* commaChild = root->getChild()->getChild();
            string tuple;
            while (commaChild != NULL && commaChild->getToken() != "") { //changes
                tuple += commaChild->getToken() + ",";
                commaChild = commaChild->getSibling();
            }
            TreeNode*  lambdaclosure = new TreeNode();
            lambdaclosure->setToken("lambda"); //
            lambdaclosure->setTokenType("lambdaclosure");
            lambdaclosure->setLambdaParam(tuple);
            lambdaclosure->setLambdaNum(++deltaCounter);
            lambdaclosure->setIsTuple(true);
            currentDelta.push_back(lambdaclosure);
        }
        pDeltaQueue.push(root->getChild()->getSibling());
        if (root->getSibling() != NULL && root->getSibling()->getToken() != "") { // changes
            preOrderTraversal(root->getSibling(), currentDelta);
        }
    }
    else if (root->getToken() == "->" && root->getToken() != "") { //changes
        TreeNode* beta = new TreeNode();
        beta->setTokenType("beta");
        beta->setBetaIfNum(deltaCounter + 1);
        beta->setBetaElseNum(deltaCounter + 2);
        currentDelta.push_back(beta);
        pDeltaQueue.push(root->getChild()->getSibling());
        pDeltaQueue.push(root->getChild()->getSibling()->getSibling());
        root->getChild()->getSibling()->setSibling(NULL);
        root->getChild()->setSibling(NULL);
        deltaCounter += 2;
        if (root->getChild() != NULL && root->getChild()->getToken() != "") { //changes
            preOrderTraversal(root->getChild(), currentDelta);
        }
        if (root->getSibling() != NULL && root->getSibling()->getToken() != "") { //changes
            preOrderTraversal(root->getSibling(), currentDelta);
        }
    }
    else {
        currentDelta.push_back(root);
        if (root->getChild() != NULL && root->getChild()->getToken() != "") { //changes
            preOrderTraversal(root->getChild(), currentDelta);
        }
        if (root->getSibling() != NULL && root->getSibling()->getToken() != "") { //changes
            preOrderTraversal(root->getSibling(), currentDelta);
        }
    }
}

string intToString(int intValue) {
    ostringstream oss;
    oss << intValue;
    return oss.str();
}

vector<string> split(string inputString, char delimiter) {
    vector<string> result;
    string::iterator it;
    string temp;
    for (it = inputString.begin(); it < inputString.end(); it++) {
        if (*it == delimiter) {
            result.push_back(temp);
            temp = string();
        }
        else {
            temp += *it;
        }
    }
    return result;
}


bool isParameter(TreeNode* currNode) {
    string varName = currNode->getToken();
    int temp = currentEnv;
    keyPair kp(temp, varName);
    map<keyPair, TreeNode*>::iterator it = parameterMap.find(kp);
    while (parameterMap.end() == it && temp >= 0) {
        temp = environmentMap[temp];
        kp.first = temp;
        it = parameterMap.find(kp);
    }
    if (parameterMap.end() != it) {
        return true;
    }
    else {
        return false;
    }
}

TreeNode* applyOperator(TreeNode* first, TreeNode* second, TreeNode* current) {
    string nodeVal = current->getToken();
    TreeNode* resultNode = new TreeNode();
    if (first->getTokenType() == "<INTEGER>") {
        int firstVal = atoi(first->getToken().c_str());
        int secondVal = atoi(second->getToken().c_str());
        int result = 0;
        if (nodeVal == "*") {
            result = firstVal*secondVal;
            resultNode->setToken(intToString(result));
            resultNode->setTokenType(first->getTokenType());
            return resultNode;
        }
        else if (nodeVal == "+") {
            result = firstVal + secondVal;
            resultNode->setToken(intToString(result));
            resultNode->setTokenType(first->getTokenType());
            return resultNode;
        }
        else if (nodeVal == "-") {
            result = firstVal - secondVal;
            resultNode->setToken(intToString(result));
            resultNode->setTokenType(first->getTokenType());
            return resultNode;
        }
        else if (nodeVal == "/") {
            result = firstVal / secondVal;
            resultNode->setToken(intToString(result));
            resultNode->setTokenType(first->getTokenType());
            return resultNode;
        }
        else if (nodeVal == "**") {
            result = (int)pow(firstVal, secondVal);
            resultNode->setToken(intToString(result));
            resultNode->setTokenType(first->getTokenType());
            return resultNode;
        }
        else if (nodeVal == "gr") {
            if (firstVal > secondVal) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
            return resultNode;
        }
        else if (nodeVal == "ls") {
            if (firstVal < secondVal) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
            return resultNode;
        }
        else if (nodeVal == "ge") {
            if (firstVal >= secondVal) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
            return resultNode;
        }
        else if (nodeVal == "le") {
            if (firstVal <= secondVal) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
            return resultNode;
        }
        else if (nodeVal == "eq") {
            if (firstVal == secondVal) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
            return resultNode;
        }
        else if (nodeVal == "ne") {
            if (firstVal != secondVal) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
            return resultNode;
        }
    }
    else if (first->getTokenType() == "<STRING>") {
        if (nodeVal == "eq") {
            if (first->getToken() == second->getToken()) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
        }
        else if (nodeVal == "ne") {
            if (first->getToken() != second->getToken()) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
        }
    }
    else if (first->getToken() == "true" || first->getToken() == "false") {
        if (nodeVal == "or") {
            if (first->getToken() == "true" || second->getToken() == "true") {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
        }
        else if (nodeVal == "&") {
            if (first->getToken() == "true" && second->getToken() == "true") {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
        }
        else if (nodeVal == "eq") {
            if (first->getTokenType() == second->getTokenType()) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
        }
        else if (nodeVal == "ne") {
            if (first->getTokenType() != second->getTokenType()) {
                resultNode->setToken("true");
                resultNode->setTokenType("true");
            }
            else {
                resultNode->setToken("false");
                resultNode->setTokenType("false");
            }
        }

    }
    resultNode->setToken("");
    resultNode->setTokenType("");
    return resultNode;
}

void processCurrentNode(TreeNode* &currNode, stack<TreeNode*> &controlStack, stack<TreeNode*> &executionStack) {

    if (isOperators(currNode->getToken()) == true) { //changes currNode->getTokenType() == "<OPERATOR>" 
        TreeNode* first = new TreeNode();
        first = executionStack.top();
        executionStack.pop();
        TreeNode* second = new TreeNode();
        second = executionStack.top();
        executionStack.pop();
        TreeNode* resultNode = new TreeNode();
        resultNode = applyOperator(first, second, currNode);
        executionStack.push(resultNode);
    }
    else if (currNode->getToken() == "neg") {
        TreeNode* first = new TreeNode();
        first = executionStack.top();
        executionStack.pop();
        int paramVal = atoi(first->getToken().c_str());
        paramVal = -paramVal;
        TreeNode* newNode = new TreeNode();
        newNode->setToken(intToString(paramVal));
        newNode->setTokenType("<INTEGER>");
        executionStack.push(newNode);
    }
    else if (currNode->getToken() == "not") {
        TreeNode* first = new TreeNode();
        TreeNode* result = new TreeNode();
        first = executionStack.top();
        executionStack.pop();
        if (first->getToken() == "true") {
            result->setToken("false");
            result->setTokenType("false");
            executionStack.push(result);
        }
        else {
            result->setToken("true");
            result->setTokenType("true");
            executionStack.push(result);
        }
    }
    else if (currNode->getTokenType() == "<IDENTIFIER>" && isParameter(currNode)) {
        string varName = currNode->getToken();
        int temp = currentEnv;
        keyPair kp(temp, varName);
        map<keyPair, TreeNode*>::iterator it = parameterMap.find(kp);
        while (parameterMap.end() == it && temp >= 0) {
            temp = environmentMap[temp];
            kp.first = temp;
            it = parameterMap.find(kp);
        }
        if (parameterMap.end() != it) {
            TreeNode* paramValNode = new TreeNode();
            paramValNode = it->second;
            executionStack.push(paramValNode);
        }
    }
    else if (currNode->getToken() == "gamma") //changes
    {
        TreeNode* topExeNode = new TreeNode();
        topExeNode = executionStack.top();

        executionStack.pop();
        if (topExeNode->getTokenType() == "lambdaclosure") {
            TreeNode* env = new TreeNode();
            env->setToken("env");
            env->setTokenType("env");
            env->setEnvNum(++envCounter);
            environmentMap[envCounter] = topExeNode->getLambdaEnv();
            environmentStack.push(envCounter);
            currentEnv = envCounter;
            if (topExeNode->getIsTuple() == false) {
                string paramName = topExeNode->getLambdaParam();
                TreeNode* paramNode = new TreeNode();
                paramNode = executionStack.top();
                executionStack.pop();
                keyPair kp(envCounter, paramName);
                parameterMap[kp] = paramNode;
            }
            else {
                string tuple = topExeNode->getLambdaParam();
                vector<string> params = split(tuple, ',');
                TreeNode* valTuple = executionStack.top();
                executionStack.pop();
                vector<TreeNode*> tupVector = valTuple->getTuple();
                for (unsigned int i = 0; i < params.size(); i++) {
                    if (params[i] != "") {
                        keyPair kp(envCounter, params[i].c_str());
                        parameterMap[kp] = tupVector[i];
                    }
                }

            }

            controlStack.push(env);
            executionStack.push(env);
            int lambdaNum = topExeNode->getLambdaNum();
            vector<TreeNode*> delta = deltaMap[lambdaNum];
            for (unsigned int i = 0; i < delta.size(); i++) {
                controlStack.push(delta[i]);
            }
        }
        else if (topExeNode->getTokenType() == "YSTAR" || topExeNode->getToken() == "YSTAR") {// changes made
            TreeNode* nextNode = new TreeNode();
            nextNode = executionStack.top();
            executionStack.pop();
            nextNode->setToken("eta"); //changes
            nextNode->setTokenType("eta");
            executionStack.push(nextNode);
        }
        else if (topExeNode->getTokenType() == "eta") {
            TreeNode* lambdaNode = new TreeNode();
            lambdaNode = topExeNode;
            lambdaNode->setToken("lambdaclosure");
            lambdaNode->setTokenType("lambdaclosure");
            executionStack.push(topExeNode);
            executionStack.push(lambdaNode);
            TreeNode* gammaNode = new TreeNode();
            gammaNode->setToken("gamma");
            gammaNode->setTokenType("gamma");
            controlStack.push(gammaNode);
            controlStack.push(gammaNode);
        }
        else if (topExeNode->getToken() == "Stern" || topExeNode->getToken() == "stern") {
            TreeNode* stringNode = new TreeNode();
            stringNode = executionStack.top();
            executionStack.pop();
            string tokenValue = stringNode->getToken();
            tokenValue = tokenValue.substr(2, tokenValue.size() - 3);
            tokenValue = "'" + tokenValue + "'";
            stringNode->setToken(tokenValue);
            executionStack.push(stringNode);
        }
        else if (topExeNode->getToken() == "Stem" || topExeNode->getToken() == "stem") {
            TreeNode* stringNode = new TreeNode();
            stringNode = executionStack.top();
            executionStack.pop();
            string tokenValue = stringNode->getToken();
            tokenValue = tokenValue.substr(1, 1);
            tokenValue = "'" + tokenValue + "'";
            stringNode->setToken(tokenValue);
            executionStack.push(stringNode);
        }
        else if (topExeNode->getToken() == "Conc" || topExeNode->getToken() == "conc") {
            TreeNode* first = new TreeNode();
            first = executionStack.top();
            executionStack.pop();
            TreeNode* second = new TreeNode();
            second = executionStack.top();
            executionStack.pop();
            string concatValue = first->getToken().substr(1, first->getToken().size() - 2) + second->getToken().substr(1, second->getToken().size() - 2);
            concatValue = "'" + concatValue + "'";
            TreeNode* newNode = new TreeNode();
            newNode->setToken(concatValue);
            newNode->setTokenType("<STRING>");
            executionStack.push(newNode);
            controlStack.pop();
        }
        else if (topExeNode->getToken() == "ItoS" || topExeNode->getToken() == "itos") {
            TreeNode* first = new TreeNode();
            first = executionStack.top();
            executionStack.pop();
            first->setTokenType("<STRING>");
            first->setToken("'" + first->getToken() + "'");
            executionStack.push(first);
            }
        else if (topExeNode->getToken() == "Print" || topExeNode->getToken() == "print") {
            printCalled = true;
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            if (t->getIsTuple() == false) {
                if (t->getTokenType() == "<STRING>") {

                    string tempStr = t->getToken();
                                                   //string tempStr = t->getToken().substr(1, t->getToken().size() - 2);
                    cout << tempStr.substr(1, t->getToken().size() - 2);
                    if (tempStr[tempStr.size() - 1] == '\n')
                        cout << endl;
                    //cout << t.value.substr(1,t.value.size()-2);
                }
                else if (t->getTokenType() == "lambdaclosure" || t->getToken() == "lambdaclosure") {//
                    cout << "[lambda closure: " << t->getLambdaParam() << ": " << t->getLambdaNum() << "]";
                }
                else {
                    cout << t->getToken();
                }
                TreeNode* dummyNode = new TreeNode();
                dummyNode->setToken("dummy");
                dummyNode->setTokenType("dummy");
                executionStack.push(dummyNode);
            }
            else {
                vector<TreeNode*> tupleVector = t->getTuple();
                for (unsigned int i = 0; i < tupleVector.size(); i++) {
                    if (i == 0) {
                        cout << "(";
                    }
                    else {
                        cout << ", ";
                    }
                    if (tupleVector[i]->getTokenType() == "<STRING>") {
                        string tempstr = tupleVector[i]->getToken();
                        cout << tempstr.substr(1, tempstr.size() - 2);
                    }
                    else if (tupleVector[i]->getIsTuple() == true) {
                        cout << "Inside else if" << endl;
                        vector<TreeNode*> innerTuple = tupleVector[i]->getTuple();
                        cout << "Size" << innerTuple.size() << endl;
                        if (innerTuple.size() == 1) {
                            if (innerTuple[0]->getTokenType() == "<STRING>") {
                                string tempStr = innerTuple[0]->getToken();
                                cout << tempStr.substr(1, innerTuple[0]->getToken().size() - 2);
                            }
                        }
                    }
                    else {
                        cout << tupleVector[i]->getToken();
                    }
                    if (i == tupleVector.size() - 1) {
                        cout << ")";
                    }
                }
            }
            
        }
        else if (topExeNode->getToken() == "Isinteger") {
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            TreeNode* node = new TreeNode();
            if (t->getTokenType() == "<INTEGER>") {
                node->setToken("true");
                node->setTokenType("true");
            }
            else {
                node->setToken("false");
                node->setTokenType("false");
            }
            executionStack.push(node);
        }
        else if (topExeNode->getToken() == "Istruthvalue") {
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            TreeNode* node = new TreeNode();
            if (t->getToken() == "true" || t->getToken() == "false") {
                node->setToken("true");
                node->setTokenType("true");
            }
            else {
                node->setToken("false");
                node->setTokenType("false");
            }
            executionStack.push(node);
        }
        else if (topExeNode->getToken() == "Isstring") {
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            TreeNode* node = new TreeNode();
            if (t->getTokenType() == "<STRING>") {
                node->setToken("true");
                node->setTokenType("true");
            }
            else {
                node->setToken("false");
                node->setTokenType("false");
            }
            executionStack.push(node);
        }
        else if (topExeNode->getToken() == "Istuple") {
            
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            TreeNode* node = new TreeNode();
            if (t->getIsTuple() == true) {
                node->setToken("true");
                node->setTokenType("true");
            }
            else {
                node->setToken("false");
                node->setTokenType("false");
            }
            executionStack.push(node);
        }
        else if (topExeNode->getToken() == "Isdummy") {
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            TreeNode* node = new TreeNode();
            if (t->getToken() == "dummy") {
                node->setToken("true");
                node->setTokenType("true");
            }
            else {
                node->setToken("false");
                node->setTokenType("false");
            }
            executionStack.push(node);
        }
        else if (topExeNode->getToken() == "Isfunction") {
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            TreeNode* node = new TreeNode();
            if (t->getTokenType() == "lambdaclosure") {
                node->setToken("true");
                node->setTokenType("true");
            }
            else {
                node->setToken("false");
                node->setTokenType("false");
            }
            executionStack.push(node);
        }
        else if (topExeNode->getToken() == "Order") {
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            TreeNode* node = new TreeNode();
            node->setToken(intToString(t->getTuple().size()));
            node->setTokenType("<INTEGER>");
            executionStack.push(node);
        }
        else if (topExeNode->getToken() == "Null") {
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            TreeNode* node = new TreeNode();
            if (t->getToken() == "nil") {
                node->setToken("true");
                node->setTokenType("true");
            }
            else {
                node->setToken("false");
                node->setTokenType("false");
            }
            executionStack.push(node);
        }
        else if (topExeNode->getIsTuple() == true) {
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            executionStack.pop();
            if (t->getTokenType() == "<INTEGER>") {
                cout << "let us print index fetch " << t->getToken().c_str() << endl;
                int index = atoi(t->getToken().c_str());
                //cout << index << endl;
                //while (index > 0) {
                    vector<TreeNode*> innerTuple = topExeNode->getTuple();
                    cout << "let us print index " << index << endl;
                    if(innerTuple.size()>0)
                    executionStack.push(innerTuple[index]);
        
                //} //changes
            }
        }
    }
    else if (currNode->getTokenType() == "env") {
        TreeNode* topNode = new TreeNode();
        topNode = executionStack.top();
        executionStack.pop();
        executionStack.pop();
        executionStack.push(topNode);
        environmentStack.pop();
        currentEnv = environmentStack.top();
    }
    else if (currNode->getTokenType() == "beta") {
        TreeNode* topNode = new TreeNode();
        topNode = executionStack.top();
        executionStack.pop();
        vector<TreeNode*> delta;
        if (topNode->getToken() == "true") {
            delta = deltaMap[currNode->getBetaIfNum()];
        }
        else {
            delta = deltaMap[currNode->getBetaElseNum()];
        }
        for (unsigned int i = 0; i<delta.size(); i++) {
            controlStack.push(delta[i]);
        }
    }
    else if (currNode->getToken() == "tau") {
        int tauCount = currNode->getTauCount();
        string tuple = "(";
        vector<TreeNode*> tupleVector;
        for (int i = 0; i<tauCount; i++) {
            TreeNode* t = new TreeNode();
            t = executionStack.top();
            tupleVector.push_back(t);
            executionStack.pop();
            if (i == tauCount - 1)
                tuple += t->getToken();
            else
                tuple += t->getToken() + ", ";
        }
        tuple += ")";
        TreeNode* newNode = new TreeNode();
        newNode->setToken(tuple);
        newNode->setTokenType("tuple");
        newNode->setTuple(tupleVector);
        newNode->setIsTuple(true);
        executionStack.push(newNode);
    }
    else if (currNode->getToken() == "nil") {
        currNode->setIsTuple(true);
        executionStack.push(currNode);
    }
    else if (currNode->getToken() == "aug") {
        TreeNode* tuple = new TreeNode();
        tuple = executionStack.top();
        executionStack.pop();
        TreeNode* toAdd = new TreeNode();
        toAdd = executionStack.top();
        executionStack.pop();
        if (tuple->getToken() == "nil") {
            TreeNode* newNode = new TreeNode();
            newNode->setToken(toAdd->getToken());
            newNode->setTokenType("tuple");
            newNode->setIsTuple(true);
            newNode->setTuple(vector<TreeNode*>());
            newNode->getTuple().push_back(toAdd);
            executionStack.push(newNode);
        }
        else {
            tuple->getTuple().push_back(toAdd);
            executionStack.push(tuple);
        }
    }
    else if (currNode->getTokenType() == "lambdaclosure") {
        currNode->setLambdaEnv(currentEnv);
        executionStack.push(currNode);
    }
    else {
        executionStack.push(currNode);
    }
}

void evaluateTree(TreeNode* input) {
    createControlStruc(input);
    TreeNode* envNode = new TreeNode();
    envNode->setToken("env");
    envNode->setTokenType("env");
    envNode->setEnvNum(envCounter);
    stack<TreeNode*> controlStack;
    stack<TreeNode*> executionStack;
    controlStack.push(envNode);
    environmentMap[0] = -1;


    vector<TreeNode*> delta0 = deltaMap[0];
    for (unsigned int i = 0; i<delta0.size(); i++) {
        controlStack.push(delta0[i]);
    }
    executionStack.push(envNode);
    int whileCount = 0;
    while (controlStack.size() != 1) {

        TreeNode* currNode = new TreeNode();
        currNode = controlStack.top();
        controlStack.pop();
        processCurrentNode(currNode, controlStack, executionStack);
        if (whileCount > 5000) {
            break;
        }
        whileCount++;
    }
    if (printCalled == false)
        cout << endl;
}




