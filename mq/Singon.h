

#define DECLARE_SINGLE(className) \
public: \
	classNmae & getInstance() { \
		static className instance; \
		return instance; \
	}

