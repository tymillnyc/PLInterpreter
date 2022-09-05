#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
bool boolReadCharacter = true;
string q = "";
bool boolStringOrInt = true;
enum typeOfToken {
    PROGRAM, AND, OR, NOT, TRUE, FALSE, BOOLEAN, ELSE, IF, INT, READ, WHILE, WRITE, FOR, STRING, GOTO,//15
    POINT, SEMICOLON, COMMA, COLON, LBRACKET, RBRACKET, EQUAL, LESS, GREATER, PLUS, MINUS, MULTUPLICATION, SLASH, LESSEQUAL,//28
    NOTEQUAL, GREATEREQUAL, TWOEQUAL, LCURLYBRACE, RCURLYBRACE, PERCENT, QUOTE, //36
    NUMBER, IDENTIFIER, NULLP, STRINGVALUE, LABEL, //35,36,37
    POLIZLABEL,                                                                                /*38*/
    POLIZADDRESS,                                                                              /*39*/
    POLIZGO,                                                                                   /*40*/
    POLIZFGO
};

class Identifier {
    static int identifierCount;
    int programStringNumber;
    string name;
    int value;
    string valueString;
    bool assign;
    bool initialization;
    typeOfToken type;
    int tableInNumber;
public:
    Identifier() {
        initialization = false;
        if (!assign) {
            assign = false;
        }
    }

    void putAssign() {
        assign = true;
    }

    bool getAssign() {
        return assign;
    }

    int getProgramString() {
        return programStringNumber;
    }

    int getValue() {
        return value;
    }

    string getValueString() {
        return valueString;
    }

    void putValue(string valueS) {
        valueString = valueS;
    }

    void putValue(int value) {
        this->value = value;
    }

    void putProgramString(int programString) {
        this->programStringNumber = programString;
    }

    void putType(typeOfToken type) {
        this->type = type;
    }

    typeOfToken getType() {
        return type;
    }

    bool getInitialization() {
        return initialization;
    }

    bool operator==(const string &buffer) const {
        return name == buffer;
    }

    string getName() {
        return name;
    }

    Identifier(const string &valueS) {
        this->name = valueS;
        tableInNumber = identifierCount + 1;
        initialization = false;
        assign = false;
    }

    bool putInitialization() {
        initialization = true;
        return initialization;
    }
};

int Identifier::identifierCount = 0;

class Token {
    typeOfToken type;
    int tableInNumber;
    int programStringNumber;
    string valueString;
public:
    int getProgramString() {
        return programStringNumber;
    }

    void putTableInNUmber(int boolToInt) {
        tableInNumber = boolToInt;
    }


    Token(string &buffer) {
        //if buffer == true or buffer  == false then valueb = true or false
        type = STRINGVALUE;
        valueString = buffer;
        tableInNumber = -1;
    }

    Token(typeOfToken type = NULLP, int tableInNumber = 0, int programString = 1) : type(type),
                                                                                    tableInNumber(tableInNumber),
                                                                                    programStringNumber(programString) {
    }

    string getValueString() {
        return valueString;
    }

    typeOfToken getType() const {
        return type;
    }

    int getTableInNumber() const {
        return tableInNumber;
    }


    friend ostream &operator<<(ostream &s, Token &token);
};

vector<Identifier> identifiersTable;

int putInIdentifiersTable(const string &buffer) {
    vector<Identifier>::iterator iIdentifier;
    if ((iIdentifier = find(identifiersTable.begin(), identifiersTable.end(), buffer)) != identifiersTable.end())
        return iIdentifier - identifiersTable.begin();
    identifiersTable.push_back(Identifier(buffer));
    return identifiersTable.size() - 1;
}

class LexicalAnalyzer {
    ifstream inFile;
    char character;

    int searchInTable(const string &buffer, const string *table, int size) {
        for (int i = 0; i < size; i++) {
            if (buffer == table[i])
                return i;
        }
        return 0;
    }

    void addBuffer(string &buffer) {
        buffer += character;
    }

    void getCharacter() {
        inFile.get(character);
    }

public:
    ~LexicalAnalyzer() {
        inFile.close();
    }

    static string serviceWordsTable[], separatorsTable[];
    static int programStringCount;

    LexicalAnalyzer(const char *program = "f1.txt") {
        inFile.open(program);
        if (!inFile.is_open()) {
            string error = "error: file can't open.\n";
            throw error;
        }
    }

    bool checkForLetter(char symbol) {
        if ((symbol >= 65 and symbol <= 90) or (symbol >= 97 and symbol <= 122))
            return true;
        return false;
    }

    bool checkForNumber(char symbol) {
        if (symbol >= 48 && symbol <= 57)
            return true;
        return false;
    }

    Token getToken();
};

int LexicalAnalyzer::programStringCount = 1;
string LexicalAnalyzer::serviceWordsTable[] = {"program", "and", "or", "not", "true", "false", "boolean", "else", "if",
                                               "int",
                                               "read", "while", "write", "for", "string", "goto"}; //16 всего

string LexicalAnalyzer::separatorsTable[] = {";", ",", ":", "(", ")", "=", "<", ">", "+", "-", "*", "/", "<=", "!=",
                                             ">=", "==", "{", "}", "%", "\""};//21 всего

