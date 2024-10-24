#ifndef ENUMS_HPP
#define ENUMS_HPP

namespace data::submission_status {
	enum Enum {
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

	inline std::string repr(const Enum val) {
		switch (val) {
			case Queued: return "QUEUED";
			case Running: return "RUNNING";
			case Accepted: return "ACCEPTED";
			case WrongAnswer: return "WRONG_ANSWER";
			case TimeLimitExceeded: return "TIME_LIMIT_EXCEEDED";
			case MemoryLimitExceeded: return "MEMORY_LIMIT_EXCEEDED";
			case RuntimeError: return "RUNTIME_ERROR";
			case CompilationError: return "COMPILATION_ERROR";
			case InternalError: return "INTERNAL_ERROR";
			default: return "UNKNOWN_STATUS";
		}
	}
}

#endif