//
// Generated file, do not edit! Created by opp_msgtool 6.0 from src/msg/twtScheduleBeacon.msg.
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
#include "../../rtwt/msg/twtScheduleBeacon_m.h"

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

twtScheduleEntry::twtScheduleEntry()
{
}

twtScheduleEntry::twtScheduleEntry(const twtScheduleEntry& other)
{
    copy(other);
}

twtScheduleEntry::~twtScheduleEntry()
{
}

twtScheduleEntry& twtScheduleEntry::operator=(const twtScheduleEntry& other)
{
    if (this == &other) return *this;
    copy(other);
    return *this;
}

void twtScheduleEntry::copy(const twtScheduleEntry& other)
{
    this->staAddress = other.staAddress;
    this->wakeTime = other.wakeTime;
    this->wakeDuration = other.wakeDuration;
    this->intervalMantissa = other.intervalMantissa;
    this->intervalExponent = other.intervalExponent;
    this->restriction = other.restriction;
}

void twtScheduleEntry::parsimPack(omnetpp::cCommBuffer *b) const
{
    doParsimPacking(b,this->staAddress);
    doParsimPacking(b,this->wakeTime);
    doParsimPacking(b,this->wakeDuration);
    doParsimPacking(b,this->intervalMantissa);
    doParsimPacking(b,this->intervalExponent);
    doParsimPacking(b,this->restriction);
}

void twtScheduleEntry::parsimUnpack(omnetpp::cCommBuffer *b)
{
    doParsimUnpacking(b,this->staAddress);
    doParsimUnpacking(b,this->wakeTime);
    doParsimUnpacking(b,this->wakeDuration);
    doParsimUnpacking(b,this->intervalMantissa);
    doParsimUnpacking(b,this->intervalExponent);
    doParsimUnpacking(b,this->restriction);
}

const ::inet::MacAddress& twtScheduleEntry::getStaAddress() const
{
    return this->staAddress;
}

void twtScheduleEntry::setStaAddress(const ::inet::MacAddress& staAddress)
{
    this->staAddress = staAddress;
}

::omnetpp::simtime_t twtScheduleEntry::getWakeTime() const
{
    return this->wakeTime;
}

void twtScheduleEntry::setWakeTime(::omnetpp::simtime_t wakeTime)
{
    this->wakeTime = wakeTime;
}

uint32_t twtScheduleEntry::getWakeDuration() const
{
    return this->wakeDuration;
}

void twtScheduleEntry::setWakeDuration(uint32_t wakeDuration)
{
    this->wakeDuration = wakeDuration;
}

uint16_t twtScheduleEntry::getIntervalMantissa() const
{
    return this->intervalMantissa;
}

void twtScheduleEntry::setIntervalMantissa(uint16_t intervalMantissa)
{
    this->intervalMantissa = intervalMantissa;
}

uint8_t twtScheduleEntry::getIntervalExponent() const
{
    return this->intervalExponent;
}

void twtScheduleEntry::setIntervalExponent(uint8_t intervalExponent)
{
    this->intervalExponent = intervalExponent;
}

uint8_t twtScheduleEntry::getRestriction() const
{
    return this->restriction;
}

void twtScheduleEntry::setRestriction(uint8_t restriction)
{
    this->restriction = restriction;
}

class twtScheduleEntryDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_staAddress,
        FIELD_wakeTime,
        FIELD_wakeDuration,
        FIELD_intervalMantissa,
        FIELD_intervalExponent,
        FIELD_restriction,
    };
  public:
    twtScheduleEntryDescriptor();
    virtual ~twtScheduleEntryDescriptor();

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

Register_ClassDescriptor(twtScheduleEntryDescriptor)

twtScheduleEntryDescriptor::twtScheduleEntryDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::ieee80211::twtScheduleEntry)), "")
{
    propertyNames = nullptr;
}

twtScheduleEntryDescriptor::~twtScheduleEntryDescriptor()
{
    delete[] propertyNames;
}

bool twtScheduleEntryDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<twtScheduleEntry *>(obj)!=nullptr;
}

const char **twtScheduleEntryDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *twtScheduleEntryDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int twtScheduleEntryDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 6+base->getFieldCount() : 6;
}

unsigned int twtScheduleEntryDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        0,    // FIELD_staAddress
        FD_ISEDITABLE,    // FIELD_wakeTime
        FD_ISEDITABLE,    // FIELD_wakeDuration
        FD_ISEDITABLE,    // FIELD_intervalMantissa
        FD_ISEDITABLE,    // FIELD_intervalExponent
        FD_ISEDITABLE,    // FIELD_restriction
    };
    return (field >= 0 && field < 6) ? fieldTypeFlags[field] : 0;
}

