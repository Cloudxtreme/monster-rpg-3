#include <Nooskewl_Engine/Nooskewl_Engine.h>

#include "monster-rpg-3.h"
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
	Speech::Callback_Data *scbd = (Speech::Callback_Data *)data;

	Callback_Data *d = (Callback_Data *)scbd->userdata;

	if (d->entity->should_face_activator()) {
		d->entity->set_direction(d->direction);
	}

	if (d->user_callback) {
		d->user_callback(d);
	}

	((Talk_Brain *)d->entity->get_brain())->talking = false;
}

Talk_Brain::Talk_Brain(std::string name, Callback callback, void *callback_data) :
	name(name),
	user_callback(callback),
	user_callback_data(callback_data),
	talking(false)
{
	if (name == "") {
		return;
	}

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
			std::string conditions_s = line.substr(0, pipepos);

			bool all_milestones = false;
			std::string milestone_s;
			bool has_status = false;
			Stats::Status status;

			Tokenizer tok(conditions_s, ',');

			std::string option;

			while ((option = tok.next()) != "") {
				if (option.substr(0, 7) == "status=") {
					std::string s = option.substr(7);
					if (s == "SICK") {
						has_status = true;
						status = Stats::SICK;
					}
					else if (s == "DRUNK") {
						has_status = true;
						status = Stats::DRUNK;
					}
				}
				else {
					if (option == "all") {
						all_milestones = true;
					}
					else {
						milestone_s = option;
					}
				}
			}

			std::string speech = line.substr(pipepos+1);
			Talk *t = new Talk;
			t->all_milestones = all_milestones;
			if (milestone_s == "") {
				t->milestone = -1;
			}
			else {
				t->milestone = noo.milestone_name_to_number(milestone_s);
			}
			t->has_status = has_status;
			t->status = status;
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

bool Talk_Brain::activate(Map_Entity *activator)
{
	std::string text = get_speech(activator, map_entity);
	if (text != "") {
		talking = true;
		noo.map->add_speech(text, callback, &callback_data);
		return true;
	}
	return false;
}

bool Talk_Brain::compare_milestones(Talk *a, Talk *b)
{
	if (a->all_milestones || a->milestone == b->milestone) {
		return a->has_status ? true : false;
	}
	return a->milestone >= b->milestone;
}

std::string Talk_Brain::get_speech(Map_Entity *activator, Map_Entity *activated)
{
	for (size_t i = 0; i < sayings.size(); i++) {
		Talk *t = sayings[i];
		if ((t->milestone < 0 || noo.check_milestone(t->milestone)) && (t->has_status == false || t->status == activator->get_stats()->status)){
			callback_data.direction = activated->get_direction();
			callback_data.entity = activated;
			callback_data.user_callback = user_callback;
			callback_data.user_callback_data = user_callback_data;
			if (activated->should_face_activator()) {
				Direction dir = get_facing_direction(activator, activated);
				activated->set_direction(dir);
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

Sign_Brain::Sign_Brain(std::string name) :
	Talk_Brain(name, 0, 0)
{
}

Sign_Brain::~Sign_Brain()
{
}

bool Sign_Brain::activate(Map_Entity *activator)
{
	if (activator->get_direction() == N) {
		return Talk_Brain::activate(activator);
	}
	return false;
}

bool Sign_Brain::save(std::string &out)
{
	out += string_printf("brain=n_talk_brain,1\n%s\n", name.c_str());
	return true;
}


//--

Animated_Brain::Animated_Brain(std::string name) :
	Talk_Brain(name)
{
}

Animated_Brain::~Animated_Brain()
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

Talk_Then_Animate_Brain::~Talk_Then_Animate_Brain()
{
}

bool Talk_Then_Animate_Brain::activate(Map_Entity *activator)
{
	if (!animating) {
		Talk_Brain::activate(activator);
		return true;
	}
	return false;
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

bool Door_Brain::activate(Map_Entity *activator)
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
		return true;
	}
	return false;
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

bool Item_Brain::activate(Map_Entity *activator)
{
	if (give_item(activator, item_name, quantity, milestone)) {
		noo.map->schedule_destroy(map_entity);
		return true;
	}
	return false;
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
	else {
		brain->drop_time = noo.get_play_time();
	}
}

Item_Drop_Brain::Item_Drop_Brain(Inventory *inventory, int drop_time) :
	inventory(inventory),
	drop_time(drop_time)
{
	int now = noo.get_play_time();
	int diff = now - drop_time;
	int items_to_remove = diff / (10 * 60); // lose one item every 10 minutes

	int count = 0;

	for (size_t i = 0; i < inventory->items.size(); i++) {
		count += inventory->items[i].size();
	}

	if (count <= items_to_remove) {
		die = true;
	}
	else {
		die = false;
		for (int i = 0; i < items_to_remove; i++) {
			int r = rand() % count;
			count--;

			for (size_t i = 0; i < inventory->items.size(); i++) {
				int size = inventory->items[i].size();
				if (r < size) {
					inventory->items[i].erase(inventory->items[i].begin());
					if (inventory->items[i].size() == 0) {
						inventory->items.erase(inventory->items.begin() + i);
					}
					break;
				}
				else {
					r -= size;
				}
			}
		}
	}
}

Item_Drop_Brain::~Item_Drop_Brain()
{
	delete inventory;
}

bool Item_Drop_Brain::activate(Map_Entity *activator)
{
	std::vector<int> unused;
	Buy_Sell_GUI *gui = new Buy_Sell_GUI(100, inventory, unused, true, callback, this);
	gui->start();
	noo.guis.push_back(gui);
	return true;
}

bool Item_Drop_Brain::save(std::string &out)
{
	std::string inventory_s = inventory->to_string();
	out += "brain=item_drop," + itos(std::count(inventory_s.begin(), inventory_s.end(), '\n')+1) + "\n" + itos(drop_time) + "\n" + inventory->to_string();
	return true;
}

bool Item_Drop_Brain::killme()
{
	return die;
}

Inventory *Item_Drop_Brain::get_inventory()
{
	return inventory;
}

//--

void Base_Shop_Brain::buy_sell_callback(void *data)
{
	Base_Shop_Brain *brain = (Base_Shop_Brain *)data;
	if (brain->direction_set) {
		brain->map_entity->set_direction(brain->original_direction);
	}
}

void Base_Shop_Brain::callback(void *data)
{
	Multiple_Choice_GUI::Callback_Data *d = (Multiple_Choice_GUI::Callback_Data *)data;
	Base_Shop_Brain *brain = (Base_Shop_Brain *)d->userdata;

	int choice = d->choice;

	if (choice == 0) {
		int multiplier = brain->get_multiplier();
		Inventory *inventory = brain->map_entity->get_stats()->inventory;
		std::vector<int> &costs = brain->get_costs();

		Buy_Sell_GUI *gui = new Buy_Sell_GUI(multiplier, inventory, costs, false, buy_sell_callback, brain);
		gui->start();
		noo.guis.push_back(gui);
	}
	else if (brain->direction_set) {
		brain->map_entity->set_direction(brain->original_direction);
	}
}

Base_Shop_Brain::Base_Shop_Brain(std::string caption, std::string yes_option, std::string no_option, int multiplier, std::vector<int> costs, Inventory *original_inventory, std::vector<int> original_costs, int last_visit) :
	caption(caption),
	yes_option(yes_option),
	no_option(no_option),
	multiplier(multiplier),
	costs(costs),
	original_inventory(original_inventory),
	original_costs(original_costs),
	direction_set(false)
{
}

Base_Shop_Brain::~Base_Shop_Brain()
{
	delete original_inventory;
}

void Base_Shop_Brain::init()
{
	Inventory *inventory = map_entity->get_stats()->inventory;

	// All of this trends inventory back towards default

	Inventory *added_items = inventory->clone();
	Inventory *removed_items = original_inventory->clone();

	added_items->remove(original_inventory);

	removed_items->remove(inventory);
	removed_items->remove(added_items);

	int num_added = 0;
	for (size_t i = 0; i < added_items->items.size(); i++) {
		num_added += added_items->items[i].size();
	}

	int num_removed = 0;
	for (size_t i = 0; i < removed_items->items.size(); i++) {
		num_removed += removed_items->items[i].size();
	}

	int now = noo.get_play_time();
	int diff = now - last_visit;
	int num_to_change = diff / (2 * 60);

	int num_to_remove = MIN(num_to_change / 2, num_added);
	int num_to_add = MIN(num_to_change - num_to_remove, num_removed);

	for (int i = 0; i < num_to_remove; i++) {
		int r = rand() % num_added;
		std::string id;
		for (size_t ii = 0; ii < added_items->items.size(); ii++) {
			int size = added_items->items[ii].size();
			if (r < size) {
				id = added_items->items[ii][0]->id;
				break;
			}
			else {
				r -= size;
			}
		}
		if (id != "") {
			for (size_t ii = 0; ii < inventory->items.size(); ii++) {
				if (inventory->items[ii][0]->id == id) {
					delete inventory->items[ii][0];
					inventory->items[ii].erase(inventory->items[ii].begin());
					if (inventory->items[ii].size() == 0) {
						inventory->items.erase(inventory->items.begin() + ii);
						costs.erase(costs.begin() + ii);
					}
				}
			}
		}
	}

	for (int i = 0; i < num_to_add; i++) {
		int r = rand() % num_removed;
		std::string id;
		for (size_t ii = 0; ii < removed_items->items.size(); ii++) {
			int size = removed_items->items[ii].size();
			if (r < size) {
				id = removed_items->items[ii][0]->id;
				break;
			}
			else {
				r -= size;
			}
		}
		if (id != "") {
			Item *item = new Item(id);
			inventory->add(item);
			inventory->gold += item->get_value();
		}
	}

	delete added_items;
	delete removed_items;
}

bool Base_Shop_Brain::activate(Map_Entity *activator)
{
	original_direction = map_entity->get_direction();
	map_entity->set_direction(get_facing_direction(activator, map_entity));
	direction_set = true;

	std::vector<std::string> choices;
	choices.push_back(yes_option);
	choices.push_back(no_option);
	Multiple_Choice_GUI *gui = new Multiple_Choice_GUI(caption, choices, 1, callback, this);
	gui->start();
	noo.guis.push_back(gui);
	return true;
}

bool Base_Shop_Brain::save(std::string &out)
{
	real_save("shop", out);
	return true;
}

std::vector<int> &Base_Shop_Brain::get_costs()
{
	return costs;
}

void Base_Shop_Brain::real_save(std::string brain_name, std::string &out)
{
	std::string original_inventory_s = original_inventory->to_string();
	int count = costs.size();
	int original_count = original_costs.size();
	out += string_printf(
		"brain=%s,%d\n%s\n%s,%s,%s,%d,%d,%d,%d,%d,", brain_name.c_str(),
		2 + std::count(original_inventory_s.begin(), original_inventory_s.end(), '\n'),
		caught_pick_pocket_text.c_str(),
		escape_string(caption, ',').c_str(),
		escape_string(yes_option, ',').c_str(),
		escape_string(no_option, ',').c_str(),
		multiplier,
		count,
		original_count,
		noo.get_play_time(),
		can_pick_pocket
	);
	for (int i = 0; i < count; i++) {
		out += string_printf("%d,", costs[i]);
	}
	for (int i = 0; i < original_count; i++) {
		out += string_printf("%d%s", original_costs[i], i < count-1 ? "," : "");
	}
	out += "\n";
	out += original_inventory_s;
}

int Base_Shop_Brain::get_multiplier()
{
	return multiplier;
}

//--

Shop_Brain::Shop_Brain(std::string caption, std::string yes_option, std::string no_option, int multiplier, std::vector<int> costs, Inventory *original_inventory, std::vector<int> original_costs, int last_visit) :
	Base_Shop_Brain(caption, yes_option, no_option, multiplier, costs, original_inventory, original_costs, last_visit)
{
}

Shop_Brain::~Shop_Brain()
{
}

//--

void Growing_Brain::callback(void *data)
{
	Multiple_Choice_GUI::Callback_Data *d = static_cast<Multiple_Choice_GUI::Callback_Data *>(data);

	if (d->choice == 0) {
		Callback_Data *cbd = static_cast<Callback_Data *>(d->userdata);
		Stats *stats = cbd->activator->get_stats();
		DEC_KARMA(stats, cbd->brain->karma);
		cbd->brain->give(cbd->activator);
	}
}

Growing_Brain::Growing_Brain(std::string baby_item, std::string fresh_item, std::string rotten_item, int instantiation_time, int karma) :
	baby_item(baby_item),
	fresh_item(fresh_item),
	rotten_item(rotten_item),
	item_name(""),
	karma(karma)
{
	int now = noo.get_play_time();

	if (instantiation_time < 0 || now - instantiation_time > STAGE_TIME * 10) {
		this->instantiation_time = now - rand() % ((5 * 60) * 4);
	}
}

bool Growing_Brain::activate(Map_Entity *activator)
{
	if (item_name == "") {
		return false;
	}

	if (karma != 0) {
		callback_data.brain = this;
		callback_data.activator = activator;

		std::vector<std::string> choices;
		choices.push_back(noo.game_t->translate(148));
		choices.push_back(noo.game_t->translate(106));

		Multiple_Choice_GUI *gui = new Multiple_Choice_GUI(noo.game_t->translate(133), choices, 1, callback, &callback_data);
		gui->start();
		noo.guis.push_back(gui);

		return true;
	}
	else {
		return give(activator);
	}
}

bool Growing_Brain::save(std::string &out)
{
	out += string_printf("brain=growing_brain,1\nbaby_item=%s,fresh_item=%s,rotten_item=%s,instantiation_time=%d,karma=%d\n", baby_item.c_str(), fresh_item.c_str(), rotten_item.c_str(), instantiation_time, karma);
	return true;
}

void Growing_Brain::update()
{
	if (noo.map == 0) {
		return;
	}

	std::vector<Map_Entity *> v = noo.map->get_colliding_entities(-1, map_entity->get_position(), Size<int>(1, 1));

	for (size_t i = 0; i < v.size(); i++) {
		if (v[i] != map_entity) {
			instantiation_time = noo.get_play_time();
			return;
		}
	}

	int now = noo.get_play_time();
	int diff = now - instantiation_time;


	Sprite *sprite = map_entity->get_sprite();

	if (diff > 3 * STAGE_TIME) {
		item_name = rotten_item;
		sprite->set_animation("rotten");
		map_entity->set_solid(true);
	}
	else if (diff > 2 * STAGE_TIME) {
		item_name = fresh_item;
		sprite->set_animation("fresh");
		map_entity->set_solid(true);
	}
	else if (diff > STAGE_TIME) {
		item_name = baby_item;
		sprite->set_animation("baby");
		map_entity->set_solid(true);
	}
	else {
		item_name = "";
		sprite->set_animation("picked");
		map_entity->set_solid(false);
	}
}

int Growing_Brain::get_instantiation_time()
{
	return instantiation_time;
}

void Growing_Brain::set_instantiation_time(int instantiation_time)
{
	this->instantiation_time = instantiation_time;
}

bool Growing_Brain::give(Map_Entity *activator)
{
	if (give_item(activator, item_name, 1, -1)) {
		instantiation_time = noo.get_play_time();
		return true;
	}
	return false;
}

Wander_Brain::Wander_Brain(std::string name, int max_distance, int delay, Point<int> start_pos, Callback callback, void *callback_data) :
	Talk_Brain(name, callback, callback_data),
	max_distance(max_distance),
	delay(delay),
	start_pos(start_pos),
	count(delay)
{
}

Wander_Brain::~Wander_Brain()
{
}

void Wander_Brain::update()
{
	if (talking == false) {
		count++;
		if (count >= delay) {
			count = 0;
			Point<int> pos = map_entity->get_position();
			Point<int> diff = pos - start_pos;
			if (diff.x <= -max_distance) {
				r = true;
			}
			else if (diff.x >= max_distance) {
				l = true;
			}
			else if (diff.y <= -max_distance) {
				d = true;
			}
			else if (diff.y >= max_distance) {
				u = true;
			}
			else {
				int n = rand() % 4;
				if (n == 0) {
					l = true;
				}
				else if (n == 1) {
					r = true;
				}
				else if (n == 2) {
					u = true;
				}
				else {
					d = true;
				}
			}
		}
		else {
			l = r = u = d = false;
		}
	}
	else {
		l = r = u = d = false;
	}
}

bool Wander_Brain::save(std::string &out)
{
	out += string_printf("brain=wander_brain,1\nname=%s,max_distance=%d,delay=%d,start_pos=%d:%d\n", name.c_str(), max_distance, delay, start_pos.x, start_pos.y);
	return true;
}

//--

Base_No_Activate_Shop_Brain::Base_No_Activate_Shop_Brain(std::string caption, std::string yes_option, std::string no_option, int multiplier, std::vector<int> costs, Inventory *original_inventory, std::vector<int> original_costs, int last_visit) :
	Base_Shop_Brain(caption, yes_option, no_option, multiplier, costs, original_inventory, original_costs, last_visit)
{
}

Base_No_Activate_Shop_Brain::~Base_No_Activate_Shop_Brain()
{
}

void Base_No_Activate_Shop_Brain::init()
{
	Base_Shop_Brain::init();
}

bool Base_No_Activate_Shop_Brain::activate(Map_Entity *activator)
{
	return false;
}

bool Base_No_Activate_Shop_Brain::save(std::string &out)
{
	real_save("no_activate_shop", out);
	return true;
}

void Base_No_Activate_Shop_Brain::manual_activate()
{
	Buy_Sell_GUI *gui = new Buy_Sell_GUI(multiplier, map_entity->get_stats()->inventory, costs, false);
	gui->start();
	noo.guis.push_back(gui);
}

//--

No_Activate_Shop_Brain::No_Activate_Shop_Brain(std::string caption, std::string yes_option, std::string no_option, int multiplier, std::vector<int> costs, Inventory *original_inventory, std::vector<int> original_costs, int last_visit) :
	Base_No_Activate_Shop_Brain(caption, yes_option, no_option, multiplier, costs, original_inventory, original_costs, last_visit)
{
}

No_Activate_Shop_Brain::~No_Activate_Shop_Brain()
{
}

//--

NULL_Brain::NULL_Brain(std::string action) :
	action(action)
{
}

std::string NULL_Brain::get_action()
{
	return action;
}
