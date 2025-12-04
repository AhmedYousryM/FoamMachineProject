// 1.Golbal variable to be added to isindex -- done
// 2.Save Response (handle) after recieving a response to question --?? 
// 3.arr(Struct) -- done
/*
    Struct variable{
    char datatype;
    union  value; -> size for big data type and for string save addres.
    }
*/ 
// 4. add answer index check -- done
// 5. Serial to be set as variable -- done
// 6. add periorty -- done


// 1. //Fc(code)=msg; Saving resonse -- done
// 2. isindex!! -- done

// 3. Reverse sender and reciever -- done
// 4. Res -> FC045=25 -- done

// 5. Resend after certain time = 300ms -- done
// 6. add cases to update var --done
// 7. add byte array to Variable struct --done 
// 8. remove last time pointer from message and use a vriable instead --done
// 9. priorty 30 condition!! --done
// 10. Overflow and timer to resend --done

/*
---- sender -------
Serial.print(FC101=);
Serial.write(highByte(FCVariable[101].value.intvalue));
Serial.write(lowByte(FCVariable[101].value.intvalue));
Serial.write('\0');

---- reciver ---
char msg_str[MAX_MESSAGE_LENGTH];
Serial.readBytesUntil('\0', msg_str,sizeof(msg_str));

*/
