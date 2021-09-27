/*++

Copyright (c) 2004 - 2006, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials                          
are licensed and made available under the terms and conditions of the BSD License         
which accompanies this distribution.  The full text of the license may be found at        
http://opensource.org/licenses/bsd-license.php                                            
                                                                                          
THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.              


Module Name:

  Console.c
  
Abstract: 

  UEFI library functions.

--*/

#include "EdkIIGlueUefi.h"

typedef struct {
  CHAR16  WChar;
  UINT32  Width;
} UNICODE_WIDTH_ENTRY;

UNICODE_WIDTH_ENTRY mUnicodeWidthTable[] = {
  //
  // General script area
  //
  {(CHAR16)0x1FFF,  1},
  /*
   * Merge the blocks and replace them with the above entry as they fall to 
   * the same category and they are all narrow glyph. This will reduce search
   * time and table size. The merge will omit the reserved code.
   *
   * Remove the above item if below is un-commented.
   *
  {(CHAR16)0x007F,  1},       // C0 controls and basic Latin. 0x0000-0x007F
  {(CHAR16)0x00FF,  1},       // C1 controls and Latin-1 support. 0x0080-0x00FF
  {(CHAR16)0x017F,  1},       // Latin extended-A. 0x0100-0x017F
  {(CHAR16)0x024F,  1},       // Latin extended-B. 0x0180-0x024F
  {(CHAR16)0x02AF,  1},       // IPA extensions. 0x0250-0x02AF
  {(CHAR16)0x02FF,  1},       // Spacing modifier letters. 0x02B0-0x02FF
  {(CHAR16)0x036F,  1},       // Combining diacritical marks. 0x0300-0x036F
  {(CHAR16)0x03FF,  1},       // Greek. 0x0370-0x03FF
  {(CHAR16)0x04FF,  1},       // Cyrillic. 0x0400-0x04FF
  {(CHAR16)0x052F,  0},       // Unassigned. As Armenian in ver3.0. 0x0500-0x052F
  {(CHAR16)0x058F,  1},       // Armenian. 0x0530-0x058F
  {(CHAR16)0x05FF,  1},       // Hebrew. 0x0590-0x05FF
  {(CHAR16)0x06FF,  1},       // Arabic. 0x0600-0x06FF
  {(CHAR16)0x08FF,  0},       // Unassigned. 0x0700-0x08FF
  {(CHAR16)0x097F,  1},       // Devanagari. 0x0900-0x097F
  {(CHAR16)0x09FF,  1},       // Bengali. 0x0980-0x09FF
  {(CHAR16)0x0A7F,  1},       // Gurmukhi. 0x0A00-0x0A7F
  {(CHAR16)0x0AFF,  1},       // Gujarati. 0x0A80-0x0AFF
  {(CHAR16)0x0B7F,  1},       // Oriya. 0x0B00-0x0B7F
  {(CHAR16)0x0BFF,  1},       // Tamil. (See page 7-92). 0x0B80-0x0BFF
  {(CHAR16)0x0C7F,  1},       // Telugu. 0x0C00-0x0C7F
  {(CHAR16)0x0CFF,  1},       // Kannada. (See page 7-100). 0x0C80-0x0CFF
  {(CHAR16)0x0D7F,  1},       // Malayalam (See page 7-104). 0x0D00-0x0D7F
  {(CHAR16)0x0DFF,  0},       // Unassigned. 0x0D80-0x0DFF
  {(CHAR16)0x0E7F,  1},       // Thai. 0x0E00-0x0E7F
  {(CHAR16)0x0EFF,  1},       // Lao. 0x0E80-0x0EFF
  {(CHAR16)0x0FBF,  1},       // Tibetan. 0x0F00-0x0FBF
  {(CHAR16)0x109F,  0},       // Unassigned. 0x0FC0-0x109F
  {(CHAR16)0x10FF,  1},       // Georgian. 0x10A0-0x10FF
  {(CHAR16)0x11FF,  1},       // Hangul Jamo. 0x1100-0x11FF
  {(CHAR16)0x1DFF,  0},       // Unassigned. 0x1200-0x1DFF
  {(CHAR16)0x1EFF,  1},       // Latin extended additional. 0x1E00-0x1EFF
  {(CHAR16)0x1FFF,  1},       // Greek extended. 0x1F00-0x1FFF
  *
  */

  //
  // Symbol area
  //
  {(CHAR16)0x2FFF,  1},
  /*
   * Merge the blocks and replace them with the above entry as they fall to 
   * the same category and they are all narrow glyph. This will reduce search
   * time and table size. The merge will omit the reserved code.
   *
   * Remove the above item if below is un-commented.
   *
  {(CHAR16)0x206F,  1},       // General punctuation. (See page7-154). 0x200-0x206F
  {(CHAR16)0x209F,  1},       // Superscripts and subscripts. 0x2070-0x209F
  {(CHAR16)0x20CF,  1},       // Currency symbols. 0x20A0-0x20CF
  {(CHAR16)0x20FF,  1},       // Combining diacritical marks for symbols. 0x20D0-0x20FF
  {(CHAR16)0x214F,  1},       // Letterlike sympbols. 0x2100-0x214F
  {(CHAR16)0x218F,  1},       // Number forms. 0x2150-0x218F
  {(CHAR16)0x21FF,  1},       // Arrows. 0x2190-0x21FF
  {(CHAR16)0x22FF,  1},       // Mathematical operators. 0x2200-0x22FF
  {(CHAR16)0x23FF,  1},       // Miscellaneous technical. 0x2300-0x23FF
  {(CHAR16)0x243F,  1},       // Control pictures. 0x2400-0x243F
  {(CHAR16)0x245F,  1},       // Optical character recognition. 0x2440-0x245F
  {(CHAR16)0x24FF,  1},       // Enclosed alphanumerics. 0x2460-0x24FF
  {(CHAR16)0x257F,  1},       // Box drawing. 0x2500-0x257F
  {(CHAR16)0x259F,  1},       // Block elements. 0x2580-0x259F
  {(CHAR16)0x25FF,  1},       // Geometric shapes. 0x25A0-0x25FF
  {(CHAR16)0x26FF,  1},       // Miscellaneous symbols. 0x2600-0x26FF
  {(CHAR16)0x27BF,  1},       // Dingbats. 0x2700-0x27BF
  {(CHAR16)0x2FFF,  0},       // Reserved. 0x27C0-0x2FFF
  *
  */

  //
  // CJK phonetics and symbol area
  //
  {(CHAR16)0x33FF,  2},
  /*
   * Merge the blocks and replace them with the above entry as they fall to 
   * the same category and they are all wide glyph. This will reduce search
   * time and table size. The merge will omit the reserved code.
   *
   * Remove the above item if below is un-commented.
   *
  {(CHAR16)0x303F,  2},       // CJK symbols and punctuation. 0x3000-0x303F
  {(CHAR16)0x309F,  2},       // Hiragana. 0x3040-0x309F
  {(CHAR16)0x30FF,  2},       // Katakana. 0x30A0-0x30FF
  {(CHAR16)0x312F,  2},       // Bopomofo. 0x3100-0x312F
  {(CHAR16)0x318F,  2},       // Hangul compatibility jamo. 0x3130-0x318F
  {(CHAR16)0x319F,  2},       // Kanbun. 0x3190-0x319F
  {(CHAR16)0x31FF,  0},       // Reserved. As Bopomofo extended in ver3.0. 0x31A0-0x31FF
  {(CHAR16)0x32FF,  2},       // Enclosed CJK letters and months. 0x3200-0x32FF
  {(CHAR16)0x33FF,  2},       // CJK compatibility. 0x3300-0x33FF
  *
  */

  //
  // CJK ideograph area
  //
  {(CHAR16)0x9FFF,  2},
  /*
   * Merge the blocks and replace them with the above entry as they fall to 
   * the same category and they are all wide glyph. This will reduce search
   * time and table size. The merge will omit the reserved code.
   *
   * Remove the above item if below is un-commented.
   *
  {(CHAR16)0x4DFF,  0},       // Reserved. 0x3400-0x4DBF as CJK unified ideographs 
                      // extension A in ver3.0. 0x3400-0x4DFF
  {(CHAR16)0x9FFF,  2},       // CJK unified ideographs. 0x4E00-0x9FFF
  *
  */

  //
  // Reserved
  //
  {(CHAR16)0xABFF,  0},       // Reserved. 0xA000-0xA490 as Yi syllables. 0xA490-0xA4D0
  // as Yi radicals in ver3.0. 0xA000-0xABFF
  //
  // Hangul syllables
  //
  {(CHAR16)0xD7FF,  2},
  /*
   * Merge the blocks and replace them with the above entry as they fall to 
   * the same category and they are all wide glyph. This will reduce search
   * time and table size. The merge will omit the reserved code.
   *
   * Remove the above item if below is un-commented.
   *
  {(CHAR16)0xD7A3,  2},       // Hangul syllables. 0xAC00-0xD7A3
  {(CHAR16)0xD7FF,  0},       // Reserved. 0xD7A3-0xD7FF
  *
  */

  //
  // Surrogates area
  //
  {(CHAR16)0xDFFF,  0},       // Surrogates, not used now. 0xD800-0xDFFF

  //
  // Private use area
  //
  {(CHAR16)0xF8FF,  0},       // Private use area. 0xE000-0xF8FF

  //
  // Compatibility area and specials
  //
  {(CHAR16)0xFAFF,  2},       // CJK compatibility ideographs. 0xF900-0xFAFF
  {(CHAR16)0xFB4F,  1},       // Alphabetic presentation forms. 0xFB00-0xFB4F
  {(CHAR16)0xFDFF,  1},       // Arabic presentation forms-A. 0xFB50-0xFDFF
  {(CHAR16)0xFE1F,  0},       // Reserved. As variation selectors in ver3.0. 0xFE00-0xFE1F
  {(CHAR16)0xFE2F,  1},       // Combining half marks. 0xFE20-0xFE2F
  {(CHAR16)0xFE4F,  2},       // CJK compatibility forms. 0xFE30-0xFE4F
  {(CHAR16)0xFE6F,  1},       // Small Form Variants. 0xFE50-0xFE6F
  {(CHAR16)0xFEFF,  1},       // Arabic presentation forms-B. 0xFE70-0xFEFF
  {(CHAR16)0xFFEF,  1},       // Half width and full width forms. 0xFF00-0xFFEF
  {(CHAR16)0xFFFF,  0},       // Speicials. 0xFFF0-0xFFFF
};

