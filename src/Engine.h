
#ifndef __ENGINE_H__
#define __ENGINE_H__

namespace Engine
{
	bool	init();
	bool	createWindow();
	bool	loadConfig();
	void	saveConfig();
	bool	mainLoop();
	void	shutDown();
	void	resizeWindow(int width, int height);
}

#endif//__ENGINE_H__
