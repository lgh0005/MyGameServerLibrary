#pragma once

namespace MGSL::Shared
{
	class Asserter
	{
		MGSL_DECLARE_SINGLE(Asserter);

	private:
		Asserter();
		~Asserter();

	public:
		static void _Internal_ReportFailure
		(
			cstr expression,
			cstr file,
			int32 line,
			cstr message = nullptr
		);
	};
}

/*=========================//
//     Debug assertion     //
//=========================*/
#ifdef _DEBUG
	#define MGSL_ASSERT(expr)																     \
		do																						 \
		{																						 \
			if (!(expr))																		 \
			{																					 \
				MGSL::Shared::Asserter::_Internal_ReportFailure(#expr, __FILE__, __LINE__);		 \
				__debugbreak();																	 \
			}																					 \
		} while (false)

	#define MGSL_ASSERT_MSG(expr, msg)															 \
        do																						 \
		{																						 \
            if (!(expr))																		 \
			{																					 \
                MGSL::Shared::Asserter::_Internal_ReportFailure(#expr, __FILE__, __LINE__, msg); \
                __debugbreak();																	 \
            }																					 \
        } while (false)
#else
	#define MGSL_ASSERT(expr)        ((void)0)
	#define MGSL_ASSERT_MSG(expr, msg) ((void)0)
#endif