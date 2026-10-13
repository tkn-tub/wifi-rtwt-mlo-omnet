//
// Generated file, do not edit! Created by opp_msgtool 6.4 from src/rtwt/msg/twtFrame.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "twtFrame_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


template<typename T>
std::string toStringIfPrintable(const T& t) {
    if constexpr (omnetpp::internal::is_printable<T>::value) {
        std::ostringstream os;
        os << t;
        return os.str();
    }
    return omnetpp::cClassDescriptor::UNPRINTABLE;
}

template<typename T>
bool fromStringIfExtractable(T& t, const char *s) {
    if constexpr (omnetpp::internal::is_extractable<T>::value) {
        std::istringstream is(s);
        is >> t;
        return true;
    }
    return false;
}

namespace inet {
namespace ieee80211 {

Register_Class(twtFrame)

twtFrame::twtFrame() : ::inet::FieldsChunk()
{
}

twtFrame::twtFrame(const twtFrame& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

twtFrame::~twtFrame()
{
}

twtFrame& twtFrame::operator=(const twtFrame& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void twtFrame::copy(const twtFrame& other)
{
    this->STAaddress = other.STAaddress;
    this->flowId = other.flowId;
    this->type = other.type;
    this->restriction = other.restriction;
    this->interfaceId = other.interfaceId;
    this->wakeTime = other.wakeTime;
    this->wakeDuration = other.wakeDuration;
    this->intervalMantissa = other.intervalMantissa;
    this->intervalExponent = other.intervalExponent;
}

void twtFrame::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->STAaddress);
    doParsimPacking(b,this->flowId);
    doParsimPacking(b,this->type);
    doParsimPacking(b,this->restriction);
    doParsimPacking(b,this->interfaceId);
    doParsimPacking(b,this->wakeTime);
    doParsimPacking(b,this->wakeDuration);
    doParsimPacking(b,this->intervalMantissa);
    doParsimPacking(b,this->intervalExponent);
}

void twtFrame::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->STAaddress);
    doParsimUnpacking(b,this->flowId);
    doParsimUnpacking(b,this->type);
    doParsimUnpacking(b,this->restriction);
    doParsimUnpacking(b,this->interfaceId);
    doParsimUnpacking(b,this->wakeTime);
    doParsimUnpacking(b,this->wakeDuration);
    doParsimUnpacking(b,this->intervalMantissa);
    doParsimUnpacking(b,this->intervalExponent);
}

const ::inet::MacAddress& twtFrame::getSTAaddress() const
{
    return this->STAaddress;
}

void twtFrame::setSTAaddress(const ::inet::MacAddress& STAaddress)
{
    handleChange();
    this->STAaddress = STAaddress;
}

uint8_t twtFrame::getFlowId() const
{
    return this->flowId;
}

void twtFrame::setFlowId(uint8_t flowId)
{
    handleChange();
    this->flowId = flowId;
}

uint8_t twtFrame::getType() const
{
    return this->type;
}

void twtFrame::setType(uint8_t type)
{
    handleChange();
    this->type = type;
}

uint8_t twtFrame::getRestriction() const
{
    return this->restriction;
}

void twtFrame::setRestriction(uint8_t restriction)
{
    handleChange();
    this->restriction = restriction;
}

uint8_t twtFrame::getInterfaceId() const
{
    return this->interfaceId;
}

void twtFrame::setInterfaceId(uint8_t interfaceId)
{
    handleChange();
    this->interfaceId = interfaceId;
}

::omnetpp::simtime_t twtFrame::getWakeTime() const
{
    return this->wakeTime;
}

void twtFrame::setWakeTime(::omnetpp::simtime_t wakeTime)
{
    handleChange();
    this->wakeTime = wakeTime;
}

uint8_t twtFrame::getWakeDuration() const
{
    return this->wakeDuration;
}

void twtFrame::setWakeDuration(uint8_t wakeDuration)
{
    handleChange();
    this->wakeDuration = wakeDuration;
}

uint16_t twtFrame::getIntervalMantissa() const
{
    return this->intervalMantissa;
}

void twtFrame::setIntervalMantissa(uint16_t intervalMantissa)
{
    handleChange();
    this->intervalMantissa = intervalMantissa;
}

uint8_t twtFrame::getIntervalExponent() const
{
    return this->intervalExponent;
}

void twtFrame::setIntervalExponent(uint8_t intervalExponent)
{
    handleChange();
    this->intervalExponent = intervalExponent;
}

class twtFrameDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_STAaddress,
        FIELD_flowId,
        FIELD_type,
        FIELD_restriction,
        FIELD_interfaceId,
        FIELD_wakeTime,
        FIELD_wakeDuration,
        FIELD_intervalMantissa,
        FIELD_intervalExponent,
    };
  public:
    twtFrameDescriptor();
    virtual ~twtFrameDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual std::string getValueAsString(omnetpp::any_ptr object) const override;
    virtual void setValueAsString(omnetpp::any_ptr object, const char *value) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(twtFrameDescriptor)

twtFrameDescriptor::twtFrameDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::ieee80211::twtFrame)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

twtFrameDescriptor::~twtFrameDescriptor()
{
    delete[] propertyNames;
}

bool twtFrameDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<twtFrame *>(obj)!=nullptr;
}

const char **twtFrameDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *twtFrameDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

std::string twtFrameDescriptor::getValueAsString(omnetpp::any_ptr object) const
{
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    return ((cObject*)pp)->str();
}

void twtFrameDescriptor::setValueAsString(omnetpp::any_ptr object, const char *value) const
{
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    if (!fromStringIfExtractable(*pp, value))
        cClassDescriptor::setValueAsString(object, value);
}

int twtFrameDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 9+base->getFieldCount() : 9;
}

