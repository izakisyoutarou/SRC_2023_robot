#pragma once

#define STRING(str) STR_CP(str)
#define STR_CP(str) #str

enum class ENUM_CLASS__ {
	MODE00__,
	MODE01__,
	MODE02__,
	MODE03__,
	MODE04__,
	MODE05__,
	MODE06__,
	MODE07__,
	MODE08__,
	MODE09__,
	MODE10__,
	MODE11__,
	MODE12__,
	MODE13__,
	MODE14__,
	MODE15__
};

class MODE_CLASS__ {
public:
	void operator=(const ENUM_CLASS__ &mode_) {
		mode = mode_;
	}
	bool operator==(const ENUM_CLASS__ &mode_) const {
		return mode == mode_;
	}
	bool operator!=(const ENUM_CLASS__ &mode_) const {
		return !(*this == mode_);
	}
	operator int() const {
		return static_cast<int>(mode);
	}
	const char * str() const {
		switch(mode) {
			case ENUM_CLASS__::MODE00__: return STRING(MODE00__);
			case ENUM_CLASS__::MODE01__: return STRING(MODE01__);
			case ENUM_CLASS__::MODE02__: return STRING(MODE02__);
			case ENUM_CLASS__::MODE03__: return STRING(MODE03__);
			case ENUM_CLASS__::MODE04__: return STRING(MODE04__);
			case ENUM_CLASS__::MODE06__: return STRING(MODE06__);
			case ENUM_CLASS__::MODE07__: return STRING(MODE07__);
			case ENUM_CLASS__::MODE08__: return STRING(MODE08__);
			case ENUM_CLASS__::MODE09__: return STRING(MODE09__);
			case ENUM_CLASS__::MODE10__: return STRING(MODE10__);
			case ENUM_CLASS__::MODE11__: return STRING(MODE11__);
			case ENUM_CLASS__::MODE12__: return STRING(MODE12__);
			case ENUM_CLASS__::MODE13__: return STRING(MODE13__);
			case ENUM_CLASS__::MODE14__: return STRING(MODE14__);
			case ENUM_CLASS__::MODE15__: return STRING(MODE15__);
			default: return "undefined mode\n";
		}
}
private:
	ENUM_CLASS__ mode = ENUM_CLASS__::MODE00__;
};

#undef MODE_CLASS__
#undef ENUM_CLASS__
#undef MODE00__
#undef MODE01__
#undef MODE02__
#undef MODE03__
#undef MODE04__
#undef MODE05__
#undef MODE06__
#undef MODE07__
#undef MODE08__
#undef MODE09__
#undef MODE10__
#undef MODE11__
#undef MODE12__
#undef MODE13__
#undef MODE14__
#undef MODE15__

#undef STRING
#undef STR_CP