#include <iostream>

int main(int argc, char* argv[])
{
	std::locale::global(std::locale("kor"));

	std::wcout<<L"end of aCapRender"<<std::endl;
	return 0;
}