export module Common.Assert;

export void ASSERT(bool cond, ...)
{
	if (!cond)
	{
		__debugbreak();
	}
}