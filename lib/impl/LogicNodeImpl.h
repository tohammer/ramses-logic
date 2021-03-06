//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------


#pragma once

#include <memory>
#include <string>
#include <vector>

#include "generated/logicnode_gen.h"

namespace flatbuffers
{
    template<typename T> struct Offset;
    class FlatBufferBuilder;
}

namespace rlogic_serialization
{
    struct LogicNode;
}

namespace rlogic
{
    class Property;
}

namespace rlogic::internal
{
    class PropertyImpl;

    class LogicNodeImpl
    {
    public:
        // Move-able (noexcept); Not copy-able
        virtual ~LogicNodeImpl() noexcept;
        LogicNodeImpl(LogicNodeImpl&& other) noexcept = default;
        LogicNodeImpl& operator=(LogicNodeImpl&& other) noexcept = default;
        LogicNodeImpl(const LogicNodeImpl& other)                = delete;
        LogicNodeImpl& operator=(const LogicNodeImpl& other) = delete;

        [[nodiscard]] Property*       getInputs();
        [[nodiscard]] const Property* getInputs() const;

        [[nodiscard]] Property* getOutputs();
        [[nodiscard]] const Property* getOutputs() const;

        virtual bool update() = 0;

        [[nodiscard]] const std::vector<std::string>& getErrors() const;
        void                            clearErrors();

        [[nodiscard]] std::string_view getName() const;

        void setDirty(bool dirty);
        [[nodiscard]] bool isDirty() const;

    protected:
        explicit LogicNodeImpl(std::string_view name) noexcept;
        LogicNodeImpl(std::string_view name, std::unique_ptr<PropertyImpl> inputs, std::unique_ptr<PropertyImpl> outputs);

        void addError(std::string_view error);

        flatbuffers::Offset<rlogic_serialization::LogicNode> serialize(flatbuffers::FlatBufferBuilder& builder) const;

    private:
        std::string               m_name;
        std::unique_ptr<Property> m_inputs;
        std::unique_ptr<Property> m_outputs;
        std::vector<std::string>  m_errors;
        bool                      m_dirty = true;

    };
}
