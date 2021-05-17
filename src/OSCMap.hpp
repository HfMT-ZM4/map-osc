#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <type_traits>

#ifdef WITH_EIGEN
#include <Eigen/Dense>
#endif

struct OSCMap;

class OSCAtom
{
    
public:
    OSCAtom(float val )         : type('f'), f_val(val) {}
    OSCAtom(double val )        : type('d'), d_val(val) {}
    OSCAtom(int32_t val )       : type('i'), i_val(val) {}
    OSCAtom(int64_t val )       : type('l'), l_val(val) {}
    OSCAtom(char val )          : type('c'), c_val(val) {}
    OSCAtom(bool val )          : type('b'), b_val(val) {}
    OSCAtom(const std::string val )  : type('s'), str_val(val) {}
    OSCAtom(const char * val )  : type('s'), str_val(val) {}
    OSCAtom(const OSCMap & val) : type('.'), map_val( std::make_unique<OSCMap>( val ) ) {}
    OSCAtom(const OSCAtom & other);
    ~OSCAtom(){}
    
    template <typename T> T get();
    
    inline int getInt() { return (int)get<int32_t>(); }
    inline float getFloat(){ return (float)get<float>(); }
    
    inline char typetag() {
       if( type == 'b' )
           return b_val ? 'T' : 'F';
       else
           return type;
    }
    
    size_t getSizeInBytes();
    
    
private:

    const char type;
    
    union {
        float f_val;
        double d_val;
        int32_t i_val;
        int64_t l_val;
        char c_val;
        bool b_val;

    };

    std::unique_ptr<OSCMap> map_val;
    
    std::string str_val;

    
};


class OSCAtomVector
{
    
public:
    
    OSCAtomVector(){}
    OSCAtomVector( std::vector< std::unique_ptr<OSCAtom> > vec );
    OSCAtomVector( const OSCAtomVector & other );
    
    OSCAtomVector( std::vector< OSCAtom > vec )
    {
        for( auto & v : vec )
            obj_vec.emplace_back(std::make_unique<OSCAtom>(v));
    }

    OSCAtomVector(float val ) {   obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    OSCAtomVector(double val ) {  obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    OSCAtomVector(int32_t val ) {  obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    OSCAtomVector(int64_t val ) {  obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    OSCAtomVector(char val ) {  obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    OSCAtomVector(const char * val ) {  obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    OSCAtomVector(const std::string& val ) {  obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    OSCAtomVector(const OSCMap & val ) {obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }

    inline void appendValue(float val ) {   obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    inline void appendValue(double val ) {  obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    inline void appendValue(int32_t val ) { obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    inline void appendValue(int64_t val ) { obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    inline void appendValue(char val ) {    obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    inline void appendValue(const char * val ) { obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    inline void appendValue(const std::string& val ) { obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    inline void appendValue(bool val ) { obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }
    inline void appendValue(const OSCMap & val ) { obj_vec.emplace_back(std::make_unique<OSCAtom>(val)); }

    
    template <typename Derived>
    void appendValue(std::vector<Derived> &val)
    {
       for( auto it = val.begin(); it != val.end(); it++ )
       {
           appendValue(*it);
       }
    }

    template <typename Derived>
    void appendValue(const std::vector<Derived> &val)
    {
        for( auto it = val.begin(); it != val.end(); it++ )
        {
            appendValue(*it);
        }
    }
    

#ifdef WITH_EIGEN
    template <typename Derived>
    void appendValue(const Eigen::ArrayBase<Derived> &val)
    {
        if( val.size() > 0 )
        {
           for( size_t i = 0; i < val.rows(); ++i)
           {
               appendValue( val(i) );
           }
        }
    }
    
    template <typename Derived>
    void appendValue(Eigen::ArrayBase<Derived> &val)
    {
        if( val.size() > 0 )
        {
           for( size_t i = 0; i < val.rows(); ++i)
           {
               appendValue( val(i) );
           }
        }
    }
#endif
    
    
    template <typename T>
    inline T get(size_t idx = 0) {
        return obj_vec[idx]->get<T>();
    }
    
    OSCAtom& operator[](size_t idx) { return *obj_vec[idx]; }

    inline std::vector< std::unique_ptr<OSCAtom> > & getAtomVector(){ return obj_vec; }
    
    inline std::vector<char> typetags()
    {
        std::vector<char> tags;
        for( auto& it : obj_vec )
        {
            tags.emplace_back( it->typetag() );
        }
        return tags;
    }
    
    inline void reserve(size_t size){ obj_vec.reserve(size); }
    inline size_t size(){ return obj_vec.size(); }
    
    void print(int tabs = 0) const;


private:
    
    std::vector< std::unique_ptr<OSCAtom> > obj_vec;

};


class OSCMap
{
    
public:
    
    OSCMap(){}
    OSCMap( const OSCMap & other );
    OSCMap( long len, char * ptr ) { inputOSC(len, ptr); }
    OSCMap (std::unordered_map<std::string, OSCAtomVector > ){
        printf("test");
    }
    
    void inputOSC( long len, char * ptr );

    inline void addMessage(const char* address)
    {
        if (!address_lookup.count(address))
            address_lookup[address] = std::make_unique<OSCAtomVector>();
    }
    
    template <typename... Ts>
    void addMessage (const std::string& address, Ts&&... args)
    {
        using expand = int[];
        
        if( !address_lookup.count(address) )
            address_lookup[address] = std::make_unique<OSCAtomVector>();
        
        (void)expand{0, ((void)address_lookup[address]->appendValue( std::forward<Ts>(args) ), 0) ... };
    }

    template <typename... Ts>
    void addMessage (const char * address, Ts&&... args)
    {
        using expand = int[];
        
        if( !address_lookup.count(address) )
            address_lookup[address] = std::make_unique<OSCAtomVector>();
        
        (void)expand{0, ((void)address_lookup[address]->appendValue( std::forward<Ts>(args) ), 0) ... };
    }

    OSCAtomVector* operator[](std::string& addr) { return address_lookup[addr].get(); }
    OSCAtomVector* operator[](const char * addr) { return address_lookup[addr].get(); }
    
    inline bool addressExists(const char * address) {
        return address_lookup.count(address);
    }
       
    inline OSCAtomVector* getMessage(const char * address) {
       return address_lookup[address].get();
    }
    
    std::string getSerializedString() const;

    size_t getSerializedSizeInBytes() const;
    void serializeIntoBuffer(char *ptr, size_t size ) const;
    
    void print(int tabs = 0) const;

        
private:
    
    std::unordered_map<std::string, std::unique_ptr<OSCAtomVector>> address_lookup;
    
};
