#include <Nooskewl_Engine/Nooskewl_Engine.h>

class Widget_Quest_Details : public Widget
{
public:
	Widget_Quest_Details(Widget_List *list, std::vector<int> indices);

	void handle_event(TGUI_Event *event);
	void draw();

private:
	Widget_List *list;
	std::vector<int> indices;

	int current_quest;
	int offset;
};
