#pragma once
#include <QObject>
#include <Serialization/NetworkMessage.hpp>
#include <iscore/tools/IdentifiedObject.hpp>
#include "../client/Client.hpp"
class NetworkSocket;
class RemoteClient;
class MasterSession;
class QTcpSocket;
class RemoteClientBuilder : public QObject
{
        Q_OBJECT
    public:
        RemoteClientBuilder(MasterSession& session, QTcpSocket* sock);

    signals:
        void clientReady(RemoteClientBuilder* builder, RemoteClient*);

    public slots:
        void on_messageReceived(NetworkMessage m);


    private:
        MasterSession& m_session;
        NetworkSocket* m_socket;
        RemoteClient* m_remoteClient{};

        id_type<Client> m_clientId;
};
