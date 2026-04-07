//
// Generated file, do not edit! Created by opp_msgtool 6.0 from src/msg/twtLinkRequirement.msg.
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
#include "../../rtwt/msg/twtLinkRequirement_m.h"

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

namespace inet {
namespace ieee80211 {

Register_Class(twtLinkRequirement)

twtLinkRequirement::twtLinkRequirement() : ::inet::FieldsChunk()
{
}

twtLinkRequirement::twtLinkRequirement(const twtLinkRequirement& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

twtLinkRequirement::~twtLinkRequirement()
{
}

twtLinkRequirement& twtLinkRequirement::operator=(const twtLinkRequirement& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void twtLinkRequirement::copy(const twtLinkRequirement& other)
{
    this->dataRate = other.dataRate;
    this->wakeTime = other.wakeTime;
    this->restriction = other.restriction;
    this->latency = other.latency;
    this->receiver = other.receiver;
    this->STAaddress = other.STAaddress;
}

void twtLinkRequirement::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->dataRate);
    doParsimPacking(b,this->wakeTime);
    doParsimPacking(b,this->restriction);
    doParsimPacking(b,this->latency);
    doParsimPacking(b,this->receiver);
    doParsimPacking(b,this->STAaddress);
}

void twtLinkRequirement::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->dataRate);
    doParsimUnpacking(b,this->wakeTime);
    doParsimUnpacking(b,this->restriction);
    doParsimUnpacking(b,this->latency);
    doParsimUnpacking(b,this->receiver);
    doParsimUnpacking(b,this->STAaddress);
}

uint32_t twtLinkRequirement::getDataRate() const
{
    return this->dataRate;
}

void twtLinkRequirement::setDataRate(uint32_t dataRate)
{
    handleChange();
    this->dataRate = dataRate;
}

uint32_t twtLinkRequirement::getWakeTime() const
{
    return this->wakeTime;
}

void twtLinkRequirement::setWakeTime(uint32_t wakeTime)
{
    handleChange();
    this->wakeTime = wakeTime;
}

uint8_t twtLinkRequirement::getRestriction() const
{
    return this->restriction;
}

void twtLinkRequirement::setRestriction(uint8_t restriction)
{
    handleChange();
    this->restriction = restriction;
}

uint32_t twtLinkRequirement::getLatency() const
{
    return this->latency;
}

void twtLinkRequirement::setLatency(uint32_t latency)
{
    handleChange();
    this->latency = latency;
}

const ::inet::MacAddress& twtLinkRequirement::getReceiver() const
{
    return this->receiver;
}

void twtLinkRequirement::setReceiver(const ::inet::MacAddress& receiver)
{
    handleChange();
    this->receiver = receiver;
}

const ::inet::MacAddress& twtLinkRequirement::getSTAaddress() const
{
    return this->STAaddress;
}

void twtLinkRequirement::setSTAaddress(const ::inet::MacAddress& STAaddress)
{
    handleChange();
    this->STAaddress = STAaddress;
}

class twtLinkRequirementDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_dataRate,
        FIELD_wakeTime,
        FIELD_restriction,
        FIELD_latency,
        FIELD_receiver,
        FIELD_STAaddress,
    };
  public:
    twtLinkRequirementDescriptor();
    virtual ~twtLinkRequirementDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
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

Register_ClassDescriptor(twtLinkRequirementDescriptor)

twtLinkRequirementDescriptor::twtLinkRequirementDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::ieee80211::twtLinkRequirement)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

twtLinkRequirementDescriptor::~twtLinkRequirementDescriptor()
{
    delete[] propertyNames;
}

bool twtLinkRequirementDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<twtLinkRequirement *>(obj)!=nullptr;
}

const char **twtLinkRequirementDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *twtLinkRequirementDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int twtLinkRequirementDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int twtLinkRequirementDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_dataRate
        FD_ISEDITABLE,    // FIELD_wakeTime
        FD_ISEDITABLE,    // FIELD_restriction
        FD_ISEDITABLE,    // FIELD_latency
        0,    // FIELD_receiver
        0,    // FIELD_STAaddress
    };
    return (field >= 0 && field < 6) ? fieldTypeFlags[field] : 0;
}

const char *twtLinkRequirementDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "dataRate",
        "wakeTime",
        "restriction",
        "latency",
        "receiver",
        "STAaddress",
    };
    return (field >= 0 && field < 6) ? fieldNames[field] : nullptr;
}

int twtLinkRequirementDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "dataRate") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "wakeTime") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "restriction") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "latency") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "receiver") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "STAaddress") == 0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *twtLinkRequirementDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "uint32_t",    // FIELD_dataRate
        "uint32_t",    // FIELD_wakeTime
        "uint8_t",    // FIELD_restriction
        "uint32_t",    // FIELD_latency
        "inet::MacAddress",    // FIELD_receiver
        "inet::MacAddress",    // FIELD_STAaddress
    };
    return (field >= 0 && field < 6) ? fieldTypeStrings[field] : nullptr;
}

const char **twtLinkRequirementDescriptor::getFieldPropertyNames(int field) const
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

const char *twtLinkRequirementDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int twtLinkRequirementDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    twtLinkRequirement *pp = omnetpp::fromAnyPtr<twtLinkRequirement>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void twtLinkRequirementDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    twtLinkRequirement *pp = omnetpp::fromAnyPtr<twtLinkRequirement>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'twtLinkRequirement'", field);
    }
}

const char *twtLinkRequirementDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    twtLinkRequirement *pp = omnetpp::fromAnyPtr<twtLinkRequirement>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string twtLinkRequirementDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    twtLinkRequirement *pp = omnetpp::fromAnyPtr<twtLinkRequirement>(object); (void)pp;
    switch (field) {
        case FIELD_dataRate: return ulong2string(pp->getDataRate());
        case FIELD_wakeTime: return ulong2string(pp->getWakeTime());
        case FIELD_restriction: return ulong2string(pp->getRestriction());
        case FIELD_latency: return ulong2string(pp->getLatency());
        case FIELD_receiver: return pp->getReceiver().str();
        case FIELD_STAaddress: return pp->getSTAaddress().str();
        default: return "";
    }
}

void twtLinkRequirementDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    twtLinkRequirement *pp = omnetpp::fromAnyPtr<twtLinkRequirement>(object); (void)pp;
    switch (field) {
        case FIELD_dataRate: pp->setDataRate(string2ulong(value)); break;
        case FIELD_wakeTime: pp->setWakeTime(string2ulong(value)); break;
        case FIELD_restriction: pp->setRestriction(string2ulong(value)); break;
        case FIELD_latency: pp->setLatency(string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtLinkRequirement'", field);
    }
}

omnetpp::cValue twtLinkRequirementDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    twtLinkRequirement *pp = omnetpp::fromAnyPtr<twtLinkRequirement>(object); (void)pp;
    switch (field) {
        case FIELD_dataRate: return (omnetpp::intval_t)(pp->getDataRate());
        case FIELD_wakeTime: return (omnetpp::intval_t)(pp->getWakeTime());
        case FIELD_restriction: return (omnetpp::intval_t)(pp->getRestriction());
        case FIELD_latency: return (omnetpp::intval_t)(pp->getLatency());
        case FIELD_receiver: return omnetpp::toAnyPtr(&pp->getReceiver()); break;
        case FIELD_STAaddress: return omnetpp::toAnyPtr(&pp->getSTAaddress()); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'twtLinkRequirement' as cValue -- field index out of range?", field);
    }
}

void twtLinkRequirementDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    twtLinkRequirement *pp = omnetpp::fromAnyPtr<twtLinkRequirement>(object); (void)pp;
    switch (field) {
        case FIELD_dataRate: pp->setDataRate(omnetpp::checked_int_cast<uint32_t>(value.intValue())); break;
        case FIELD_wakeTime: pp->setWakeTime(omnetpp::checked_int_cast<uint32_t>(value.intValue())); break;
        case FIELD_restriction: pp->setRestriction(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        case FIELD_latency: pp->setLatency(omnetpp::checked_int_cast<uint32_t>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtLinkRequirement'", field);
    }
}

const char *twtLinkRequirementDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr twtLinkRequirementDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    twtLinkRequirement *pp = omnetpp::fromAnyPtr<twtLinkRequirement>(object); (void)pp;
    switch (field) {
        case FIELD_receiver: return omnetpp::toAnyPtr(&pp->getReceiver()); break;
        case FIELD_STAaddress: return omnetpp::toAnyPtr(&pp->getSTAaddress()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void twtLinkRequirementDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    twtLinkRequirement *pp = omnetpp::fromAnyPtr<twtLinkRequirement>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtLinkRequirement'", field);
    }
}

}  // namespace ieee80211
}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

