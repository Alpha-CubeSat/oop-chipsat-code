#include "SFRField.hpp"

arx::map<int, SFRInterface *> SFRInterface::opcode_lookup;
arx::vector<SFRInterface *> SFRInterface::sfr_fields_vector;

void SFRInterface::setFieldValByOpcode(int opcode, uint32_t arg1)
{
    if (opcode_lookup.find(opcode) != opcode_lookup.end()) {
        // Valid Op Code
        SFRInterface *ptr = opcode_lookup[opcode];
        ptr->setValue(arg1);
    }
}

int SFRInterface::getFieldValue()
{
    return field_value;
}

int SFRInterface::getDefaultValue()
{
    return default_value;
}

int SFRInterface::getDataType()
{
    return data_type;
}

int SFRInterface::getAddressOffset()
{
    return address_offset;
}

void SFRInterface::setRestore(bool restore_on_boot)
{
    restore = restore_on_boot;
}

bool SFRInterface::getRestore()
{
    return restore;
}