Token LexicalAnalyzer::getToken() {
    enum States {
        Start, Number, Separator, Comment, IntermediateIdAndSW, serviceWord, Identifier, Assign, Quote, NotEqual, Error,
    };
    int sizeServiceWords = sizeof(serviceWordsTable) / sizeof(serviceWordsTable[0]);
    int sizeSeparators = sizeof(separatorsTable) / sizeof(separatorsTable[0]);
    int number, boolIsTable;
    string buffer = "";
    string error;
    int commentFlag = 0;
    States currentState = Start;
    if (boolReadCharacter) {
        getCharacter();
        boolReadCharacter = false;
    }
    while (true) {
        switch (currentState) {
            case Start:
                if (inFile.eof()) {
                    return Token(POINT);
                } else if (character == ' ' or character == '\n') {
                    if (character == '\n')
                        programStringCount += 1;
                    getCharacter();
                } else if (checkForLetter(character)) {
                    addBuffer(buffer);
                    currentState = IntermediateIdAndSW;
                    getCharacter();
                } else if (checkForNumber(character)) {
                    number = character - '0';
                    getCharacter();
                    currentState = Number;
                } else if (character == '/') {
                    currentState = Comment;
                    addBuffer(buffer);
                    getCharacter();
                } else if (character == '"') {
                    currentState = Quote;

                } else if (character == ':' or character == '<' or character == '>' or character == '=') {
                    addBuffer(buffer);
                    getCharacter();
                    currentState = Assign;
                } else if (character == '!') {
                    addBuffer(buffer);
                    getCharacter();
                    currentState = NotEqual;
                } else {
                    currentState = Separator;
                    addBuffer(buffer);
                }
                continue;
            case IntermediateIdAndSW:
                if (checkForLetter(character) or checkForNumber(character)) {
                    addBuffer(buffer);
                    getCharacter();
                } else {
                    if ((boolIsTable = searchInTable(buffer, serviceWordsTable, sizeServiceWords)) or
                        (buffer == "program")) {
                        currentState = serviceWord;
                    } else {
                        currentState = Identifier;
                    }
                }
                continue;
            case Identifier:
                boolIsTable = putInIdentifiersTable(buffer);
                return Token(IDENTIFIER, boolIsTable, programStringCount);
            case serviceWord:
                return Token((typeOfToken) (boolIsTable), boolIsTable);
            case Number:
                if (checkForNumber(character)) {
                    number = number * 10 + (character - '0');
                    getCharacter();
                } else if (checkForLetter(character)) {
                    error = "error: the initial character of the identifier is a digit.\n";
                    currentState = Error;

                } else {
                    return Token(NUMBER, number);
                }
                continue;
            case Quote:
                getCharacter();
                if (character == '"') {
                    getCharacter();
                    return Token(buffer);
                }
                addBuffer(buffer);
                continue;
            case Comment:
                char helperCharacter;
                if (character == '*') {
                    commentFlag += 1;
                    if (commentFlag > 1) {
                        inFile.get(helperCharacter);
                        if (helperCharacter == '/') {
                            currentState = Start;
                            buffer = "";
                        }
                    }
                    getCharacter();
                } else if (commentFlag == 0) {
                    currentState = Separator;
                } else if (character == '.') {
                    inFile.get(helperCharacter);
                    if (inFile.eof()) {
                        error = "error:unfinished comment.\n";
                        currentState = Error;
                    }
                    getCharacter();
                } else {
                    getCharacter();
                }
                continue;
            case Assign:
                if (character == '=') {
                    addBuffer(buffer);
                }
                currentState = Separator;
                continue;
            case NotEqual:
                if (character == '=') {
                    addBuffer(buffer);
                }
                currentState = Separator;
                continue;
            case Separator:
                if ((boolIsTable = searchInTable(buffer, separatorsTable, sizeSeparators)) or buffer == ";") {
                    int flag = (boolIsTable) + (int) SEMICOLON;
                    if (flag != 28 and flag != 22 and flag != 19 and flag != 23 and flag != 24) {
                        getCharacter();
                    }
                    return Token((typeOfToken) ((boolIsTable) + (int) SEMICOLON), boolIsTable);
                } else {
                    error = "error: invalid character.\n";
                    currentState = Error;
                }
                continue;
            case Error:

                throw error;
        }
    }
}

ostream &operator<<(ostream &out, Token &token) {
    int tokenType = token.type;
    string stringTokenType;
    // cout << "tableInNumber" << token.tableInNumber;
    //out << " f = " << identifiersTable[token.tableInNumber].getValue() << " c = " << identifiersTable[token.tableInNumber].getInitialization()
    // << "d = " <<  identifiersTable[token.tableInNumber].getType() << " a = " << identifiersTable[token.tableInNumber].getProgramString() << endl;
    if (tokenType < POINT) {
        stringTokenType = LexicalAnalyzer::serviceWordsTable[tokenType];
    } else if (tokenType > POINT and tokenType <= PERCENT) {
        stringTokenType = LexicalAnalyzer::separatorsTable[(tokenType - (int) SEMICOLON)];
    } else if (tokenType == NUMBER) {
        stringTokenType = "number";
    } else if (tokenType == STRINGVALUE) {
        stringTokenType = token.valueString;
    } else if (tokenType == IDENTIFIER) {
        stringTokenType = "identifier";
        out << stringTokenType << " = " << identifiersTable[token.tableInNumber].getName() << "\n";
        return out;
    } else if (tokenType == POLIZLABEL) {
        stringTokenType = "poliz label";
        out << stringTokenType << " " << token.getTableInNumber();
        return out;
    } else if (tokenType == LABEL) {
        stringTokenType = "label";
        out << stringTokenType << " " << identifiersTable[token.getTableInNumber()].getName() << "\n";
        return out;
    } else if (tokenType == POLIZADDRESS) {
        stringTokenType = "adress variable";
        out << stringTokenType << " " << identifiersTable[token.getTableInNumber()].getName() << "\n";
        return out;
    } else if (tokenType == POLIZGO) {
        stringTokenType = "!";
    } else if (tokenType == POLIZFGO) {
        stringTokenType = "!F";
    }

    out << stringTokenType;// << ") numeric representation: (" << tokenType << "," << token.tableInNumber
    // << ")\n";
    return out;
}

