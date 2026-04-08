#pragma once

#include <vector>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTimer>
#include <QThread>
#include <QHostAddress>

#include "core/pokewalker/pocketwalker.h"
#include "core/utils/logger.h"

class QtNetworkSystem : public QObject
{
    Q_OBJECT

public:
    explicit QtNetworkSystem(PocketWalker& emulator, bool server_mode,
                             const QString& host, quint16 port,
                             int packet_timeout_ms, QObject* parent = nullptr);

public slots:
    void start();

private slots:
    void flushPacket();
    void tryConnect();
    void startServer();
    void onReadyRead();

private:
    PocketWalker& emulator;
    bool server_mode;
    QString host;
    quint16 port;
    int packet_timeout_ms;

    QTcpSocket* socket = nullptr;
    QTcpServer* server = nullptr;

    QTimer* accumulator_timer = nullptr;
    QTimer* reconnect_timer = nullptr;

    std::vector<uint8_t> tx_buffer;
};
