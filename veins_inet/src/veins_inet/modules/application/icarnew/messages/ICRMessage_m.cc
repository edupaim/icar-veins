//
// Generated file, do not edit! Created by nedtool 5.3 from veins_inet/modules/application/icarnew/messages/ICRMessage.msg.
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
#include "ICRMessage_m.h"

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
    for (int i=0; i<n; i++) {
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
    for (int i=0; i<n; i++) {
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
    for (int i=0; i<n; i++) {
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


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(ICRMessage)

ICRMessage::ICRMessage(const char *name, short kind) : ::WaveShortMessage(name,kind)
{
    this->numMsg = 0;
    this->msgType = 0;
    this->msgLifeTime = 0;
    this->validityDataTimeStamp = 0;
    this->confidence = 0;
    this->hopNumber = 0;
    this->msgTimeStamp = 0;
    routeNodes_arraysize = 0;
    this->routeNodes = 0;
}

ICRMessage::ICRMessage(const ICRMessage& other) : ::WaveShortMessage(other)
{
    routeNodes_arraysize = 0;
    this->routeNodes = 0;
    copy(other);
}

ICRMessage::~ICRMessage()
{
    delete [] this->routeNodes;
}

ICRMessage& ICRMessage::operator=(const ICRMessage& other)
{
    if (this==&other) return *this;
    ::WaveShortMessage::operator=(other);
    copy(other);
    return *this;
}

void ICRMessage::copy(const ICRMessage& other)
{
    this->numMsg = other.numMsg;
    this->msgType = other.msgType;
    this->msgLifeTime = other.msgLifeTime;
    this->validityDataTimeStamp = other.validityDataTimeStamp;
    this->confidence = other.confidence;
    this->hopNumber = other.hopNumber;
    this->msgTimeStamp = other.msgTimeStamp;
    delete [] this->routeNodes;
    this->routeNodes = (other.routeNodes_arraysize==0) ? nullptr : new ICRNode[other.routeNodes_arraysize];
    routeNodes_arraysize = other.routeNodes_arraysize;
    for (unsigned int i=0; i<routeNodes_arraysize; i++)
        this->routeNodes[i] = other.routeNodes[i];
}

void ICRMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::WaveShortMessage::parsimPack(b);
    doParsimPacking(b,this->numMsg);
    doParsimPacking(b,this->msgType);
    doParsimPacking(b,this->msgLifeTime);
    doParsimPacking(b,this->validityDataTimeStamp);
    doParsimPacking(b,this->confidence);
    doParsimPacking(b,this->hopNumber);
    doParsimPacking(b,this->msgTimeStamp);
    b->pack(routeNodes_arraysize);
    doParsimArrayPacking(b,this->routeNodes,routeNodes_arraysize);
}

void ICRMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::WaveShortMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->numMsg);
    doParsimUnpacking(b,this->msgType);
    doParsimUnpacking(b,this->msgLifeTime);
    doParsimUnpacking(b,this->validityDataTimeStamp);
    doParsimUnpacking(b,this->confidence);
    doParsimUnpacking(b,this->hopNumber);
    doParsimUnpacking(b,this->msgTimeStamp);
    delete [] this->routeNodes;
    b->unpack(routeNodes_arraysize);
    if (routeNodes_arraysize==0) {
        this->routeNodes = 0;
    } else {
        this->routeNodes = new ICRNode[routeNodes_arraysize];
        doParsimArrayUnpacking(b,this->routeNodes,routeNodes_arraysize);
    }
}

unsigned long ICRMessage::getNumMsg() const
{
    return this->numMsg;
}

void ICRMessage::setNumMsg(unsigned long numMsg)
{
    this->numMsg = numMsg;
}

int ICRMessage::getMsgType() const
{
    return this->msgType;
}

void ICRMessage::setMsgType(int msgType)
{
    this->msgType = msgType;
}

::omnetpp::simtime_t ICRMessage::getMsgLifeTime() const
{
    return this->msgLifeTime;
}

void ICRMessage::setMsgLifeTime(::omnetpp::simtime_t msgLifeTime)
{
    this->msgLifeTime = msgLifeTime;
}

::omnetpp::simtime_t ICRMessage::getValidityDataTimeStamp() const
{
    return this->validityDataTimeStamp;
}

void ICRMessage::setValidityDataTimeStamp(::omnetpp::simtime_t validityDataTimeStamp)
{
    this->validityDataTimeStamp = validityDataTimeStamp;
}

double ICRMessage::getConfidence() const
{
    return this->confidence;
}

void ICRMessage::setConfidence(double confidence)
{
    this->confidence = confidence;
}

unsigned int ICRMessage::getHopNumber() const
{
    return this->hopNumber;
}