class SyntaxAnalyzer {
    Token token;
    typeOfToken tokenType;
    int equalCount;
    int value;
    vector<int> gotoForTransitionNumbers;
    int programStringNumber;
    typeOfToken bufferTypeOfToken;
    bool flag;
    stack<typeOfToken> tokenTypes;
    stack<int> identifierNumbers;
    vector<int> unusedLabel;
    vector<int> labels;
    LexicalAnalyzer lexicalAnalyzer;
    int equalCount2;

    void checkOperands();

    void checkOperandTypes();

    void program();

    void descriptions();

    void description();

    void variable();

    void constant();

    void operators();

    void assignTypeVariable(typeOfToken type);

    void checkIdentifierDescription();

    void checkBooleanExpression();

    void operatorr();

    void checkOperandInRead();

    void expression();

    void operatorEqual();

    void operatorOr();

    void operatorAnd();

    void operatorPlusMinus();

    void operation();

    void operand();

    void compoundOperator();

    void expressionOperator();

    void getToken();

public:
    vector<Token> polysisElements;

    void parse();

    SyntaxAnalyzer() {
        flag = true;
    }

};


template<class T, class T_EL>
void takeFromStack(T &t, T_EL &x) {
    x = t.top();
    t.pop();
}

void SyntaxAnalyzer::getToken() {
    token = lexicalAnalyzer.getToken();
    tokenType = token.getType();
    programStringNumber = token.getProgramString();
    value = token.getTableInNumber();
}

void SyntaxAnalyzer::parse() {
    getToken();
    program();
    if (tokenType != POINT) {
        throw token;
    }
   // for (int i = 0; i < polysisElements.size(); i++) {
     //   cout << "\n" << i << ") " << polysisElements[i];

   // }
}

void SyntaxAnalyzer::program() {
    equalCount = 1;
    if (tokenType == PROGRAM) {
        getToken();
    } else throw token;
    if (tokenType == LCURLYBRACE) {
        getToken();
    } else throw token;
    descriptions();
    operators();
    if (tokenType == RCURLYBRACE) {
        int i = 0;
        for (i = 0; i < unusedLabel.size(); i++) {
            cout << "string " << identifiersTable[unusedLabel[i]].getProgramString() << " "
                 << identifiersTable[unusedLabel[i]].getValue() << "\n";
        }
        if (i > 0) throw "error";
        getToken();
    } else throw token;

}

void SyntaxAnalyzer::descriptions() {
    description();
    while (tokenType == SEMICOLON) {
        getToken();
        description();
    }
}

void SyntaxAnalyzer::description() {
    if (tokenType == INT or tokenType == STRING or tokenType == BOOLEAN) {
        bufferTypeOfToken = tokenType;
        tokenTypes.push(tokenType);
        getToken();
        variable();
        while (tokenType == COMMA) {
            getToken();
            variable();

        }
        tokenTypes.pop();
        assignTypeVariable(bufferTypeOfToken);
    }

}

void SyntaxAnalyzer::variable() {
    if (tokenType == IDENTIFIER) {
        identifiersTable[value].putProgramString(programStringNumber);
        identifierNumbers.push(value);
        int identifierNumberBuffer = value;
        getToken();
        if (tokenType == EQUAL) {
            identifiersTable[identifierNumbers.top()].putAssign();
            getToken();
            constant();
            checkOperandTypes();
            // tokenTypes.pop();
        }
    } else throw token;
}

void SyntaxAnalyzer::constant() {
    if (tokenType == PLUS or tokenType == MINUS) {
        getToken();
        if (tokenType == NUMBER) {
            tokenTypes.push(INT);
            //cout << value;
            identifiersTable[identifierNumbers.top()].putValue(value);
            getToken();
        } else throw token;
    } else if (tokenType == NUMBER) {
        tokenTypes.push(INT);
        identifiersTable[identifierNumbers.top()].putValue(value);
        getToken();
    } else if (tokenType == TRUE or tokenType == FALSE) {
        tokenTypes.push(BOOLEAN);
        identifiersTable[identifierNumbers.top()].putValue(5 - value);
       // cout << 5 - value;
        getToken();
    } else if (tokenType == STRINGVALUE) {
        tokenTypes.push(STRING);
        identifiersTable[identifierNumbers.top()].putValue(token.getValueString());
        //cout << "he;lo = " << identifiersTable[identifierNumbers.top()].getValueString();
        //cout << identifierNumbers.top();
        getToken();
    } else throw token;
}

void SyntaxAnalyzer::operators() {
    operatorr();
    while (flag) {
        operatorr();
    }
}