const char *twtScheduleEntryDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "staAddress",
        "wakeTime",
        "wakeDuration",
        "intervalMantissa",
        "intervalExponent",
        "restriction",
    };
    return (field >= 0 && field < 6) ? fieldNames[field] : nullptr;
}

int twtScheduleEntryDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "staAddress") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "wakeTime") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "wakeDuration") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "intervalMantissa") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "intervalExponent") == 0) return baseIndex + 4;
    if (strcmp(fieldName, "restriction") == 0) return baseIndex + 5;
    return base ? base->findField(fieldName) : -1;
}

const char *twtScheduleEntryDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::MacAddress",    // FIELD_staAddress
        "omnetpp::simtime_t",    // FIELD_wakeTime
        "uint32_t",    // FIELD_wakeDuration
        "uint16_t",    // FIELD_intervalMantissa
        "uint8_t",    // FIELD_intervalExponent
        "uint8_t",    // FIELD_restriction
    };
    return (field >= 0 && field < 6) ? fieldTypeStrings[field] : nullptr;
}

const char **twtScheduleEntryDescriptor::getFieldPropertyNames(int field) const
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

const char *twtScheduleEntryDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int twtScheduleEntryDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    twtScheduleEntry *pp = omnetpp::fromAnyPtr<twtScheduleEntry>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void twtScheduleEntryDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    twtScheduleEntry *pp = omnetpp::fromAnyPtr<twtScheduleEntry>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'twtScheduleEntry'", field);
    }
}

const char *twtScheduleEntryDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    twtScheduleEntry *pp = omnetpp::fromAnyPtr<twtScheduleEntry>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string twtScheduleEntryDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    twtScheduleEntry *pp = omnetpp::fromAnyPtr<twtScheduleEntry>(object); (void)pp;
    switch (field) {
        case FIELD_staAddress: return pp->getStaAddress().str();
        case FIELD_wakeTime: return simtime2string(pp->getWakeTime());
        case FIELD_wakeDuration: return ulong2string(pp->getWakeDuration());
        case FIELD_intervalMantissa: return ulong2string(pp->getIntervalMantissa());
        case FIELD_intervalExponent: return ulong2string(pp->getIntervalExponent());
        case FIELD_restriction: return ulong2string(pp->getRestriction());
        default: return "";
    }
}

void twtScheduleEntryDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    twtScheduleEntry *pp = omnetpp::fromAnyPtr<twtScheduleEntry>(object); (void)pp;
    switch (field) {
        case FIELD_wakeTime: pp->setWakeTime(string2simtime(value)); break;
        case FIELD_wakeDuration: pp->setWakeDuration(string2ulong(value)); break;
        case FIELD_intervalMantissa: pp->setIntervalMantissa(string2ulong(value)); break;
        case FIELD_intervalExponent: pp->setIntervalExponent(string2ulong(value)); break;
        case FIELD_restriction: pp->setRestriction(string2ulong(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtScheduleEntry'", field);
    }
}

omnetpp::cValue twtScheduleEntryDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    twtScheduleEntry *pp = omnetpp::fromAnyPtr<twtScheduleEntry>(object); (void)pp;
    switch (field) {
        case FIELD_staAddress: return omnetpp::toAnyPtr(&pp->getStaAddress()); break;
        case FIELD_wakeTime: return pp->getWakeTime().dbl();
        case FIELD_wakeDuration: return (omnetpp::intval_t)(pp->getWakeDuration());
        case FIELD_intervalMantissa: return (omnetpp::intval_t)(pp->getIntervalMantissa());
        case FIELD_intervalExponent: return (omnetpp::intval_t)(pp->getIntervalExponent());
        case FIELD_restriction: return (omnetpp::intval_t)(pp->getRestriction());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'twtScheduleEntry' as cValue -- field index out of range?", field);
    }
}

void twtScheduleEntryDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    twtScheduleEntry *pp = omnetpp::fromAnyPtr<twtScheduleEntry>(object); (void)pp;
    switch (field) {
        case FIELD_wakeTime: pp->setWakeTime(value.doubleValue()); break;
        case FIELD_wakeDuration: pp->setWakeDuration(omnetpp::checked_int_cast<uint32_t>(value.intValue())); break;
        case FIELD_intervalMantissa: pp->setIntervalMantissa(omnetpp::checked_int_cast<uint16_t>(value.intValue())); break;
        case FIELD_intervalExponent: pp->setIntervalExponent(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        case FIELD_restriction: pp->setRestriction(omnetpp::checked_int_cast<uint8_t>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtScheduleEntry'", field);
    }
}

const char *twtScheduleEntryDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr twtScheduleEntryDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    twtScheduleEntry *pp = omnetpp::fromAnyPtr<twtScheduleEntry>(object); (void)pp;
    switch (field) {
        case FIELD_staAddress: return omnetpp::toAnyPtr(&pp->getStaAddress()); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void twtScheduleEntryDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    twtScheduleEntry *pp = omnetpp::fromAnyPtr<twtScheduleEntry>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtScheduleEntry'", field);
    }
}

