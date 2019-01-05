#include "Utils.h"


void findFile(const string & str, vector<string> *res)
{
	long handle;
	struct _finddata_t fileinfo;
	//��һ�β���
	handle = _findfirst(str.c_str(), &fileinfo);
	if (handle == -1)
		return;
	do
	{
		//�ҵ����ļ����ļ���
		//printf("%s\n", fileinfo.name);
		res->push_back(fileinfo.name);

	} while (!_findnext(handle, &fileinfo));

	_findclose(handle);
}


void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}


void stringReplace(std::string &strBig, const std::string &strsrc, const std::string &strdst)
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}
