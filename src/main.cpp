
#include <iostream>
#include "OSCMap.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    

    OSCMap bndl, bndl2;

    // adding a message
    bndl.addMessage("/float", (float)1.1);
    bndl.addMessage("/double", (double)2.2);
    bndl.addMessage("/int", (int32_t)10);
    bndl.addMessage("/long", (int64_t)20);
    bndl.addMessage("/string", "hello osc!");
    
    // adding a message with a list
    bndl.addMessage("/bar", "hei", 2, 3, 4 );
    
    // creating an empty message
    bndl2.addMessage("/accumuate");
    
    
    for( int i = 0; i < 10; i++ )
    {
        // appending values to make a list iterably
        bndl2["/accumuate"]->appendValue(i);
        // note that the messages are pointers
    }
    
    // adding a subbbundle
    bndl.addMessage("/sub", bndl2);
    
    // print
    bndl.print();
    
    //
    cout << "this is a float " << bndl["/float"]->get<float>() << endl;
    
    // by default get<>() returns the first element
    cout << "this is the first element of /bar " << bndl["/bar"]->get<string>() << endl;
    
    cout << "this is the second element /bar " << bndl["/bar"]->get<string>(1) << endl;

    cout << bndl["/float"]->size() << endl;
    
    // this will crash since we ask for an element that is outside the list size
    // cout << "this is the second element /float " << bndl["/float"]->get<string>(1) << endl;

  
    // example of writing into a std::string
    string serialString = bndl.getSerializedString();
    
    // test printing the bytes
    /*
    for( int i = 0; i < serialString.size(); i++ )
    {
       printf("%d 0x%x %d\n", i, (unsigned char)serialString[i], (unsigned char)serialString[i]);
    }
    */
    
    // example of reading an OSC encoded char array
    OSCMap deserialized( serialString.length(), serialString.data() );
    cout << "\n printing the deserialized bundle: \n " << endl;
    deserialized.print();
    
   
    /*
    // example of writing into a C char array
    size_t size = bndl.getSerializedSizeInBytes();
    char * ptr = (char *)malloc(size);

    bndl.serializeIntoBuffer(ptr, size);

    for( int i = 0; i < size; i++ )
    {
       printf("%d 0x%x %d\n", i, (unsigned char)ptr[i], (unsigned char)ptr[i]);
    }

    free(ptr);
     */

    /*
    // in development, figuring out a method for literal definition
    OSCMap test({
        {"/t", 1},
        {"/sub",
            OSCMap({
                {"/foo", 1} // << unfortunately a list doesn't work here..
            })
        }
    });
    */
    
    
    
    return 0;
}