void SyntaxAnalyzer::operatorr() {
    int bufferNextAdress0, bufferNextAdress1, bufferNextAdress2, bufferNextAdress3;
    if (tokenType == IDENTIFIER) {
        int bufferValue = value;
        bool existenceTwoIdenticalLabels = false;
        identifiersTable[value].putProgramString(programStringNumber);
        for (int i = 0; i < labels.size(); i++) {
            if (value == labels[i]) {
                for (int j = 0; j < unusedLabel.size(); j++) {
                    if (value == unusedLabel[j]) {
                        unusedLabel.erase(unusedLabel.begin() + j);
                    }
                    existenceTwoIdenticalLabels = true;
                }
                if (!existenceTwoIdenticalLabels) {
                    throw "such a label has already been used above, twice is unacceptable\n";
                }
            }
        }
        for (int l = 0; l < gotoForTransitionNumbers.size(); l++) {
            if (value == polysisElements[gotoForTransitionNumbers[l]].getTableInNumber()) {
                polysisElements[gotoForTransitionNumbers[l]] = Token(POLIZLABEL, polysisElements.size());
                gotoForTransitionNumbers.erase(gotoForTransitionNumbers.begin() + l);
                l -= 1;
            }
        }
        getToken();
        if (tokenType == EQUAL) {
            if (identifiersTable[bufferValue].getInitialization())
                tokenTypes.push(identifiersTable[bufferValue].getType());
            else throw "variable is not described\n";
            polysisElements.push_back(Token(POLIZADDRESS, bufferValue));
            getToken();
            checkIdentifierDescription();
            operatorOr();
            // operatorEqual();
            while (tokenType == EQUAL) {
                if (equalCount == 1) {
                    int k = polysisElements[polysisElements.size() - 1].getTableInNumber();
                    polysisElements.erase(polysisElements.end() - 1);
                    polysisElements.push_back(Token(POLIZADDRESS, k));
                }
                equalCount++;
                getToken();
                //  operatorOr();
                operatorEqual();
            }
            for (int i = 0; i < equalCount; i++) {
                checkOperandTypes();
                polysisElements.push_back(Token(EQUAL));
            }
            polysisElements.push_back(Token(SEMICOLON));
            equalCount = 1;
            tokenTypes.pop();
            if (tokenType == SEMICOLON) {
                getToken();
            } else throw token;
        } else if (tokenType == COLON) {  // dlya operatora goto
            typeOfToken currentType = identifiersTable[bufferValue].getType();
            if (currentType == STRING or currentType == BOOLEAN or currentType == NUMBER or currentType == INT) {
                throw "nonono";
            }
            polysisElements.push_back(Token(LABEL, bufferValue));
            if (!existenceTwoIdenticalLabels) labels.push_back(bufferValue);
            identifiersTable[bufferValue].putType(LABEL);

            getToken();
            operatorr();
            // operatorEqual();
        } else throw token;

    } else if (tokenType == IF) {
        getToken();
        if (tokenType == LBRACKET) {
            getToken();
            // operatorOr();
            operatorEqual();
            checkBooleanExpression();
            bufferNextAdress2 = polysisElements.size();
            polysisElements.push_back(Token());
            polysisElements.push_back(Token(POLIZFGO));
        } else throw token;
        if (tokenType == RBRACKET) {
            getToken();
            operatorr();
            if (tokenType != ELSE) {
                polysisElements[bufferNextAdress2] = Token(POLIZLABEL, polysisElements.size());
            } else {
                bufferNextAdress3 = polysisElements.size();
                polysisElements.push_back(Token());
                polysisElements.push_back(Token(POLIZGO));
                polysisElements[bufferNextAdress2] = Token(POLIZLABEL, polysisElements.size());
            }
            if (flag == 0) throw token;
        } else throw token;
        if (tokenType == ELSE) {
            getToken();
            operatorr();
            polysisElements[bufferNextAdress3] = Token(POLIZLABEL, polysisElements.size());
            if (flag == 0) throw token;
        }

    } else if (tokenType == WHILE) {
        bufferNextAdress0 = polysisElements.size();
        getToken();
        if (tokenType == LBRACKET) {
            getToken();
            //operatorOr();
            operatorEqual();
            checkBooleanExpression();
            bufferNextAdress1 = polysisElements.size();
            polysisElements.push_back(Token());
            polysisElements.push_back(Token(POLIZFGO));
        } else throw token;
        if (tokenType == RBRACKET) {
            getToken();
            operatorr();
            polysisElements.push_back(Token(POLIZLABEL, bufferNextAdress0));
            polysisElements.push_back(Token(POLIZGO));
            polysisElements[bufferNextAdress1] = Token(POLIZLABEL, polysisElements.size());
            if (flag == 0) throw token;
        }
    } else if (tokenType == READ) {
        getToken();
        if (tokenType == LBRACKET) {
            getToken();
            if (tokenType == IDENTIFIER) {
                checkOperandInRead();
                polysisElements.push_back(Token(POLIZADDRESS, value));
                getToken();
            } else throw token;
            if (tokenType == RBRACKET) {
                getToken();
                polysisElements.push_back(Token(READ));
            } else throw token;
            if (tokenType == SEMICOLON) {
                getToken();
            } else throw token;
        } else throw token;

    } else if (tokenType == WRITE) {
        getToken();
        if (tokenType == LBRACKET) {
            getToken();
            // operatorOr();
            operatorEqual();
            while (tokenType == COMMA) {
                getToken();
                // operatorOr();
                operatorEqual();
            }
            if (tokenType == RBRACKET) {
                getToken();
                polysisElements.push_back(Token(WRITE));
            } else throw token;
            if (tokenType == SEMICOLON) {
                getToken();
            } else throw token;
        } else throw token;
    } else if (tokenType == GOTO) {
        getToken();
        if (tokenType == IDENTIFIER) {
            int i;
            bool existUnusedLabel = false;
            bool valueEqualLabels = false;
            for (int l = 0; l < unusedLabel.size(); l++) {
                if (value == unusedLabel[l]) {
                    existUnusedLabel = true;
                }
            }
            for (i = 0; i < labels.size(); i++) {
                if (existUnusedLabel) {
                    break;
                }
                if (value == labels[i]) {
                    valueEqualLabels = true;
                    for (int j = 0; j < polysisElements.size(); j++) {
                        if (identifiersTable[polysisElements[j].getTableInNumber()].getValue() ==
                            identifiersTable[value].getValue()) {
                            polysisElements.push_back(Token(POLIZLABEL, j));
                            polysisElements.push_back(Token(POLIZGO));

                            break;
                        }
                    }
                }
            }
            if (!valueEqualLabels) {
                typeOfToken currentType = identifiersTable[value].getType();
                if (currentType == STRING or currentType == BOOLEAN or currentType == NUMBER) {
                    throw "already initialized and it is not a label\n";
                } else {
                    unusedLabel.push_back(value);
                    identifiersTable[value].putProgramString(programStringNumber);
                    gotoForTransitionNumbers.push_back(polysisElements.size());
                    polysisElements.push_back(Token(NULLP, value));

                    polysisElements.push_back(Token(POLIZGO));
                    labels.push_back(value);
                    identifiersTable[value].putType(LABEL);
                }
            }
            getToken();
        } else throw token;
        if (tokenType == SEMICOLON) {
            getToken();
        } else throw token;

    } else if (tokenType == LCURLYBRACE) {
        getToken();
        operatorr();
        if (!flag) {
            flag = true;
        }
        while (tokenType != RCURLYBRACE) {
            operatorr();
            if (flag == false) {
                break;
            }
        }
        if (tokenType == RCURLYBRACE) {
            getToken();
        } else throw token;
    } else {
        flag = false;
    }
}

