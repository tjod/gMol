static GLubyte *mask; /* 4 8-bit-bytes is 32 bits and 32*4 = 128 */

  static GLubyte mask9[128] = {
  0x02, 0x00, 0x82, 0x00, 0x00, 0x10, 0x00, 0x08, 0x01, 0x00, 0x04, 0x00, 0x01, 0x00, 0x80, 0x40, 
  0x00, 0x00, 0x04, 0x00, 0x10, 0x02, 0x02, 0x00, 0x44, 0x00, 0x00, 0x19, 0x42, 0x00, 0x00, 0x00, 
  0x80, 0x00, 0x20, 0x84, 0x10, 0x00, 0x40, 0x21, 0x04, 0xc1, 0x81, 0x00, 0x02, 0x02, 0x04, 0x00, 
  0x00, 0x00, 0x4c, 0x88, 0x00, 0x00, 0x15, 0x00, 0x02, 0x02, 0x00, 0x01, 0x40, 0x04, 0x01, 0x00, 
  0x00, 0x04, 0x11, 0x02, 0x14, 0x12, 0x10, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x24, 0x80, 0x00, 
  0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x20, 0x00, 0x12, 0x00, 0x05, 0x00, 0x40, 0x44, 0x00, 0x40, 
  0x30, 0x02, 0x00, 0x01, 0x04, 0x02, 0x08, 0x20, 0x20, 0x00, 0x00, 0x00, 0x80, 0x00, 0x04, 0x00, 
  0x06, 0x20, 0x00, 0x10, 0x04, 0x06, 0xa4, 0x01, 0x02, 0x00, 0x00, 0x00, 0x08, 0x12, 0x00, 0x00}; 
  
  static GLubyte mask8[128] = {
  0x81, 0x0c, 0x00, 0x90, 0x00, 0x18, 0x0a, 0x12, 0xc8, 0x82, 0x00, 0x00, 0x00, 0x40, 0x08, 0x50, 
  0x00, 0x81, 0x00, 0x11, 0x0a, 0x1a, 0x2c, 0x00, 0x0a, 0x01, 0x06, 0x00, 0x02, 0x22, 0xcc, 0x01, 
  0x12, 0xa2, 0x02, 0xc8, 0x20, 0x18, 0x00, 0x02, 0x90, 0x61, 0x01, 0x04, 0x22, 0x09, 0x00, 0x04, 
  0x00, 0x00, 0x00, 0x0c, 0x02, 0x11, 0x00, 0x81, 0x14, 0x01, 0x03, 0x40, 0x04, 0x0a, 0x6c, 0x2a, 
  0x38, 0x44, 0xc5, 0x02, 0x01, 0x02, 0x12, 0x0a, 0x20, 0x68, 0xb1, 0x30, 0x15, 0x03, 0x84, 0x03, 
  0x80, 0xe4, 0x10, 0x05, 0x01, 0x51, 0xa0, 0x80, 0xdc, 0x35, 0x50, 0x04, 0x40, 0x08, 0x20, 0x00, 
  0x22, 0x80, 0x40, 0x4d, 0x90, 0x80, 0xc0, 0x04, 0x24, 0x12, 0x48, 0x05, 0x22, 0x03, 0x04, 0x40, 
  0x10, 0x80, 0x08, 0x00, 0x14, 0x00, 0x85, 0x00, 0x00, 0x41, 0x00, 0x1a, 0x01, 0x00, 0x0d, 0x80};
  
  static GLubyte mask7[128] = {
  0x00, 0x03, 0xb1, 0x29, 0x00, 0x27, 0x0c, 0x00, 0x00, 0x80, 0x20, 0x31, 0x98, 0x61, 0x02, 0x0a, 
  0x10, 0x04, 0x5c, 0x6c, 0x0c, 0x51, 0x01, 0x81, 0x08, 0x46, 0x81, 0x34, 0x50, 0x0a, 0x07, 0xa8, 
  0x2a, 0x20, 0x22, 0xd0, 0x14, 0xc4, 0x10, 0x21, 0x82, 0x11, 0x41, 0x01, 0x01, 0x30, 0xd1, 0x1c, 
  0x11, 0xc2, 0xd6, 0x39, 0xcc, 0x60, 0x04, 0x44, 0x25, 0x75, 0x20, 0x29, 0x85, 0x58, 0x80, 0x40, 
  0x21, 0xb6, 0xa5, 0xb5, 0x82, 0x60, 0x0c, 0xde, 0x10, 0x22, 0x80, 0x10, 0x74, 0x00, 0x82, 0x10, 
  0x48, 0x40, 0x60, 0xcd, 0x42, 0x09, 0x18, 0x1c, 0xd8, 0x08, 0xce, 0x13, 0x0d, 0x82, 0x40, 0x2a, 
  0x0e, 0x2c, 0x12, 0x3a, 0x01, 0x18, 0x82, 0x84, 0xac, 0x98, 0x01, 0x32, 0x01, 0xa5, 0xcc, 0x00, 
  0x48, 0x32, 0xc7, 0x85, 0x06, 0x6c, 0x23, 0x4c, 0x2c, 0x0c, 0x11, 0x80, 0x45, 0x40, 0x96, 0x59}; 
  
  static GLubyte mask6[128] = {
  0x85, 0x90, 0x08, 0x9a, 0x84, 0x69, 0xe0, 0xce, 0x23, 0xe6, 0xa0, 0x80, 0x0a, 0x86, 0x12, 0x1a, 
  0x10, 0x41, 0xd4, 0x84, 0x86, 0x68, 0x14, 0x75, 0x18, 0xa6, 0x2e, 0xd0, 0xd4, 0x1a, 0x1f, 0x2a, 
  0x39, 0xb5, 0xa5, 0x62, 0x47, 0x8c, 0x2a, 0x0a, 0x42, 0x80, 0x27, 0x03, 0x5e, 0x2c, 0xc1, 0xde, 
  0xd3, 0x7c, 0x01, 0xde, 0x9d, 0x09, 0x5d, 0x47, 0x58, 0x01, 0x26, 0xab, 0x38, 0x43, 0xa4, 0x3a, 
  0x20, 0x2f, 0x1d, 0x08, 0x15, 0xae, 0x01, 0x21, 0x5c, 0xa4, 0x02, 0x22, 0x25, 0x17, 0x10, 0x4a, 
  0x43, 0xd0, 0x03, 0x24, 0x6d, 0x25, 0xab, 0x0b, 0x09, 0x92, 0xc7, 0x35, 0x72, 0xd4, 0x36, 0x12, 
  0x88, 0x60, 0x27, 0x75, 0x28, 0xd4, 0x54, 0xa5, 0x89, 0x38, 0x31, 0x77, 0x03, 0x6b, 0x02, 0xb9, 
  0x22, 0x44, 0x3a, 0xe4, 0x72, 0xc4, 0x4a, 0x0f, 0x68, 0x8c, 0x8b, 0x63, 0x89, 0xcc, 0x8a, 0x11}; 
  
  static GLubyte mask5[128] = {
  0xea, 0x66, 0x81, 0xd8, 0x0a, 0x06, 0xdd, 0xf7, 0x54, 0x22, 0xf7, 0xcc, 0xcd, 0xa7, 0xaf, 0x67, 
  0x5a, 0xf6, 0xde, 0x05, 0x2a, 0x85, 0xfc, 0xf1, 0x83, 0xf2, 0xa0, 0x6d, 0x57, 0xb9, 0xb3, 0x28, 
  0x1b, 0xb4, 0xcf, 0xa0, 0xa0, 0x70, 0x7e, 0xba, 0x0d, 0xc6, 0xa0, 0xf7, 0x44, 0x46, 0x66, 0xda, 
  0x61, 0x8e, 0x7f, 0xd6, 0x53, 0xe1, 0x8d, 0x7a, 0xc1, 0xc5, 0x14, 0x88, 0x1b, 0x0a, 0xf9, 0x1f, 
  0xd9, 0x86, 0x98, 0x9c, 0x4a, 0x13, 0x1c, 0x97, 0x74, 0x80, 0x54, 0x70, 0xee, 0x29, 0xb0, 0xad, 
  0x15, 0x8a, 0x40, 0xbc, 0x27, 0x14, 0xf9, 0x2d, 0xcd, 0x6e, 0x3c, 0xba, 0xb6, 0x2b, 0xd8, 0xe5, 
  0x42, 0x24, 0xfa, 0xf3, 0xf7, 0x4e, 0x5c, 0xe1, 0x66, 0xb6, 0x82, 0xe6, 0x92, 0x23, 0x1d, 0xa0, 
  0xfb, 0x98, 0xac, 0x8e, 0xbf, 0x59, 0x83, 0x6d, 0xd1, 0x06, 0xaa, 0xac, 0x23, 0x29, 0xe1, 0xa9}; 
  
  static GLubyte mask4[128] = {
  0xb6, 0xe9, 0x69, 0x74, 0x4c, 0xbc, 0xb1, 0x39, 0xe6, 0x56, 0xae, 0x69, 0x7f, 0x70, 0xd1, 0x59, 
  0x5d, 0x63, 0x4d, 0x5b, 0x96, 0xc6, 0x76, 0x43, 0xae, 0xae, 0x77, 0x99, 0xe3, 0xdb, 0xec, 0x46, 
  0xeb, 0xf4, 0xbf, 0x7a, 0x75, 0xf4, 0x77, 0x64, 0xaf, 0x6e, 0x2b, 0x1c, 0x38, 0x65, 0x77, 0xe8, 
  0x56, 0x89, 0xc4, 0xc6, 0x61, 0xbd, 0x8a, 0xa6, 0x99, 0xfc, 0xcd, 0xba, 0xcd, 0x8c, 0xe9, 0x70, 
  0x6f, 0xfe, 0xe6, 0x87, 0xc5, 0x87, 0x61, 0xd9, 0xf1, 0x7f, 0x9d, 0x6d, 0x1e, 0x6f, 0xf7, 0x73, 
  0x79, 0x5c, 0x7f, 0xe9, 0xe2, 0x6f, 0x30, 0xab, 0xab, 0xb2, 0xfc, 0xf9, 0xf6, 0xc3, 0x9e, 0xbf, 
  0xc7, 0xff, 0x82, 0xda, 0xff, 0xec, 0xbb, 0x6e, 0xbe, 0x87, 0xbe, 0xbf, 0xc5, 0x77, 0xbf, 0xf9, 
  0x57, 0x5d, 0xeb, 0xc1, 0x20, 0xb9, 0x7d, 0xb0, 0xff, 0xa7, 0x9d, 0xfd, 0x8f, 0x56, 0x26, 0xf8}; 
  
  static GLubyte mask3[128] = {
  0x3f, 0xd6, 0x01, 0xfe, 0x6d, 0x1e, 0xf1, 0xdf, 0xde, 0x7d, 0xbf, 0xd5, 0xf5, 0x17, 0xf7, 0xd1, 
  0x6f, 0x6e, 0xaf, 0xfc, 0xad, 0xed, 0xcf, 0xd7, 0xf1, 0x7d, 0xde, 0xff, 0x86, 0xcd, 0xb7, 0xef, 
  0xb1, 0xff, 0xff, 0x87, 0x73, 0x5d, 0xfe, 0x84, 0xed, 0x93, 0xec, 0x37, 0xe7, 0xdf, 0xfa, 0xbf, 
  0xb7, 0xff, 0x8f, 0xd3, 0xfb, 0xdf, 0xee, 0xda, 0x77, 0xff, 0xfe, 0xdb, 0x9e, 0x03, 0x5b, 0x1b, 
  0xcb, 0x77, 0x47, 0xa9, 0x3d, 0xfd, 0xdf, 0x7b, 0x7c, 0xbb, 0x0e, 0x95, 0xdf, 0x15, 0xd9, 0xf9, 
  0xe9, 0x63, 0xbf, 0xfc, 0xd3, 0x79, 0xff, 0x09, 0xbe, 0xfd, 0xd2, 0xec, 0xaf, 0xaf, 0xbf, 0x35, 
  0xd7, 0xdf, 0xfb, 0xa7, 0xae, 0xc5, 0xad, 0xff, 0x88, 0x7f, 0x5f, 0xe1, 0x6f, 0xfa, 0xfc, 0xff, 
  0xdf, 0xff, 0xaf, 0x3d, 0xcf, 0xbd, 0xdf, 0xfc, 0xb7, 0xf9, 0x17, 0x9a, 0xad, 0xf6, 0xff, 0x7f}; 
  
  static GLubyte mask2[128] = {
  0xf7, 0xff, 0xdf, 0xde, 0xdf, 0xff, 0x99, 0xef, 0xef, 0xec, 0x5c, 0xcf, 0xff, 0xaf, 0xff, 0xff, 
  0xf6, 0x3d, 0xff, 0x5f, 0xef, 0xfb, 0xd9, 0x9f, 0xa7, 0xf7, 0xff, 0xdf, 0x3f, 0xef, 0xff, 0xef, 
  0xeb, 0xdf, 0xb7, 0xd9, 0x58, 0xe9, 0xff, 0x2e, 0xff, 0xef, 0x7d, 0xbf, 0xf7, 0xde, 0xcf, 0xfe, 
  0xb7, 0x9d, 0xed, 0x75, 0xdf, 0x6d, 0x97, 0xe7, 0xbe, 0xff, 0x7e, 0xff, 0xab, 0xf6, 0x7d, 0xed, 
  0xfe, 0xbb, 0xdf, 0xdf, 0xc2, 0xfe, 0xec, 0xee, 0xee, 0xdb, 0xfe, 0xff, 0xf7, 0x7f, 0xdf, 0xbb, 
  0xfd, 0x39, 0x7d, 0xfe, 0xbf, 0x5b, 0x75, 0xbe, 0xdf, 0xff, 0xf7, 0xff, 0xff, 0xff, 0x37, 0xbe, 
  0xbe, 0xff, 0xf4, 0xbf, 0xee, 0x7f, 0xdd, 0xff, 0xef, 0xc3, 0xff, 0xb7, 0xef, 0x7a, 0x6b, 0xf7, 
  0xf3, 0xf6, 0xfe, 0xf9, 0xdb, 0xff, 0xcf, 0xe7, 0xfd, 0xdf, 0xbf, 0xff, 0xfd, 0xf3, 0xff, 0x7b};
  
  static GLubyte mask1[128] = {
  0xfd, 0xfe, 0xff, 0xf8, 0xf7, 0xff, 0xff, 0xff, 0xff, 0x73, 0xff, 0xfd, 0xfd, 0xff, 0xfb, 0x7f, 
  0xff, 0xf7, 0x9f, 0x5f, 0xdf, 0xdf, 0xff, 0xfd, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xf7, 0xbf, 0xec, 
  0xff, 0xff, 0xfd, 0xef, 0xec, 0xff, 0xff, 0xff, 0xbd, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x5f, 0xf9, 
  0xef, 0xdf, 0xf7, 0xff, 0xff, 0xff, 0xef, 0xee, 0xdf, 0x7f, 0xbb, 0xfb, 0xef, 0xff, 0xb7, 0xbf, 
  0xff, 0xff, 0xff, 0xdf, 0xf7, 0xff, 0xbf, 0xff, 0xef, 0x7f, 0xdf, 0xfe, 0xff, 0xff, 0xd7, 0xff, 
  0xef, 0xfd, 0xdf, 0x3f, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x7f, 0xff, 0x9f, 
  0xff, 0xfd, 0xff, 0x7f, 0xb7, 0xf7, 0x72, 0xff, 0xff, 0xff, 0xb7, 0xf9, 0xff, 0xff, 0xbd, 0x77, 
  0xbf, 0xff, 0xbf, 0xff, 0xd7, 0xfd, 0xfa, 0xbf, 0xff, 0xff, 0xfb, 0xff, 0xfe, 0xff, 0xeb, 0xff};
