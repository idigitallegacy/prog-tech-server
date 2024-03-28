//
// Created by elfxe on 11.02.2024.
//

#ifndef DOTAPARSERSERVER_SERVEROPTIONS_H
#define DOTAPARSERSERVER_SERVEROPTIONS_H

#include "../processor/GetRequestProcessor.h"
#include "../processor/PostRequestProcessor.h"
#include "../storage/TimelineJsonStorage.h"

struct ServerOptions {
    TimelineJsonStorage *timelineStorage = new TimelineJsonStorage();
    GetRequestProcessor *getRequestProcessor = new GetRequestProcessor(timelineStorage);
    PostRequestProcessor *postRequestProcessor = new PostRequestProcessor(timelineStorage);
};

#endif //DOTAPARSERSERVER_SERVEROPTIONS_H
