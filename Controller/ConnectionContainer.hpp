/**
 * @file ConnectionHandler.hpp
 * @author paul
 * @date 02.03.19
 * @brief Combines all stateful components of a connection
 */

#ifndef SERIALTOOLBOX_CONNECTIONHANDLER_HPP
#define SERIALTOOLBOX_CONNECTIONHANDLER_HPP

#include "SendThread.hpp"

namespace controller {
    class ConnectionContainer {
    public:
        std::shared_ptr<SendThread> sendThread;
        std::shared_ptr<util::serial::Interface> interface;
        std::shared_ptr<controller::SerialProxy> serialProxy;
    };
}

#endif //SERIALTOOLBOX_CONNECTIONHANDLER_HPP
