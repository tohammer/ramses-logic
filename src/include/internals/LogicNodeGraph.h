//  -------------------------------------------------------------------------
//  Copyright (C) 2020 BMW AG
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>

namespace rlogic::internal
{
    class LogicNodeImpl;

    class LogicNodeGraph {
    public:

        LogicNodeGraph()  = default;
        ~LogicNodeGraph() = default;

        void addLink(LogicNodeImpl& source, LogicNodeImpl& target);
        void removeLink(LogicNodeImpl& source, const LogicNodeImpl& target);
        void removeLinksForNode(LogicNodeImpl& source);
        bool isLinked(LogicNodeImpl& node) const ;
        std::vector<LogicNodeImpl*>::iterator begin();
        std::vector<LogicNodeImpl*>::iterator end();

    private:

        void updateOrder();

        struct Link
        {
            LogicNodeImpl* target;
            uint32_t bindingCount;
        };

        std::vector<LogicNodeImpl*> findUnboundInputs() const;

        std::unordered_map<LogicNodeImpl*, std::vector<Link>> m_links;
        std::vector<LogicNodeImpl*>                           m_order;
        bool m_dirty = false;
    };
}
