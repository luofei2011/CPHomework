#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<math.h>
using namespace std;

char *dectionary = "dectionary.dat";
char *keyWords = "keyWords.dat";

/*judge the two array'value*/
bool isEqualArr(char ch1[],char ch2[]){
	int MinLength = sizeof(ch1) < sizeof(ch2) ? sizeof(ch1) : sizeof(ch2);
	int MaxLength = sizeof(ch1) > sizeof(ch2) ? sizeof(ch1) : sizeof(ch2);
	for(int i=0; i<MinLength; i++){
		if(ch1[i] != ch2[i])
			return false;
	}
	for(int i=0; i<MaxLength-MinLength; i++)
		if(ch2[i])
			return false;
	return true;
}

/*对涉及到的二目,单运算符,比较符进行处理*/
bool dealDoubleSym(char ch){
	if(ch == '!' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>'){
		return true;
	}
	return false;
}

/*处理第二位运算符或则比较符*/
bool dealSecond(char ch){
	if(ch == '+' || ch == '-' || ch == '=' || ch == '>' || ch == '<'){
		return true;
	}
	return false;
}

/*search the keyWords.dat*/
int getPosition(char *fileName,string str, bool isKey){
	ifstream file;
    file.open(fileName,ios::in);
	char buffer[1024] = {0};
	int position = 0;
	int j = 0;

	while(file.getline(buffer,1024)){
		char buff[100] = {0};
		int i =0;
		while(buffer[i]){
			if(buffer[i] >= 'a' && buffer[i] <= 'z' && isKey){
				buff[j++] = buffer[i];
			}
			else if(!isKey && !(buffer[i] >= '0' && buffer[i] <= '9') && buffer[i] != '|'){
				buff[j++] = buffer[i];
			}
			if(buffer[i] == '|'){
				if(strcmp(str.c_str(),buff) == 0) 
					return position;
				j = 0;
				memset(buff,0,100);
				position = 0;
			}
			if(buffer[i] >= '0' && buffer[i] <= '9'){
				position = 10*position + (buffer[i] - '0');
			}
			i++;
		}
		memset(buffer,0,1024);
	}
	return 0;
}

//bool isIdentifier()
bool notNumLett(char ch){
	if(!(ch >= 'a' && ch <= 'z'))  
		if(!(ch >= 'A' && ch <= 'Z'))
			if(!(ch >= '0' && ch <= '9'))
				if(ch != '_')
					return false;
	return true;
}

/*非数字的判断*/
bool notNum(char ch){
	if(!(ch >= '0' && ch <= '9'))
		if(ch != '.')
			return false;
	return true;
}

/*单行注释的判断*/
bool isRowNote(char ch){
	if(ch == '/')
		return true;
	return false;
}

/*多行注释判断*/
bool isRowsNote(char ch){
	if(ch == '*')
		return true;
	return false;
}

/*返回多行注释结束的位置*/
int endPosition(char ch[]){
	int len = strlen(ch);
	for(int i=0; i<len; i++)
		if(ch[i] == '*' && ch[i+1] == '/')
			return (i+2);
	return 0;
}

int main(){
	ifstream file;
	file.open("test.txt",ios::in);
	char buff[100] = {0};
	//多行注释是否结束的标志
	bool NoteEnd = false;

	while(file.getline(buff,100)){
		int i =0;
		if(NoteEnd){
			i = endPosition(buff);	
			if(!i)
				continue;
			NoteEnd = false;
		}
		while(buff[i]){
			
			//去掉空格 特别的还有tab键 由于是按行读取故不用考虑回车换行符号
			if(buff[i] == ' ' || buff[i] == '\t'){
				i++;
				continue;
			}
			/*忽略掉注释*/
			/*标识符或则关键字*/
			else if((buff[i] >= 'a' && buff[i] <= 'z') || (buff[i] >= 'A' && buff[i] <= 'Z') || buff[i] == '_'){
				string strBuff = "";
				strBuff += buff[i];
				i++;
				while(notNumLett(buff[i])){
					strBuff += buff[i];
					i++;
				}
				int pos = getPosition(keyWords,strBuff,true);
				if(pos){
					cout << "<" << pos << "," << strBuff << ">"  << endl;
					pos = 0;
				}
				else{ 
					cout << "<" << getPosition(dectionary,"MARK",false) << "," << strBuff << ">" << endl;
				}
					
			}
			//无符号整数,浮点数
			else if(buff[i] >= '0' && buff[i] <= '9'){
				int num = 0;
				float fl = 0;
				bool isF = false;
				num = buff[i] - '0';
				i++;
				while(notNum(buff[i])){
					if(buff[i] == '.'){
						//判断是否语法错误
						isF = true;
						int _num = 0;
						i++;
						int label = 0;
						while(notNum(buff[i])){
							label ++;
							_num = _num*10 + (buff[i] - '0');
							i++;
						}
						fl = _num / pow(10,label);
						fl = num + fl;
					}
					else{
						num = num*10 + buff[i] - '0';
						i++;
					}
				}
				//浮点
				if(isF)
					cout << "<" << getPosition(dectionary,"UNSIGNEDFLOAT",false) << "," << fl << ">" << endl;
				//整数
				else
					cout << "<" << getPosition(dectionary,"UNSIGNEDINT",false) << "," << num << ">" << endl;
			}
			//! != + += - -= * *= / /+ = == -- ++
			else if(dealDoubleSym(buff[i])){
				string str = "";
				str += buff[i];
				if(isRowNote(buff[i+1]))
					break;
				if(isRowsNote(buff[i+1])){
					NoteEnd = true;
					i = endPosition(buff);	
					if(!i)
						break;
					continue;
				}
				if(dealSecond(buff[i+1])){
					str += buff[i+1];
					i++;
				}
				cout << "<" << getPosition(dectionary,str,false) << "," << str << ">" << endl;	
				i++;
			}
			/*处理字符串*/
			else if(buff[i] == '"'){
				cout << "<" << "76" << "," << '"' << ">" << endl;	
				string str = "";
				i++;
				while(buff[i] != '"'){
					str += buff[i];
					i++;
				}
				i++;
				cout << "<" << getPosition(dectionary,"STRING",false) << "," << str << ">" << endl;	
				cout << "<" << "76" << "," << '"' << ">" << endl;	
			}
			/*处理字符*/
			else if(buff[i] == '\''){
				if(buff[i+2] == '\''){
					cout << "<" << "75" << "," << '\'' << ">" << endl;	
					cout << "<" << getPosition(dectionary,"CHAR",false) << "," << buff[i+1] << ">" << endl;	
					cout << "<" << "75" << "," << '\'' << ">" << endl;	
				}
				i+=3;
			}
			else {
				string str = "";
				str = buff[i];
				cout << "<" << getPosition(dectionary,str,false) << "," << str << ">" << endl;
				i++;
			}
		}
	}
	return 0;
}
