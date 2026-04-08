#include "qt_network_system.h"

QtNetworkSystem::QtNetworkSystem(PocketWalker& emulator, bool server_mode,
                                 const QString& host, quint16 port,
                                 int packet_timeout_ms, QObject* parent)
    : QObject(parent)
      , emulator(emulator)
      , server_mode(server_mode)
      , host(host)
      , port(port)
      , packet_timeout_ms(packet_timeout_ms)
{
}

void QtNetworkSystem::start()
{
    accumulator_timer = new QTimer(this);
    accumulator_timer->setSingleShot(true);
    connect(accumulator_timer, &QTimer::timeout, this, &QtNetworkSystem::flushPacket);

    reconnect_timer = new QTimer(this);
    reconnect_timer->setInterval(1000);
    connect(reconnect_timer, &QTimer::timeout, this, &QtNetworkSystem::tryConnect);

    emulator.OnTransmitIR([this](uint8_t byte)
    {
        QMetaObject::invokeMethod(this, [this, byte]()
        {
            tx_buffer.push_back(byte);
            accumulator_timer->start(packet_timeout_ms);
        }, Qt::QueuedConnection);
    });

    if (server_mode)
        startServer();
    else
        tryConnect();
}

void QtNetworkSystem::flushPacket()
{
    if (tx_buffer.empty())
        return;

    if (socket && socket->state() == QAbstractSocket::ConnectedState)
    {
        const QByteArray bytes(reinterpret_cast<const char*>(tx_buffer.data()),
            static_cast<qsizetype>(tx_buffer.size()));
        socket->write(bytes);
    }

    tx_buffer.clear();
}

void QtNetworkSystem::tryConnect()
{
    if (socket)
    {
        const auto state = socket->state();
        if (state == QAbstractSocket::ConnectedState ||
            state == QAbstractSocket::ConnectingState ||
            state == QAbstractSocket::HostLookupState)
            return;
    }

    if (!socket)
    {
        socket = new QTcpSocket(this);
        socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

        connect(socket, &QTcpSocket::connected, this, [this]()
        {
            Log::Info("Connected to {}:{}", host.toStdString(), port);
            reconnect_timer->stop();
        });

        connect(socket, &QTcpSocket::disconnected, this, [this]()
        {
            Log::Info("Disconnected");
            reconnect_timer->start();
        });

        connect(socket, &QTcpSocket::readyRead, this, &QtNetworkSystem::onReadyRead);

        connect(socket, &QTcpSocket::errorOccurred, this,
            [this](QAbstractSocket::SocketError err)
            {
                if (err == QAbstractSocket::RemoteHostClosedError)
                    return;

                socket->abort();
                reconnect_timer->start();
            });
    }

    socket->connectToHost(host, port);
}

void QtNetworkSystem::startServer()
{
    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, [this]()
    {
        if (socket && socket->state() == QAbstractSocket::ConnectedState)
            socket->disconnectFromHost();

        socket = server->nextPendingConnection();
        socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
        socket->setParent(this);

        Log::Info("Client connected from {}",
            socket->peerAddress().toString().toStdString());

        connect(socket, &QTcpSocket::disconnected, this, [this]()
        {
            Log::Info("Client disconnected");
            socket->deleteLater();
            socket = nullptr;
        });

        connect(socket, &QTcpSocket::readyRead, this, &QtNetworkSystem::onReadyRead);
    });

    if (!server->listen(QHostAddress::Any, port))
        Log::Info("Failed to start server: {}", server->errorString().toStdString());
    else
        Log::Info("Listening on port {}", port);
}

void QtNetworkSystem::onReadyRead()
{
    const QByteArray data = socket->readAll();
    for (const uint8_t byte : data)
        emulator.ReceiveIR(byte);
}
