//
// Created by jae on 13/02/2023.
//

#include "EventLooper.h"

EventLoop::EventLoop(android_app *app) : mApplication(app){ }

void EventLoop::run() {
    while (true) {
        int ident;
        int events;
        struct android_poll_source* source;

        while ((ident = ALooper_pollAll(0, nullptr, &events, (void**)&source)) >= 0) {
            if (source != nullptr) {
                source->process(mApplication, source);
            }

            EventLoop::draw();

            // Check if we are exiting.
            if (mApplication->destroyRequested != 0) {
                Log::info("Exiting event loop");
                EventLoop::onAppDestroy();
                return;
            }
        }
    }
}
