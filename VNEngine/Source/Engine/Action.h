#ifndef ACTION_H
#define ACTION_H

#include <Core/Variant.h>

#include <SFML/Window.hpp>

#include <functional>
#include <vector>

namespace vne
{

class Scene;

// ============================================================================

class Action
{
public:
	Action();
	virtual ~Action();

	/// <summary>
	/// Execute the action if the condition is met
	/// </summary>
	virtual void run() = 0;

	/// <summary>
	/// Update any actions that occur over time
	/// </summary>
	/// <param name="dt">Elapsed time</param>
	virtual void update(float dt);

	/// <summary>
	/// Handle user input
	/// </summary>
	/// <param name="e">SFML event</param>
	virtual void handleEvent(const sf::Event& e);

	/// <summary>
	/// Set the scene this action should modify
	/// </summary>
	/// <param name="scene">Scene</param>
	void setScene(Scene* scene);

	/// <summary>
	/// Set the action's run condition.
	/// This action will only execute if the given condition evaluates to true
	/// </summary>
	/// <param name="cond">Function or lambda</param>
	void setCondition(const std::function<bool()>& cond);

	/// <summary>
	/// Returns true if action has been completed
	/// </summary>
	/// <returns>State of completion</returns>
	bool isComplete() const;

protected:
	/// <summary>
	/// Shorthand for checking if run conditions are met
	/// </summary>
	/// <returns>Condition met</returns>
	bool conditionMet() const;

protected:
	/// <summary>
	/// The scene the action should modify
	/// </summary>
	Scene* mScene;

	/// <summary>
	/// The action's run condition
	/// </summary>
	std::function<bool()> mCondition;

	/// <summary>
	/// True if action has finished running
	/// </summary>
	bool mIsComplete;
};

// ============================================================================

class ActionGroup : public Action
{
public:
	ActionGroup();
	~ActionGroup();

	/// <summary>
	/// Run all children actions
	/// </summary>
	void run() override;

	/// <summary>
	/// Add an action as a child of this group
	/// </summary>
	/// <param name="action">Pointer to action</param>
	void addAction(Action* action);

private:
	/// <summary>
	/// Children action
	/// </summary>
	std::vector<Action*> mActions;
};

// ============================================================================

class DialogueAction : public Action
{
public:
	DialogueAction();
	~DialogueAction();

	/// <summary>
	/// Start dialogue
	/// </summary>
	void run() override;

	/// <summary>
	/// Update displayed text
	/// </summary>
	/// <param name="dt">Elapsed time</param>
	void update(float dt) override;

	/// <summary>
	/// Handle user input
	/// </summary>
	/// <param name="e">SFML event</param>
	virtual void handleEvent(const sf::Event& e) override;

	/// <summary>
	/// Set the speaker name.
	/// If an empty string is provided, the name box is hidden,
	/// and quotation symbols aren't used
	/// </summary>
	/// <param name="name">Speaker name</param>
	void setName(const sf::String& name);

	/// <summary>
	/// Set the dialogue string.
	/// This string will automatically be surrounded by quotation symbols,
	/// unless an empty name string is set.
	/// </summary>
	/// <param name="dialogue">Dialogue</param>
	void setDialogue(const sf::String& dialogue);

private:
	/// <summary>
	/// Name of the speaker
	/// </summary>
	sf::String mName;

	/// <summary>
	/// Dialogue the speaker is saying
	/// </summary>
	sf::String mDialogue;

	Uint32 mCurrentLine;
};

// ============================================================================

}

#endif