/**
  This function computes and returns the width of the Unicode character 
  specified by UnicodeChar.

  @param  UnicodeChar   A Unicode character.

  @retval 0             The width if UnicodeChar could not be determined.
  @retval 1             UnicodeChar is a narrow glyph.
  @retval 2             UnicodeChar is a wide glyph.

**/
UINTN
EFIAPI
GlueGetGlyphWidth (
  IN CHAR16  UnicodeChar
  )
{
  UINTN               Index;
  UINTN               Low;
  UINTN               High;
  UNICODE_WIDTH_ENTRY *Item;

  Item  = NULL;
  Low   = 0;
  High  = (sizeof (mUnicodeWidthTable)) / (sizeof (UNICODE_WIDTH_ENTRY)) - 1;
  while (Low <= High) {
    Index = (Low + High) >> 1;
    Item  = &(mUnicodeWidthTable[Index]);
    if (Index == 0) {
      if (UnicodeChar <= Item->WChar) {
        break;
      }

      return 0;
    }

    if (UnicodeChar > Item->WChar) {
      Low = Index + 1;
    } else if (UnicodeChar <= mUnicodeWidthTable[Index - 1].WChar) {
      High = Index - 1;
    } else {
      //
      // Index - 1 < UnicodeChar <= Index. Found
      //
      break;
    }
  }

  if (Low <= High) {
    return Item->Width;
  }

  return 0;
}

/**
  This function computes and returns the display length of
  the Null-terminated Unicode string specified by String.
  If String is NULL, then 0 is returned.
  If any of the widths of the Unicode characters in String
  can not be determined, then 0 is returned.

  @param  String      A pointer to a Null-terminated Unicode string.

  @return The display length of the Null-terminated Unicode string specified by String.
  
**/
UINTN
EFIAPI
UnicodeStringDisplayLength (
  IN CONST CHAR16  *String
  )
{
  UINTN      Length;
  UINTN      Width;

  if (String == NULL) {
    return 0;
  }

  Length = 0;
  while (*String != 0) {
    Width = GetGlyphWidth (*String);
    if (Width == 0) {
      return 0;
    }

    Length += Width;
    String++;
  }

  return Length;
}
