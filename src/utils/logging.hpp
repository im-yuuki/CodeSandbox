#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <string>

namespace logging {

	const static auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

	inline string get_log_filename(const string& name) {
		return "logs/"+ name + ".txt";
	}

	inline void init() {
		using namespace spdlog;
		auto file_sink = std::make_shared<sinks::basic_file_sink_mt>(get_log_filename("default"), true);
		set_default_logger(std::make_shared<logger>("default", sinks_init_list{console_sink, file_sink}));
	}

	inline shared_ptr<spdlog::logger> create_logger(const std::string& name) {
		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(get_log_filename(name), true);
		const auto logger = std::make_shared<spdlog::logger>(spdlog::logger(name, { console_sink, file_sink }));
		logger->set_level(spdlog::level::info);
		logger->set_pattern("[%d-%m-%Y %H:%M:%S] %n [%^%l%$] [thread %t] %v");
		return logger;
	}

	inline void info(const std::string& message){
		spdlog::info(message);
	}

	inline void warn(const std::string& message){
		spdlog::warn(message);
	}

	inline void error(const std::string& message){
		spdlog::error(message);
	}

}

namespace crow {
	class SpdlogLogger final : public ILogHandler {
	public:
		explicit SpdlogLogger(const std::shared_ptr<spdlog::logger>& logger) : logger(logger) {}

		void log(const std::string message, const LogLevel level) override {
			switch (level) {
			case LogLevel::DEBUG:
				logger->debug(message);
				break;
			case LogLevel::INFO:
				logger->info(message);
				break;
			case LogLevel::WARNING:
				logger->warn(message);
				break;
			case LogLevel::ERROR:
				logger->error(message);
				break;
			case LogLevel::CRITICAL:
				logger->critical(message);
				break;
			default:
				logger->info(message);
				break;
			}
		}

	private:
		std::shared_ptr<spdlog::logger> logger;
	};

}
#endif
