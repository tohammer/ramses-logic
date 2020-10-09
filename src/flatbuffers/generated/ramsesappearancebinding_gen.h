// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_RAMSESAPPEARANCEBINDING_RLOGIC_SERIALIZATION_H_
#define FLATBUFFERS_GENERATED_RAMSESAPPEARANCEBINDING_RLOGIC_SERIALIZATION_H_

#include "flatbuffers/flatbuffers.h"

#include "logicnode_gen.h"
#include "property_gen.h"

namespace rlogic_serialization {

struct RamsesAppearanceBinding;
struct RamsesAppearanceBindingBuilder;

struct RamsesAppearanceBinding FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef RamsesAppearanceBindingBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_LOGICNODE = 4,
    VT_RAMSESAPPEARANCE = 6
  };
  const rlogic_serialization::LogicNode *logicnode() const {
    return GetPointer<const rlogic_serialization::LogicNode *>(VT_LOGICNODE);
  }
  uint64_t ramsesAppearance() const {
    return GetField<uint64_t>(VT_RAMSESAPPEARANCE, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_LOGICNODE) &&
           verifier.VerifyTable(logicnode()) &&
           VerifyField<uint64_t>(verifier, VT_RAMSESAPPEARANCE) &&
           verifier.EndTable();
  }
};

struct RamsesAppearanceBindingBuilder {
  typedef RamsesAppearanceBinding Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_logicnode(flatbuffers::Offset<rlogic_serialization::LogicNode> logicnode) {
    fbb_.AddOffset(RamsesAppearanceBinding::VT_LOGICNODE, logicnode);
  }
  void add_ramsesAppearance(uint64_t ramsesAppearance) {
    fbb_.AddElement<uint64_t>(RamsesAppearanceBinding::VT_RAMSESAPPEARANCE, ramsesAppearance, 0);
  }
  explicit RamsesAppearanceBindingBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  RamsesAppearanceBindingBuilder &operator=(const RamsesAppearanceBindingBuilder &);
  flatbuffers::Offset<RamsesAppearanceBinding> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<RamsesAppearanceBinding>(end);
    return o;
  }
};

inline flatbuffers::Offset<RamsesAppearanceBinding> CreateRamsesAppearanceBinding(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<rlogic_serialization::LogicNode> logicnode = 0,
    uint64_t ramsesAppearance = 0) {
  RamsesAppearanceBindingBuilder builder_(_fbb);
  builder_.add_ramsesAppearance(ramsesAppearance);
  builder_.add_logicnode(logicnode);
  return builder_.Finish();
}

struct RamsesAppearanceBinding::Traits {
  using type = RamsesAppearanceBinding;
  static auto constexpr Create = CreateRamsesAppearanceBinding;
};

inline const rlogic_serialization::RamsesAppearanceBinding *GetRamsesAppearanceBinding(const void *buf) {
  return flatbuffers::GetRoot<rlogic_serialization::RamsesAppearanceBinding>(buf);
}

inline const rlogic_serialization::RamsesAppearanceBinding *GetSizePrefixedRamsesAppearanceBinding(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<rlogic_serialization::RamsesAppearanceBinding>(buf);
}

inline bool VerifyRamsesAppearanceBindingBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<rlogic_serialization::RamsesAppearanceBinding>(nullptr);
}

inline bool VerifySizePrefixedRamsesAppearanceBindingBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<rlogic_serialization::RamsesAppearanceBinding>(nullptr);
}

inline void FinishRamsesAppearanceBindingBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<rlogic_serialization::RamsesAppearanceBinding> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedRamsesAppearanceBindingBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<rlogic_serialization::RamsesAppearanceBinding> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace rlogic_serialization

#endif  // FLATBUFFERS_GENERATED_RAMSESAPPEARANCEBINDING_RLOGIC_SERIALIZATION_H_
