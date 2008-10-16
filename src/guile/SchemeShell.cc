/*
 * SchemeShell.c
 *
 * Simple scheme shell
 * Copyright (c) 2008 Linas Vepstas <linas@linas.org>
 */

#ifdef HAVE_GUILE

#include "platform.h"
#include "SchemeShell.h"

using namespace opencog;

SchemeShell::SchemeShell(void)
{
	show_output = true;
	normal_prompt = "guile> ";
	pending_prompt = "... ";
}

void SchemeShell::hush_output(bool hush)
{
	show_output = !hush;
}

/* ============================================================== */

/**
 * Evaluate the expression
 */
std::string SchemeShell::eval(const std::string &expr)
{
	// Don't evaluate if the line is terminated by 
	// escape (^[), cancel (^X) or quit (^C)
	size_t len = expr.length();
	if (1 <= len) 
	{
		char c = expr[len-1];
		if ((0x6 == c) || (0x16 == c) || (0x18 == c) || (0x1b == c))
		{
			evaluator.clear_pending();
			return normal_prompt;
		}
	}

	if (2 <= len) 
	{
		char c = expr[len-2];
		if ((0x6 == c) || (0x16 == c) || (0x18 == c) || (0x1b == c))
		{
			evaluator.clear_pending();
			return normal_prompt;
		}
	}

	if (0 == len) 
	{
		if (evaluator.input_pending())
		{
			return pending_prompt;
		}
		else
		{
			return normal_prompt;
		}
	}

	/* The #$%^& opecog command shell processor cuts off the 
	 * newline character. Re-insert it; otherwise, comments within
	 * procedures will have the effect of commenting out the rest
	 * of the procedure, leading to garbage.
	 *
	 * (This is a pointless string copy, it should be eliminated)
	 */
	std::string input = expr + "\n";

	std::string result = evaluator.eval(input.c_str());

	if (evaluator.input_pending())
	{
		if (show_output)
			return pending_prompt;
		else
			return "";
	}

	if (show_output || evaluator.eval_error())
	{
		result += normal_prompt;
		return result;
	}
	else
	{
		return "";
	}
	return "#<Error: Unreachable statement reached>";
}

#endif
/* ===================== END OF FILE ============================ */
