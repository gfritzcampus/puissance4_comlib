#ifndef _P4_DECODE
#define _P4_DECODE

/**
 * @brief Convert hexadecimal representation of half byte
 *
 * @param hb Half byte representing in hexa
 * @return 0 - 15 if conversion is possible, otherwise -1
 */
static unsigned char decodeHalfByte(const char hb) {
  if ('0' <= hb && hb <= '9') {
    return hb - '0';
  }
  else if ('A' <= hb && hb <= 'F') {
    return 10 + hb - 'A';
  }
  else if ('a' <= hb && hb <= 'f') {
    return 10 + hb - 'a';
  }
  else {
    return -1;
  }
}

/**
 * @brief Decode a byte
 * 
 * @param mshb most significant half byte
 * @param lshb most significant halg byte
 * @param result Pointer to where to store value
 * @return true if conversion if OK, otherwise false
 */
static bool decodeByte(const char mshb, const char lshb, unsigned char * const result) {
  unsigned char decoded_mshb = decodeHalfByte(mshb);
  unsigned char decoded_lshb = decodeHalfByte(lshb);

  if (decoded_mshb == -1 || decoded_lshb == -1) {
    return false;
  }

  *result = ((decoded_mshb << 4) | decoded_lshb);

  return true;
}

#endif
