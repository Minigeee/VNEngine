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

class ImageBox;

// ============================================================================

/// <summary>
/// Transition effects
/// </summary>
enum class Transition
{
	/// <summary>
	/// This effect works with images and audio.
	/// No transition effect. Instantly switches
	/// </summary>
	None,

	/// <summary>
	/// This effect works with images and audio.
	/// Fade between the previous and the new (smooth transition)
	/// For fading images, the alpha channel is changed.
	/// For fading audio, the volume is changed.
	/// </summary>
	Fade,

	/// <summary>
	/// This effect works with image transitions.
	/// This will always fade the image to the color black,
	/// and it will ignore any other specified image
	/// (Meaning that the final background image will be black)
	/// </summary>
	FadeToBlack,

	/// <summary>
	/// This effect works with image transitions.
	/// This will always fade the image from the color black
	/// </summary>
	FadeFromBlack
};

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
	/// Set the completion status of action
	/// </summary>
	/// <param name="complete">Completion status</param>
	void setComplete(bool complete);

	/// <summary>
	/// Returns true if action has been completed
	/// </summary>
	/// <returns>State of completion</returns>
	bool isComplete() const;

	/// <summary>
	/// Check if run conditions are met
	/// </summary>
	/// <returns>Condition met</returns>
	bool isConditionMet() const;

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
	/// Update actions inside the group
	/// </summary>
	/// <param name="dt">Elapsed time</param>
	void update(float dt) override;

	/// <summary>
	/// Pass events to actions inside group
	/// </summary>
	/// <param name="e">Event</param>
	void handleEvent(const sf::Event& e) override;

	/// <summary>
	/// Add an action as a child of this group
	/// </summary>
	/// <param name="action">Pointer to action</param>
	void addAction(Action* action);

	/// <summary>
	/// Set the parallel execution option for the action group.
	/// If parallel is set to true, all actions inside the group will run without waiting for
	/// the previous action to complete (they will all run in parallel).
	/// If parallel is set to false, all actions will run sequentially
	/// </summary>
	/// <param name="parallel">Parallel option</param>
	void setParallel(bool parallel);

private:
	/// <summary>
	/// Children action
	/// </summary>
	std::vector<Action*> mActions;

	/// <summary>
	/// Index of the current action in execution
	/// </summary>
	int mActionIndex;

	/// <summary>
	/// Should the actions run in parallel
	/// </summary>
	bool mIsParallel;
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
	void setTransition(Transition effect);

	/// <summary>
	/// Set the duration of the transition effect in seconds
	/// </summary>
	/// <param name="duration">Duration of the transition</param>
	void setDuration(float duration);

private:
	/// <summary>
	/// Called when animation completes
	/// </summary>
	void onAnimComplete();

private:
	/// <summary>
	/// Background texture
	/// </summary>
	sf::Texture* mTexture;

	/// <summary>
	/// The transition effect
	/// </summary>
	Transition mTransition;

	/// <summary>
	/// Duration of transition
	/// </summary>
	float mDuration;
};

// ============================================================================

class ImageAction : public Action
{
public:
	enum Mode
	{
		/// <summary>
		/// Show image
		/// </summary>
		Show,

		/// <summary>
		/// Hide image
		/// </summary>
		Hide
	};

public:
	ImageAction();
	~ImageAction();

	/// <summary>
	/// Show or hide an image
	/// </summary>
	void run() override;

	/// <summary>
	/// Set image action mode (either show or hide image).
	/// If the mode is "Hide" and the image is already hidden, nothing happens.
	/// If the mode is "Show" and the image is already shown, the new texture is applied to the image box,
	/// and the transition effect is used
	/// </summary>
	/// <param name="mode"></param>
	void setMode(Mode mode);

	/// <summary>
	/// Set the new texture to apply to the image box
	/// </summary>
	/// <param name="texture">New texture</param>
	void setTexture(sf::Texture* texture);

	/// <summary>
	/// Set the transition effect
	/// </summary>
	/// <param name="effect"></param>
	void setTransition(Transition effect);

	/// <summary>
	/// Set the duration of the transition effect in seconds
	/// </summary>
	/// <param name="duration">Duration of the transition</param>
	void setDuration(float duration);

	/// <summary>
	/// Set the image box to apply the texture to
	/// </summary>
	/// <param name="image">Image box element</param>
	void setImageBox(ImageBox* image); 

private:
	/// <summary>
	/// Show image mode
	/// </summary>
	void show();

	/// <summary>
	/// Hide image mode
	/// </summary>
	void hide();

	/// <summary>
	/// Called when an animation completes
	/// </summary>
	void onAnimComplete();

private:
	/// <summary>
	/// Show / hide image
	/// </summary>
	Mode mMode;

	/// <summary>
	/// Texture to switch for
	/// </summary>
	sf::Texture* mTexture;

	/// <summary>
	/// Transition effect to use when switching
	/// </summary>
	Transition mTransition;

	/// <summary>
	/// Duration of transition
	/// </summary>
	float mDuration;

	/// <summary>
	/// Image box to apply texture to
	/// </summary>
	ImageBox* mImageBox;
};

// ============================================================================

}

#endif