void SyntaxAnalyzer::operatorEqual() {
    operatorOr();
    equalCount2 = 0;
    // if (tokenType == EQUAL) {
    while (tokenType == EQUAL) {
        if (equalCount2 >= 0) {
            int k = polysisElements[polysisElements.size() - 1].getTableInNumber();
            polysisElements.erase(polysisElements.end() - 1);
            polysisElements.push_back(Token(POLIZADDRESS, k));
        }
        equalCount2++;
        getToken();
        operatorOr();
    }
    for (int i = 0; i < equalCount2; i++) {
        checkOperandTypes();
        polysisElements.push_back(Token(EQUAL));
    }
    return;
    getToken();
    if (identifiersTable[value].getInitialization())
        tokenTypes.push(identifiersTable[value].getType());
    else throw "variable is not described\n";
    polysisElements.push_back(Token(POLIZADDRESS, value));

    getToken();
    if (tokenType != RBRACKET) {
        checkIdentifierDescription();
        operatorOr();
        while (tokenType == EQUAL) {
            if (equalCount == 1) {
                int k = polysisElements[polysisElements.size() - 1].getTableInNumber();
                polysisElements.erase(polysisElements.end() - 1);
                polysisElements.push_back(Token(POLIZADDRESS, k));
            }
            equalCount++;
            getToken();
            operatorOr();
        }
        for (int i = 0; i < equalCount; i++) {
            checkOperandTypes();
            polysisElements.push_back(Token(EQUAL));
        }
        polysisElements.push_back(Token(SEMICOLON));
        equalCount = 1;
        tokenTypes.pop();
        getToken();
    }
    //  }
}

void SyntaxAnalyzer::operatorOr() {
    operatorAnd();
    while (tokenType == OR) {
        tokenTypes.push(tokenType);
        getToken();
        operatorAnd();
        checkOperands();
    }
}

void SyntaxAnalyzer::operatorAnd() {
    expression();
    while (tokenType == AND) {
        tokenTypes.push(tokenType);
        getToken();
        expression();
        checkOperands();
    }
}

void SyntaxAnalyzer::expression() {
    operatorPlusMinus();
    while (tokenType == NOTEQUAL or tokenType == LESS or tokenType == GREATER or tokenType == GREATEREQUAL or
           tokenType == LESSEQUAL or tokenType == TWOEQUAL) {
        tokenTypes.push(tokenType);
        getToken();
        operatorPlusMinus();
        checkOperands();
    }
}

void SyntaxAnalyzer::operatorPlusMinus() {
    operation();
    while (tokenType == PLUS or tokenType == MINUS) {
        tokenTypes.push(tokenType);
        getToken();
        operation();
        checkOperands();
    }
}

void SyntaxAnalyzer::operation() {
    operand();
    while (tokenType == SLASH or tokenType == MULTUPLICATION or tokenType == PERCENT) {
        tokenTypes.push(tokenType);
        getToken();
        operand();
        checkOperands();
    }
}

