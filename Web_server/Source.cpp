#include "WebServer.h"

int main()
{
	WebServer ws("0.0.0.0", 8080);
	if (ws.init_listener() != 0)
		return -1;
	ws.run_listener();
	return 0;
}