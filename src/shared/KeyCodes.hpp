#ifndef KEYCODES_H
#define KEYCODES_H

  class KeyCode
  {
    
  public:
    
    KeyCode() { };
    
    KeyCode(int generic, int osx, int windows) : generic_(generic), osx_(osx), windows_(windows) { };
    
    int windows_;
    int osx_;
    int generic_;
    
  };

  class KeyCodes
  {
    typedef std::map<int, KeyCode> KeyCodeMap;
    
  public:
    
    KeyCodes()
    {
      key_codes_[0] = KeyCode(0, 0, 65);
      key_codes_[1] = KeyCode(1, 1, 83);
      key_codes_[2] = KeyCode(2, 2, 68);
      key_codes_[3] = KeyCode(3, 3, 70);
      key_codes_[4] = KeyCode(4, 4, 72);
      key_codes_[5] = KeyCode(5, 5, 71);
      key_codes_[6] = KeyCode(6, 6, 90);
      key_codes_[7] = KeyCode(7, 7, 88);
      key_codes_[8] = KeyCode(8, 8, 67);
      key_codes_[9] = KeyCode(9, 9, 86);
      
      key_codes_[10] = KeyCode(10, 12, 81);
      key_codes_[11] = KeyCode(11, 13, 87);
      key_codes_[12] = KeyCode(12, 14, 69);
      key_codes_[13] = KeyCode(13, 15, 82);
      key_codes_[14] = KeyCode(14, 16, 89);
      key_codes_[15] = KeyCode(15, 17, 84);
      key_codes_[16] = KeyCode(16, 18, 49);
      key_codes_[17] = KeyCode(17, 19, 50);
      key_codes_[18] = KeyCode(18, 20, 51);
      key_codes_[19] = KeyCode(19, 21, 52);
      
      key_codes_[20] = KeyCode(20, 22, 54);
      key_codes_[21] = KeyCode(21, 23, 53);
      key_codes_[22] = KeyCode(22, 25, 57);
      key_codes_[23] = KeyCode(23, 26, 55);
      key_codes_[24] = KeyCode(24, 28, 56);
      key_codes_[25] = KeyCode(25, 29, 48);
      key_codes_[26] = KeyCode(26, 31, 79);
      key_codes_[27] = KeyCode(27, 32, 85);
      key_codes_[28] = KeyCode(28, 34, 73);
      key_codes_[29] = KeyCode(29, 35, 80);
      
      key_codes_[30] = KeyCode(30, 36, 13);
      key_codes_[31] = KeyCode(31, 37, 76);
      key_codes_[32] = KeyCode(32, 38, 74);
      key_codes_[33] = KeyCode(33, 40, 75);
      key_codes_[34] = KeyCode(34, 43, 188);
      key_codes_[35] = KeyCode(35, 44, 191);
      key_codes_[36] = KeyCode(36, 45, 78);
      key_codes_[37] = KeyCode(37, 46, 77);
      key_codes_[38] = KeyCode(38, 47, 190);
      key_codes_[39] = KeyCode(39, 48, 9);
      key_codes_[40] = KeyCode(40, 49, 32);
      
      key_codes_[41] = KeyCode(41, 51, 8);
      key_codes_[42] = KeyCode(42, 53, 27);
      key_codes_[43] = KeyCode(43, 56, 10);
      key_codes_[44] = KeyCode(44, 57, 20);
      key_codes_[45] = KeyCode(45, 58, 18);
      key_codes_[46] = KeyCode(46, 59, 11);
      key_codes_[47] = KeyCode(47, 65, 110);
      key_codes_[48] = KeyCode(48, 67, 106);
      key_codes_[49] = KeyCode(49, 69, 107);

      key_codes_[50] = KeyCode(50, 75, 111);
      key_codes_[51] = KeyCode(51, 78, 109);
      key_codes_[52] = KeyCode(52, 82, 96);
      key_codes_[53] = KeyCode(53, 83, 97);
      key_codes_[54] = KeyCode(54, 84, 98);
      key_codes_[55] = KeyCode(55, 85, 99);
      key_codes_[56] = KeyCode(56, 86, 100);
      key_codes_[57] = KeyCode(57, 87, 101);
      key_codes_[58] = KeyCode(58, 88, 102);
      key_codes_[59] = KeyCode(59, 89, 103);
      
      key_codes_[60] = KeyCode(60, 91, 104);
      key_codes_[61] = KeyCode(61, 92, 105);
      key_codes_[62] = KeyCode(62, 96, 116);
      key_codes_[63] = KeyCode(63, 97, 117);
      key_codes_[64] = KeyCode(64, 98, 118);
      key_codes_[65] = KeyCode(65, 100, 119);
      key_codes_[66] = KeyCode(66, 101, 120);
      key_codes_[67] = KeyCode(67, 103, 122);
      key_codes_[68] = KeyCode(68, 109, 121);
      key_codes_[69] = KeyCode(69, 110, 93);
      
      key_codes_[70] = KeyCode(70, 111, 123);
      key_codes_[71] = KeyCode(71, 114, 47);
      key_codes_[72] = KeyCode(72, 115, 36);
      key_codes_[73] = KeyCode(73, 116, 33);
      key_codes_[74] = KeyCode(74, 117, 46);
      key_codes_[75] = KeyCode(75, 118, 115);
      key_codes_[76] = KeyCode(76, 119, 35);
      key_codes_[77] = KeyCode(77, 120, 113);
      key_codes_[78] = KeyCode(78, 121, 34);
      key_codes_[79] = KeyCode(79, 122, 112);
      
      key_codes_[80] = KeyCode(80, 123, 37);
      key_codes_[81] = KeyCode(81, 124, 39);
      key_codes_[82] = KeyCode(82, 125, 40);
      key_codes_[83] = KeyCode(83, 126, 38);
      
      key_codes_[84] = KeyCode(84, 11, 66);
      
      key_codes_[85] = KeyCode(85, 24, 53); // =+
      key_codes_[86] = KeyCode(86, 41, 75); // ;:
      key_codes_[87] = KeyCode(87, 50, 75); // `~
      key_codes_[88] = KeyCode(88, 27, 75); // -_
      key_codes_[89] = KeyCode(89, 30, 75); // }]
      key_codes_[90] = KeyCode(90, 33, 75); // {[
      key_codes_[91] = KeyCode(91, 42, 75); // \|
      key_codes_[92] = KeyCode(92, 39, 75); // '"
      key_codes_[93] = KeyCode(93, 41, 75); // ;:
      key_codes_[94] = KeyCode(94, 44, 75); // /?
      key_codes_[95] = KeyCode(95, 47, 75); // .>
      key_codes_[96] = KeyCode(96, 43, 75); // ,<
      key_codes_[97] = KeyCode(97, 10, 75); // §±
    };
    
    int osx_to_generic(int keycode)
    {	
      for(KeyCodeMap::iterator i = key_codes_.begin(); i != key_codes_.end(); ++i)
      {
        if ((*i).second.osx_ == keycode)
        {
          return (*i).second.generic_;
        }
      }
      
      return 0;
    };
    
    int generic_to_osx(int keycode)
    {
      for(KeyCodeMap::iterator i = key_codes_.begin(); i != key_codes_.end(); ++i)
      {
        if ((*i).second.generic_ == keycode)
        {
          return (*i).second.osx_;
        }
      }
      
      return 0;
    };
    
    int generic_to_windows(int keycode)
    {	
      //std::clog << key_codes_[keycode].osx_ << std::endl;
      return key_codes_[keycode].windows_;
    }
    
  private:
    
    KeyCodeMap key_codes_;	

  };

#endif