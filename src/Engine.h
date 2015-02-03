
#ifndef __ENGINE_H__
#define __ENGINE_H__

namespace Engine
{
	bool	init();
	bool	createWindow();
	bool	loadConfig();
	void	saveConfig();
	bool	mainLoop();
	bool	processEvents();
	void	shutDown();
	void	resizeWindow(int width, int height);
	void	lockMouse(bool lock);
}

#endif//__ENGINE_H__