void SyntaxAnalyzer::operand() {
    if (tokenType == PLUS or tokenType == MINUS) {
        getToken();
        if (tokenType == NUMBER) {
            int k = token.getTableInNumber();
            token.putTableInNUmber(-k);
            tokenTypes.push(INT);
            polysisElements.push_back(token);
            getToken();
        } else throw token;
    } else if (tokenType == IDENTIFIER) {
        checkIdentifierDescription();
        if (equalCount > 1) {
            polysisElements.push_back(Token(POLIZADDRESS, value));
        } else
            polysisElements.push_back(Token(IDENTIFIER, value));
        getToken();
        if (tokenType != EQUAL && equalCount > 1) {
            int k = polysisElements[polysisElements.size() - 1].getTableInNumber();
            polysisElements.erase(polysisElements.end() - 1);
            polysisElements.push_back(Token(IDENTIFIER, k));
        }
    } else if (tokenType == STRINGVALUE) {
        tokenTypes.push(STRING);
        polysisElements.push_back(token);
        getToken();
    } else if (tokenType == NUMBER) {
        tokenTypes.push(INT);
        polysisElements.push_back(token);
        getToken();
    } else if (tokenType == TRUE) {
        tokenTypes.push(BOOLEAN);
        token.putTableInNUmber(1);
        polysisElements.push_back(Token(TRUE, 1));
        getToken();
    } else if (tokenType == FALSE) {
        tokenTypes.push(BOOLEAN);
        token.putTableInNUmber(0);
        polysisElements.push_back(Token(FALSE, 0));
        getToken();
    } else if (tokenType == NOT) {
        getToken();
        operand();
        if (tokenTypes.top() != BOOLEAN)
            throw "wrong type is in not\n";
        else {
            polysisElements.push_back(Token(NOT));
        }
    } else if (tokenType == LBRACKET) {
        getToken();
        //operatorOr();
        operatorEqual();
        if (tokenType == RBRACKET) {
            getToken();
        } else throw token;
    } else throw token;
}


void SyntaxAnalyzer::assignTypeVariable(typeOfToken type) {
    int variableFromStack;
    while (!identifierNumbers.empty()) {
        takeFromStack(identifierNumbers, variableFromStack);
        if (identifiersTable[variableFromStack].getInitialization()) {
            throw "trying to initialize a second time\n";

        } else {
            identifiersTable[variableFromStack].putInitialization();
            identifiersTable[variableFromStack].putType(type);
        }
    }
}

void SyntaxAnalyzer::checkIdentifierDescription() {
    if (tokenType != IDENTIFIER) {
        return;
    }
    if (identifiersTable[value].getInitialization())
        tokenTypes.push(identifiersTable[value].getType());
    else throw "identifier not described\n";
}


void SyntaxAnalyzer::checkOperands() {
    typeOfToken tokenType1, tokenType2, operation, currentType = INT, operationType = BOOLEAN;
    takeFromStack(tokenTypes, tokenType2);
    takeFromStack(tokenTypes, operation);
    takeFromStack(tokenTypes, tokenType1);
    if (tokenType1 == STRING and tokenType2 == STRING) {
        if (operation == PLUS) {
            operationType = STRING;
        } else if (operation != EQUAL and operation != GREATER and operation != LESS and operation != NOTEQUAL and
                   operation != TWOEQUAL) {
            throw "invalid operation for string type\n";
        }
        currentType = STRING;
    }
    if (operation == PLUS and currentType == INT) {
        operationType = INT;
    }
    if (operation == MINUS or operation == MULTUPLICATION or operation == SLASH or operation == PERCENT) {
        operationType = INT;
    }

    if (operation == OR or operation == AND) {
        currentType = BOOLEAN;
    }
    if (tokenType1 == tokenType2 && tokenType1 == currentType)
        tokenTypes.push(operationType);
    else {
        throw "wrong types are in operation";
    }
    polysisElements.push_back(Token(operation));
}

void SyntaxAnalyzer::checkOperandTypes() {
    typeOfToken bufferTokenType;
    takeFromStack(tokenTypes, bufferTokenType);
    if (bufferTokenType != tokenTypes.top())
        throw "different types in operation =\n";
}

void SyntaxAnalyzer::checkBooleanExpression() {
    if (tokenTypes.top() != BOOLEAN)
        throw "expression is not boolean\n";
    else tokenTypes.pop();
}

void SyntaxAnalyzer::checkOperandInRead() {
    if (!identifiersTable[value].getInitialization())
        throw "not declared\n";
    if (identifiersTable[value].getType() == BOOLEAN) {
        throw "read variable type of boolean\n";
    }
}

class Executer {
public:
    void execute(vector<Token> &polysisElements);
};
class StringOrInt {
public:
    int numberInTable;
    bool isInt;
    int isIntConst;
    string constString;
    StringOrInt(int number = 0, bool Int = true, int isIntConst = 0, string constS = "") {
       isInt = Int;
       numberInTable = number;
       this->isIntConst = isIntConst;
       constString = constS;
    };
};

