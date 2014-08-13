namespace hgl
{
	namespace os
	{
		void CopyTextToClipboard(const u16char *str)
		{
			if(!str||!(*str))return;

			OSXCopyToClipboard(str);
		}

		const u16char *GetTextFromClipboard()
		{
			return (OSXCopyFromClipboard());
		}
	}//namespace os
}//namespace hgl
