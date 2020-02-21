#include <Engine/Action.h>

using namespace vne;

// ============================================================================
// ============================================================================

Action::Action()
{

}

Action::~Action()
{

}

// ============================================================================

void Action::setCondition(const std::function<bool()>& cond)
{
	mCondition = cond;
}

bool Action::conditionMet() const
{
	return !mCondition || (mCondition && mCondition());
}

// ============================================================================
// ============================================================================

ActionGroup::ActionGroup()
{

}

ActionGroup::~ActionGroup()
{

}

// ============================================================================

void ActionGroup::run()
{
	if (!conditionMet()) return;

	// Run all actions
	for (Uint32 i = 0; i < mActions.size(); ++i)
		mActions[i]->run();
}

// ============================================================================

void ActionGroup::addAction(Action* action)
{
	mActions.push_back(action);
}

// ============================================================================
// ============================================================================