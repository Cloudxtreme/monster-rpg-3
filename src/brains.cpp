#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "brains.h"
#include "gui.h"

bool start_brains()
{
	if (Door_Brain::start() == 0) {
		return false;
	}

	return true;
}

void end_brains()
{
	Door_Brain::end();
}

void Talk_Brain::callback(void *data)
{
	Callback_Data *d = (Callback_Data *)data;

	d->entity->set_direction(d->direction);

	if (d->user_callback) {
		d->user_callback(data);
	}

	((Talk_Brain *)d->entity->get_brain())->talking = false;
}

Talk_Brain::Talk_Brain(std::string name, Callback callback, void *callback_data) :
	name(name),
	user_callback(callback),
	user_callback_data(callback_data),
	talking(false)
{
	std::string text = noo.load_text("text/speech/" + name + ".utf8");

	int offset = 0;
	bool done = false;

	while (done == false) {
		while (text.c_str()[offset] != 0 && isspace(text.c_str()[offset])) {
			offset++;
		}
		if (text.c_str()[offset] == 0) {
			done = true;
		}

		int nlpos = text.find('\n', offset);
		int crpos = text.find('\r', offset);
		std::string line;

		if (nlpos == std::string::npos && crpos == std::string::npos) {
			done = true;
			line = text.substr(offset);
		}
		else {
			int less = MIN(nlpos, crpos);
			line = text.substr(offset, less-offset);
		}

		int pipepos = line.find('|');

		if (pipepos == std::string::npos) {
			printf("Malformed character speech file: %s.utf8\n", name.c_str());
		}
		else {
			std::string milestone_s = line.substr(0, pipepos);
			std::string speech = line.substr(pipepos+1);
			Talk *t = new Talk;
			if (milestone_s == "") {
				t->milestone = -1;
			}
			else {
				t->milestone = noo.milestone_name_to_number(milestone_s);
			}
			t->text = speech;
			sayings.push_back(t);
		}

		offset = MAX(nlpos, crpos) + 1;

		if (text.c_str()[offset] == 0) {
			done = true;
		}
	}

	std::sort(sayings.begin(), sayings.end(), &Talk_Brain::compare_milestones);
}

Talk_Brain::~Talk_Brain()
{
	for (size_t i = 0; i < sayings.size(); i++) {
		delete sayings[i];
	}
}

void Talk_Brain::activate(Map_Entity *activator)
{
	std::string text = get_speech(activator, map_entity);
	if (text != "") {
		talking = true;
		noo.map->add_speech(text, callback, &callback_data);
	}
}

bool Talk_Brain::compare_milestones(Talk *a, Talk *b)
{
	return a->milestone >= b->milestone;
}

std::string Talk_Brain::get_speech(Map_Entity *activator, Map_Entity *activated)
{
	for (size_t i = 0; i < sayings.size(); i++) {
		Talk *t = sayings[i];
		if (t->milestone < 0 || noo.check_milestone(t->milestone)) {
			callback_data.direction = activated->get_direction();
			callback_data.entity = activated;
			callback_data.user_callback = user_callback;
			callback_data.user_callback_data = user_callback_data;
			Point<int> offset = activator->get_position() - activated->get_position();
			if (offset.x < 0) {
				activated->set_direction(W);
			}
			else if (offset.x > 0) {
				activated->set_direction(E);
			}
			else if (offset.y < 0) {
				activated->set_direction(N);
			}
			else {
				activated->set_direction(S);
			}
			Point<float> entity_pos = (activated->get_position() + activated->get_offset()) * noo.tile_size + noo.map->get_offset();
			int pipe = t->text.find('|');
			if (pipe != std::string::npos) {
				std::string options = t->text.substr(0, pipe);
				Tokenizer tokenizer(options, ',');
				std::string s;
				bool x_set = false;
				bool y_set = false;
				while ((s = tokenizer.next()) != "") {
					if (s == "top" || s == "bottom") {
						y_set = true;
					}
					else if (s == "left" || s == "right") {
						x_set = true;
					}
				}
				if (x_set == false && entity_pos.x <= noo.screen_size.w/2) {
					t->text = std::string("right,") + t->text;
				}
				if (y_set == false && entity_pos.y > noo.screen_size.h/2) {
					t->text = std::string("top,") + t->text;
				}
			}
			return t->text;
		}
	}

	return "";
}

bool Talk_Brain::save(std::string &out)
{
	out += string_printf("brain=talk_brain,1\n%s\n", name.c_str());
	return true;
}

