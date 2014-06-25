#include<hgl/graph/HeightMapTerrain.h>
#include<hgl/graph/Renderable.h>
#include<hgl/graph/VertexBuffer.h>
#include<hgl/type/Pool.h>

namespace hgl
{
	namespace graph
	{
		namespace
		{
			//现默认256x256为一个块，高度使用32位浮点值。即一个块的完整字节数为256k
			//超过到1MB会产生读取品颈，而256则可以是瞬时读取

			const int BlockSize=256;											///<地块尺寸
			const int BlockBytes=BlockSize*BlockSize*sizeof(float);				///<地块字节数

			//以块中心为0，边界为1
			static float hm_convert_pos[BlockSize];
			static float dm_convert_pos[BlockSize];

			void InitHeightMapConvertPos()
			{
				const float total_width=float(BlockSize-1)/2.0f;

				for(int i=0;i<BlockSize;i++)
				{
					hm_convert_pos[i]=-1.0f+float(i)/(float(BlockSize-1)/2.0f);

					dm_convert_pos[i]=float(i)/float(BlockSize-1);
				}
			}

			static void terrainCrossProduct(float *auxNormal,float *terrainHeights,int x1,int z1,int x2,int z2,int x3,int z3)
			{
				float v1[3],v2[3];

				v1[0] = x2-x1;
				v1[1] = -terrainHeights[z1 * BlockSize + x1]
						+terrainHeights[z2 * BlockSize + x2];
				v1[2] = z2-z1;


				v2[0] = x3-x1;
				v2[1] = -terrainHeights[z1 * BlockSize + x1]
						+terrainHeights[z3 * BlockSize + x3];
				v2[2] = z3-z1;

				auxNormal[2] = v1[0] * v2[1] - v1[1] * v2[0];
				auxNormal[0] = v1[1] * v2[2] - v1[2] * v2[1];
				auxNormal[1] = v1[2] * v2[0] - v1[0] * v2[2];
			}

			static void terrainNormalize(float *v)
			{
				double d;

				d = sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]));

