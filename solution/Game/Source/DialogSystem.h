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

	// Called before quitting
	bool CleanUp();

	bool LoadDialog(const char* filename);

	void StartDialog(const char* id);

	void NextDialog();

private:
	DialogNode* ParseDialogXML(pugi::xml_node dialogRoot);

	DialogNode* currentDialog = nullptr;

	std::map<std::string, DialogNode*> dialogues;

	std::map<std::string, DialogNode*> nodeRoutes;

	std::map<std::string, std::function<void()>> callbacks;

	int selectedOption = 0;

	std::string folder;
	pugi::xml_document dialogFile;
};