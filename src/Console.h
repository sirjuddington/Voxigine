
#ifndef	__CONSOLE_H__
#define	__CONSOLE_H__

#include <SFML/Window/Event.hpp>

// Forward declarations
namespace sf { class RenderWindow; }

namespace Console
{
	class Command
	{
	private:
		string	name;
		void(*commandFunc)(vector<string>);
		size_t	min_args;
		bool	show_in_list;

	public:
		Command(string name, void(*commandFunc)(vector<string>), int min_args, bool show_in_list = true);
		~Command() {}

		string	getName() { return name; }
		bool	showInList() { return show_in_list; }
		void	execute(vector<string> args);
		size_t	minArgs() { return min_args; }

		inline bool operator<(Command c) const { return name < c.getName(); }
		inline bool operator>(Command c) const { return name > c.getName(); }
	};

	unsigned	numCommands();
	Command&	command(size_t index);
	void		addCommand(Command& c);
	void		execute(string command);
	void		logMessage(string message);
	string		lastLogLine();
	string		lastCommand();
	string		dumpLog();
	string		prevCommand(int index);
	unsigned	numPrevCommands();
	bool		isActive();
	void		activate(bool enable = true);
	bool		handleKeyPress(sf::Event::KeyEvent& event);
	bool		handleText(sf::Event::TextEvent& event);

	// Drawing
	void	draw(sf::RenderWindow* window);
};

// Define for neat console command definitions
#define CONSOLE_COMMAND(name, min_args, show_in_list) \
	void c_##name(vector<string> args); \
	Console::Command name(#name, &c_##name, min_args, show_in_list); \
	void c_##name(vector<string> args)

#endif //__CONSOLE_H__
