#include "ShellTestApp.h"

using namespace std;

int main()
{
	SSD ssd;
	ShellTestApp shellTestApp(&ssd);
	shellTestApp.Start();
	return 0;
}