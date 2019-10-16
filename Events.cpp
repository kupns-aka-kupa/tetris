#include "Header.h"
#include "Events.h"

bool *Events::get_key()
{
	int i = 0;
	while (i < 256)
	{
		if (GetAsyncKeyState(i))
		{
			KEY[i] = 1;
		}
		else
		{
			KEY[i] = 0;
		}
		i++;
	}
	return KEY;
}