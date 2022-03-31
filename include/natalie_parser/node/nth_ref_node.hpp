#pragma once

#include "natalie_parser/node/node.hpp"

namespace NatalieParser {

using namespace TM;

class NthRefNode : public Node {
public:
    NthRefNode(const Token &token, long long num)
        : Node { token }
        , m_num { num } { }

    virtual Type type() const override { return Type::NthRef; }

    virtual void transform(Creator *creator) const override {
        creator->set_type("nth_ref");
        creator->append_integer(m_num);
    }

protected:
    long long m_num { 0 };
};
}
