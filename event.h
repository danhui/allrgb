#ifndef __EVENT_H__
#define __EVENT_H__

// Event stores two things:
// 1. What type of event (e.g. mouse down, key up, key down, etc.)
// 2. The value from the event (eg. an ascii code).

class Event {
  public:
    Event (int type, int value) {
      type_ = type;
      value_ = value;
    }
    Event () {
      type_ = 0;
      value_ = 0;
    }
    int GetType() {
      return type_;
    }
    int GetValue() {
      return value_;
    }
  private:
    int type_, value_;
};

#endif