				v[0] = v[0] / d;
				v[1] = v[1] / d;
				v[2] = v[2] / d;
			}

			static void terrainAddVector(float *a, float *b)
			{
				a[0] += b[0];
				a[1] += b[1];
				a[2] += b[2];
			}

			void terrainComputeNormals(float *height_map,float *normal_map)
			{
				float norm1[3],norm2[3],norm3[3],norm4[3];
				int i,j,k;

				for(i = 0; i < BlockSize; i++)
					for(j = 0; j < BlockSize; j++)
					{
						memset(norm1,0,3*sizeof(float));
						memset(norm2,0,3*sizeof(float));
						memset(norm3,0,3*sizeof(float));
						memset(norm4,0,3*sizeof(float));

						/* normals for the four corners */
						if (i == 0 && j == 0)
						{
							terrainCrossProduct(norm1,height_map,0,0, 0,1, 1,0);
							terrainNormalize(norm1);
						}
						else if (j == BlockSize-1 && i == BlockSize-1)
						{
							terrainCrossProduct(norm1,height_map,j,i, j,i-1, j-1,i);
							terrainNormalize(norm1);
						}
						else if (j == 0 && i == BlockSize-1)
						{
							terrainCrossProduct(norm1,height_map,j,i, j,i-1, j+1,i);
							terrainNormalize(norm1);
						}
						else if (j == BlockSize-1 && i == 0)
						{
							terrainCrossProduct(norm1,height_map,j,i, j,i+1, j-1,i);
							terrainNormalize(norm1);
						}

						/* normals for the borders */
						else if (i == 0)
						{
							terrainCrossProduct(norm1,height_map,j,0, j-1,0, j,1);
							terrainNormalize(norm1);
							terrainCrossProduct(norm2,height_map,j,0,j,1,j+1,0);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);
						}
						else if (j == 0)
						{
							terrainCrossProduct(norm1,height_map,0,i, 1,i, 0,i-1);
							terrainNormalize(norm1);
							terrainCrossProduct(norm2,height_map,0,i, 0,i+1, 1,i);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);
						}
						else if (i == BlockSize-1)
						{
							terrainCrossProduct(norm1,height_map,j,i, j,i-1, j+1,i);
							terrainNormalize(norm1);
							terrainCrossProduct(norm2,height_map,j,i, j+1,i, j,i-1);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);
						}
						else if (j == BlockSize-1)
						{
							terrainCrossProduct(norm1,height_map,j,i, j,i-1, j-1,i);
							terrainNormalize(norm1);
							terrainCrossProduct(norm2,height_map,j,i, j-1,i, j,i+1);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);
						}
						else /* normals for the inner vertices using 8 neighbours */
						{
							terrainCrossProduct(norm1,height_map,j,i, j-1,i, j-1,i+1);
							terrainNormalize(norm1);
							terrainCrossProduct(norm2,height_map,j,i, j-1,i+1, j,i+1);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);

							terrainCrossProduct(norm2,height_map,j,i, j,i+1, j+1,i+1);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);

							terrainCrossProduct(norm2,height_map,j,i, j+1,i+1, j+1,i);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);

							terrainCrossProduct(norm2,height_map,j,i, j+1,i, j+1,i-1);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);

							terrainCrossProduct(norm2,height_map,j,i, j+1,i-1, j,i-1);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);

							terrainCrossProduct(norm2,height_map,j,i, j,i-1, j-1,i-1);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);

							terrainCrossProduct(norm2,height_map,j,i, j-1,i-1, j-1,i);
							terrainNormalize(norm2);
							terrainAddVector(norm1,norm2);
						}

						terrainNormalize(norm1);
						norm1[2] = - norm1[2];

						for (k = 0; k< 3; k++)
							normal_map[3*(i*BlockSize + j) + k] = norm1[k];
					}
			}

			Renderable *ConvertHeightMapToRenderable(float *hm)
			{
				Renderable *able=new Renderable;
				Material *mat=new Material;
				float *normal_map=new float[BlockSize*BlockSize*3];

				int tri_number=(BlockSize-1)*(BlockSize-1)*6;

				VB3f *vb_height =new VB3f(tri_number);
				VB2f *vb_diffuse=new VB2f(tri_number);
				VB3f *vb_normal	=new VB3f(tri_number);

				float *curr_line=hm;			//当前行高度
				float *next_line=hm+BlockSize;	//下一行高度

				vb_height->Begin();
				vb_diffuse->Begin();
				vb_normal->Begin();

				terrainComputeNormals(hm,normal_map);

				for(int row=0;row<BlockSize-1;row++)
				{
					for(int col=0;col<BlockSize-1;col++)
					{
						//高度
						Vector3f h_lt(hm_convert_pos[col  ],* curr_line   ,hm_convert_pos[row  ]);
						Vector3f h_rt(hm_convert_pos[col+1],*(curr_line+1),hm_convert_pos[row  ]);
						Vector3f h_ld(hm_convert_pos[col  ],* next_line   ,hm_convert_pos[row+1]);
						Vector3f h_rd(hm_convert_pos[col+1],*(next_line+1),hm_convert_pos[row+1]);

						vb_height->WriteTriangle(h_lt,h_ld,h_rd);
						vb_height->WriteTriangle(h_lt,h_rd,h_rt);

						curr_line++;
						next_line++;

						//法线
						Vector3f n_lt=from(normal_map+( col   + row   *BlockSize)*3);
						Vector3f n_rt=from(normal_map+((col+1)+ row   *BlockSize)*3);
						Vector3f n_ld=from(normal_map+( col   +(row+1)*BlockSize)*3);
						Vector3f n_rd=from(normal_map+((col+1)+(row+1)*BlockSize)*3);

						vb_normal->WriteTriangle(n_lt,n_ld,n_rd);
						vb_normal->WriteTriangle(n_lt,n_rd,n_rt);

						//漫反射贴图
						Vector2f d_lt(dm_convert_pos[col  ],dm_convert_pos[row  ]);
						Vector2f d_rt(dm_convert_pos[col+1],dm_convert_pos[row  ]);
						Vector2f d_ld(dm_convert_pos[col  ],dm_convert_pos[row+1]);
						Vector2f d_rd(dm_convert_pos[col+1],dm_convert_pos[row+1]);

						//vb_diffuse->WriteTriangle(d_lt,d_ld,d_rd);
						//vb_diffuse->WriteTriangle(d_lt,d_rd,d_rt);

						vb_diffuse->WriteQuad(d_lt,d_rt,d_ld,d_rd);
					}

					//加掉最后一个点
					curr_line++;
					next_line++;
				}

				vb_normal->End();
				vb_diffuse->End();
				vb_height->End();

				able->SetPrimitive(HGL_TRIANGLES);
				able->SetMaterial(mat);
				able->SetVertexBuffer(vbtVertex,vb_height);
				able->SetVertexBuffer(vbtTexCoord0,vb_diffuse);
				able->SetVertexBuffer(vbtNormal,vb_normal);

				delete[] normal_map;
				return able;
			}
		}//namespace

		HeightMapTerrain::HeightMapTerrain()
		{
			InitHeightMapConvertPos();

			block_pool=new MemBlockPool<float>(BlockSize*BlockSize);		//创建一个内存块池

			HeightMapTerrainBlock *tb=new HeightMapTerrainBlock;

			tb->data=block_pool->Acquire();

			memset(tb->data,0,BlockBytes);

			tb->able=ConvertHeightMapToRenderable(tb->data);

			block=tb;
		}

		HeightMapTerrain::HeightMapTerrain(float *block_data,int block_width,int block_height,float height)
		{
			InitHeightMapConvertPos();

			block_pool=new MemBlockPool<float>(BlockSize*BlockSize);		//创建一个内存块池

			HeightMapTerrainBlock *tb=new HeightMapTerrainBlock;

			tb->data=block_pool->Acquire();

			float *tp=tb->data;
			float *sp=block_data;

			for(int i=0;i<BlockSize*BlockSize;i++)
				*tp++=(float(*sp++)/255.0f)*height;

			tb->able=ConvertHeightMapToRenderable(tb->data);

			block=tb;
		}

		HeightMapTerrain::~HeightMapTerrain()
		{
			delete block->able;

			delete block;
			delete block_pool;
		}
	}//namespace graph
}//namespace hgl
