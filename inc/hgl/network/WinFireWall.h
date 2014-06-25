#ifndef HGL_NETWORK_WINDOW_FIREWALL_INCLUDE
#define HGL_NETWORK_WINDOW_FIREWALL_INCLUDE

struct INetFwProfile;

namespace hgl
{
	namespace network
	{
		const int FW_MAX_ERROR_MESSAGE=256;

		enum FW_ERROR_CODE
		{
			FW_NOERROR = 0,
			FW_ERR_INITIALIZED,					// Already initialized or doesn't call Initialize()
			FW_ERR_CREATE_SETTING_MANAGER,		// Can't create an instance of the firewall settings manager
			FW_ERR_LOCAL_POLICY,				// Can't get local firewall policy
			FW_ERR_PROFILE,						// Can't get the firewall profile
			FW_ERR_FIREWALL_IS_ENABLED,			// Can't get the firewall enable information
			FW_ERR_FIREWALL_ENABLED,			// Can't set the firewall enable option
			FW_ERR_INVALID_ARG,					// Invalid Arguments
			FW_ERR_AUTH_APPLICATIONS,			// Failed to get authorized application list
			FW_ERR_APP_ENABLED,					// Failed to get the application is enabled or not
			FW_ERR_CREATE_APP_INSTANCE,			// Failed to create an instance of an authorized application
			FW_ERR_SYS_ALLOC_STRING,			// Failed to alloc a memory for BSTR
			FW_ERR_PUT_PROCESS_IMAGE_NAME,		// Failed to put Process Image File Name to Authorized Application
			FW_ERR_PUT_REGISTER_NAME,			// Failed to put a registered name
			FW_ERR_ADD_TO_COLLECTION,			// Failed to add to the Firewall collection
			FW_ERR_REMOVE_FROM_COLLECTION,		// Failed to remove from the Firewall collection
			FW_ERR_GLOBAL_OPEN_PORTS,			// Failed to retrieve the globally open ports
			FW_ERR_PORT_IS_ENABLED,				// Can't get the firewall port enable information
			FW_ERR_PORT_ENABLED,				// Can't set the firewall port enable option
			FW_ERR_CREATE_PORT_INSTANCE,		// Failed to create an instance of an authorized port
			FW_ERR_SET_PORT_NUMBER,				// Failed to set port number
			FW_ERR_SET_IP_PROTOCOL,				// Failed to set IP Protocol
			FW_ERR_EXCEPTION_NOT_ALLOWED,		// Failed to get or put the exception not allowed
			FW_ERR_NOTIFICATION_DISABLED,		// Failed to get or put the notification disabled
			FW_ERR_UNICAST_MULTICAST,			// Failed to get or put the UnicastResponses To MulticastBroadcast Disabled Property
		};

		/**
		* Windows 防火墙访问类<br>
		* 原作者：Kim Youngjin
		*/
		class WinFireWall
		{
			INetFwProfile* m_pFireWallProfile;

		public:

			enum PROTOCOL
			{
				ANY=256,
				UDP=17,
				TCP=6,
			};

		public:

			WinFireWall();
			~WinFireWall();

			FW_ERROR_CODE Init();
			FW_ERROR_CODE Close();

			FW_ERROR_CODE CheckFirewall(bool &);
			FW_ERROR_CODE OpenFirewall();
			FW_ERROR_CODE CloseFirewall();

			FW_ERROR_CODE CheckApplication(const char16_t *,bool &);
			FW_ERROR_CODE AddApplication(const char16_t *,const char16_t *);
			FW_ERROR_CODE RemoveApplication(const char16_t *);

			FW_ERROR_CODE CheckPort(unsigned int,PROTOCOL,bool &);
			FW_ERROR_CODE OpenPort(unsigned int,PROTOCOL,const char16_t *);
			FW_ERROR_CODE ClosePort(unsigned int,PROTOCOL);


			FW_ERROR_CODE IsExceptionNotAllowed(bool &);
			FW_ERROR_CODE SetExceptionNotAllowed(bool);

			FW_ERROR_CODE IsNotificationDiabled(bool &);
			FW_ERROR_CODE SetNotificationDiabled(bool);

			FW_ERROR_CODE IsUnicastResponsesToMulticastBroadcastDisabled(bool &);
			FW_ERROR_CODE SetUnicastResponsesToMulticastBroadcastDisabled(bool);
		};
	}//namespace network

	using namespace network;
}//namespace hgl
#endif//HGL_NETWORK_WINDOW_FIREWALL_INCLUDE
