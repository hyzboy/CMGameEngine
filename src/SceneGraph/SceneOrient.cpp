#include<hgl/graph/SceneOrient.h>
namespace hgl
{
	namespace graph
	{
		Matrix4f Ortho2DMatrix;														///<全局2D视图矩阵
		Matrix4f ViewportMatrix;													///<全局视口矩阵

		SceneOrient::SceneOrient()
		{
			LocalMatrix=
			LocalToWorldMatrix=
			InverseLocalMatrix=
			InverseLocalToWorldMatrix=identity();
		}

		Matrix4f &SceneOrient::SetLocalMatrix(const Matrix4f &m)
		{
			LocalMatrix=m;

			InverseLocalMatrix=inverse(LocalMatrix);

			return LocalMatrix;
		}

		Matrix4f &SceneOrient::SetLocalToWorldMatrix(const Matrix4f &m)
		{
			LocalToWorldMatrix=m;

			InverseLocalToWorldMatrix=inverse(LocalToWorldMatrix);

			return LocalToWorldMatrix;
		}

		void SceneOrient::RefreshLocalToWorldMatrix(const Matrix4f *m)
		{
			SetLocalToWorldMatrix((*m)*LocalMatrix);
		}
	}//namespace graph
}//namespace hgl
