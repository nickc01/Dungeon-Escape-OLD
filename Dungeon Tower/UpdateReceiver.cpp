#include "UpdateReceiver.h"

UpdateReceiver::UpdateReceiver(bool enabled) :
	ObjectManager<UpdateReceiver>(enabled)
{

}

void UpdateReceiver::EnableUpdating(bool enabled)
{
	SetActive(enabled);
}

std::vector<UpdateReceiver*>& UpdateReceiver::GetUpdatables()
{
	return ObjectManager<UpdateReceiver>::GetEventList();
}