unsigned int twtFrameDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        0,    // FIELD_STAaddress
        FD_ISEDITABLE,    // FIELD_flowId
        FD_ISEDITABLE,    // FIELD_type
        FD_ISEDITABLE,    // FIELD_restriction
        FD_ISEDITABLE,    // FIELD_interfaceId
        FD_ISEDITABLE,    // FIELD_wakeTime
        FD_ISEDITABLE,    // FIELD_wakeDuration
        FD_ISEDITABLE,    // FIELD_intervalMantissa
        FD_ISEDITABLE,    // FIELD_intervalExponent
    };
    return (field >= 0 && field < 9) ? fieldTypeFlags[field] : 0;
}

const char *twtFrameDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "STAaddress",
        "flowId",
        "type",
        "restriction",
        "interfaceId",
        "wakeTime",
        "wakeDuration",
        "intervalMantissa",
        "intervalExponent",
    };
    return (field >= 0 && field < 9) ? fieldNames[field] : nullptr;
}

int twtFrameDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "STAaddress") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "flowId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "type") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "restriction") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "interfaceId") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "wakeTime") == 0) return baseIndex + 5;
    if (strcmp(fieldName, "wakeDuration") == 0) return baseIndex + 6;
    if (strcmp(fieldName, "intervalMantissa") == 0) return baseIndex + 7;
    if (strcmp(fieldName, "intervalExponent") == 0) return baseIndex + 8;
    return base ? base->findField(fieldName) : -1;
}

const char *twtFrameDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::MacAddress",    // FIELD_STAaddress
        "uint8_t",    // FIELD_flowId
        "uint8_t",    // FIELD_type
        "uint8_t",    // FIELD_restriction
        "uint8_t",    // FIELD_interfaceId
        "omnetpp::simtime_t",    // FIELD_wakeTime
        "uint8_t",    // FIELD_wakeDuration
        "uint16_t",    // FIELD_intervalMantissa
        "uint8_t",    // FIELD_intervalExponent
    };
    return (field >= 0 && field < 9) ? fieldTypeStrings[field] : nullptr;
}

const char **twtFrameDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *twtFrameDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int twtFrameDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void twtFrameDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'twtFrame'", field);
    }
}

const char *twtFrameDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string twtFrameDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    switch (field) {
        case FIELD_STAaddress: return pp->getSTAaddress().str();
        case FIELD_flowId: return ulong2string(pp->getFlowId());
        case FIELD_type: return ulong2string(pp->getType());
        case FIELD_restriction: return ulong2string(pp->getRestriction());
        case FIELD_interfaceId: return ulong2string(pp->getInterfaceId());
        case FIELD_wakeTime: return simtime2string(pp->getWakeTime());
        case FIELD_wakeDuration: return ulong2string(pp->getWakeDuration());
        case FIELD_intervalMantissa: return ulong2string(pp->getIntervalMantissa());
        case FIELD_intervalExponent: return ulong2string(pp->getIntervalExponent());
        default: return "";
    }
}

void twtFrameDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    switch (field) {
        case FIELD_flowId: pp->setFlowId(string2ulong(value)); break;
        case FIELD_type: pp->setType(string2ulong(value)); break;
        case FIELD_restriction: pp->setRestriction(string2ulong(value)); break;
        case FIELD_interfaceId: pp->setInterfaceId(string2ulong(value)); break;
        case FIELD_wakeTime: pp->setWakeTime(string2simtime(value)); break;
        case FIELD_wakeDuration: pp->setWakeDuration(string2ulong(value)); break;
        case FIELD_intervalMantissa: pp->setIntervalMantissa(string2ulong(value)); break;
        case FIELD_intervalExponent: pp->setIntervalExponent(string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtFrame'", field);
    }
}

omnetpp::cValue twtFrameDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    switch (field) {
        case FIELD_STAaddress: return omnetpp::toAnyPtr(&pp->getSTAaddress()); break;
        case FIELD_flowId: return (omnetpp::intval_t)(pp->getFlowId());
        case FIELD_type: return (omnetpp::intval_t)(pp->getType());
        case FIELD_restriction: return (omnetpp::intval_t)(pp->getRestriction());
        case FIELD_interfaceId: return (omnetpp::intval_t)(pp->getInterfaceId());
        case FIELD_wakeTime: return pp->getWakeTime().dbl();
        case FIELD_wakeDuration: return (omnetpp::intval_t)(pp->getWakeDuration());
        case FIELD_intervalMantissa: return (omnetpp::intval_t)(pp->getIntervalMantissa());
        case FIELD_intervalExponent: return (omnetpp::intval_t)(pp->getIntervalExponent());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'twtFrame' as cValue -- field index out of range?", field);
    }
}

void twtFrameDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    switch (field) {
        case FIELD_flowId: pp->setFlowId(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        case FIELD_type: pp->setType(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        case FIELD_restriction: pp->setRestriction(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        case FIELD_interfaceId: pp->setInterfaceId(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        case FIELD_wakeTime: pp->setWakeTime(value.doubleValue()); break;
        case FIELD_wakeDuration: pp->setWakeDuration(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        case FIELD_intervalMantissa: pp->setIntervalMantissa(omnetpp::checked_int_cast<uint16_t>(value.intValue())); break;
        case FIELD_intervalExponent: pp->setIntervalExponent(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtFrame'", field);
    }
}

const char *twtFrameDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr twtFrameDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    switch (field) {
        case FIELD_STAaddress: return omnetpp::toAnyPtr(&pp->getSTAaddress()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void twtFrameDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    twtFrame *pp = omnetpp::fromAnyPtr<twtFrame>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtFrame'", field);
    }
}

}  // namespace ieee80211
}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

