#include<hgl/graph/VertexBuffer.h>

namespace hgl
{
	namespace graph
	{
		template<> uint VertexBuffer1<int8	>::GetDataType(){return HGL_BYTE;			}
		template<> uint VertexBuffer1<int16	>::GetDataType(){return HGL_SHORT;			}
		template<> uint VertexBuffer1<int32	>::GetDataType(){return HGL_INT;			}
		template<> uint VertexBuffer1<uint8	>::GetDataType(){return HGL_UNSIGNED_BYTE;	}
		template<> uint VertexBuffer1<uint16>::GetDataType(){return HGL_UNSIGNED_SHORT;}
		template<> uint VertexBuffer1<uint32>::GetDataType(){return HGL_UNSIGNED_INT;	}
		template<> uint VertexBuffer1<float	>::GetDataType(){return HGL_FLOAT;			}
		template<> uint VertexBuffer1<double>::GetDataType(){return HGL_DOUBLE;		}

		template<> uint VertexBuffer2<int8	>::GetDataType(){return HGL_BYTE;			}
		template<> uint VertexBuffer2<int16	>::GetDataType(){return HGL_SHORT;			}
		template<> uint VertexBuffer2<int32	>::GetDataType(){return HGL_INT;			}
		template<> uint VertexBuffer2<uint8	>::GetDataType(){return HGL_UNSIGNED_BYTE;	}
		template<> uint VertexBuffer2<uint16>::GetDataType(){return HGL_UNSIGNED_SHORT;}
		template<> uint VertexBuffer2<uint32>::GetDataType(){return HGL_UNSIGNED_INT;	}
		template<> uint VertexBuffer2<float	>::GetDataType(){return HGL_FLOAT;			}
		template<> uint VertexBuffer2<double>::GetDataType(){return HGL_DOUBLE;		}

		template<> uint VertexBuffer3<int8	>::GetDataType(){return HGL_BYTE;			}
		template<> uint VertexBuffer3<int16	>::GetDataType(){return HGL_SHORT;			}
		template<> uint VertexBuffer3<int32	>::GetDataType(){return HGL_INT;			}
		template<> uint VertexBuffer3<uint8	>::GetDataType(){return HGL_UNSIGNED_BYTE;	}
		template<> uint VertexBuffer3<uint16>::GetDataType(){return HGL_UNSIGNED_SHORT;}
		template<> uint VertexBuffer3<uint32>::GetDataType(){return HGL_UNSIGNED_INT;	}
		template<> uint VertexBuffer3<float	>::GetDataType(){return HGL_FLOAT;			}
		template<> uint VertexBuffer3<double>::GetDataType(){return HGL_DOUBLE;		}

		template<> uint VertexBuffer4<int8	>::GetDataType(){return HGL_BYTE;			}
		template<> uint VertexBuffer4<int16	>::GetDataType(){return HGL_SHORT;			}
		template<> uint VertexBuffer4<int32	>::GetDataType(){return HGL_INT;			}
		template<> uint VertexBuffer4<uint8	>::GetDataType(){return HGL_UNSIGNED_BYTE;	}
		template<> uint VertexBuffer4<uint16>::GetDataType(){return HGL_UNSIGNED_SHORT;}
		template<> uint VertexBuffer4<uint32>::GetDataType(){return HGL_UNSIGNED_INT;	}
		template<> uint VertexBuffer4<float	>::GetDataType(){return HGL_FLOAT;			}
		template<> uint VertexBuffer4<double>::GetDataType(){return HGL_DOUBLE;		}
	}//namespace graph
}//namespace hgl