//--

Animated_Brain::Animated_Brain(std::string name) :
	Talk_Brain(name)
{
}

void Animated_Brain::update()
{
	if (map_entity == 0) {
		return;
	}

	Sprite *sprite = map_entity->get_sprite();

	if (!sprite->is_started()) {
		sprite->start();
	}

	if (name == "laughing_man") {
		if (((Animated_Brain *)map_entity->get_brain())->talking == false) {
			Uint32 ticks = SDL_GetTicks() % 5000;
			if (ticks < 600) {
				if (sprite->get_animation() != "laugh") {
					sprite->reset();
				}
				sprite->set_animation("laugh");
			}
			else {
				sprite->set_animation("stand_s");
			}
		}
	}
}

bool Animated_Brain::save(std::string &out)
{
	out += string_printf("brain=animated_brain,1\n%s\n", name.c_str());
	return true;
}

//--

void Talk_Then_Animate_Brain::animation_callback(void *data)
{
	Map_Entity *entity = (Map_Entity *)data;
	Talk_Then_Animate_Brain *brain = dynamic_cast<Talk_Then_Animate_Brain *>(entity->get_brain());
	Sprite *sprite = entity->get_sprite();
	std::string name = entity->get_name();

	brain->animating = false;
	sprite->set_animation(sprite->get_previous_animation());

	if (name == "drinker") {
		Map_Entity *bottle = noo.map->find_entity("drinker_bottle");
		if (bottle) {
			Sprite *sprite = bottle->get_sprite();
			sprite->set_animation(sprite->get_previous_animation());
		}
	}
}

void Talk_Then_Animate_Brain::speech_callback(void *data)
{
	Talk_Brain::Callback_Data *d = (Talk_Brain::Callback_Data *)data;
	Map_Entity *entity = d->entity;
	Talk_Then_Animate_Brain *brain = dynamic_cast<Talk_Then_Animate_Brain *>(entity->get_brain());
	Sprite *sprite = entity->get_sprite();
	std::string name = entity->get_name();

	if (name == "drinker") {
		Map_Entity *bottle = noo.map->find_entity("drinker_bottle");
		if (bottle) {
			brain->animating = true;
			sprite->set_animation("drinking", animation_callback, (void *)entity);
			sprite->reset();
			sprite = bottle->get_sprite();
			sprite->set_animation("drinking");
			sprite->reset();
		}
	}
}

Talk_Then_Animate_Brain::Talk_Then_Animate_Brain(std::string name) :
	Talk_Brain(name, speech_callback),
	animating(false)
{
}

void Talk_Then_Animate_Brain::activate(Map_Entity *activator)
{
	if (!animating) {
		Talk_Brain::activate(activator);
	}
}

bool Talk_Then_Animate_Brain::save(std::string &out)
{
	out += string_printf("brain=talk_then_animate_brain,1\n%s\n", name.c_str());
	return true;
}

//--

MML *Door_Brain::mml;

bool Door_Brain::start()
{
	mml = new MML("door.mml");
	return true;
}

void Door_Brain::end()
{
	delete mml;
}

Door_Brain::Door_Brain(bool open) :
	open(open)
{
}

void Door_Brain::activate(Map_Entity *activator)
{
	if (activator == noo.player) {
		if (map_entity->entity_collides(activator) == false) {
			mml->play(false);

			open = !open;

			if (open) {
				do_open();
			}
			else {
				do_close();
			}
		}
	}
}

void Door_Brain::collide(Map_Entity *collider)
{
	if (collider == noo.player && open == false) {
		mml->play(false);

		open = true;

		do_open();
	}
}

void Door_Brain::set_map_entity(Map_Entity *entity)
{
	Brain::set_map_entity(entity);

	if (open) {
		do_open();
	}
	else {
		do_close();
	}
}

bool Door_Brain::save(std::string &out)
{
	out += string_printf("brain=door_brain,1\n%s\n", open ? "open" : "");
	return true;
}

void Door_Brain::do_open()
{
	map_entity->get_sprite()->set_animation("open");
}

void Door_Brain::do_close()
{
	map_entity->get_sprite()->set_animation("closed");
}

//--

Item_Brain::Item_Brain(std::string item_name, int quantity, int milestone) :
	item_name(item_name),
	quantity(quantity),
	milestone(milestone)
{
}

