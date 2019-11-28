#include<string>
#include<iostream>
#include<list>
#include<fstream>
enum Status {
        SUCCESS,
        FAILED,
        UNKNOWN
};

namespace keymap{ // 使用的快捷键映射在此更改
    const char begin = 'b';
    const char chLine = 'c';
    const char delLine = 'd';
    const char end = 'e';
    const char find = 'f';
    const char gotoLine = 'g';
    const char help1 = '?';
    const char help2 = 'h';
    const char insert = 'i';
    const char next = 'n';
    const char prev = 'p';
    const char quit = 'q';
    const char undo = 'u';
    const char view = 'v';
    const char write = 'w';
}

namespace interact{
    static char GetChar(std::istream &inStream = std::cin){
        char ch;
        while ((ch = (inStream).peek()) != EOF
               && ((ch = (inStream).get()) == ' '
                   || ch == '\t'));

        return ch;
    }

    static bool UserSaysYes(){
        char ch;
        bool initialResponse = true;

        do{
            if (initialResponse)
            {
                std::cout << "(y, n)?";
            }
            else
            {
                std::cout << "请用Y/N回答:";
            }

            while ((ch = GetChar()) == '\n');
            initialResponse = false;
        } while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N');
        while (GetChar() != '\n');

        return ch == 'y' || ch == 'Y';
    }
}

namespace listOperation{
    template <class T>
    void Traverse(std::list<T> &l){
        auto it = l.begin();
        size_t lineNum = 1;
        for(; it!=l.end(); it++){
            std::cout<<lineNum<<": "<< *it << std::endl;
            lineNum++;
        }
    }

    std::string GetElem(std::list<std::string> &l, size_t ind){
        auto it = l.begin();
        ind--;
        while(ind--){
            it++;
        }
        return *it;
    }

    void SetElem(std::list<std::string> &l, size_t ind, std::string &str){
        auto it = l.begin();
        ind--;
        while(ind--){
            it++;
        }
        *it = str;
    }

    Status InsertElem(std::list<std::string> &l, size_t ind, std::string &str){
        auto it = l.begin();
        ind--;
        while(ind--){
            it++;
        }
        l.insert(it, str);
        return SUCCESS;
    }

    Status DelElem(std::list<std::string> &l, size_t ind){
        auto it = l.begin();
        ind--;
        while(ind--){
            it++;
        }
        l.erase(it);
        return SUCCESS;
    }
}

class LineEditor{
private:
    std::list<std::string> textBuffer;
    int curLineNo;
    std::list<std::string> textUndoBuffer;
    int curUndoLineNo;
    std::ifstream inFile;
    std::ofstream outFile;

    Status NextLine();
    Status PrevLine();
    Status GotoLine();
    Status InsertLine();
    Status ChangeLine();
    void ReadFile();
    void WriteFile();
    void FindString();
    void View(); // 遍历双向链表

public:
    LineEditor(std::string inFileName, std::string outFileName);
    void Run();
};

LineEditor::LineEditor(std::string inFileName, std::string outFileName) {
    if(!inFileName.empty() && !outFileName.empty()){
        inFile.open(inFileName, std::ios::in);
        if(!inFile.is_open()) throw "输入文件不存在！";
        outFile.open(outFileName, std::ios::app);
        if(!outFile.is_open()) throw "输出文件打开失败！";
        ReadFile();
    }else if(inFileName.empty()){
        std::cerr<<"输入文件名不能为空！"<<std::endl;
    }else{
        std::cerr<<"输出文件名不能为空！"<<std::endl;
    }
}

void LineEditor::ReadFile() {
    bool proceed;
    if(!textBuffer.empty()) {
        std::cout << "缓存区不为空！继续将清空缓存区！" << std::endl;
        std::cout << "确认吗？" << std::endl;
        proceed = interact::UserSaysYes();
        if (proceed) {
            textBuffer.clear();
        }
    }
    std::string strTemp;
    int lineNumber = 1;
    while(std::getline(inFile, strTemp)){
        textBuffer.push_back(strTemp);
        lineNumber++;
    }
    if(textBuffer.empty()) curLineNo = 0;
    else curLineNo = 1;

}

void LineEditor::WriteFile() {
    std::string curLine;
    for(auto it = textBuffer.begin(); it != textBuffer.end(); it++){
        curLine = *it;
        outFile<< curLine.c_str() <<std::endl;
    }
}

void LineEditor::FindString() {
    int index;
    std::cout<<"输入被查找的字符串："<<std::endl;
    std::string target;
    std::cin>>target;
    std::string curLine = listOperation::GetElem(textBuffer, curLineNo);
    while((index = curLine.find(target, 0)) == -1){
        if(curLineNo < textBuffer.size()){
            curLineNo++;
            curLine = listOperation::GetElem(textBuffer, curLineNo);
        }else{
            break;
        }
    }
    if(index == std::string::npos){
        std::cout<<"查找串失败！"<<std::endl;
    }else{
        std::cout<<curLine<<std::endl;
        for(int i = 0; i < index; i++){
            std::cout<<" ";
        }
        for(int j = 0; j < target.size(); j++){
            std::cout<<"^";
        }
    }
    std::cout<<std::endl;
}

