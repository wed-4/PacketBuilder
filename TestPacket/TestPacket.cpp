#include "PacketBuilder.h"
#include <iostream>

int main() {
    PacketBuilder builder;

    // パケットを構築
    builder.addField("id", 42)
        .addField("name", "Alice")
        .addField("score", 95.7f);

    builder.debug();

    // シリアライズ
    auto packet = builder.build();
    std::cout << "Serialized Packet: ";
    for (uint8_t byte : packet) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    // デシリアライズ
    PacketBuilder deserializer;
    if (deserializer.deserialize(packet)) {
        std::cout << "Deserialized Packet:" << std::endl;
        deserializer.debug();
    }
    else {
        std::cerr << "Failed to deserialize packet." << std::endl;
    }

    return 0;
}
