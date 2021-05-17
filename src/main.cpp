
#include <iostream>
#include "OSCMap.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    
    /*
    OSCAtomVector vec;
    
    vec.appendValue(1.1);
    vec.appendValue("foo");

    vec.print();
    
    OSCAtomVector vec2;
    
    vec2.appendValue( vec );
    
    vec2.print();

*/
    OSCMap bndl, bndl2;

    bndl2.addMessage("/bar", "hei", 2, 3, 4 );
    bndl2.addMessage("/self_sub", bndl2);
    bndl.addMessage("/foo", bndl2);
    
    bndl2.print();
   // cout << bndl["/foo"].get<string>() << endl;
  
    
    OSCMap test({
        {"/t", 1},
        {"/sub",
            OSCMap({
                {"/foo", 1} // << unfortunately a list doesn't work here..
            })
        }
    });
    
    
    
    
    
    /*
    size_t size = bndl.getSerializedSizeInBytes();
    char * ptr = (char *)malloc(size);
    
    bndl.serializeIntoBuffer(ptr, size);
    
    
    for( int i = 0; i < size; i++ )
    {
        //printf("%i %c %u\n", i, ptr[i], (unsigned int)ptr[i] );
        printf("%d 0x%x %d\n", i, (unsigned char)ptr[i], (unsigned char)ptr[i]);
        //std::cout << i << " " << ptr[i] << " " << (unsigned int)ptr[i] << std::endl;
    }
    
    free(ptr);

    */

    string serialString = bndl.getSerializedString();
    for( int i = 0; i < serialString.size(); i++ )
    {
       printf("%d 0x%x %d\n", i, (unsigned char)serialString[i], (unsigned char)serialString[i]);
    }
    
    OSCMap deserialized( serialString.length(), serialString.data() );
    
  //  deserialized.print();
    return 0;
}
