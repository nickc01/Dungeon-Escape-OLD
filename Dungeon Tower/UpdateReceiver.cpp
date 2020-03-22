#include "UpdateReceiver.h" //Contains the UpdateReceiver class for receiving update events

//Constructs a new update receiver
UpdateReceiver::UpdateReceiver(bool enabled) :
	ObjectManager<UpdateReceiver>(enabled)
{

}

//Whether to enable updating or not
void UpdateReceiver::EnableUpdating(bool enabled)
{
	SetActive(enabled);
}

//Gets the list of all updatable objects
std::vector<UpdateReceiver*>& UpdateReceiver::GetUpdatables()
{
	return ObjectManager<UpdateReceiver>::GetObjectList();
}
