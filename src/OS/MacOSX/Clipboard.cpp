namespace hgl
{
	namespace os
	{
		void CopyTextToClipboard(const char16_t *str)
		{
			if(!str||!(*str))return;

			OSXCopyToClipboard(str);
		}

		const char16_t *GetTextFromClipboard()
		{
			return (OSXCopyFromClipboard());
		}
	}//namespace os
}//namespace hgl
