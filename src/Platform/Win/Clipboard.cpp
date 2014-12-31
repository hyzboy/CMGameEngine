#include<windows.h>
#include<shobjidl.h>

namespace hgl
{
	namespace os
	{
		void CopyTextToClipboard(const u16char *str)
		{
			if(!str||!(*str))return;

			if (!OpenClipboard(nullptr))return;

			EmptyClipboard();

			HGLOBAL clipbuffer;
			u16char * buffer;

			clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(str)+1);
			buffer = (u16char *)GlobalLock(clipbuffer);

			strcpy(buffer, str);

			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_UNICODETEXT, clipbuffer);
			CloseClipboard();
		}

		const u16char *GetTextFromClipboard()
		{
			if (!OpenClipboard(nullptr))
				return 0;

			u16char * buffer = 0;

			HANDLE hData = GetClipboardData( CF_UNICODETEXT );
			buffer = (u16char *)GlobalLock( hData );
			GlobalUnlock( hData );
			CloseClipboard();
			return buffer;
		}
	}//namespace os
}//namespace hgl
