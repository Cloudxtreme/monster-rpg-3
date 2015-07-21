#ifndef TALK_BRAIN
#define TALK_BRAIN

#include "Nooskewl_Engine/brain.h"

using namespace Nooskewl_Engine;

class Talk_Brain : public Brain {
public:
	Talk_Brain(std::string name, Callback callback = NULL, void *callback_data = NULL);
	~Talk_Brain();

	void activate(Map_Entity *activator, Map_Entity *activated);

private:
	struct Talk {
		int milestone;
		std::string text;
	};

	static bool compare_milestones(Talk *a, Talk *b);

	std::string name;

	// sorted: highest milestone first
	std::vector<Talk *> sayings;
	Callback callback;
	void *callback_data;
};

#endif // TALK_BRAIN