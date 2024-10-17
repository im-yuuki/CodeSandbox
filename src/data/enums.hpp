#ifndef ENUMS_HPP
#define ENUMS_HPP

namespace data {

	namespace submission_status {
		enum Enum {
			pending,
			queued,
			running,
			accepted,
			wrong_answer,
			time_limit_exceeded,
			memory_limit_exceeded,
			runtime_error,
			compilation_error,
			internal_error
		};
	}

	namespace profile {
		enum Enum {
			c17,
			cpp17,
			python312
		};
	}

}
#endif