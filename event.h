#ifndef __EVENT_H__
#define __EVENT_H__

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
