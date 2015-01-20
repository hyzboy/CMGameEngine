#include<hgl/graph/SceneNode.h>
#include<hgl/graph/Renderable.h>
//#include<hgl/graph/Frustum.h>
namespace hgl
{
	namespace graph
	{
		SceneNode::SceneNode()
		{
		}

		SceneNode::~SceneNode()
		{
			ClearSubNode();
			Clear();
		}

		void SceneNode::Clear()
		{
			SubData.Clear();
		}

		void SceneNode::ClearSubNode()
		{
			SubNode.Clear();
		}

		/**
		* ˢ������任����
		* @param root ���ڵ�����
		*/
		void SceneNode::RefreshMatrix(const Matrix4f *root_matrix)
		{
			if(root_matrix)
				RefreshLocalToWorldMatrix(root_matrix);
			else
				LocalToWorldMatrix=LocalMatrix;

			const int count=SubNode.GetCount();

			SceneNode **sub=SubNode.GetData();

			for(int i=0;i<count;i++)
			{
				(*sub)->RefreshMatrix(&LocalToWorldMatrix);

				sub++;
			}
		}

		/**
		* ˢ�°󶨺�
		*/
		void SceneNode::RefreshBoundingBox()
		{
			int count=SubNode.GetCount();
			SceneNode **sub=SubNode.GetData();

			Vector3f min_v,max_v;

			for(int i=0;i<count;i++)
			{
				(*sub)->RefreshBoundingBox();

				if(i==0)	//��һ��
				{
//					min_v=(*sub)->GetBounding();
				}

				sub++;
			}
		}

		/**
		* չ������Ⱦ�б�
		* @param rl ��Ⱦ�б�
		* @param func �ڵ����˺���
		* @param func_data �������˺����ĸ�������
		* @return չ���Ƿ��ɹ�
		*/
		bool SceneNode::ExpendToList(RenderList *rl,FilterSceneNodeFunc func,void *func_data)const
		{
			if(!rl)return(false);

			if(func)	//�����й��˺���
				if(!func(this,func_data))
					return(false);

			if(SubData.GetCount())
				rl->Add(this);											//���ӵ�ǰ�ڵ�

			int count=SubNode.GetCount();
			SceneNode **sub=SubNode.GetData();

			for(int i=0;i<count;i++)
			{
				(*sub)->ExpendToList(rl,func,func_data);				//չ���ӽڵ�

				sub++;
			}

			return(true);
		}

		/**
		* չ������Ⱦ�б�(ʹ��������ƽ��ͷ�ü�������)
		* @param rl ��Ⱦ�б�
		* @param cam ������
		* @param comp_func �����ñȽϺ���
		*/
		bool SceneNode::ExpendToList(RenderList *rl,Camera *cam,RenderListCompFunc comp_func)const
		{
			if(!rl||!cam)return(false);

//			Frustum f;

//			MakeCameraFrustum(&f,cam);

//			if(!ExpendToList(rl,&f))
			if(!ExpendToList(rl))
				return(false);

			if(comp_func)
			{
				//����
			}

			return(true);
		}
	}//namespace graph
}//namespace hgl
