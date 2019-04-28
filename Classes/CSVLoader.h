#ifndef __CSVLoader_H__
#define __CSVLoader_H__

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

/*csv解析工具*/
class CSVLoader
{
public:
	CSVLoader();
	~CSVLoader();

	//加载csv文件
	bool loadCSV(const std::string& filename);
	//加载字符串
	bool loadStr(const std::string& text);

	//跳过下一行
	void skip(int num=1);

	//是否还有下一行
	bool hasNextLine();

	int nextInt();
	float nextFloat();
	std::string nextStr();
private:
	void readLine();
private:
	//是否还有更多行
	bool _hasMore;
	//分隔符
	std::string _token;
	std::stringstream _reader;
	std::vector<std::string> _queue;
};
#endif