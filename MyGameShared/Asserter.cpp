#include "SharedPch.h"
#include "Asserter.h"

namespace MGSL::Shared
{
	Asserter::Asserter() = default;
	Asserter::~Asserter() = default;

	void Asserter::_Internal_ReportFailure(cstr expression, cstr file, int32 line, cstr message)
	{
		MGSL_LOG_FATAL("==================================================");
		MGSL_LOG_FATAL("ASSERTION FAILED!");
		MGSL_LOG_FATAL("Condition: {}", expression);
		MGSL_LOG_FATAL("File     : {}", file);
		MGSL_LOG_FATAL("Line     : {}", line);
		if (message) MGSL_LOG_FATAL("Message  : {}", message);
		MGSL_LOG_FATAL("==================================================");
		MGSL_LOG_FLUSH();
	}
}