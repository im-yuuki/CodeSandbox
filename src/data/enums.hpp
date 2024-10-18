#ifndef ENUMS_HPP
#define ENUMS_HPP

namespace data::submission_status {
	enum Enum {
		Pending,
		Queued,
		Running,
		Accepted,
		WrongAnswer,
		TimeLimitExceeded,
		MemoryLimitExceeded,
		RuntimeError,
		CompilationError,
		InternalError
	};
}

#endif