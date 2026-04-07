//
// Generated file, do not edit! Created by opp_msgtool 6.0 from src/msg/managementInfoFrame.msg.
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
#include "../../rtwt/msg/managementInfoFrame_m.h"

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

Register_Class(ManagementInfoFrame)

ManagementInfoFrame::ManagementInfoFrame() : ::inet::FieldsChunk()
{
}

ManagementInfoFrame::ManagementInfoFrame(const ManagementInfoFrame& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

ManagementInfoFrame::~ManagementInfoFrame()
{
}

ManagementInfoFrame& ManagementInfoFrame::operator=(const ManagementInfoFrame& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void ManagementInfoFrame::copy(const ManagementInfoFrame& other)
{
    this->staAddress = other.staAddress;
    this->interfaceId = other.interfaceId;
    this->status = other.status;
}

void ManagementInfoFrame::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    doParsimPacking(b,this->staAddress);
    doParsimPacking(b,this->interfaceId);
    doParsimPacking(b,this->status);
}

void ManagementInfoFrame::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    doParsimUnpacking(b,this->staAddress);
    doParsimUnpacking(b,this->interfaceId);
    doParsimUnpacking(b,this->status);
}

const ::inet::MacAddress& ManagementInfoFrame::getStaAddress() const
{
    return this->staAddress;
}

void ManagementInfoFrame::setStaAddress(const ::inet::MacAddress& staAddress)
{
    handleChange();
    this->staAddress = staAddress;
}

uint8_t ManagementInfoFrame::getInterfaceId() const
{
    return this->interfaceId;
}

void ManagementInfoFrame::setInterfaceId(uint8_t interfaceId)
{
    handleChange();
    this->interfaceId = interfaceId;
}

uint8_t ManagementInfoFrame::getStatus() const
{
    return this->status;
}

void ManagementInfoFrame::setStatus(uint8_t status)
{
    handleChange();
    this->status = status;
}

class ManagementInfoFrameDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_staAddress,
        FIELD_interfaceId,
        FIELD_status,
    };
  public:
    ManagementInfoFrameDescriptor();
    virtual ~ManagementInfoFrameDescriptor();

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

Register_ClassDescriptor(ManagementInfoFrameDescriptor)

ManagementInfoFrameDescriptor::ManagementInfoFrameDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::ieee80211::ManagementInfoFrame)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

ManagementInfoFrameDescriptor::~ManagementInfoFrameDescriptor()
{
    delete[] propertyNames;
}

bool ManagementInfoFrameDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ManagementInfoFrame *>(obj)!=nullptr;
}

const char **ManagementInfoFrameDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ManagementInfoFrameDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ManagementInfoFrameDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int ManagementInfoFrameDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        0,    // FIELD_staAddress
        FD_ISEDITABLE,    // FIELD_interfaceId
        FD_ISEDITABLE,    // FIELD_status
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *ManagementInfoFrameDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "staAddress",
        "interfaceId",
        "status",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int ManagementInfoFrameDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "staAddress") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "interfaceId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "status") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *ManagementInfoFrameDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::MacAddress",    // FIELD_staAddress
        "uint8_t",    // FIELD_interfaceId
        "uint8_t",    // FIELD_status
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **ManagementInfoFrameDescriptor::getFieldPropertyNames(int field) const
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

const char *ManagementInfoFrameDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int ManagementInfoFrameDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ManagementInfoFrame *pp = omnetpp::fromAnyPtr<ManagementInfoFrame>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ManagementInfoFrameDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ManagementInfoFrame *pp = omnetpp::fromAnyPtr<ManagementInfoFrame>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ManagementInfoFrame'", field);
    }
}

const char *ManagementInfoFrameDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ManagementInfoFrame *pp = omnetpp::fromAnyPtr<ManagementInfoFrame>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ManagementInfoFrameDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ManagementInfoFrame *pp = omnetpp::fromAnyPtr<ManagementInfoFrame>(object); (void)pp;
    switch (field) {
        case FIELD_staAddress: return pp->getStaAddress().str();
        case FIELD_interfaceId: return ulong2string(pp->getInterfaceId());
        case FIELD_status: return ulong2string(pp->getStatus());
        default: return "";
    }
}

void ManagementInfoFrameDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ManagementInfoFrame *pp = omnetpp::fromAnyPtr<ManagementInfoFrame>(object); (void)pp;
    switch (field) {
        case FIELD_interfaceId: pp->setInterfaceId(string2ulong(value)); break;
        case FIELD_status: pp->setStatus(string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ManagementInfoFrame'", field);
    }
}

omnetpp::cValue ManagementInfoFrameDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ManagementInfoFrame *pp = omnetpp::fromAnyPtr<ManagementInfoFrame>(object); (void)pp;
    switch (field) {
        case FIELD_staAddress: return omnetpp::toAnyPtr(&pp->getStaAddress()); break;
        case FIELD_interfaceId: return (omnetpp::intval_t)(pp->getInterfaceId());
        case FIELD_status: return (omnetpp::intval_t)(pp->getStatus());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ManagementInfoFrame' as cValue -- field index out of range?", field);
    }
}

void ManagementInfoFrameDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ManagementInfoFrame *pp = omnetpp::fromAnyPtr<ManagementInfoFrame>(object); (void)pp;
    switch (field) {
        case FIELD_interfaceId: pp->setInterfaceId(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        case FIELD_status: pp->setStatus(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ManagementInfoFrame'", field);
    }
}

const char *ManagementInfoFrameDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr ManagementInfoFrameDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ManagementInfoFrame *pp = omnetpp::fromAnyPtr<ManagementInfoFrame>(object); (void)pp;
    switch (field) {
        case FIELD_staAddress: return omnetpp::toAnyPtr(&pp->getStaAddress()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ManagementInfoFrameDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ManagementInfoFrame *pp = omnetpp::fromAnyPtr<ManagementInfoFrame>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ManagementInfoFrame'", field);
    }
}

}  // namespace ieee80211
}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