void ICRMessage::setHopNumber(unsigned int hopNumber)
{
    this->hopNumber = hopNumber;
}

::omnetpp::simtime_t ICRMessage::getMsgTimeStamp() const
{
    return this->msgTimeStamp;
}

void ICRMessage::setMsgTimeStamp(::omnetpp::simtime_t msgTimeStamp)
{
    this->msgTimeStamp = msgTimeStamp;
}

void ICRMessage::setRouteNodesArraySize(unsigned int size)
{
    ICRNode *routeNodes2 = (size==0) ? nullptr : new ICRNode[size];
    unsigned int sz = routeNodes_arraysize < size ? routeNodes_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        routeNodes2[i] = this->routeNodes[i];
    routeNodes_arraysize = size;
    delete [] this->routeNodes;
    this->routeNodes = routeNodes2;
}

unsigned int ICRMessage::getRouteNodesArraySize() const
{
    return routeNodes_arraysize;
}

ICRNode& ICRMessage::getRouteNodes(unsigned int k)
{
    if (k>=routeNodes_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", routeNodes_arraysize, k);
    return this->routeNodes[k];
}

void ICRMessage::setRouteNodes(unsigned int k, const ICRNode& routeNodes)
{
    if (k>=routeNodes_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", routeNodes_arraysize, k);
    this->routeNodes[k] = routeNodes;
}

class ICRMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    ICRMessageDescriptor();
    virtual ~ICRMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(ICRMessageDescriptor)

ICRMessageDescriptor::ICRMessageDescriptor() : omnetpp::cClassDescriptor("ICRMessage", "WaveShortMessage")
{
    propertynames = nullptr;
}

ICRMessageDescriptor::~ICRMessageDescriptor()
{
    delete[] propertynames;
}

bool ICRMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ICRMessage *>(obj)!=nullptr;
}

const char **ICRMessageDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *ICRMessageDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int ICRMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int ICRMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISCOMPOUND,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *ICRMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "numMsg",
        "msgType",
        "msgLifeTime",
        "validityDataTimeStamp",
        "confidence",
        "hopNumber",
        "msgTimeStamp",
        "routeNodes",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int ICRMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='n' && strcmp(fieldName, "numMsg")==0) return base+0;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgType")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgLifeTime")==0) return base+2;
    if (fieldName[0]=='v' && strcmp(fieldName, "validityDataTimeStamp")==0) return base+3;
    if (fieldName[0]=='c' && strcmp(fieldName, "confidence")==0) return base+4;
    if (fieldName[0]=='h' && strcmp(fieldName, "hopNumber")==0) return base+5;
    if (fieldName[0]=='m' && strcmp(fieldName, "msgTimeStamp")==0) return base+6;
    if (fieldName[0]=='r' && strcmp(fieldName, "routeNodes")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *ICRMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "unsigned long",
        "int",
        "simtime_t",
        "simtime_t",
        "double",
        "unsigned int",
        "simtime_t",
        "ICRNode",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **ICRMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *ICRMessageDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int ICRMessageDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    ICRMessage *pp = (ICRMessage *)object; (void)pp;
    switch (field) {
        case 7: return pp->getRouteNodesArraySize();
        default: return 0;
    }
}

const char *ICRMessageDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    ICRMessage *pp = (ICRMessage *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ICRMessageDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    ICRMessage *pp = (ICRMessage *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getNumMsg());
        case 1: return long2string(pp->getMsgType());
        case 2: return simtime2string(pp->getMsgLifeTime());
        case 3: return simtime2string(pp->getValidityDataTimeStamp());
        case 4: return double2string(pp->getConfidence());
        case 5: return ulong2string(pp->getHopNumber());
        case 6: return simtime2string(pp->getMsgTimeStamp());
        case 7: {std::stringstream out; out << pp->getRouteNodes(i); return out.str();}
        default: return "";
    }
}

bool ICRMessageDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    ICRMessage *pp = (ICRMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setNumMsg(string2ulong(value)); return true;
        case 1: pp->setMsgType(string2long(value)); return true;
        case 2: pp->setMsgLifeTime(string2simtime(value)); return true;
        case 3: pp->setValidityDataTimeStamp(string2simtime(value)); return true;
        case 4: pp->setConfidence(string2double(value)); return true;
        case 5: pp->setHopNumber(string2ulong(value)); return true;
        case 6: pp->setMsgTimeStamp(string2simtime(value)); return true;
        default: return false;
    }
}

const char *ICRMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 7: return omnetpp::opp_typename(typeid(ICRNode));
        default: return nullptr;
    };
}

void *ICRMessageDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    ICRMessage *pp = (ICRMessage *)object; (void)pp;
    switch (field) {
        case 7: return (void *)(&pp->getRouteNodes(i)); break;
        default: return nullptr;
    }
}


