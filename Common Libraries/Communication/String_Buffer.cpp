#include"String_Buffer.h"

/*String_Buffer::String_Buffer(){
  for(int i=0;i<buffer_length;i++ ){
    List[ i ].reserve(64);
  }
}*/

void String_Buffer::add_one(int *var){
    *var += 1;
    if ( var == buffer_length ){
      *var = 0;
    }
  }

  void String_Buffer::Write(String str){
    List[index_to_write] = str;
    add_one(&index_to_write);
    // check if the buffer is full
    if( index_to_write == index_to_read  ){
      // write all the buffer without reading
      Write("The string buffer is full and overflow");
    }
   }

      String String_Buffer::Read(){
    //if (streaming == true){
      if(index_to_read==index_to_write){
        return "None";
      }
      else{
        return List[index_to_read] ;
        add_one(&index_to_read);
      }
    /*}
    else{
      return "No streaming";
    }*/
   }

   String String_Buffer::Resend_Last_message(){
    index_to_read -= 1;
    return Read();
   }
