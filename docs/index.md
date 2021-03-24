# Introduction

Dialog systems are a gameplay mechanic commonly used in adventure and RP games to allow the player to interact with NPCs and make choices that will affect their experience. Video game genres such as visual novels and dating sims revolve almost entirely around these mechanics.

The most common type of dialog system is the dialog tree, consisting of branches that represent choices made by the player and nodes representing NPC lines. The tree is a deterministic system which needs to be completely designed from the get-go. Other systems often include procedural generation tools to interpret intent and meaning from the player and to produce coherent responses from the computer.

### History

Dialog trees have existed for far longer than video games have. The earliest known dialog tree was presented in [The Garden of Forking Paths](https://en.wikipedia.org/wiki/The_Garden_of_Forking_Paths) by Jorge Luis Borges in 1941, in the form of a short story that allowed the reader to branch their decisions into their respective chapters. Since then, the concept has been transported into the video game world with computers offering more efficient ways to manage large dialog trees.



# Market study

A wide range of games are using dialog systems as part of the gameplay experience. In this list, I will include some good examples that use dialog trees as well as some examples that use procedural models.

### Dialog Trees

| Name             | Description                                                  | Image                                                        |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Witcher 3        | A dialog system with branching choices, voice lines and game state consequences. | <img src="https://github.com/OCA99/ResearchProject-Dialog/blob/master/docs/images/witcher.jpg?raw=true" style="zoom:100%;" /> |
| Persona 5        | A dialog system with branching choices, game state consequences and game context (character mood, time of day...). | <img src="https://github.com/OCA99/ResearchProject-Dialog/blob/master/docs/images/persona.jpg?raw=true" style="zoom:100%;" /> |
| The Walking Dead | A fully voiced visual novel in which the gameplay consists in dialog and its consequences. | <img src="https://github.com/OCA99/ResearchProject-Dialog/blob/master/docs/images/twd.jpg?raw=true" style="zoom:100%;" /> |

### Procedural models

| Name      | Description                                                  | Link                           |
| --------- | ------------------------------------------------------------ | ------------------------------ |
| Cleverbot | A simple chatbot that will follow your conversation.         | [](https://www.cleverbot.com/) |
| AIDungeon | A GPT-2 based story telling platform which builds a story around your actions. | [](https://play.aidungeon.io/) |



# Selected approach

The objective of my implementation will be to provide a simple way of creating dialog trees which will be loaded into the game on runtime and can be triggered at any time. My system will also include a way to run callback functions on certain lines of dialog in order to modify the game state according to the player's decisions.

The selected approach presents the following challenges:

- What data model should be used to store dialogs in files?
- What data model should be used to store dialogs on runtime?
- How can branching be made easy to use?
- How can we define game state modifications in the dialog file?
- How can we offer a way to jump to another dialog line at any time?

### Data model

The approach will be to use a symmetrical data model for the files and the runtime so transitioning from one to the other is as easy as possible. Since the data structure is a tree, a ```DialogNode``` struct will be defined to represent the tree nodes. The implementation will be roughly as follows:

````c++
struct DialogNode {
	std::vector<DialogNode*>* children;

	DialogNode* prev = nullptr;
	DialogNode* next = nullptr;
	DialogNode* parent = nullptr;
};
````

As you can see, each node contains references to its children, siblings and parent nodes. This will be used in order to navigate the tree easily.

On the file system side, the XML format will be used to store dialog data. Since XML is a tree based format, the conversion between the two systems will be straightforward. In XML, 5 different types of node will be defined according to their behavior in-game:

- ````dialog````: This node encapsulates a whole dialog tree and serves as document root.
  - ````id````: This attribute will be used as unique identifier of the dialog tree within the game system.
- ````options````: This node will be used to define a fork in the dialog tree. It will contain various ```option``` nodes that will define each possible response by the player.
  - ````speaker````: This attribute will be used to define the character that says the line. It can be used to show an appropriate image, change the color of the text...
  - ````value````: This attribute contains the actual text that will be shown to the player for this node.
  - ````id````: This attribute is a unique identifier for this line in order to be able to go back to it using ``goto``.
  - ````callback````: This attribute is a unique identifier referencing a function to be run when this line is reached.
- ````option````: This node represents a possible choice a player can make within an ````options```` node. It will contain more nodes within it that will define how the conversation will continue when following this branch.
  - ````value````: This attribute contains the text that will be shown to the player for the option.
- ````line````: This node will be used to show a line of text to the player. It cannot contain any nodes since it doesn't define a decision.
  - ```speaker```: See above.
  - ```value```: See above.
  - ````id````: This attribute is a unique identifier for this line in order to be able to go back to it using ``goto``.
  - ````callback````: See above.
- ```goto```: This node will be used to jump to another dialog line by using an ``id`` defined on any other node.
  - ````route````: This attribute contains the ``id`` of the node to jump to.

The type of each node will be stored in the ````DialogNode```` struct in order to be able to execute different behaviors for each of them. The attributes defined above will be stored in a dictionary.

![](https://github.com/OCA99/ResearchProject-Dialog/blob/master/docs/images/tree.png?raw=true)

### Traversing the tree

Once the tree is defined and loaded from file it's a simple case of running a depth first search in order to show the lines in the correct order. The only special consideration to take into account is that only one of the children of ````options```` nodes should be explored, depending on the user's selection. To aid us in this process, each node will have a ````Next()```` function which will give us the next node we should explore from there.

In order to centralize the processing of dialog trees, we will create a Module that will have the following responsibilities:

- Storing all loaded dialog trees.
- Rendering the current dialog line, if there is one.
- Processing user keystrokes in order to show the next line or choose the desired option.
- Storing callback functions and executing them when needed.



# Possible Improvements

Although the current system is quite capable, there are still some obvious improvements that could be done:

- The callback system is rudimentary. It doesn't allow us to use class functions or pass parameters to those functions.
- The system does not use context in order to guide the conversation. It would be interesting if the game state could affect what options and responses are shown to the player.
- The system does not offer a method to use variables within the text. If players were able to set their name dynamically, there would be no way to update its occurrences in text.

# Documentation

[0]: https://en.wikipedia.org/wiki/Dialogue_tree	"Dialogue Tree"
[1]: https://www.reddit.com/r/truegaming/comments/78vfsa/dialogue_systems_in_games/	"Dialogue systems in games"
[2]: https://www.youtube.com/watch?v=0hMiPBe_VRc	"Dialogue Systems in Double Fine Games"

