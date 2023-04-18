//
// Created by jae on 13/02/2023.
//

#ifndef AS_UB_EVENTLOOPER_H
#define AS_UB_EVENTLOOPER_H

#include "../../../../../third-party/native_app_glue/android_native_app_glue.h"

class EventLoop {
public:
    EventLoop(android_app* app);
    void run();
    virtual void draw();
    virtual void onAppDestroy();
private:

    android_app* mApplication;
};

#endif //AS_UB_EVENTLOOPER_H