Status LineEditor::ChangeLine() {
    Status result = SUCCESS;
    std::cout<<"输入当前行想要被替换的指定文本串："<<std::endl;
    std::string oldStr, newStr;
    std::cin>>oldStr;
    std::cout<<"输入新文本串："<<std::endl;
    std::cin>>newStr;

    std::string curLine = listOperation::GetElem(textBuffer, curLineNo);
    size_t index = curLine.find(oldStr, 0);
    if(index == std::string::npos){
        return FAILED;
    }else {
        std::string newLine = curLine.substr(0, index);
        newLine += newStr;
        newLine += curLine.substr(index + oldStr.size()); // 直接截取到最后
        listOperation::SetElem(textBuffer, curLineNo, newLine);
        return SUCCESS;
    }
}

Status LineEditor::NextLine() {
    if(curLineNo<textBuffer.size()){
        curLineNo++;
        return SUCCESS;
    }else{
        return FAILED;
    }
}

Status LineEditor::PrevLine() {
    if(curLineNo<=1){
        return FAILED;
    }else{
        curLineNo++;
        return SUCCESS;
    }
}

Status LineEditor::GotoLine() {
    int lineNum;
    std::cout<<"请输入指定行号："<<std::endl;
    std::cin>>lineNum;
    if(lineNum<1 || lineNum > textBuffer.size()){
        return FAILED;
    }else{
        curLineNo = lineNum;
        return SUCCESS;
    }
}

Status LineEditor::InsertLine() {
    int lineNum;
    std::cout<<"请输入插入行号："<<std::endl;
    std::cin>>lineNum;
    while(std::cin.get()!='\n'); // skip all chars that isn't '\n' to avoid left '\n' in buffer
    std::cout<<"输入插入的行所对应的文本串："<<std::endl;
    std::string newLine;
    std::cin>>newLine;
    if(listOperation::InsertElem(textBuffer, lineNum, newLine) == SUCCESS){
        curLineNo = lineNum;
        return SUCCESS;
    }else{
        return FAILED;
    }

}

void LineEditor::Run() {
    char userCommand;
    do{
        std::string tmpStr;
        std::string curLine;

        if(curLineNo != 0){
            curLine = listOperation::GetElem(textBuffer, curLineNo);
            std::cout<<curLineNo<<": "<<curLine<<std::endl<<"?";
        }else{
            std::cout<<"文本缓存区为空"<<std::endl<<"?";
        }
        while(std::cin.get()!='\n');
        userCommand = interact::GetChar();
        userCommand = std::tolower(userCommand);

        if(userCommand != keymap::undo && userCommand != keymap::help1 && userCommand != keymap::help2
        && userCommand != keymap::view){ // 无需记录缓存的几个命令
            textUndoBuffer = textBuffer;
            curUndoLineNo = curLineNo;
        }
        switch(userCommand){
            case keymap::begin :
                if(textBuffer.empty()){
                    std::cerr<<"文本缓存区为空"<<std::endl;
                }else{
                    curLineNo = 1;
                }
                break;
            case keymap::chLine :
                if(textBuffer.empty()){
                    std::cerr<<"文本缓存区为空"<<std::endl;
                }else{
                    ChangeLine();
                }
                break;
            case keymap::delLine:
                if(listOperation::DelElem(textBuffer, curLineNo) != SUCCESS){
                    std::cerr<<"删除行失败"<<std::endl;
                }
                break;
            case keymap::end :
                if(textBuffer.empty()){
                    std::cerr<<"错误：文本缓存为空"<<std::endl;
                }else{
                    curLineNo = textBuffer.size();
                }
                break;
            case keymap::find :
                if(textBuffer.empty()){
                    std::cerr<<"错误：文本缓存为空"<<std::endl;
                }else{
                    FindString();
                }
                break;
            case keymap::gotoLine :
                if(GotoLine() != SUCCESS){
                    std::cerr<<"错误：没有那样的行"<<std::endl;
                }
                break;

            case keymap::help1:
            case keymap::help2:
                std::cout << "有效命令: b(egin) c(hange) d(el) e(nd)" << std::endl
                     << "f(ind) g(o) h(elp) i(nsert) n(ext) p(rior) " << std::endl
                     << "q(uit) r(ead) v(iew) w(rite) " << std::endl;
                break;

            case keymap::insert:
                if(InsertLine()!=SUCCESS){
                    std::cerr<<"错误：插入行出错！"<<std::endl;
                }
                break;

            case keymap::next:
                if(NextLine()!=SUCCESS){
                    std::cout<<"当前已是最后一行！"<<std::endl;
                }
                break;

            case keymap::prev:
                if(PrevLine()!=SUCCESS){
                    std::cout<<"当前已是第一行！"<<std::endl;
                }
                break;

            case keymap::quit:
                break;

            case keymap::undo:
                std::swap(textUndoBuffer, textBuffer);
                std::swap(curUndoLineNo, curLineNo);
                break;
            case keymap::view:
                View();
                break;
            case keymap::write:
                if(textBuffer.empty()){
                    std::cerr<<"文本缓存区为空"<<std::endl;
                }else{
                    WriteFile();
                }
                break;
            default:
                std::cout << "输入h或?获得帮助或输入有效命令字符: \n";
                break;
        }
    }while(userCommand!=keymap::quit);
}

void LineEditor::View() {
    listOperation::Traverse(textBuffer);
}

