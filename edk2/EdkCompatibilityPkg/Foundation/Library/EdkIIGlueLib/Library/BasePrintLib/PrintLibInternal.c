/*++

Copyright (c) 2004 - 2009, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.  


Module Name:

  PrintLibInternal.c
  
Abstract: 

  Print Library worker functions.

--*/

#include "PrintLibInternal.h"

GLOBAL_REMOVE_IF_UNREFERENCED CONST CHAR8 mHexStr[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};


/**
  Internal function that places the character into the Buffer.

  Internal function that places ASCII or Unicode character into the Buffer.

  @param  Buffer      Buffer to place the Unicode or ASCII string.
  @param  EndBuffer   The end of the input Buffer. No characters will be
                      placed after that. 
  @param  Length      Count of character to be placed into Buffer.
  @param  Character   Character to be placed into Buffer.
  @param  Increment   Character increment in Buffer.

  @return Number of characters printed.

**/
CHAR8 *
BasePrintLibFillBuffer (
  CHAR8   *Buffer,
  CHAR8   *EndBuffer,
  INTN    Length,
  UINTN   Character,
  INTN    Increment
  )
{
  INTN  Index;

  for (Index = 0; Index < Length && Buffer < EndBuffer; Index++) {
    *Buffer       =  (CHAR8) Character;
    *(Buffer + 1) =  (CHAR8) (Character >> 8);
    Buffer        += Increment;
  }
  return Buffer;
}

/**
  Internal function that convert a decimal number to a string in Buffer.

  Print worker function that convert a decimal number to a string in Buffer.

  @param  Buffer    Location to place the Unicode or ASCII string of Value.
  @param  Value     Value to convert to a Decimal or Hexidecimal string in Buffer.
  @param  Radix     Radix of the value

  @return Number of characters printed.

**/
UINTN
EFIAPI
BasePrintLibValueToString (
  IN OUT CHAR8  *Buffer,
  IN INT64      Value,
  IN UINTN      Radix
  )
{
  UINTN   Digits;
  UINT32  Remainder;

  //
  // Loop to convert one digit at a time in reverse order
  //
  *(Buffer++) = 0;
  Digits = 0;
  do {
    Value = (INT64)DivU64x32Remainder ((UINT64)Value, (UINT32)Radix, &Remainder);
    *(Buffer++) = mHexStr[Remainder];
    Digits++;
  } while (Value != 0);
  return Digits;
}

/**
  Internal function that converts a decimal value to a Null-terminated string.
  
  Converts the decimal number specified by Value to a Null-terminated  
  string specified by Buffer containing at most Width characters.
  If Width is 0 then a width of  MAXIMUM_VALUE_CHARACTERS is assumed.
  The number of characters in Buffer is returned not including the Null-terminator.
  If the conversion contains more than Width characters, then only the first
  Width characters are returned, and the total number of characters 
  required to perform the conversion is returned.
  Additional conversion parameters are specified in Flags.  
  The Flags bit LEFT_JUSTIFY is always ignored.
  All conversions are left justified in Buffer.
  If Width is 0, PREFIX_ZERO is ignored in Flags.
  If COMMA_TYPE is set in Flags, then PREFIX_ZERO is ignored in Flags, and commas
  are inserted every 3rd digit starting from the right.
  If RADIX_HEX is set in Flags, then the output buffer will be formatted in hexadecimal format.
  If Value is < 0 and RADIX_HEX is not set in Flags, then the fist character in Buffer is a '-'.
  If PREFIX_ZERO is set in Flags and PREFIX_ZERO is not being ignored, 
  then Buffer is padded with '0' characters so the combination of the optional '-' 
  sign character, '0' characters, digit characters for Value, and the Null-terminator
  add up to Width characters.
  If both COMMA_TYPE and RADIX_HEX are set in Flags, then ASSERT().

  If Buffer is NULL, then ASSERT().
  If unsupported bits are set in Flags, then ASSERT().
  If Width >= MAXIMUM_VALUE_CHARACTERS, then ASSERT()

  @param  Buffer    Pointer to the output buffer for the produced Null-terminated
                    string.
  @param  Flags     The bitmask of flags that specify left justification, zero pad,
                    and commas.
  @param  Value     The 64-bit signed value to convert to a string.
  @param  Width     The maximum number of characters to place in Buffer, not including
                    the Null-terminator.
  @param  Increment Character increment in Buffer.
  
  @return The number of characters in Buffer not including the Null-terminator.

**/
UINTN
BasePrintLibConvertValueToString (
  IN OUT CHAR8   *Buffer,
  IN UINTN       Flags,
  IN INT64       Value,
  IN UINTN       Width,
  IN UINTN       Increment
  )
{
  CHAR8  *OriginalBuffer;
  CHAR8  *EndBuffer;
  CHAR8  ValueBuffer[MAXIMUM_VALUE_CHARACTERS];
  UINTN  Count;
  UINTN  Digits;
  UINTN  Index;
  UINTN  Radix;

  ASSERT (Buffer != NULL);
  ASSERT (Width < MAXIMUM_VALUE_CHARACTERS);
  //
  // Make sure Flags can only contain supported bits.
  //
  ASSERT ((Flags & ~(LEFT_JUSTIFY | COMMA_TYPE | PREFIX_ZERO | RADIX_HEX)) == 0);

  //
  // If both COMMA_TYPE and RADIX_HEX are set, then ASSERT ()
  //
  ASSERT (((Flags & COMMA_TYPE) != 0 && (Flags & RADIX_HEX) != 0) == FALSE);

  OriginalBuffer = Buffer;

  if (Width == 0 || (Flags & COMMA_TYPE) != 0) {
    Flags &= (~PREFIX_ZERO);
  }

  if (Width == 0) {
    Width = MAXIMUM_VALUE_CHARACTERS - 1;
  }
  //
  // Set the tag for the end of the input Buffer.
  //
  EndBuffer = Buffer + Width * Increment;

  if ((Value < 0) && ((Flags & RADIX_HEX) == 0)) {
    Value = -Value;
    Buffer = BasePrintLibFillBuffer (Buffer, EndBuffer, 1, '-', Increment);
    Width--;
  }

  Radix = ((Flags & RADIX_HEX) == 0)? 10 : 16;
  Count = BasePrintLibValueToString (ValueBuffer, Value, Radix);

  if ((Flags & PREFIX_ZERO) != 0) {
    Buffer = BasePrintLibFillBuffer (Buffer, EndBuffer, Width - Count, '0', Increment);
  }

  Digits = Count % 3;
  if (Digits != 0) {
    Digits = 3 - Digits;
  }
  for (Index = 0; Index < Count; Index++) {
    Buffer = BasePrintLibFillBuffer (Buffer, EndBuffer, 1, ValueBuffer[Count - Index], Increment);
    if ((Flags & COMMA_TYPE) != 0) {
      Digits++;
      if (Digits == 3) {
        Digits = 0;
        if ((Index + 1) < Count) {
          Buffer = BasePrintLibFillBuffer (Buffer, EndBuffer, 1, ',', Increment);
        }
      }
    }
  }

  BasePrintLibFillBuffer (Buffer, EndBuffer + Increment, 1, 0, Increment);

  return ((Buffer - OriginalBuffer) / Increment);
}

