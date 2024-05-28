#ifndef STRING_BUFFER
#define STRING_BUFFER

#include <Arduino.h>
# define buffer_length  10

class String_Buffer{

  private:
    String List[ buffer_length ];
    int index_to_read = 0;
    int index_to_write = 0;
    void add_one(int *var);

  public:
    //String_Buffer();
   //bool streaming = false;
   void Write(String str); 
   String Read();
   String Resend_Last_message();
};

#endif
