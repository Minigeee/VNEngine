#ifndef ACTION_H
#define ACTION_H

#include <Core/Variant.h>

#include <functional>
#include <vector>

namespace vne
{

// ============================================================================

class Action
{
public:
	Action();
	virtual ~Action();

	/// <summary>
	/// Set the action's run condition.
	/// This action will only execute if the given condition evaluates to true
	/// </summary>
	/// <param name="cond">Function or lambda</param>
	void setCondition(const std::function<bool()>& cond);

	/// <summary>
	/// Execute the action if the condition is met
	/// </summary>
	virtual void run() = 0;

protected:
	/// <summary>
	/// Shorthand for checking if run conditions are met
	/// </summary>
	/// <returns>Condition met</returns>
	bool conditionMet() const;

protected:
	/// <summary>
	/// The action's run condition
	/// </summary>
	std::function<bool()> mCondition;
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

}

#endif