void Item_Brain::activate(Map_Entity *activator)
{
	Stats *stats = activator->get_stats();

	if (stats) {
		Inventory *inventory = stats->inventory;
		if (inventory) {
			if (quantity > 0) {
				std::string name;
				for (int i = 0; i < quantity; i++) {
					Item *item = new Item(item_name);
					inventory->add(item);
					if (i == 0) {
						name = item->name;
					}
				}
				noo.item_mml->play(false);
				// FIXME: a/an and adding s isn't foolproof
				std::string found = TRANSLATE("Found")END;
				if (quantity > 1) {
					noo.map->add_speech(found + " " + itos(quantity) + " " + name + "s");
				}
				else {
					char c = tolower(name.c_str()[0]);
					if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
						noo.map->add_speech(found + " " + TRANSLATE("an")END + " " + name + "...");
					}
					else {
						noo.map->add_speech(found + " " + TRANSLATE("a") + " " + name + "...");
					}
				}

				if (milestone >= 0) {
					noo.set_milestone(milestone, true);
				}
			}
			noo.map->schedule_destroy(map_entity);
		}
	}
}

bool Item_Brain::save(std::string &out)
{
	out += string_printf("brain=item_brain,1\nname=%s,quantity=%d,milestone=%d\n", item_name.c_str(), quantity, milestone);
	return true;
}

//--

void Item_Drop_Brain::callback(void *data)
{
	Item_Drop_Brain *brain = (Item_Drop_Brain *)data;

	if (brain->get_inventory()->items.size() == 0) {
		noo.map->schedule_destroy(brain->get_map_entity());
	}
}

Item_Drop_Brain::Item_Drop_Brain(Inventory *inventory) :
	inventory(inventory)
{
}

Item_Drop_Brain::~Item_Drop_Brain()
{
	delete inventory;
}

void Item_Drop_Brain::activate(Map_Entity *activator)
{
	std::vector<int> unused;
	Buy_Sell_GUI *gui = new Buy_Sell_GUI(inventory, unused, true, callback, this);
	gui->start();
	noo.guis.push_back(gui);
}

bool Item_Drop_Brain::save(std::string &out)
{
	std::string inventory_s = inventory->to_string();
	out += "brain=item_drop," + itos(std::count(inventory_s.begin(), inventory_s.end(), '\n')+1) + "\n" + inventory->to_string() + "\n";
	return true;
}

Inventory *Item_Drop_Brain::get_inventory()
{
	return inventory;
}

//--

void Shop_Brain::callback(void *data)
{
	Multiple_Choice_GUI::Callback_Data *d = (Multiple_Choice_GUI::Callback_Data *)data;

	int choice = d->choice;

	if (choice == 0) {
		Shop_Brain *brain = (Shop_Brain *)d->userdata;

		Inventory *inventory = brain->get_inventory();
		std::vector<int> &costs = brain->get_costs();

		Buy_Sell_GUI *gui = new Buy_Sell_GUI(inventory, costs, false);
		gui->start();
		noo.guis.push_back(gui);
	}
}

Shop_Brain::Shop_Brain(std::string caption, std::string yes_option, std::string no_option, Inventory *inventory, std::vector<int> costs) :
	caption(caption),
	yes_option(yes_option),
	no_option(no_option),
	inventory(inventory),
	costs(costs)
{
}

Shop_Brain::~Shop_Brain()
{
	delete inventory;
}

void Shop_Brain::activate(Map_Entity *activator)
{
	std::vector<std::string> choices;
	choices.push_back(yes_option);
	choices.push_back(no_option);
	Multiple_Choice_GUI *gui = new Multiple_Choice_GUI(caption, choices, callback, this);
	gui->start();
	noo.guis.push_back(gui);
}

bool Shop_Brain::save(std::string &out)
{
	std::string inventory_s = inventory->to_string();
	int count = costs.size();
	out += string_printf(
		"brain=shop,%d\n%s,%s,%s,%d,",
		1 + std::count(inventory_s.begin(), inventory_s.end(), '\n'),
		escape_string(caption, ',').c_str(),
		escape_string(yes_option, ',').c_str(),
		escape_string(no_option, ',').c_str(),
		count
	);
	for (int i = 0; i < count; i++) {
		out += string_printf("%d%s", costs[i], i < count-1 ? "," : "");
	}
	out += "\n";
	out += inventory_s;
	return true;
}

Inventory *Shop_Brain::get_inventory()
{
	return inventory;
}

std::vector<int> &Shop_Brain::get_costs()
{
	return costs;
}
