#ifndef QUEST_H
#define QUEST_H

class Quest {
public:
	class Step {
	public:
		Step(std::string description, std::string conditions);

		std::string get_description();

		bool is_finished();

		void update();

	private:
		std::string description; // should be short for start_step
		std::vector<int> conditions;
		bool finished;
	};

	Quest(Step *start_step, std::vector<Step *> steps);
	~Quest();

	Step *get_start_step();
	std::vector<Step *> get_steps();

	bool is_finished();

	void update();

private:
	Step *start_step;
	std::vector<Step *> steps;
	bool finished;
};

extern std::vector<Quest *> quests;

void init_quests();

#endif // QUEST_H
