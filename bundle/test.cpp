#include<iostream>
#include<fstream>
#include"bundle.h"
using namespace std;

int main()
{
	ifstream ifs;
	ifs.open("text.txt", ios::binary);
	ifs.seekg(0, std::ios::end);
	size_t fsize = ifs.tellg();
	ifs.seekg(0, ios::beg);

	string body;
	body.resize(fsize);
	ifs.read(&body[0], fsize);
	string packed = bundle::pack(bundle::LZIP, body);

	std::ofstream ofs;
	ofs.open("hello.zip", ios::binary);
	ofs.write(&packed[0], packed.size());
	ifs.close();
	ofs.close();

}