Register_Class(twtScheduleBeacon)

twtScheduleBeacon::twtScheduleBeacon() : ::inet::FieldsChunk()
{
}

twtScheduleBeacon::twtScheduleBeacon(const twtScheduleBeacon& other) : ::inet::FieldsChunk(other)
{
    copy(other);
}

twtScheduleBeacon::~twtScheduleBeacon()
{
    delete [] this->scheduleEntries;
}

twtScheduleBeacon& twtScheduleBeacon::operator=(const twtScheduleBeacon& other)
{
    if (this == &other) return *this;
    ::inet::FieldsChunk::operator=(other);
    copy(other);
    return *this;
}

void twtScheduleBeacon::copy(const twtScheduleBeacon& other)
{
    delete [] this->scheduleEntries;
    this->scheduleEntries = (other.scheduleEntries_arraysize==0) ? nullptr : new twtScheduleEntry[other.scheduleEntries_arraysize];
    scheduleEntries_arraysize = other.scheduleEntries_arraysize;
    for (size_t i = 0; i < scheduleEntries_arraysize; i++) {
        this->scheduleEntries[i] = other.scheduleEntries[i];
    }
}

void twtScheduleBeacon::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::inet::FieldsChunk::parsimPack(b);
    b->pack(scheduleEntries_arraysize);
    doParsimArrayPacking(b,this->scheduleEntries,scheduleEntries_arraysize);
}

void twtScheduleBeacon::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::inet::FieldsChunk::parsimUnpack(b);
    delete [] this->scheduleEntries;
    b->unpack(scheduleEntries_arraysize);
    if (scheduleEntries_arraysize == 0) {
        this->scheduleEntries = nullptr;
    } else {
        this->scheduleEntries = new twtScheduleEntry[scheduleEntries_arraysize];
        doParsimArrayUnpacking(b,this->scheduleEntries,scheduleEntries_arraysize);
    }
}

size_t twtScheduleBeacon::getScheduleEntriesArraySize() const
{
    return scheduleEntries_arraysize;
}

const twtScheduleEntry& twtScheduleBeacon::getScheduleEntries(size_t k) const
{
    if (k >= scheduleEntries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)scheduleEntries_arraysize, (unsigned long)k);
    return this->scheduleEntries[k];
}

void twtScheduleBeacon::setScheduleEntriesArraySize(size_t newSize)
{
    handleChange();
    twtScheduleEntry *scheduleEntries2 = (newSize==0) ? nullptr : new twtScheduleEntry[newSize];
    size_t minSize = scheduleEntries_arraysize < newSize ? scheduleEntries_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        scheduleEntries2[i] = this->scheduleEntries[i];
    delete [] this->scheduleEntries;
    this->scheduleEntries = scheduleEntries2;
    scheduleEntries_arraysize = newSize;
}

void twtScheduleBeacon::setScheduleEntries(size_t k, const twtScheduleEntry& scheduleEntries)
{
    if (k >= scheduleEntries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)scheduleEntries_arraysize, (unsigned long)k);
    handleChange();
    this->scheduleEntries[k] = scheduleEntries;
}

void twtScheduleBeacon::insertScheduleEntries(size_t k, const twtScheduleEntry& scheduleEntries)
{
    if (k > scheduleEntries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)scheduleEntries_arraysize, (unsigned long)k);
    handleChange();
    size_t newSize = scheduleEntries_arraysize + 1;
    twtScheduleEntry *scheduleEntries2 = new twtScheduleEntry[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        scheduleEntries2[i] = this->scheduleEntries[i];
    scheduleEntries2[k] = scheduleEntries;
    for (i = k + 1; i < newSize; i++)
        scheduleEntries2[i] = this->scheduleEntries[i-1];
    delete [] this->scheduleEntries;
    this->scheduleEntries = scheduleEntries2;
    scheduleEntries_arraysize = newSize;
}

void twtScheduleBeacon::appendScheduleEntries(const twtScheduleEntry& scheduleEntries)
{
    insertScheduleEntries(scheduleEntries_arraysize, scheduleEntries);
}

