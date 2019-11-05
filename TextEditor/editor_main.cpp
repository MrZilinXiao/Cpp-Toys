//
// Created by Zilin Xiao on 2019/11/5.
//

#include "texteditor.h"


int main(){
    std::string inFilePath, outFilePath;
    try{
        std::cout<<"输入文件路径:"<<std::endl;
        std::cin>>inFilePath;
        std::cout<<"输出文件路径:"<<std::endl;
        std::cin>>outFilePath;
        LineEditor editor(inFilePath, outFilePath);
        editor.Run();
    }catch (const char* err){
        std::cerr<<err<<std::endl;
        exit(0);
    }
    return 0;
}