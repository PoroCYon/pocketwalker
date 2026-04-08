#include <fstream>
#include <print>
#include <thread>
#include <QApplication>
#include <QThread>

#include "argparse/argparse.hpp"
#include "core/pokewalker/pocketwalker.h"
#include "qt/window/qt_window_system.h"
#include "qt/audio/qt_audio_system.h"
#include "qt/network/qt_network_system.h"

int main(int argc, char* argv[])
{
    argparse::ArgumentParser arguments("pocketwalker");

    arguments.add_argument("rom")
             .help("The path to the PokeWalker rom file.")
             .default_value("rom.bin");

    arguments.add_argument("save")
             .help("The path to your PokeWalker save file.")
             .default_value("pocketwalker.sav");

    arguments.add_argument("--server")
             .help("Run as TCP server instead of client.")
             .flag();

    arguments.add_argument("--ip")
             .help("IP address to connect to (client mode).")
             .default_value(std::string("127.0.0.1"));

    arguments.add_argument("--port")
             .help("TCP port for server or client.")
             .default_value(8081)
             .scan<'i', int>();

    arguments.add_argument("--packet-timeout")
             .help("Packet accumulator timeout in milliseconds.")
             .default_value(5)
             .scan<'i', int>();

    try
    {
        arguments.parse_args(argc, argv);
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << arguments;
        std::println("Press any key to exit...");
        std::cin.get();
        return 1;
    }

    auto rom_path = arguments.get<std::string>("rom");
    RomBuffer rom_buffer = {};
    if (std::filesystem::exists(rom_path))
    {
        std::ifstream rom_file(rom_path, std::ios::binary);
        rom_file.read(reinterpret_cast<char*>(rom_buffer.data()), 0xC000);
        rom_file.close();
    }
    else
    {
        std::println("Failed to find a rom with the name \"{}\"", rom_path);
        std::cin.get();
        return 1;
    }

    auto save_path = arguments.get<std::string>("save");
    EepromBuffer save_buffer = {};
    if (std::filesystem::exists(save_path))
    {
        std::ifstream eeprom_file(save_path, std::ios::binary);
        eeprom_file.read(reinterpret_cast<char*>(save_buffer.data()), save_buffer.size());
        eeprom_file.close();
    }

    QApplication app(argc, argv);
    app.setStyle("Fusion");

    PocketWalker emulator(rom_buffer, save_buffer);

    QtAudioSystem audio;
    emulator.OnSamplePushed([&](BuzzerInformation info)
    {
        audio.PushSample(info);
    });

    const bool server_mode = arguments.is_used("--server");
    const QString ip = QString::fromStdString(arguments.get<std::string>("--ip"));
    const auto tcp_port = static_cast<quint16>(arguments.get<int>("--port"));
    const int packet_timeout = arguments.get<int>("--packet-timeout");

    QThread tcp_thread;
    QtNetworkSystem network(emulator, server_mode, ip, tcp_port, packet_timeout);
    network.moveToThread(&tcp_thread);

    QObject::connect(&tcp_thread, &QThread::started, &network, &QtNetworkSystem::start);

    tcp_thread.start();

    QtWindowSystem window(emulator);
    window.show();

    std::thread emulator_thread([&] { emulator.Start(); });

    app.exec();

    emulator.Stop();
    emulator_thread.join();

    tcp_thread.quit();
    tcp_thread.wait();

    return 0;
}
