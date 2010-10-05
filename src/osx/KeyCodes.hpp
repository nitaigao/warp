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
		key_codes_[0] = KeyCode(0, 12, 81);
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
	
	int generic_to_windows(int keycode)
	{
		return key_codes_[keycode].windows_;
	}
	
private:
	
	KeyCodeMap key_codes_;	

};