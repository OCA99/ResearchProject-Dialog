#pragma once

#include <map>
#include <string>
#include <functional>

#include "Module.h"

class DialogNode;

class DialogSystem : public Module {
public:
	DialogSystem();

	// Destructor
	virtual ~DialogSystem();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	void DrawDialog();

	// Called before quitting.
	bool CleanUp();

	// LoadDialog from file.
	bool LoadDialog(const char* filename);

	// Start the dialog sequence for a dialog id.
	void StartDialog(const char* id);

	// Jump to the next dialog line.
	void NextDialog();

private:
	// Parse a dialog tree into nodes.
	DialogNode* ParseDialogXML(pugi::xml_node dialogRoot);

	// The current dialog line being shown to the player.
	DialogNode* currentDialog = nullptr;

	// The dictionary of loaded dialogues.
	std::map<std::string, DialogNode*> dialogues;

	// A dictionary that maps node ids to their respective nodes. This will be used to process "goto" nodes.
	std::map<std::string, DialogNode*> nodeRoutes;

	// A dictionary that maps function ids to their respective callback functions.
	std::map<std::string, std::function<void()>> callbacks;

	// The current selected option for "OPTIONS" nodes.
	int selectedOption = 0;

	// The folder that contains the dialogues.
	std::string folder;
	pugi::xml_document dialogFile;
};