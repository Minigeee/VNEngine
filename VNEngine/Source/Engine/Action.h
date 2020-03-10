#ifndef ACTION_H
#define ACTION_H

#include <Core/Variant.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

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
	/// If an empty string is provided, the name box is hidden
	/// </summary>
	/// <param name="name">Speaker name</param>
	void setName(const sf::String& name);

	/// <summary>
	/// Set the dialogue string.
	/// </summary>
	/// <param name="dialogue">Dialogue</param>
	void setDialogue(const sf::String& dialogue);

	/// <summary>
	/// Set the speed the text is displayed / revealed,
	/// in coordinate space units per second
	/// </summary>
	/// <param name="speed">Speed of text reveal</param>
	void setTextSpeed(float speed);

	/// <summary>
	/// Set the style of the text to use when this dialogue action is run (i.e. italics, bold, strikethrough)
	/// </summary>
	/// <param name="style">SFML text style flags</param>
	void setTextStyle(Uint32 style);

private:
	/// <summary>
	/// Name of the speaker
	/// </summary>
	sf::String mName;

	/// <summary>
	/// Dialogue the speaker is saying
	/// </summary>
	sf::String mDialogue;

	/// <summary>
	/// Keeps track of current line that is being printed
	/// </summary>
	Uint32 mCurrentLine;

	/// <summary>
	/// The speed the text is displayed
	/// </summary>
	float mTextSpeed;

	/// <summary>
	/// Text style
	/// </summary>
	Uint32 mStyle;
};

// ============================================================================

/// <summary>
/// This action changes the background image
/// (It does not start an action in the background, or on a seperate thread)
/// </summary>
class BackgroundAction : public Action
{
public:
	enum Effect
	{
		/// <summary>
		/// No transition effect. Instantly switches to the new background
		/// </summary>
		None,

		/// <summary>
		/// Fade between the previous background and the new one (Smooth transition)
		/// </summary>
		Fade
	};

public:
	BackgroundAction();
	~BackgroundAction();

	/// <summary>
	/// Show image
	/// </summary>
	void run() override;

	/// <summary>
	/// Set the background texture. If NULL is passed as a value, the background will be hidden
	/// </summary>
	/// <param name="texture"></param>
	void setTexture(sf::Texture* texture);

	/// <summary>
	/// Set the background transition effect
	/// </summary>
	/// <param name="effect">Transition effect</param>
	void setTransition(Effect effect);

private:
	/// <summary>
	/// Background texture
	/// </summary>
	sf::Texture* mTexture;

	Effect mEffect;
};

// ============================================================================

}

#endif