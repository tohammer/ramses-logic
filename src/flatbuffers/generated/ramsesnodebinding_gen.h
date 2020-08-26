// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_RAMSESNODEBINDING_RLOGIC_SERIALIZATION_H_
#define FLATBUFFERS_GENERATED_RAMSESNODEBINDING_RLOGIC_SERIALIZATION_H_

#include "flatbuffers/flatbuffers.h"

#include "logicnode_gen.h"
#include "property_gen.h"

namespace rlogic {
namespace serialization {

struct RamsesNodeBinding;
struct RamsesNodeBindingBuilder;

struct RamsesNodeBinding FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef RamsesNodeBindingBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_LOGICNODE = 4
  };
  const rlogic::serialization::LogicNode *logicnode() const {
    return GetPointer<const rlogic::serialization::LogicNode *>(VT_LOGICNODE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_LOGICNODE) &&
           verifier.VerifyTable(logicnode()) &&
           verifier.EndTable();
  }
};

struct RamsesNodeBindingBuilder {
  typedef RamsesNodeBinding Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_logicnode(flatbuffers::Offset<rlogic::serialization::LogicNode> logicnode) {
    fbb_.AddOffset(RamsesNodeBinding::VT_LOGICNODE, logicnode);
  }
  explicit RamsesNodeBindingBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  RamsesNodeBindingBuilder &operator=(const RamsesNodeBindingBuilder &);
  flatbuffers::Offset<RamsesNodeBinding> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<RamsesNodeBinding>(end);
    return o;
  }
};

inline flatbuffers::Offset<RamsesNodeBinding> CreateRamsesNodeBinding(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<rlogic::serialization::LogicNode> logicnode = 0) {
  RamsesNodeBindingBuilder builder_(_fbb);
  builder_.add_logicnode(logicnode);
  return builder_.Finish();
}

struct RamsesNodeBinding::Traits {
  using type = RamsesNodeBinding;
  static auto constexpr Create = CreateRamsesNodeBinding;
};

inline const rlogic::serialization::RamsesNodeBinding *GetRamsesNodeBinding(const void *buf) {
  return flatbuffers::GetRoot<rlogic::serialization::RamsesNodeBinding>(buf);
}

inline const rlogic::serialization::RamsesNodeBinding *GetSizePrefixedRamsesNodeBinding(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<rlogic::serialization::RamsesNodeBinding>(buf);
}

inline bool VerifyRamsesNodeBindingBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<rlogic::serialization::RamsesNodeBinding>(nullptr);
}

inline bool VerifySizePrefixedRamsesNodeBindingBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<rlogic::serialization::RamsesNodeBinding>(nullptr);
}

inline void FinishRamsesNodeBindingBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<rlogic::serialization::RamsesNodeBinding> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedRamsesNodeBindingBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<rlogic::serialization::RamsesNodeBinding> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace serialization
}  // namespace rlogic

#endif  // FLATBUFFERS_GENERATED_RAMSESNODEBINDING_RLOGIC_SERIALIZATION_H_