#include "CSVLoader.h"

CSVLoader::CSVLoader()
	:_hasMore(false)
	,_token(",")
{
}

CSVLoader::~CSVLoader()
{
}

bool CSVLoader::loadCSV(const std::string& filename)
{
	std::ifstream in;
	in.open(filename);

	//打开文件失败
	if (!in.is_open()) 
		return false;
	//定位到文件的末尾
	in.seekg(0, std::ios::end);
	//获取总长度
	int size = (int)in.tellg();

	char* buffer = new char[size + 1];
	memset(buffer, '\0', size + 1);

	//读取
	in.seekg(0, std::ios::beg);
	in.read(buffer, size);
	//关闭文件
	in.close();

	//设置变量
	_reader = std::stringstream(buffer);
	_hasMore = true;

	//释放变量
	delete[] buffer;

	return true;
}

bool CSVLoader::loadStr(const std::string& text)
{
	if (text.size() == 0)
		return false;

	_reader = std::stringstream(text);
	_hasMore = true;

	return true;
}

void CSVLoader::skip(int num)
{
	std::string src;
	int index = 0;

	//读取一行
	while (index < num)
	{
		if (getline(_reader, src))
		{
		}
		else
		{
			_hasMore = false;
			return;
		}

		index += 1;
	}
}

bool CSVLoader::hasNextLine()
{
	if (_queue.size() > 0)
		return true;
	else if (_hasMore)
	{
		this->readLine();
		return _queue.size() > 0;
	}
	return false;
}

int CSVLoader::nextInt()
{
	//获取
	const std::string& token = _queue.front();
	int number = std::stoi(token);
	_queue.erase(_queue.begin());

	return number;
}

float CSVLoader::nextFloat()
{
	//获取
	const std::string& token = _queue.front();
	float number = std::stof(token);
	_queue.erase(_queue.begin());

	return number;
}

std::string CSVLoader::nextStr()
{
	std::string token = _queue.front();
	_queue.erase(_queue.begin());

	return token;
}

void CSVLoader::readLine()
{
	std::string src;
	//读取一行,之后把读取到的单词保存到_queue中
	if (getline(_reader, src))
	{
		size_t nend = 0;
		size_t nbegin = 0;
		size_t tokenSize = _token.size();

		while (nend != std::string::npos)
		{
			nend = src.find(_token, nbegin);
			if (nend == std::string::npos)
				_queue.push_back(src.substr(nbegin, src.length() - nbegin));
			else
			{
				_queue.push_back(src.substr(nbegin, nend - nbegin));
				nbegin = nend + tokenSize;
			}
		}//end while
	}//end if
	else
		_hasMore = false;
}
