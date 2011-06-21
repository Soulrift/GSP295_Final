#ifndef TELEGRAM_H
#define TELEGRAM_H


#include <iostream>
#include <math.h>


struct Telegram
{
	int Sender;
	int Receiver;
	int Message;
	double DispatchTime;
	void* ExtraMsg;


  Telegram():DispatchTime(-1),
                  Sender(-1),
                  Receiver(-1),
                  Message(-1)
  {}


  Telegram(double time, int sender, int receiver, int message, void* info = NULL): DispatchTime(time),
																		   Sender(sender),
																		   Receiver(receiver),
																		   Message(message),
																		   ExtraMsg(info)
  {}
 
};

const double SmallestDelay = 0.25;


inline bool operator==(const Telegram& t1, const Telegram& t2)
{
  return ( fabs(t1.DispatchTime-t2.DispatchTime) < SmallestDelay) &&
          (t1.Sender == t2.Sender) && (t1.Receiver == t2.Receiver) && (t1.Message == t2.Message);
}

inline bool operator<(const Telegram& t1, const Telegram& t2)
{
  if(t1 == t2)
  {
    return false;
  }

  else
  {
    return (t1.DispatchTime < t2.DispatchTime);
  }
}

template <class T>
inline T DereferenceToType(void* p)
{
  return *(T*)(p);
}


#endif