void twtScheduleBeacon::eraseScheduleEntries(size_t k)
{
    if (k >= scheduleEntries_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)scheduleEntries_arraysize, (unsigned long)k);
    handleChange();
    size_t newSize = scheduleEntries_arraysize - 1;
    twtScheduleEntry *scheduleEntries2 = (newSize == 0) ? nullptr : new twtScheduleEntry[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        scheduleEntries2[i] = this->scheduleEntries[i];
    for (i = k; i < newSize; i++)
        scheduleEntries2[i] = this->scheduleEntries[i+1];
    delete [] this->scheduleEntries;
    this->scheduleEntries = scheduleEntries2;
    scheduleEntries_arraysize = newSize;
}

class twtScheduleBeaconDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_scheduleEntries,
    };
  public:
    twtScheduleBeaconDescriptor();
    virtual ~twtScheduleBeaconDescriptor();

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

Register_ClassDescriptor(twtScheduleBeaconDescriptor)

twtScheduleBeaconDescriptor::twtScheduleBeaconDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(inet::ieee80211::twtScheduleBeacon)), "inet::FieldsChunk")
{
    propertyNames = nullptr;
}

twtScheduleBeaconDescriptor::~twtScheduleBeaconDescriptor()
{
    delete[] propertyNames;
}

bool twtScheduleBeaconDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<twtScheduleBeacon *>(obj)!=nullptr;
}

const char **twtScheduleBeaconDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *twtScheduleBeaconDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int twtScheduleBeaconDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int twtScheduleBeaconDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISCOMPOUND | FD_ISRESIZABLE,    // FIELD_scheduleEntries
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *twtScheduleBeaconDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "scheduleEntries",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int twtScheduleBeaconDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "scheduleEntries") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *twtScheduleBeaconDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "inet::ieee80211::twtScheduleEntry",    // FIELD_scheduleEntries
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **twtScheduleBeaconDescriptor::getFieldPropertyNames(int field) const
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

const char *twtScheduleBeaconDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int twtScheduleBeaconDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    twtScheduleBeacon *pp = omnetpp::fromAnyPtr<twtScheduleBeacon>(object); (void)pp;
    switch (field) {
        case FIELD_scheduleEntries: return pp->getScheduleEntriesArraySize();
        default: return 0;
    }
}

void twtScheduleBeaconDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    twtScheduleBeacon *pp = omnetpp::fromAnyPtr<twtScheduleBeacon>(object); (void)pp;
    switch (field) {
        case FIELD_scheduleEntries: pp->setScheduleEntriesArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'twtScheduleBeacon'", field);
    }
}

const char *twtScheduleBeaconDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    twtScheduleBeacon *pp = omnetpp::fromAnyPtr<twtScheduleBeacon>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string twtScheduleBeaconDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    twtScheduleBeacon *pp = omnetpp::fromAnyPtr<twtScheduleBeacon>(object); (void)pp;
    switch (field) {
        case FIELD_scheduleEntries: return "";
        default: return "";
    }
}

void twtScheduleBeaconDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    twtScheduleBeacon *pp = omnetpp::fromAnyPtr<twtScheduleBeacon>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtScheduleBeacon'", field);
    }
}

omnetpp::cValue twtScheduleBeaconDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    twtScheduleBeacon *pp = omnetpp::fromAnyPtr<twtScheduleBeacon>(object); (void)pp;
    switch (field) {
        case FIELD_scheduleEntries: return omnetpp::toAnyPtr(&pp->getScheduleEntries(i)); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'twtScheduleBeacon' as cValue -- field index out of range?", field);
    }
}

void twtScheduleBeaconDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    twtScheduleBeacon *pp = omnetpp::fromAnyPtr<twtScheduleBeacon>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtScheduleBeacon'", field);
    }
}

const char *twtScheduleBeaconDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_scheduleEntries: return omnetpp::opp_typename(typeid(twtScheduleEntry));
        default: return nullptr;
    };
}

omnetpp::any_ptr twtScheduleBeaconDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    twtScheduleBeacon *pp = omnetpp::fromAnyPtr<twtScheduleBeacon>(object); (void)pp;
    switch (field) {
        case FIELD_scheduleEntries: return omnetpp::toAnyPtr(&pp->getScheduleEntries(i)); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void twtScheduleBeaconDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    twtScheduleBeacon *pp = omnetpp::fromAnyPtr<twtScheduleBeacon>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'twtScheduleBeacon'", field);
    }
}

}  // namespace ieee80211
}  // namespace inet

namespace omnetpp {

}  // namespace omnetpp

