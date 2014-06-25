#include<windows.h>
#include<shobjidl.h>

namespace hgl
{
	namespace os
	{
		void CopyTextToClipboard(const char16_t *str)
		{
			if(!str||!(*str))return;

			if (!OpenClipboard(nullptr))return;

			EmptyClipboard();

			HGLOBAL clipbuffer;
			char16_t * buffer;

			clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(str)+1);
			buffer = (char16_t *)GlobalLock(clipbuffer);

			strcpy(buffer, str);

			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_UNICODETEXT, clipbuffer);
			CloseClipboard();
		}

		const char16_t *GetTextFromClipboard()
		{
			if (!OpenClipboard(nullptr))
				return 0;

			char16_t * buffer = 0;

			HANDLE hData = GetClipboardData( CF_UNICODETEXT );
			buffer = (char16_t *)GlobalLock( hData );
			GlobalUnlock( hData );
			CloseClipboard();
			return buffer;
		}
	}//namespace os
}//namespace hgl
