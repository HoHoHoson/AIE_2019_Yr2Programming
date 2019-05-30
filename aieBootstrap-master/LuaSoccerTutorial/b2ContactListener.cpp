#include "b2ContactListener.h"
#include "b2UserData.h"
#include "LuaSoccerTutorialApp.h"

void MyContactListener::beginContact(b2Contact * contact)
{
	void* body_user_data = contact->GetFixtureA()->GetBody()->GetUserData();
	if (body_user_data)
	{
		b2UserData* data = static_cast<b2UserData*>(body_user_data);
		if (data->entity_type == BALL)
		{

		}
	}
}

void MyContactListener::endContact(b2Contact * contact)
{
}
