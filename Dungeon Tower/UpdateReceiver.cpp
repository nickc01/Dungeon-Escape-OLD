#include "UpdateReceiver.h"

UpdateReceiver::UpdateReceiver(bool enabled) :
	ObjectManager<UpdateReceiver>(enabled)
{

}

std::vector<UpdateReceiver*>& UpdateReceiver::GetUpdatables()
{
	return ObjectManager<UpdateReceiver>::GetEventList();
}