void takeFromVector(vector<StringOrInt> &stringOrInt, int &i,  string& s = q) {
 //   i = stringOrInt[stringOrInt.size()-1].numberInTable;
    //stringOrInt.pop_back();
    if (stringOrInt[stringOrInt.size()-1].isInt) {
        boolStringOrInt = true;
        if (stringOrInt[stringOrInt.size() - 1].isIntConst == 0 or
            stringOrInt[stringOrInt.size() - 1].isIntConst == 1) {
            i = stringOrInt[stringOrInt.size() - 1].numberInTable;
            //stringOrInt.pop_back();
        } else {
            i = stringOrInt[stringOrInt.size() - 1].numberInTable;
            i = identifiersTable[i].getValue();
        }
        stringOrInt.pop_back();
    }
    else {
        boolStringOrInt = false;
        if (stringOrInt[stringOrInt.size() - 1].isIntConst == 2) {
            s = stringOrInt[stringOrInt.size() - 1].constString;
        }
        else {
            i = stringOrInt[stringOrInt.size() - 1].numberInTable;
            s = identifiersTable[i].getValueString();
        }
        stringOrInt.pop_back();
    }
}
void Executer::execute(vector<Token> &polysisElements) {
    Token token;
    string iS, jS;
    typeOfToken type;
    vector <StringOrInt> stringOrInt;
    string buffer;
    int i, j, index = 0, size = polysisElements.size();
    while (index < size) {
        token = polysisElements[index];
        switch (token.getType()) {
            case TRUE:
            case FALSE:
            case NUMBER:
                i = token.getTableInNumber();
                stringOrInt.push_back(StringOrInt(i, true, 0));
                break;
            case POLIZADDRESS:
            case POLIZLABEL:
                i = token.getTableInNumber();
                if (identifiersTable[i].getType() == STRING) {
                    stringOrInt.push_back(StringOrInt(i, false, 4, identifiersTable[i].getValueString()));
                }
                else {
                    stringOrInt.push_back(StringOrInt(i, true, 1));
                }
                break;
            case STRINGVALUE:
                stringOrInt.push_back(StringOrInt(0, false, 2, token.getValueString()));
                break;
            case LABEL:
                break;
            case IDENTIFIER:
                i = token.getTableInNumber();
                type = identifiersTable[i].getType();
                if (identifiersTable[i].getAssign()) {
                    if (type == INT or type == BOOLEAN) {
                        stringOrInt.push_back(StringOrInt(i, true, 3));
                    } else if (type == STRING) {
                        stringOrInt.push_back(StringOrInt(i, false, 4, identifiersTable[i].getValueString()));
                    }

                } else {
                    if (type == INT or type == BOOLEAN) {
                        identifiersTable[i].putValue(0);
                        stringOrInt.push_back(identifiersTable[i].getValue());

                    } else if (type == STRING) {
                        identifiersTable[i].putValue("");
                        stringOrInt.push_back(StringOrInt(i, false));
                    }
                }
                break;
            case NOT:
                takeFromVector(stringOrInt, i);
               stringOrInt.push_back(StringOrInt(!i));
                break;
            case PERCENT:
                if (stringOrInt[stringOrInt.size()-1].isIntConst == 0) {
                    i = stringOrInt[stringOrInt.size()-1].numberInTable;
                    stringOrInt.pop_back();
                }
                else {
                    takeFromVector(stringOrInt, i);
                    i= identifiersTable[i].getValue();
                }
                if (stringOrInt[stringOrInt.size()-1].isIntConst == 0) {
                    j = stringOrInt[stringOrInt.size()-1].numberInTable;
                    stringOrInt.pop_back();
                }
                else {
                  //  takeFromVector(stringOrInt, j);
                  j = stringOrInt[stringOrInt.size()-1].numberInTable;
                   stringOrInt.pop_back();
                    j = identifiersTable[j].getValue();
                }
                if (i) {
                    //args.push(j / i);
                    stringOrInt.push_back(StringOrInt(j%i, true, 0));
                    break;
                } else
                    throw "\nDivide by zero";
            case OR:
                //takeFromStack(args, i);
                //takeFromStack(args, j);
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                stringOrInt.push_back(StringOrInt(j or i));
                break;

            case AND:
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                stringOrInt.push_back(StringOrInt(j and i));
                break;

            case POLIZGO:
                takeFromVector(stringOrInt, i);
                index = i - 1;
                break;

            case POLIZFGO:
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                if (!j) index = i - 1;
                break;

            case WRITE:
               for (int z = 0; z < stringOrInt.size(); z++) {
                   if (stringOrInt[z].isInt) {
                       if (stringOrInt[z].isIntConst == 0) {
                         cout << "\n" << stringOrInt[z].numberInTable << endl;
}
                       else {
                           cout << "\n" << identifiersTable[stringOrInt[z].numberInTable].getValue() << endl;
                       }
                   }
                   else {
                       if (stringOrInt[z].isIntConst == 2) {
                           cout << "\n" << stringOrInt[z].constString << endl;
                       }
                       else {
                           cout << "\n" << identifiersTable[stringOrInt[z].numberInTable].getValueString() << endl;
                       }
                   }
               }

               while (!stringOrInt.empty()) {
                   stringOrInt.erase(stringOrInt.begin() + 0);
               }
                break;
             /*   while (!args.empty()) {
                    takeFromStack(args, j);
                    if (j < 0) {
                        takeFromStack(argv, buffer);
                        writeString.push_back(buffer);
                        // cout << buffer;
                        continue;
                    }
                    auto s = std::to_string(j);
                    writeString.push_back(s);
                    // cout << j;
                }
                for (int vectorSize = writeString.size() - 1; vectorSize >= 0; vectorSize--) {
                    cout << "\n" << writeString[vectorSize] << endl;
                }
                break; */
            case READ:
                int k;
                takeFromVector(stringOrInt, i);
                if (identifiersTable[i].getType() == INT) {
                    cout << "\nInput int value for " << identifiersTable[i].getName() << endl;
                    cin >> k;
                    identifiersTable[i].putValue(k);
                } else {
                    string d;
                    cout << "\nInput string value for " << identifiersTable[i].getName() << endl;
                    cin >> d;
                    identifiersTable[i].putValue(d);
                }
                identifiersTable[i].putAssign();
                break;

            case PLUS:
                takeFromVector(stringOrInt, i, iS);
                takeFromVector(stringOrInt, j, jS);
if (boolStringOrInt) {
    stringOrInt.push_back(StringOrInt(i+j));
}
else {
    stringOrInt.push_back(StringOrInt(0, false, 2, jS+iS));
}
                break;

            case MULTUPLICATION:
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                stringOrInt.push_back(StringOrInt(i*j));
                break;

            case MINUS:
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                stringOrInt.push_back(StringOrInt(j-i));
                break;
            case SEMICOLON:
               stringOrInt.pop_back();
                break;
            case SLASH:
                if (stringOrInt[stringOrInt.size()-1].isIntConst == 0) {
                    i = stringOrInt[stringOrInt.size()-1].numberInTable;
                    stringOrInt.pop_back();
                }
                else {
                    takeFromVector(stringOrInt, i);
                   i= identifiersTable[i].getValue();
                }
                if (stringOrInt[stringOrInt.size()-1].isIntConst == 0) {
                    j = stringOrInt[stringOrInt.size()-1].numberInTable;
                    stringOrInt.pop_back();
                }
                else {
                    j = stringOrInt[stringOrInt.size()-1].numberInTable;
                    stringOrInt.pop_back();
                    j = identifiersTable[j].getValue();
                }
                if (i) {
                    stringOrInt.push_back(StringOrInt(j/i, true, 0));
                    break;
                } else
                    throw "POLIZ:divide by zero";
            case EQUAL:
                    takeFromVector(stringOrInt, i, iS);
                    takeFromVector(stringOrInt, j, jS);
                if (boolStringOrInt) {
                    stringOrInt.push_back(StringOrInt(i));
                    identifiersTable[j].putValue(i);
                    identifiersTable[j].putAssign();
                }
                else {
                    stringOrInt.push_back(StringOrInt(i, false, 2, iS));
                    identifiersTable[j].putAssign();
                    identifiersTable[j].putValue(iS);
                }
                break;
            case TWOEQUAL:
                takeFromVector(stringOrInt, i, iS);
                takeFromVector(stringOrInt, j, jS);
                if (boolStringOrInt) {
                    stringOrInt.push_back(StringOrInt(j == i));
                }
                else {
                    stringOrInt.push_back(StringOrInt(jS == iS));
                }
                break;

            case LESS:
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                if (boolStringOrInt) {
                    stringOrInt.push_back(StringOrInt(j < i));
                }
                else {
                    stringOrInt.push_back(StringOrInt(jS < iS));
                }
                break;

            case GREATER:
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                if (boolStringOrInt) {
                    stringOrInt.push_back(StringOrInt(j > i));
                }
                else {
                    stringOrInt.push_back(StringOrInt(jS > iS));
                }
                break;

            case LESSEQUAL:
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                if (boolStringOrInt) {
                    stringOrInt.push_back(StringOrInt(j <= i));
                }
                else {
                    stringOrInt.push_back(StringOrInt(jS <= iS));
                }
                break;

            case GREATEREQUAL:
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                if (boolStringOrInt) {
                    stringOrInt.push_back(StringOrInt(j >= i));
                }
                else {
                    stringOrInt.push_back(StringOrInt(jS >= iS));
                }
                break;

            case NOTEQUAL:
                takeFromVector(stringOrInt, i);
                takeFromVector(stringOrInt, j);
                if (boolStringOrInt) {
                    stringOrInt.push_back(StringOrInt(j != i));
                }
                else {
                    stringOrInt.push_back(StringOrInt(jS != iS));
                }
                break;
            default:
                throw "\nUndefined element!\n";
        }
        ++index;
    }
    cout << "\nFinish\n" << endl;
}

class Interpretator {
    SyntaxAnalyzer syntaxAnalyzer;
    Executer executer;
public:
    void interpretation();
};

void Interpretator::interpretation() {
    syntaxAnalyzer.parse();
  executer.execute(syntaxAnalyzer.polysisElements);
}


int main() {
    try {
        Interpretator interpretator;
        interpretator.interpretation();
        return 0;
    }
    catch (const char *err) {
        cout << err;
    }
    catch (Token &token) {
        cout << "плохоооооо " << token;
    }
}
/*
SyntaxAnalyzer syntaxAnalyzer;
try {
    syntaxAnalyzer.parse();
    cout << "\nвсё окей!\n";
}
catch (const char *err) {
    cout << err;
}
catch (Token &token) {
    cout << "плохоооооо " << token;
}
return 0;
Token token;
bool flag = true;
try {
    LexicalAnalyzer scanner("f1.txt");
    while (flag) {
        token = scanner.getToken();
        if (token.getType() == POINT) {
            cout << "lexical analysis completed!\n";
            break;
        }
        cout << token;
    }
}
catch (string &error) {
    cout << error;
}
}



*/