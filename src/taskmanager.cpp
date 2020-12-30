#include "taskmanager.h"

template<class T>
inline void MessageQueue<T>::send(T&& msg)
{
}

template<class T>
T MessageQueue<T>::receive(void)
{
	return T();
}

