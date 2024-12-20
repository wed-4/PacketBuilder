#ifndef PACKETBUILDER_H
#define PACKETBUILDER_H

#include <vector>
#include <string>
#include <variant>
#include <cstdint>
#include <sstream>
#include <optional>

class PacketBuilder {
public:
    using FieldValue = std::variant<int, float, std::string>;

    PacketBuilder& addField(const std::string& name, int value);
    PacketBuilder& addField(const std::string& name, float value);
    PacketBuilder& addField(const std::string& name, const std::string& value);

    std::vector<uint8_t> build() const;
    bool deserialize(const std::vector<uint8_t>& packet);

    void debug() const;

private:
    struct Field {
        std::string name;
        FieldValue value;
    };

    std::vector<Field> fields;
};

#endif // PACKETBUILDER_H
