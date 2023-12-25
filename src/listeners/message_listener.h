#ifndef CRINGE_MESSAGE_LISTENER_H
#define CRINGE_MESSAGE_LISTENER_H

#include <dpp/dpp.h>

class message_listener {
public:
    static void on_message_create(const dpp::message_create_t& event);
    static void on_message_delete(const dpp::message_delete_t& event);
};

#endif
