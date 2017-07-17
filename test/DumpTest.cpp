#include "MRDumper.h"

class CrashTest
{
public:
	void Test()
	{
		Crash();
	}

private:
	void Crash()
	{
		int i = 13;
		int j = 0;
		int m = i / j;
	}
};

int main(int argc,char* argv[])
{
	CrashTest test;
	test.Test();
	return 0;
}