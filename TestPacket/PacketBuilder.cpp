#include "PacketBuilder.h"
#include <iostream>
#include <cstring>

// �V���A���C�Y�֘A
PacketBuilder& PacketBuilder::addField(const std::string& name, int value) {
    fields.push_back({ name, value });
    return *this;
}

PacketBuilder& PacketBuilder::addField(const std::string& name, float value) {
    fields.push_back({ name, value });
    return *this;
}

PacketBuilder& PacketBuilder::addField(const std::string& name, const std::string& value) {
    fields.push_back({ name, value });
    return *this;
}

std::vector<uint8_t> PacketBuilder::build() const {
    std::vector<uint8_t> packet;

    for (const auto& field : fields) {
        // �t�B�[���h���̒����Ɩ��O��ǉ�
        uint8_t nameLength = static_cast<uint8_t>(field.name.size());
        packet.push_back(nameLength);
        packet.insert(packet.end(), field.name.begin(), field.name.end());

        // �^�ɉ������l��ǉ�
        if (std::holds_alternative<int>(field.value)) {
            packet.push_back(0x01); // int�^�̎��ʎq
            int value = std::get<int>(field.value);
            uint8_t* valueBytes = reinterpret_cast<uint8_t*>(&value);
            packet.insert(packet.end(), valueBytes, valueBytes + sizeof(value));
        }
        else if (std::holds_alternative<float>(field.value)) {
            packet.push_back(0x02); // float�^�̎��ʎq
            float value = std::get<float>(field.value);
            uint8_t* valueBytes = reinterpret_cast<uint8_t*>(&value);
            packet.insert(packet.end(), valueBytes, valueBytes + sizeof(value));
        }
        else if (std::holds_alternative<std::string>(field.value)) {
            packet.push_back(0x03); // string�^�̎��ʎq
            const std::string& value = std::get<std::string>(field.value);
            uint8_t valueLength = static_cast<uint8_t>(value.size());
            packet.push_back(valueLength);
            packet.insert(packet.end(), value.begin(), value.end());
        }
    }

    return packet;
}

// �f�V���A���C�Y�֘A
bool PacketBuilder::deserialize(const std::vector<uint8_t>& packet) {
    fields.clear(); // ���݂̃t�B�[���h���N���A
    size_t index = 0;

    while (index < packet.size()) {
        // �t�B�[���h���̒����Ɩ��O���擾
        uint8_t nameLength = packet[index++];
        if (index + nameLength > packet.size()) return false; // �s���ȃf�[�^
        std::string name(packet.begin() + index, packet.begin() + index + nameLength);
        index += nameLength;

        // �^���擾
        if (index >= packet.size()) return false; // �s���ȃf�[�^
        uint8_t type = packet[index++];

        // �l���擾
        if (type == 0x01) { // int�^
            if (index + sizeof(int) > packet.size()) return false; // �s���ȃf�[�^
            int value;
            std::memcpy(&value, &packet[index], sizeof(int));
            index += sizeof(int);
            fields.push_back({ name, value });
        }
        else if (type == 0x02) { // float�^
            if (index + sizeof(float) > packet.size()) return false; // �s���ȃf�[�^
            float value;
            std::memcpy(&value, &packet[index], sizeof(float));
            index += sizeof(float);
            fields.push_back({ name, value });
        }
        else if (type == 0x03) { // string�^
            if (index >= packet.size()) return false; // �s���ȃf�[�^
            uint8_t valueLength = packet[index++];
            if (index + valueLength > packet.size()) return false; // �s���ȃf�[�^
            std::string value(packet.begin() + index, packet.begin() + index + valueLength);
            index += valueLength;
            fields.push_back({ name, value });
        }
        else {
            return false; // �s���Ȍ^
        }
    }

    return true;
}

void PacketBuilder::debug() const {
    for (const auto& field : fields) {
        std::cout << field.name << ": ";
        if (std::holds_alternative<int>(field.value)) {
            std::cout << std::get<int>(field.value);
        }
        else if (std::holds_alternative<float>(field.value)) {
            std::cout << std::get<float>(field.value);
        }
        else if (std::holds_alternative<std::string>(field.value)) {
            std::cout << std::get<std::string>(field.value);
        }
        std::cout << std::endl;
    }
}
