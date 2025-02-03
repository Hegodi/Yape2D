#define OLC_PGE_APPLICATION
#include <olcPixelGameEngine.h>

#include <Playground.h>

int main()
{
	Playground workbench;
	if (workbench.Construct(1000, 600, 1, 1, false, true))
		workbench.Start();
	return 0;
}